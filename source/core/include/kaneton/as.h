/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * as.h
 * 
 * path          /home/mycure/kaneton/core/include
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Sat Jun 18 16:23:42 2005   mycure
 */

#ifndef KANETON_AS_H
#define KANETON_AS_H		1

/*
 * dependencies
 */

#include <machdep/arch/machdep.h>

/*
 * types
 */

typedef struct			s_as
{
  t_asid			asid;
}				t_segment;

typedef struct			s_ass
{
  t_uint32			nass;

  t_as*				ass;
}				t_ass;

/*
 * prototypes:     ../../kaneton/as/as.c
 */

/*
 * ../../kaneton/as/as.c
 */

void			as_dump(void);


int			as_init(void);

/*
 * eop
 */

#endif
