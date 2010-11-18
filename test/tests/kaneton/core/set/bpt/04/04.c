/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...TABLE/test/tests/kaneton/set/bpt/04/04.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2040]
 * updated       julien quintard   [wed nov 17 22:01:36 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "04.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_set_bpt_04(void)
{
  i_set			id;
  t_iterator		it;
  t_state		state;
  t_setsz		sz;
  void*			pdata;
  int			st;
  t_id			obj;

  TEST_ENTER();

  /*
   * reserve
   */

  if (set_reserve(bpt,
		  SET_OPT_SORT | SET_OPT_ALLOC,
                  sizeof(t_id),
		  PAGESZ,
		  &id) != ERROR_NONE)
    TEST_ERROR("[set_reserve] error\n");

  /*
   * add
   */

  obj = 80LL;
  if (set_add(id, &obj) != ERROR_NONE)
    printf("[set_add] error\n");

  obj = 98654LL;
  if (set_add(id, &obj) != ERROR_NONE)
    printf("[set_add] error\n");

  obj = 42LL;
  if (set_add(id, &obj) != ERROR_NONE)
    printf("[set_add] error\n");

  obj = 122LL;
  if (set_add(id, &obj) != ERROR_NONE)
    printf("[set_add] error\n");

  /*
   * remove
   */

  if (set_remove(id, 42LL) != ERROR_NONE)
    printf("error in set_remove()\n");

  /*
   * add
   */

  obj = 45LL;
  if (set_add(id, &obj) != ERROR_NONE)
    printf("[set_add] error\n");

  obj = 64LL;
  if (set_add(id, &obj) != ERROR_NONE)
    printf("[set_add] error\n");

  /*
   * display
   */

  if (set_size(id, &sz) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  printf("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPT_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
        TEST_ERROR("[set_object] error\n");

      if (!st++)
        printf("%qd",
	       *((t_id*)pdata));
      else
        printf(" %qd",
	       *((t_id*)pdata));
    }
  printf("\n");

  /*
   * add
   */

  obj = 987LL;
  if (set_add(id, &obj) != ERROR_NONE)
    printf("[set_add] error\n");

  obj = 368LL;
  if (set_add(id, &obj) != ERROR_NONE)
    printf("[set_add] error\n");

  obj = 2LL;
  if (set_add(id, &obj) != ERROR_NONE)
    printf("[set_add] error\n");

  obj = 123LL;
  if (set_add(id, &obj) != ERROR_NONE)
    printf("[set_add] error\n");

  /*
   * remove
   */

  if (set_remove(id, 368LL) != ERROR_NONE)
    printf("error in set_remove()\n");

  if (set_remove(id, 80LL) != ERROR_NONE)
    printf("error in set_remove()\n");

  /*
   * add
   */

  obj = 123LL;
  if (set_add(id, &obj) != ERROR_NONE)
    printf("[set_add] error\n");

  /*
   * display
   */

  if (set_size(id, &sz) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  printf("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPT_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
        TEST_ERROR("[set_object] error\n");

      if (!st++)
        printf("%qd",
	       *((t_id*)pdata));
      else
        printf(" %qd",
	       *((t_id*)pdata));
    }
  printf("\n");

  /*
   * release
   */

  if (set_release(id) != ERROR_NONE)
    TEST_ERROR("[set_release] error\n");

  TEST_LEAVE();
}