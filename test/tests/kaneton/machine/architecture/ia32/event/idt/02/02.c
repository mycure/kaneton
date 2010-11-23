/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hine/architecture/ia32/event/idt/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [sun nov 21 00:08:51 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- test ------------------------------------------------------------
 */
 
void			test_ia32_event_idt_02_handler(t_id		id)
{
}

void			test_ia32_event_idt_02(void)
{
  t_ia32_idtr		idtr;
  t_ia32_idte*		idte;
  int			i;
  t_uint16		cs;

  if (ia32_gdt_build_selector(IA32_PMODE_GDT_CORE_CS,
			      ia32_prvl_supervisor,
			      &cs) != ERROR_NONE)
    TEST_ERROR("[ia32_gdt_build_selector] error\n");

  if (event_reserve(3,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_ia32_event_idt_02_handler),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  memset(&idtr, 0, sizeof (idtr));

  SIDT(idtr);

  idte = (void*)idtr.address;

  for (i = 0; i < idtr.size / 8; i++)
    {
      if (!(idte[i].type & IA32_DESC_TYPE_PRESENT))
        continue;

      if (!(idte[i].type & ((1 << 3) | (1 << 2) | (1 << 1))))
        TEST_ERROR("IDT entry %u is neither an interrupt nor a trap gate\n",
		   i);

      if (idte[i].segsel != cs)
        TEST_ERROR("IDT entry %u references an invalidt segment selector\n",
		   i);
    }

  if (event_release(3) != ERROR_NONE)
    TEST_ERROR("[event_release] error\n");

  TEST_LEAVE();
}
