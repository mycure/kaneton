/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...E/test/tests/kaneton/core/set/ll/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [mon nov 29 19:20:48 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_ll_02(void)
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

  if (set_reserve(ll, SET_OPTION_NONE, sizeof(t_id), &id) != ERROR_OK)
    TEST_ERROR("[set_reserve] error");

  for (i = 0; i < 64; ++i)
    {
      objs[i] = (i * 234) % 6578;

      if (set_add(id, &objs[i]) != ERROR_OK)
        TEST_ERROR("[set_add] error");
    }

  if (set_size(id, &sz) != ERROR_OK)
    TEST_ERROR("[set_size] error");

  printf("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPTION_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_OK)
        TEST_ERROR("[set_object] error");

      if (!st++)
        printf("%qd", *((i_set*)pdata));
      else
	printf(" %qd", *((i_set*)pdata));
    }
  printf("\n");

  if (set_release(id) != ERROR_OK)
    TEST_ERROR("[set_release] error");

  TEST_SIGNATURE(svdkl34g3h34h0);

  TEST_LEAVE();
}
