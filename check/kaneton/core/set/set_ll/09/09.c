/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/core/kaneton/set/set_ll/09/09.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:43 2005]
 * updated       matthieu bucchianeri   [wed apr 12 12:20:16 2006]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../../common/common.h"

/*
 * tests with empty lists
 */

void		check_set_ll_09(void)
{
  i_set		id;
  t_iterator	it;
  t_state	state;
  t_id		obj;

  TEST_ENTER();

  if (set_reserve(ll, SET_OPT_NONE, sizeof(t_id), &id) != ERROR_NONE)
    printf("error set_reserve()\n");

  if (set_locate(id, 42LL, &it) == ERROR_NONE)
    printf("error set_locate()\n");

  if (set_remove(id, 42LL) == ERROR_NONE)
    printf("error set_remove()\n");

  set_foreach(SET_OPT_FORWARD, id, &it, state)
    {
      printf("error !\n");
    }

  obj = 42LL;
  if (set_append(id, &obj) != ERROR_NONE)
    printf("error set_insert_tail()\n");

  check_display_set(id, NULL);

  set_release(id);

  TEST_LEAVE();
}
