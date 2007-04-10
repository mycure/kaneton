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

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

static void	check_irq(t_id	id)
{
}

/*
 * XXX
 */

void		check_event_pic_02(void)
{
  t_uint8	mask;

  TEST_ENTER();

  ASSERT(event_reserve(32 + 9,
		       EVENT_FUNCTION,
		       EVENT_HANDLER(check_irq)) == ERROR_NONE,
	 "cannot event_reserve\n");

  printf(" - Checking Master PIC\n");
  INB(MASTER_PORT_B, mask);
  ASSERT(((mask & (1 << 2))) == 0,
	 " x Bad PIC mask\n");
  printf(" - Checking Slave PIC\n");
  INB(SLAVE_PORT_B, mask);
  ASSERT(mask == 0xfd,
	 " x Bad PIC mask\n");

  TEST_LEAVE();
}