/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/2008-K1/kaneton/check/kaneton/core/segment/simple_reserve_03/simple_reserve_03.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sat mar  3 18:59:36 2007]
 */

#include <libc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_segment_simple_reserve_03(void)
{
  i_task	task;
  i_as		as;
  i_segment	seg;
  o_segment*	o;

  TEST_ENTER();

  TEST_NEW_AS(task, as);

  ASSERT(segment_reserve(as,
			    42 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  ASSERT(segment_get(seg, &o) == ERROR_NONE, "error getting segment\n");

  ASSERT(o->segid == seg, "Bad segid field\n");
  ASSERT(o->asid == as, "Bad asid field\n");
  ASSERT(o->address == (t_uint32)seg, "Bad address field\n");
  ASSERT(o->size == 42 * PAGESZ, "Bad size field\n");
  ASSERT(o->perms == PERM_READ, "Bad perms field\n");

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
