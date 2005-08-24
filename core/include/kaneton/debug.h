/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * init.h
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:11 2005   mycure
 * last update   Wed Aug 24 12:50:27 2005   mycure
 */

#ifndef KANETON_DEBUG_H
#define KANETON_DEBUG_H		1

/*
 * ---------- defines ---------------------------------------------------------
 */

#define KANETON_DEBUG_PARAMS	(1 << 0)
#define KANETON_DEBUG_AS	(1 << 1)
#define KANETON_DEBUG_SEGMENT	(1 << 2)
#define KANETON_DEBUG_SET	(1 << 3)

#define KANETON_DEBUG_KANETON						\
  (0)

#ifdef USER_DEBUG
#define KANETON_DEBUG		(KANETON_DEBUG_KANETON | USER_DEBUG)
#else
#define KANETON_DEBUG		(KANETON_DEBUG_KANETON)
#endif

#endif
