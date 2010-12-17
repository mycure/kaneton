/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../tests/kaneton/core/region/flush/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [fri dec  3 22:06:13 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_region_flush_01(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg;
  i_region		reg;
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

  if (segment_reserve(as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(as,
		     seg,
		     0,
		     REGION_OPTION_NONE,
		     0,
		     ___kaneton$pagesz,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_flush(as) != ERROR_OK)
    TEST_ERROR("[region_flush] error");

  if (set_size(o->regions, &sz) != ERROR_OK)
    TEST_ERROR("[set_size] error");

  if (sz != 0)
    TEST_ERROR("there should not be regions left after a flush");

  TEST_SIGNATURE(923ri0ge0giaw093);

  TEST_LEAVE();
}
