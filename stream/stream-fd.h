#ifndef STREAM_FD_H
#define STREAM_FD_H 1

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct stream;
struct pstream;
struct sockaddr;

int new_fd_stream(const char *name, int fd, int connect_status,
                  struct stream **streamp);
int new_fd_pstream(const char *name, int fd,
                   int (*accept_cb)(int fd, const struct sockaddr *,
                                    size_t sa_len, struct stream **),
                   int (*set_dscp_cb)(int fd, uint8_t dscp),
                   char *unlink_path,
                   struct pstream **pstreamp);

#endif /* stream-fd.h */
