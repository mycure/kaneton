/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...test/tests/kaneton/core/as/clone/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed nov 24 09:24:37 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_as_clone_01(void)
{
  i_task		task;
  i_task		forked;
  i_as			as;
  i_as			cloned;
  i_segment		seg;
  i_region		reg;
  o_as*			o;
  o_segment*		oseg;
  o_region*		oreg;
  t_setsz		sz;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task) != ERROR_OK)
    TEST_ERROR("[task_reserve] error\n");

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &forked) != ERROR_OK)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_OK)
    TEST_ERROR("[as_reserve] error\n");

  if (segment_reserve(as,
		      3 * PAGESZ,
		      PERMISSION_READ,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error\n");

  if (region_reserve(as,
		     seg,
		     PAGESZ,
		     REGION_OPTION_NONE,
		     0,
		     2 * PAGESZ,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error\n");

  if (as_clone(forked, as, &cloned) != ERROR_OK)
    TEST_ERROR("[as_clone] error\n");

  if (as_get(cloned, &o) != ERROR_OK)
    TEST_ERROR("[as_get] error: unable to retrieve the cloned "
	       "address space\n");

  if (set_size(o->segments, &sz) != ERROR_OK)
    TEST_ERROR("[set_size] error\n");

  if (region_get(cloned, reg, &oreg) != ERROR_OK)
    TEST_ERROR("[region_get] error: unable to retrieve a region from a "
	       "cloned address space\n");

  if (segment_get(oreg->segment, &oseg) != ERROR_OK)
    TEST_ERROR("[segment_get] error: unable to retrieve a segment from a "
	       "cloned address space\n");

  if (oseg->size != 3 * PAGESZ)
    TEST_ERROR("invalid segment's size\n");

  if (oseg->permissions != PERMISSION_READ)
    TEST_ERROR("invalid segment's permissions\n");

  if (oseg->type != SEGMENT_TYPE_MEMORY)
    TEST_ERROR("invalid segment's type\n");

  if (oreg->id != reg)
    TEST_ERROR("invalid region's identifier\n");

  if (oreg->size != 2 * PAGESZ)
    TEST_ERROR("invalid region's size\n");

  if (oreg->offset != PAGESZ)
    TEST_ERROR("invalid region's offset\n");

  if (oreg->segment != oseg->id)
    TEST_ERROR("invalid region's segment identifier\n");

  TEST_LEAVE();
}
