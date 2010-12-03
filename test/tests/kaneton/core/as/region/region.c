/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...st/tests/kaneton/core/as/region/region.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 18:42:50 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "region.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_as_region(void)
{
  i_task		task;
  i_as			as;
  int			i;
  i_segment		seg;
  i_region		reg;
  t_setsz		sz_before;
  t_setsz		sz_after;
  o_as*			o;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task) != ERROR_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task, &as) != ERROR_OK)
    TEST_ERROR("[as_reserve] error");

  if (as_get(as, &o) != ERROR_OK)
    TEST_ERROR("[as_get] error");

  if (set_size(o->regions, &sz_before) != ERROR_OK)
    TEST_ERROR("[set_size] error");

  for (i = 0; i < 10; i++)
    {
      if (segment_reserve(as,
			  PAGESZ,
			  PERMISSION_READ | PERMISSION_WRITE,
			  &seg) != ERROR_OK)
	TEST_ERROR("[segment_reserve] error");

      if (region_reserve(as,
			 seg,
			 0,
			 REGION_OPTION_NONE,
			 0,
			 PAGESZ,
			 &reg) != ERROR_OK)
	TEST_ERROR("[region_reserve] error");
    }

  if (set_size(o->regions, &sz_after) != ERROR_OK)
    TEST_ERROR("[set_size] error");

  if (sz_after < (sz_before + 10))
    TEST_ERROR("the regions have not been reserved properly");

  if (as_release(as) != ERROR_OK)
    TEST_ERROR("[as_release] error");

  if (task_release(task) != ERROR_OK)
    TEST_ERROR("[task_release] error");

  TEST_SIGNATURE(r32iijogdpzfepwg0t);

  TEST_LEAVE();
}
