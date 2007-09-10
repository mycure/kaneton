/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/region/01/01.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [sun apr  9 17:41:04 2006]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

static volatile t_uint32 timed = 0;

static void	check_timer_handler(void)
{
  printf("Timer shot\n");
  timed = 1;
}

/*
 * XXX
 */

void		check_time_release_01(void)
{
  i_timer	id;
  t_uint32	start;

  TEST_ENTER();

  ASSERT(timer_reserve(TIMER_FUNCTION, TIMER_HANDLER(check_timer_handler), 0,
		       1000, TIMER_REPEAT_DISABLE, &id) == ERROR_NONE,
	 "Cannot timer_reserve\n");

  ASSERT(timer_release(id) == ERROR_NONE,
	 "Cannot timer_release\n");

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec())
    ;

  ASSERT(timed == 0, "Timer was not cancelled\n");

  TEST_LEAVE();
}
