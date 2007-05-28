#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_as_as_with_segments_01(void)
{
  i_task	task;
  i_as		as;
  int		i;
  i_segment	seg;
  t_setsz	sz_before;
  t_setsz	sz_after;
  o_as*		o;

  TEST_ENTER();

  ASSERT(task_reserve(TASK_CLASS_PROGRAM,
		      TASK_BEHAV_INTERACTIVE,
		      TASK_PRIOR_INTERACTIVE,
		      &task) == ERROR_NONE,
	 "Error creating task\n");
  ASSERT(as_reserve(task, &as) == ERROR_NONE,
	 "Error creating as\n");

  ASSERT(as_get(as, &o) == ERROR_NONE,
	 "Error in as_get\n");

  ASSERT(set_size(o->segments, &sz_before) == ERROR_NONE,
	 "Error in set_size\n");

  for (i = 0; i < 10; i++)
    {
      ASSERT(segment_reserve(as,
			     PAGESZ,
			     PERM_READ | PERM_WRITE,
			     &seg) == ERROR_NONE,
	     "Error in segment_reserve");
    }

  ASSERT(set_size(o->segments, &sz_after) == ERROR_NONE,
	 "Error in set_size\n");

  ASSERT(sz_after >= sz_before + 10,
	 "Error, segments were not reserved correctly\n");

  ASSERT(as_release(as) == ERROR_NONE,
	 "Failed to release as\n");
  ASSERT(task_release(task) == ERROR_NONE,
	 "Failed to release task\n");

  TEST_LEAVE();
}