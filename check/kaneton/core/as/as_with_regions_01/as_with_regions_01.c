#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_as_as_with_regions_01(void)
{
  i_task	task;
  i_as		as;
  int		i;
  i_segment	seg;
  i_region	reg;
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

  ASSERT(set_size(o->regions, &sz_before) == ERROR_NONE,
	 "Error in set_size\n");

  for (i = 0; i < 10; i++)
    {
      ASSERT(segment_reserve(as,
			     PAGESZ,
			     PERM_READ | PERM_WRITE,
			     &seg) == ERROR_NONE,
	     "Error in segment_reserve");

      ASSERT(region_reserve(as,
			    seg,
			    0,
			    REGION_OPT_NONE,
			    0,
			    PAGESZ,
			    &reg)  == ERROR_NONE,
	     "Error in region_reserve");
    }

  ASSERT(set_size(o->regions, &sz_after) == ERROR_NONE,
	 "Error in set_size\n");

  ASSERT(sz_after >= sz_before + 10,
	 "Error, regions were not reserved correctly\n");

  ASSERT(as_release(as) == ERROR_NONE,
	 "Failed to release as\n");
  ASSERT(task_release(task) == ERROR_NONE,
	 "Failed to release task\n");

  TEST_LEAVE();
}
