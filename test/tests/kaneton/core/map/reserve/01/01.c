/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...t/tests/kaneton/core/map/reserve/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 18:45:45 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_map_reserve_01(void)
{
  t_vaddr		addr;
  t_uint32		j;
  t_uint8*		p;

  TEST_ENTER();

  if (map_reserve(_kernel->as,
		  MAP_OPTION_NONE,
		  PAGESZ,
		  PERMISSION_READ | PERMISSION_WRITE,
		  &addr) != ERROR_OK)
    TEST_ERROR("[map_reserve] error");

  for (j = 0, p = (t_uint8*)addr;
       j < PAGESZ;
       p++, j++)
    {
      *p = 0x0d;

      if (*p != 0x0d)
	TEST_ERROR("the data read differs from the one read");
    }

  TEST_SIGNATURE(fjerioj32t4o330a0);

  TEST_LEAVE();
}
