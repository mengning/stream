#ifndef VLOG_H
#define VLOG_H 1

#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include "util.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define VLOG_LEVELS                             \
    VLOG_LEVEL(OFF, LOG_ALERT)                  \
    VLOG_LEVEL(EMER, LOG_ALERT)                 \
    VLOG_LEVEL(ERR, LOG_ERR)                    \
    VLOG_LEVEL(WARN, LOG_WARNING)               \
    VLOG_LEVEL(INFO, LOG_NOTICE)                \
    VLOG_LEVEL(DBG, LOG_DEBUG)
enum vlog_level {
#define VLOG_LEVEL(NAME, SYSLOG_LEVEL) VLL_##NAME,
    VLOG_LEVELS
#undef VLOG_LEVEL
    VLL_N_LEVELS
};

#define VLOG_FACILITIES                                                 \
    VLOG_FACILITY(SYSLOG, "ovs|%05N|%c%T|%p|%m")                            \
    VLOG_FACILITY(CONSOLE, "%D{%Y-%m-%dT%H:%M:%SZ}|%05N|%c%T|%p|%m")    \
    VLOG_FACILITY(FILE, "%D{%Y-%m-%dT%H:%M:%S.###Z}|%05N|%c%T|%p|%m")
enum vlog_facility {
#define VLOG_FACILITY(NAME, PATTERN) VLF_##NAME,
    VLOG_FACILITIES
#undef VLOG_FACILITY
    VLF_N_FACILITIES,
    VLF_ANY_FACILITY = -1
};

const char *vlog_get_facility_name(enum vlog_facility);
enum vlog_facility vlog_get_facility_val(const char *name);

struct vlog_module {
    const char *name;
    int levels[VLF_N_FACILITIES];
    int min_level;
    bool honor_rate_limits;
};

#if USE_LINKER_SECTIONS
#define VLOG_DEFINE_MODULE(MODULE)                                      \
        VLOG_DEFINE_MODULE__(MODULE)                                    \
        extern struct vlog_module *const vlog_module_ptr_##MODULE;      \
        struct vlog_module *const vlog_module_ptr_##MODULE              \
            __attribute__((section("vlog_modules"))) = &VLM_##MODULE
#else
#define VLOG_DEFINE_MODULE(MODULE) extern struct vlog_module VLM_##MODULE
#endif

#define VLOG_MSG_TOKENS (60 * 1000)

#define VLOG_RATE_LIMIT_INIT(RATE, BURST)                               \
        {                                                               \
            0,                                                          \
            0,                                                          \
            0,                                                          \
        }

#define VLOG_DEFINE_THIS_MODULE(MODULE)                                 \
        VLOG_DEFINE_MODULE(MODULE);                                     \
        static struct vlog_module *const THIS_MODULE = &VLM_##MODULE

#define VLOG_FATAL(...) vlog_fatal(THIS_MODULE, __VA_ARGS__)
#define VLOG_ABORT(...) vlog_abort(THIS_MODULE, __VA_ARGS__)
#define VLOG_EMER(...) VLOG(VLL_EMER, __VA_ARGS__)
#define VLOG_ERR(...) VLOG(VLL_ERR, __VA_ARGS__)
#define VLOG_WARN(...) VLOG(VLL_WARN, __VA_ARGS__)
#define VLOG_INFO(...) VLOG(VLL_INFO, __VA_ARGS__)
#define VLOG_DBG(...) VLOG(VLL_DBG, __VA_ARGS__)

#define VLOG_IS_ERR_ENABLED() vlog_is_enabled(THIS_MODULE, VLL_ERR)
#define VLOG_IS_WARN_ENABLED() vlog_is_enabled(THIS_MODULE, VLL_WARN)
#define VLOG_IS_INFO_ENABLED() vlog_is_enabled(THIS_MODULE, VLL_INFO)
#define VLOG_IS_DBG_ENABLED() vlog_is_enabled(THIS_MODULE, VLL_DBG)

#define VLOG_ERR_RL(RL, ...) VLOG_RL(RL, VLL_ERR, __VA_ARGS__)
#define VLOG_WARN_RL(RL, ...) VLOG_RL(RL, VLL_WARN, __VA_ARGS__)
#define VLOG_INFO_RL(RL, ...) VLOG_RL(RL, VLL_INFO, __VA_ARGS__)
#define VLOG_DBG_RL(RL, ...) VLOG_RL(RL, VLL_DBG, __VA_ARGS__)

#define VLOG_DROP_ERR(RL) vlog_should_drop(THIS_MODULE, VLL_ERR, RL)
#define VLOG_DROP_WARN(RL) vlog_should_drop(THIS_MODULE, VLL_WARN, RL)
#define VLOG_DROP_INFO(RL) vlog_should_drop(THIS_MODULE, VLL_INFO, RL)
#define VLOG_DROP_DBG(RL) vlog_should_drop(THIS_MODULE, VLL_DBG, RL)

#define VLOG_ERR_ONCE(...) VLOG_ONCE(VLL_ERR, __VA_ARGS__)
#define VLOG_WARN_ONCE(...) VLOG_ONCE(VLL_WARN, __VA_ARGS__)
#define VLOG_INFO_ONCE(...) VLOG_ONCE(VLL_INFO, __VA_ARGS__)
#define VLOG_DBG_ONCE(...) VLOG_ONCE(VLL_DBG, __VA_ARGS__)

#define VLOG_OPTION_ENUMS OPT_LOG_FILE
#define VLOG_LONG_OPTIONS                                   \
        {"verbose",     optional_argument, NULL, 'v'},         \
        {"log-file",    optional_argument, NULL, OPT_LOG_FILE}
#define VLOG_OPTION_HANDLERS                    \
        case 'v':                               \
            vlog_set_verbosity(optarg);         \
            break;                              \
        case OPT_LOG_FILE:                      \
            vlog_set_log_file(optarg);          \
            break;

#define VLOG(LEVEL, ...)                                \
    do {                                                \
        enum vlog_level level__ = LEVEL;                \
        if (THIS_MODULE->min_level >= level__) {        \
            vlog(THIS_MODULE, level__, __VA_ARGS__);    \
        }                                               \
    } while (0)
#define VLOG_RL(RL, LEVEL, ...)                                     \
    do {                                                            \
        enum vlog_level level__ = LEVEL;                            \
        if (THIS_MODULE->min_level >= level__) {                    \
            vlog_rate_limit(THIS_MODULE, level__, RL, __VA_ARGS__); \
        }                                                           \
    } while (0)
#define VLOG_ONCE(LEVEL, ...)                                           \
    do {                                                                \
        static struct ovsthread_once once = OVSTHREAD_ONCE_INITIALIZER; \
        if (ovsthread_once_start(&once)) {                              \
            vlog(THIS_MODULE, LEVEL, __VA_ARGS__);                      \
            ovsthread_once_done(&once);                                 \
        }                                                               \
    } while (0)

#define VLOG_DEFINE_MODULE__(MODULE)                                    \
        extern struct vlog_module VLM_##MODULE;                         \
        struct vlog_module VLM_##MODULE =                               \
        {                                                               \
            #MODULE,                                                    \
            { [ 0 ... VLF_N_FACILITIES - 1] = VLL_INFO },               \
            VLL_INFO,                                                   \
            true                                                        \
        };

#ifdef  __cplusplus
}
#endif


#endif
