/*
 * licence kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibc/include/libsys/errno.h
 *
 * created       julien quintard   [fri feb 11 02:40:57 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:53:16 2006]
 */

#ifndef LIBSYS_ERRNO_H
#define LIBSYS_ERRNO_H		1

/*
 * ---------- defines ---------------------------------------------------------
 */

#define EZERO			0	/* no error */

#define EPERM			100	/* operation not permitted */
#define ENOENT			200	/* no entity */
#define EIO			300	/* input/output error */
#define E2BIG			400	/* element too long */
#define ENOEXEC			500	/* exec format error */

#define EBAD			600	/* bad element */
#define EBUSY			700	/* object busy */

#endif
