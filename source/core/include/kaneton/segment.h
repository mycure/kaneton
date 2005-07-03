/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * segment.h
 * 
 * path          /home/mycure/kaneton/core/include
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Sun Jun 19 22:05:15 2005   mycure
 */

#ifndef KANETON_SEGMENT_H
#define KANETON_SEGMENT_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * segment object
 */

typedef struct
{
  t_segid			segid;

  t_paddr			address;
  t_psize			size;

  /* t_ownid			ownid; */
}				o_segment;

typedef struct
{
  t_uint32			nsegments;

  o_segment*			segments;
}				t_segments;

/*
 * segment manager
 */

typedef struct
{

}				m_segment;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/segment/segment.c
 */

/*
 * ../../kaneton/segment/segment.c
 */

int			segment_init(void);


/*
 * eop
 */

#endif
