/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/map.h
 *
 * created       matthieu bucchianeri   [sun feb 26 12:23:01 2006]
 * updated       matthieu bucchianeri   [fri jul 28 18:25:36 2006]
 */

#ifndef CORE_MAP_H
#define CORE_MAP_H	1

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct sm_map		m_map;

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * flags
 */

#define MAP_OPT_NONE		0
#define MAP_OPT_FORCE		(1 << 0)
#define MAP_OPT_USER		(0 << 1)
#define MAP_OPT_PRIVILEGED	(1 << 1)

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/id.h>
#include <core/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * map manager
 */

struct				sm_map
{
  i_stats			stats;
};

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define MAP_CHECK(_map_)						\
  {									\
    if ((_map_) == NULL)						\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define MAP_ENTER(_map_)						\
  {									\
    MAP_CHECK((_map_));							\
									\
    STATS_BEGIN((_map_)->stats);					\
  }

/*
 * leave
 */

#define MAP_LEAVE(_map_, _error_)					\
  {									\
    STATS_END((_map_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/map/map.c
 */

/*
 * ../../core/map/map.c
 */

t_error			map_reserve(i_as		asid,
				    t_opts		opts,
				    t_vsize		size,
				    t_perms		perms,
				    t_vaddr*		addr);

t_error			map_release(i_as		asid,
				    t_vaddr		addr);

t_error			map_resize(i_as			asid,
				   t_vaddr		old,
				   t_vsize		size,
				   t_vaddr*		new);

t_error			map_init(void);

t_error			map_clean(void);


/*
 * eop
 */

#endif
