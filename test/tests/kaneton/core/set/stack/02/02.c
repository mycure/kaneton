/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ABLE/test/tests/kaneton/set/stack/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [thu nov 18 05:28:45 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_set_stack_02(void)
{
  i_set			id;
  t_id			objs[64];
  t_setsz		i;
  t_iterator		it;
  t_state		state;
  t_setsz		sz;
  void*			pdata;
  int			st;

  TEST_ENTER();

  if (set_reserve(stack, SET_OPT_NONE, sizeof(t_id), &id) != ERROR_NONE)
    TEST_ERROR("[set_reserve] error\n");

  for (i = 0; i < 64; ++i)
    {
      objs[i] = (i * 234) % 6578;

      if (set_push(id, &objs[i]) != ERROR_NONE)
        TEST_ERROR("[set_push] error\n");
    }

  if (set_size(id, &sz) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  printf("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPT_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
        TEST_ERROR("[set_object] error\n");

      if (!st++)
        printf("%qd", *((i_set*)pdata));
      else
	printf(" %qd", *((i_set*)pdata));
    }
  printf("\n");

  if (set_release(id) != ERROR_NONE)
    TEST_ERROR("[set_release] error\n");

  TEST_LEAVE();
}