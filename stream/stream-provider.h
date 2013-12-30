#ifndef STREAM_PROVIDER_H
#define STREAM_PROVIDER_H 1

#include <sys/types.h>
#include "stream.h"

struct stream {
    const struct stream_class *class;
    int state;
    int error;
    ovs_be32 remote_ip;
    ovs_be16 remote_port;
    ovs_be32 local_ip;
    ovs_be16 local_port;
    char *name;
};

void stream_init(struct stream *, const struct stream_class *,
                 int connect_status, const char *name);
void stream_set_remote_ip(struct stream *, ovs_be32 remote_ip);
void stream_set_remote_port(struct stream *, ovs_be16 remote_port);
void stream_set_local_ip(struct stream *, ovs_be32 local_ip);
void stream_set_local_port(struct stream *, ovs_be16 local_port);
static void stream_assert_class(const struct stream *stream,
                                       const struct stream_class *class)
{
    ovs_assert(stream->class == class);
}

struct stream_class {
    const char *name;
    bool needs_probes;
    int (*open)(const char *name, char *suffix, struct stream **streamp,
                uint8_t dscp);
    void (*close)(struct stream *stream);
    int (*connect)(struct stream *stream);
    ssize_t (*recv)(struct stream *stream, void *buffer, size_t n);
    ssize_t (*send)(struct stream *stream, const void *buffer, size_t n);
    void (*run)(struct stream *stream);
    void (*run_wait)(struct stream *stream);
    void (*wait)(struct stream *stream, enum stream_wait_type type);
};

struct pstream {
    const struct pstream_class *class;
    char *name;
    ovs_be16 bound_port;
};

void pstream_init(struct pstream *, const struct pstream_class *, const char *name);
void pstream_set_bound_port(struct pstream *, ovs_be16 bound_port);
static void pstream_assert_class(const struct pstream *pstream,
                                        const struct pstream_class *class)
{
    ovs_assert(pstream->class == class);
}

struct pstream_class {
    const char *name;
    bool needs_probes;
    int (*listen)(const char *name, char *suffix, struct pstream **pstreamp,
                  uint8_t dscp);
    void (*close)(struct pstream *pstream);
    int (*accept)(struct pstream *pstream, struct stream **new_streamp);
    void (*wait)(struct pstream *pstream);
    int (*set_dscp)(struct pstream *pstream, uint8_t dscp);
};

extern const struct stream_class tcp_stream_class;
extern const struct pstream_class ptcp_pstream_class;
extern const struct stream_class unix_stream_class;
extern const struct pstream_class punix_pstream_class;
#ifdef HAVE_OPENSSL
extern const struct stream_class ssl_stream_class;
extern const struct pstream_class pssl_pstream_class;
#endif

#endif /* stream-provider.h */
