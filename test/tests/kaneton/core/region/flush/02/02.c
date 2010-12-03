/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo.../tests/kaneton/core/region/flush/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [mon nov 29 18:47:47 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_region_flush_02(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg;
  i_region		reg;
  t_uint32		i;
  o_as*			o;
  t_setsz		sz;

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

  if (region_flush(as) != ERROR_OK)
    TEST_ERROR("[region_flush] error");

  if (set_size(o->regions, &sz) != ERROR_OK)
    TEST_ERROR("[set_size] error");

  if (sz != 0)
    TEST_ERROR("there should not be regions left after a flush");

  if (as_release(as) != ERROR_OK)
    TEST_ERROR("[as_release] error");

  if (task_release(task) != ERROR_OK)
    TEST_ERROR("[task_release] error");

  TEST_SIGNATURE(r3t04ge0giea9wgggg);

  TEST_LEAVE();
}
