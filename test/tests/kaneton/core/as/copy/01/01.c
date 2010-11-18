/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...TABLE/test/tests/kaneton/as/copy/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed nov 17 07:52:24 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_as_copy_01(void)
{
  i_task		task1;
  i_task		task2;
  i_as			as1;
  i_as			as2;
  i_segment		seg1;
  i_segment		seg2;
  i_segment		seg3;
  i_segment		seg4;
  i_segment		seg5;
  i_segment		seg6;
  i_segment		useless;
  i_region		reg;
  t_uint32		i;
  t_uint8		buff[4 * PAGESZ];

  TEST_ENTER();

  /*
   * first address space
   */

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE,
		   &task1) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task1, &as1) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  if (segment_reserve(as1,
		      2 * PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg1) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_reserve(as1,
		      PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &useless) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_reserve(as1,
		      4 * PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg2) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_reserve(as1,
		      PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &useless) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_reserve(as1,
		      2 * PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg3) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (region_reserve(as1,
		     seg1,
		     PAGESZ,
		     REGION_OPT_FORCE,
		     0x20000000,
		     PAGESZ,
		     &reg) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error\n");

  if (region_reserve(as1,
		     seg2,
		     PAGESZ,
		     REGION_OPT_FORCE,
		     0x20001000,
		     2 * PAGESZ,
		     &reg) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error\n");

  if (region_reserve(as1,
		     seg3,
		     0,
		     REGION_OPT_FORCE,
		     0x20003000,
		     PAGESZ,
		     &reg) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error\n");

  /*
   * second address space
   */

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE,
		   &task2) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task2, &as2) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  if (segment_reserve(as2,
		      2 * PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg4) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_reserve(as2,
		      PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &useless) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_reserve(as2,
		      4 * PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg5) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_reserve(as2,
		      PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &useless) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_reserve(as2,
		      2 * PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg6) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (region_reserve(as2,
		     seg4,
		     0,
		     REGION_OPT_FORCE,
		     0x40000000,
		     PAGESZ,
		     &reg) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error\n");

  if (region_reserve(as2,
		     seg5,
		     2 * PAGESZ,
		     REGION_OPT_FORCE,
		     0x40001000,
		     PAGESZ,
		     &reg) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error\n");

  if (region_reserve(as2,
		     seg6,
		     0,
		     REGION_OPT_FORCE,
		     0x40002000,
		     2 * PAGESZ,
		     &reg) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error\n");

  /*
   * operations
   */

  for (i = 0; i < 4 * PAGESZ; i++)
    buff[i] = (i * 2 + 4) % 256;

  if (as_write(as1, buff, 4 * PAGESZ, 0x20000000) != ERROR_NONE)
    TEST_ERROR("[as_write] error\n");

  for (i = 0; i < 4 * PAGESZ; i++)
    buff[i] = 0;

  if (as_copy(as1, 0x20000000, as2, 0x40000000, 4 * PAGESZ) != ERROR_NONE)
    TEST_ERROR("[as_copy] error\n");

  if (as_read(as2, 0x40000000, 4 * PAGESZ, buff) != ERROR_NONE)
    TEST_ERROR("[as_read] error\n");

  for (i = 0; i < 4 * PAGESZ; i++)
    if (buff[i] != (i * 2 + 4) % 256)
      TEST_ERROR("the data appears invalid once read from the "
		 "address space\n");

  TEST_LEAVE();
}