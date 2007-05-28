/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/as/clone_01/clone_01.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sun may 27 20:27:06 2007]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * address space cloning
 */

void		check_as_clone_01(void)
{
  i_task	task;
  i_task	forked;
  t_asid	as;
  t_asid	cloned;
  t_segid	seg;
  t_regid	reg;
  o_as*		o;
  o_segment*	oseg;
  o_region*	oreg;
  t_setsz	sz;
  t_iterator	it;

  TEST_ENTER();

  ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &forked) == ERROR_NONE,
	   "error creating task\n");

  ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  ASSERT(segment_reserve(as,
			    3 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  ASSERT(region_reserve(as,
			   seg,
			   PAGESZ,
			   REGION_OPT_NONE,
			   0,
			   2 * PAGESZ,
			   &reg) == ERROR_NONE,
	    "error reserving region\n");

  ASSERT(as_clone(forked, as, &cloned) == ERROR_NONE, "as_clone failed\n");

  ASSERT(as_get(cloned, &o) == ERROR_NONE, "invalid cloned as object\n");

  ASSERT(set_size(o->segments, &sz) == ERROR_NONE, "invalid segment set in cloned object\n");

  ASSERT(region_get(cloned, reg, &oreg) == ERROR_NONE, "invalid region set in cloned object\n");

  ASSERT(segment_get(oreg->segid, &oseg) == ERROR_NONE, "invalid segment set in cloned object\n");

  ASSERT(oseg->size == 3 * PAGESZ, "Bad size field in cloned segment\n");
  ASSERT(oseg->perms == PERM_READ, "Bad perms field in cloned segment\n");
  ASSERT(oseg->type == SEGMENT_TYPE_MEMORY, "Bad type field in cloned segment\n");

  ASSERT(oreg->regid == reg, "Bad regid field in cloned region\n");
  ASSERT(oreg->size == 2 * PAGESZ, "Bad size field in cloned region\n");
  ASSERT(oreg->offset == PAGESZ, "Bad offset field in cloned region\n");
  ASSERT(oreg->segid == oseg->segid, "Bad segid field in cloned region\n");

  ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  ASSERT(as_release(cloned) == ERROR_NONE,
	    "failed to release as\n");

  ASSERT(task_release(forked) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}