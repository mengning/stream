#include "stream-fd.h"
#include <errno.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "socket-util.h"
#include "stream-provider.h"
#include "stream.h"

static void stream_assert_class(const struct stream *stream,
                                       const struct stream_class *class)
{
    ovs_assert(stream->class == class);
}

static void pstream_assert_class(const struct pstream *pstream,
                                        const struct pstream_class *class)
{
    ovs_assert(pstream->class == class);
}

VLOG_DEFINE_THIS_MODULE(stream_fd);

struct stream_fd
{
    struct stream stream;
    int fd;
};

static const struct stream_class stream_fd_class;

int
new_fd_stream(const char *name, int fd, int connect_status,
              struct stream **streamp)
{
    struct stream_fd *s;

    s = xmalloc(sizeof *s);
    stream_init(&s->stream, &stream_fd_class, connect_status, name);
    s->fd = fd;
    *streamp = &s->stream;
    return 0;
}

static struct stream_fd *
stream_fd_cast(struct stream *stream)
{
    stream_assert_class(stream, &stream_fd_class);
    return CONTAINER_OF(stream, struct stream_fd, stream);
}

static void
fd_close(struct stream *stream)
{
    struct stream_fd *s = stream_fd_cast(stream);
    close(s->fd);
    free(s);
}

static int
fd_connect(struct stream *stream)
{
    struct stream_fd *s = stream_fd_cast(stream);
    return check_connection_completion(s->fd);
}

static ssize_t
fd_recv(struct stream *stream, void *buffer, size_t n)
{
    struct stream_fd *s = stream_fd_cast(stream);
    ssize_t retval;

    retval = read(s->fd, buffer, n);
    return retval >= 0 ? retval : -errno;
}

static ssize_t
fd_send(struct stream *stream, const void *buffer, size_t n)
{
    struct stream_fd *s = stream_fd_cast(stream);
    ssize_t retval;

    retval = write(s->fd, buffer, n);
    return (retval > 0 ? retval
            : retval == 0 ? -EAGAIN
            : -errno);
}

static const struct stream_class stream_fd_class = {
    "fd",                       /* name */
    false,                      /* needs_probes */
    NULL,                       /* open */
    fd_close,                   /* close */
    fd_connect,                 /* connect */
    fd_recv,                    /* recv */
    fd_send,                    /* send */
    NULL,                       /* run */
    NULL,                       /* run_wait */
    NULL,                       /* wait */
};

struct fd_pstream
{
    struct pstream pstream;
    int fd;
    int (*accept_cb)(int fd, const struct sockaddr *, size_t sa_len,
                     struct stream **);
    int (*set_dscp_cb)(int fd, uint8_t dscp);
    char *unlink_path;
};

static const struct pstream_class fd_pstream_class;

static struct fd_pstream *
fd_pstream_cast(struct pstream *pstream)
{
    pstream_assert_class(pstream, &fd_pstream_class);
    return CONTAINER_OF(pstream, struct fd_pstream, pstream);
}

int
new_fd_pstream(const char *name, int fd,
               int (*accept_cb)(int fd, const struct sockaddr *sa,
                                size_t sa_len, struct stream **streamp),
               int (*set_dscp_cb)(int fd, uint8_t dscp),
               char *unlink_path, struct pstream **pstreamp)
{
    struct fd_pstream *ps = xmalloc(sizeof *ps);
    pstream_init(&ps->pstream, &fd_pstream_class, name);
    ps->fd = fd;
    ps->accept_cb = accept_cb;
    ps->set_dscp_cb = set_dscp_cb;
    ps->unlink_path = unlink_path;
    *pstreamp = &ps->pstream;
    return 0;
}

static void
pfd_close(struct pstream *pstream)
{
    struct fd_pstream *ps = fd_pstream_cast(pstream);
    close(ps->fd);
    free(ps);
}

static int
pfd_accept(struct pstream *pstream, struct stream **new_streamp)
{
    struct fd_pstream *ps = fd_pstream_cast(pstream);
    struct sockaddr_storage ss;
    socklen_t ss_len = sizeof ss;
    int new_fd;
    int retval;

    new_fd = accept(ps->fd, (struct sockaddr *) &ss, &ss_len);
    if (new_fd < 0) {
        retval = errno;
        if (retval != EAGAIN) {
        }
        return retval;
    }

    retval = set_nonblocking(new_fd);
    if (retval) {
        close(new_fd);
        return retval;
    }

    return ps->accept_cb(new_fd, (const struct sockaddr *) &ss, ss_len,
                         new_streamp);
}

static int
pfd_set_dscp(struct pstream *pstream, uint8_t dscp)
{
    struct fd_pstream *ps = fd_pstream_cast(pstream);
    if (ps->set_dscp_cb) {
        return ps->set_dscp_cb(ps->fd, dscp);
    }
    return 0;
}

static const struct pstream_class fd_pstream_class = {
    "pstream",
    false,
    NULL,
    pfd_close,
    pfd_accept,
    NULL,
    pfd_set_dscp,
};
