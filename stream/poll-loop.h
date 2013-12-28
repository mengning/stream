#ifndef POLL_LOOP_H
#define POLL_LOOP_H 1

#include <poll.h>

#ifdef  __cplusplus
extern "C" {
#endif

void poll_fd_wait_at(int fd, short int events, const char *where);
#define poll_fd_wait(fd, events) poll_fd_wait_at(fd, events, SOURCE_LOCATOR)

void poll_timer_wait_at(long long int msec, const char *where);
#define poll_timer_wait(msec) poll_timer_wait_at(msec, SOURCE_LOCATOR)

void poll_timer_wait_until_at(long long int msec, const char *where);
#define poll_timer_wait_until(msec)             \
    poll_timer_wait_until_at(msec, SOURCE_LOCATOR)

void poll_immediate_wake_at(const char *where);
#define poll_immediate_wake() poll_immediate_wake_at(SOURCE_LOCATOR)

void poll_block(void);

#ifdef  __cplusplus
}
#endif

#endif
