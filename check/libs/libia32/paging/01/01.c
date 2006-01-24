/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/libia32/paging/01/01.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:12:15 2005]
 * updated       matthieu bucchianeri   [wed jan 18 19:49:05 2006]
 */

#include <klibc.h>
#include "../common/common.h"

/*
 * checks the cr0 register.
 */

void			check_paging_01(void)
{
  register t_uint32	cr0;

  LIBIA32_TEST_ENTER;

  asm volatile("movl %%cr0, %0"
	       : "=r" (cr0));

  if (!(cr0 & 0x80000000))
    printf("bad cr0 !\n");

  LIBIA32_TEST_LEAVE;
}