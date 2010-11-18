/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...st/tests/kaneton/segment/reserve/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed nov 17 15:39:29 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_as		kasid;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_reserve_02(void)
{
  i_segment		seg;
  o_segment*		o;

  TEST_ENTER();

  if (segment_reserve(kasid,
		      12 * PAGESZ,
		      PERM_READ,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_get(seg, &o) != ERROR_NONE)
    TEST_ERROR("[segment_get] error\n");

  if (o->segid != seg)
    TEST_ERROR("invalid segment's identifier\n");

  if (o->asid != kasid)
    TEST_ERROR("invalid segment's address space identifier\n");

  if (o->size != (12 * PAGESZ))
    TEST_ERROR("invalid segment's size\n");

  if (o->perms != PERM_READ)
    TEST_ERROR("invalid segment's permissions\n");

  TEST_LEAVE();
}