/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/segment/06/06.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sat mar 25 16:30:38 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * segment_catch.
 */

void		check_segment_06(void)
{
  t_tskid	task1;
  t_tskid	task2;
  t_asid	as1;
  t_asid	as2;
  t_segid	seg;
  o_segment*	o;

  TEST_ENTER;
  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task1) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task2) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task1, &as1) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(as_reserve(task2, &as2) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(segment_reserve(as1,
			    2 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  MY_ASSERT(segment_type(seg, SEGMENT_TYPE_CATCH) == ERROR_NONE,
	    "cannot set a segment as catcheable\n");

  MY_ASSERT(segment_catch(as2, seg) == ERROR_NONE,
	    "cannot catch the segment\n");

  MY_ASSERT(segment_get(seg, &o) == ERROR_NONE, "error getting as\n");

  MY_ASSERT(o->asid = as2, "Bad type field\n");
  MY_ASSERT(o->type = SEGMENT_TYPE_MEMORY, "Bad type field\n");

  MY_ASSERT(as_release(as1) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(as_release(as2) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task1) == ERROR_NONE,
	    "failed to release task\n");

  MY_ASSERT(task_release(task2) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE;
}