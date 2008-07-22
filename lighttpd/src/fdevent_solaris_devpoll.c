#include <sys/types.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>

#include "fdevent.h"
#include "settings.h"
#include "buffer.h"

#ifdef USE_SOLARIS_DEVPOLL

static void fdevent_solaris_devpoll_free(fdevents *ev) {
	free(ev->devpollfds);
	close(ev->devpoll_fd);
}

/* return -1 is fine here */

static int fdevent_solaris_devpoll_event_del(fdevents *ev, int fde_ndx, int fd) {
	struct pollfd pfd;

	if (fde_ndx < 0) return -1;

	pfd.fd = fd;
	pfd.events = POLLREMOVE;
	pfd.revents = 0;

	if (-1 == write(ev->devpoll_fd, &pfd, sizeof(pfd))) {
		fprintf(stderr, "%s.%d: (del) write failed: (%d, %s)\n",
			__FILE__, __LINE__,
			fd, strerror(errno));

		return -1;
	}

	return -1;
}

static int fdevent_solaris_devpoll_event_add(fdevents *ev, int fde_ndx, int fd, int events) {
	struct pollfd pfd;
	int add = 0;

	if (fde_ndx == -1) add = 1;

	pfd.fd = fd;
	pfd.events = events;
	pfd.revents = 0;

	if (-1 == write(ev->devpoll_fd, &pfd, sizeof(pfd))) {
		fprintf(stderr, "%s.%d: (del) write failed: (%d, %s)\n",
			__FILE__, __LINE__,
			fd, strerror(errno));

		return -1;
	}

	return fd;
}

static int fdevent_solaris_devpoll_poll(fdevents *ev, int timeout_ms) {
	struct dvpoll dopoll;
	int ret;

	dopoll.dp_timeout = timeout_ms;
	dopoll.dp_nfds = ev->maxfds - 1;
	dopoll.dp_fds = ev->devpollfds;

	ret = ioctl(ev->devpoll_fd, DP_POLL, &dopoll);

	return ret;
}

static int fdevent_solaris_devpoll_event_get_revent(fdevents *ev, size_t ndx) {
	return ev->devpollfds[ndx].revents;
}

static int fdevent_solaris_devpoll_event_get_fd(fdevents *ev, size_t ndx) {
	return ev->devpollfds[ndx].fd;
}

static int fdevent_solaris_devpoll_event_next_fdndx(fdevents *ev, int last_ndx) {
	size_t i;

	UNUSED(ev);

	i = (last_ndx < 0) ? 0 : last_ndx + 1;

	return i;
}

int fdevent_solaris_devpoll_reset(fdevents *ev) {
	/* a forked process does only inherit the filedescriptor,
	 * but every operation on the device will lead to a EACCES */
	if ((ev->devpoll_fd = open("/dev/poll", O_RDWR)) < 0) {
		fprintf(stderr, "%s.%d: opening /dev/poll failed (%s), try to set server.event-handler = \"poll\" or \"select\"\n",
			__FILE__, __LINE__, strerror(errno));

		return -1;
	}

	if (fcntl(ev->devpoll_fd, F_SETFD, FD_CLOEXEC) < 0) {
		fprintf(stderr, "%s.%d: opening /dev/poll failed (%s), try to set server.event-handler = \"poll\" or \"select\"\n",
			__FILE__, __LINE__, strerror(errno));

		close(ev->devpoll_fd);

		return -1;
	}
	return 0;
}
int fdevent_solaris_devpoll_init(fdevents *ev) {
	ev->type = FDEVENT_HANDLER_SOLARIS_DEVPOLL;
#define SET(x) \
	ev->x = fdevent_solaris_devpoll_##x;

	SET(free);
	SET(poll);
	SET(reset);

	SET(event_del);
	SET(event_add);

	SET(event_next_fdndx);
	SET(event_get_fd);
	SET(event_get_revent);

	ev->devpollfds = malloc(sizeof(*ev->devpollfds) * ev->maxfds);

	if ((ev->devpoll_fd = open("/dev/poll", O_RDWR)) < 0) {
		fprintf(stderr, "%s.%d: opening /dev/poll failed (%s), try to set server.event-handler = \"poll\" or \"select\"\n",
			__FILE__, __LINE__, strerror(errno));

		return -1;
	}

	/* we just wanted to check if it works */
	close(ev->devpoll_fd);

	ev->devpoll_fd = -1;

	return 0;
}

#else
int fdevent_solaris_devpoll_init(fdevents *ev) {
	UNUSED(ev);

	fprintf(stderr, "%s.%d: solaris-devpoll not supported, try to set server.event-handler = \"poll\" or \"select\"\n",
			__FILE__, __LINE__);

	return -1;
}
#endif
