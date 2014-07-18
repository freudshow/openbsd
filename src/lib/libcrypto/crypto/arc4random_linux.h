/*	$OpenBSD: arc4random_linux.h,v 1.1 2014/07/18 02:05:55 deraadt Exp $	*/

/*
 * Copyright (c) 1996, David Mazieres <dm@uun.org>
 * Copyright (c) 2008, Damien Miller <djm@openbsd.org>
 * Copyright (c) 2013, Markus Friedl <markus@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * Stub functions for portability.
 */

static inline void *
_rs_allocate(size_t len)
{
	void *p;

	if ((p = mmap(NULL, sizeof(*rs), PROT_READ|PROT_WRITE,
	    MAP_ANON|MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (NULL);
	return (p);
}

static inline void
_rs_forkhandler(void)
{
	/*
	 * Race-free because we're running single-threaded in a new
	 * address space, and once allocated rs is never deallocated.
	 */
	if (rs)
		rs->rs_count = 0;
}

static inline void
_rs_forkdetect(void)
{
	static pid_t _rs_pid = 0;
	pid_t pid = getpid();

	/* If a system lacks MAP_INHERIT_ZERO, resort to getpid() */
	if (_rs_pid == 0 || _rs_pid != pid) {
		_rs_pid = pid;
		if (rs)
			rs->rs_count = 0;
	}
}

static inline void
_rs_forkdetectsetup(struct _rs *rs, size_t len)
{
	_ARC4_ATFORK(_rs_forkhandler);
}
