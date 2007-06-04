/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/as.h
 *
 * created       julien quintard   [fri feb 11 02:19:44 2005]
 * updated       julien quintard   [wed jun 28 12:47:44 2006]
 */

#ifndef CORE_AS_H
#define CORE_AS_H		1

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct so_as		o_as;
typedef struct sm_as		m_as;
typedef struct sd_as		d_as;

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * init sizes for the array data structure set
 */

#define AS_SEGMENTS_INITSZ	0x4
#define AS_REGIONS_INITSZ	0x4

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
 * as object
 */

struct				so_as
{
  i_as				asid;

  i_task			tskid;

  i_set				segments;
  i_set				regions;

  machdep_data(o_as);
};

/*
 * as manager
 */

struct				sm_as
{
  o_id				id;

  i_stats			stats;

  i_set				ass;

  machdep_data(m_as);
};

/*
 * the as architecture dependent interface
 */

struct				sd_as
{
  t_error			(*as_show)(i_as);
  t_error			(*as_give)(i_task,
					   i_as);
  t_error			(*as_vaddr)(i_as,
					    t_paddr,
					    t_vaddr*);
  t_error			(*as_paddr)(i_as,
					    t_vaddr,
					    t_paddr*);
  t_error			(*as_clone)(i_task,
					    i_as,
					    i_as*);
  t_error			(*as_reserve)(i_task,
					      i_as*);
  t_error			(*as_release)(i_as);
  t_error			(*as_init)(void);
  t_error			(*as_clean)(void);
};

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define AS_CHECK(_as_)							\
  {									\
    if ((_as_) == NULL)							\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define AS_ENTER(_as_)							\
  {									\
    AS_CHECK((_as_));							\
									\
    STATS_BEGIN((_as_)->stats);						\
  }

/*
 * leave
 */

#define AS_LEAVE(_as_, _error_)						\
  {									\
    STATS_END((_as_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/as/as.c
 */

/*
 * ../../core/as/as.c
 */

t_error			as_show(i_as				asid);

t_error			as_dump(void);

t_error			as_give(i_task			tskid,
				i_as			asid);

t_error			as_vaddr(i_as			asid,
				 t_paddr		physical,
				 t_vaddr*		virtual);

t_error			as_paddr(i_as		asid,
				 t_vaddr	virtual,
				 t_paddr*	physical);

t_error			as_read(i_as				asid,
				t_vaddr				src,
				t_vsize				size,
				void*				dst);

t_error			as_write(i_as				asid,
				 void*				src,
				 t_vsize			size,
				 t_vaddr			dst);

t_error			as_copy(i_as				src_as,
				t_vaddr				src,
				i_as				dst_as,
				t_vaddr				dst,
				t_vsize				size);

t_error			as_clone(i_task				tskid,
				 i_as				old,
				 i_as*				new);

t_error			as_reserve(i_task			tskid,
				   i_as*			asid);

t_error			as_release(i_as			asid);

t_error			as_get(i_as				asid,
			       o_as**				o);

t_error			as_init(void);

t_error			as_clean(void);


/*
 * eop
 */

#endif
