/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * errno.h
 * 
 * path          /home/mycure/kaneton/libs/klibc/include
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:33:38 2005   mycure
 * last update   Sun Jun 19 22:19:06 2005   mycure
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
