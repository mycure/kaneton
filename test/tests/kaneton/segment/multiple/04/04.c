/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...t/tests/kaneton/segment/multiple/04/04.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed oct 20 16:35:00 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "04.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_multiple_04(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg[4];

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST, TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE, &task) != ERROR_NONE)
    printf("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    printf("[as_reserve] error\n");

  MULTIPLE_ALLOCATE(as, 2, seg);
  MULTIPLE_ALLOCATE(as, 8, seg + 1);
  MULTIPLE_ALLOCATE(as, 2, seg + 2);

  if (segment_release(seg[1]) != ERROR_NONE)
    printf("[segment_release] error\n");

  MULTIPLE_ALLOCATE(as, 5, seg + 1);

  if (segment_release(seg[0]) != ERROR_NONE)
    printf("[segment_release] error\n");

  MULTIPLE_ALLOCATE(as, 8, seg);
  MULTIPLE_ALLOCATE(as, 4, seg + 3);

  if (segment_release(seg[0]) != ERROR_NONE)
    printf("[segment_release] error\n");

  if (segment_release(seg[1]) != ERROR_NONE)
    printf("[segment_release] error\n");

  if (segment_release(seg[2]) != ERROR_NONE)
    printf("[segment_release] error\n");

  if (segment_release(seg[3]) != ERROR_NONE)
    printf("[segment_release] error\n");

  if (as_release(as) != ERROR_NONE)
    printf("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    printf("[task_release] error\n");

  TEST_LEAVE();
}
