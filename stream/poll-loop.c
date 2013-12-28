#include "poll-loop.h"
#include <errno.h>
#include <stddef.h>
#include <sys/types.h>
#include <inttypes.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include <pthread.h>
#include "socket-util.h"

void
poll_fd_wait_at(int fd, short int events, const char *where)
{
fd=0;
events=0;
where=0;
}

void
poll_timer_wait_at(long long int msec, const char *where)
{
msec=0;
where=0;
}

void
poll_timer_wait_until_at(long long int when, const char *where)
{
when=0;
where=0;
}

void
poll_immediate_wake_at(const char *where)
{
    poll_timer_wait_at(0, where);
}

void
poll_block(void)
{
}
