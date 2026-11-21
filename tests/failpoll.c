/* failpoll.c - LD_PRELOAD for tests/ui.sh: the poll of mtkdemo's loop, with the
 * Wayland fd and the one fd it watches, fails as under memory pressure.
 * libwayland's own polls have one fd and go through. */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <poll.h>

int poll(struct pollfd *fds, nfds_t n, int timeout)
{
	static int (*real)(struct pollfd *, nfds_t, int);
	if (n == 2) {
		errno = ENOMEM;
		return -1;
	}
	if (!real)
		real = (int (*)(struct pollfd *, nfds_t, int))dlsym(RTLD_NEXT, "poll");
	return real(fds, n, timeout);
}

/* With _FORTIFY_SOURCE a poll of a count not known at compile time, as mtk's, goes
 * through here. */
int __poll_chk(struct pollfd *fds, nfds_t n, int timeout, size_t len)
{
	return poll(fds, n, timeout);
}
