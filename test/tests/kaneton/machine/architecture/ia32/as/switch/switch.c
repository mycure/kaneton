/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ine/architecture/ia32/as/switch/switch.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [thu dec  2 19:37:41 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "switch.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern t_init*		_init;
extern m_kernel*	_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_as_switch(void)
{
  i_task		task;
  i_as			as;
  int			i;
  int			j;
  i_segment		seg;
  i_region		reg;
  t_vaddr		addr;
  o_as*			o;
  t_ia32_directory	kdir;
  int*			ptr1;
  int*			ptr2;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_KERNEL,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task) != ERROR_OK)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_OK)
    TEST_ERROR("[as_reserve] error\n");

  if (as_get(as, &o) != ERROR_OK)
    TEST_ERROR("[as_get] error\n");

  if (region_reserve(as,
		     (i_segment)_init->kcode,
		     0,
		     REGION_OPTION_FORCE,
		     _init->kcode,
		     _init->kcodesz,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_reserve(as,
		     (i_segment)_init->kstack,
		     0,
		     REGION_OPTION_FORCE,
		     (t_vaddr)_init->kstack,
		     _init->kstacksz,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error: unable to map the kstack segment "
	       "at a precise address\n");

  if (segment_reserve(as,
		      PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(as,
		     seg,
		     0,
		     REGION_OPTION_NONE,
		     0,
		     PAGESZ,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (map_reserve(as,
		  MAP_OPTION_NONE,
		  PAGESZ,
		  PERMISSION_READ | PERMISSION_WRITE,
		  &addr) != ERROR_OK)
    TEST_ERROR("[map_reserve] error");

  if (as_get(_kernel->as, &o) != ERROR_OK)
    TEST_ERROR("[as_get] error\n");

  kdir = o->machine.pd;

  if (as_get(as, &o) != ERROR_OK)
    TEST_ERROR("[as_get] error\n");

  ptr1 = (int*)(t_uint32)reg;
  ptr2 = (int*)addr;

  if (ia32_pd_activate(o->machine.pd,
		       IA32_PAGE_DIRECTORY_CACHED,
		       IA32_PAGE_DIRECTORY_WRITEBACK) != ERROR_OK)
    TEST_ERROR("[ia32_pd_activate] error: unable to activate the task's "
	       "address space\n");

  *ptr1 = 0x41424344;
  i = *ptr1;
  *ptr2 = 0x40414243;
  j = *ptr2;

  if (ia32_pd_activate(kdir,
		       IA32_PAGE_DIRECTORY_CACHED,
		       IA32_PAGE_DIRECTORY_WRITEBACK) != ERROR_OK)
    TEST_ERROR("[ia32_pd_activate] error: unable to activate the kernel's "
	       "address space\n");

  if (i != 0x41424344)
    TEST_ERROR("the data written through the kernel address space is invalid "
	       "in the task's\n");

  if (j != 0x40414243)
    TEST_ERROR("the data written through the kernel address space is invalid "
	       "in the task's\n");

  TEST_SIGNATURE(vsdowkf43g3hg3g30hg);

  TEST_LEAVE();
}
