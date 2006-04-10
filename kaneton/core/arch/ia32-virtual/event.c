/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ia32-virtual/event.c
 *
 * created       renaud voltz   [mon feb 13 01:05:52 2006]
 * updated       matthieu bucchianeri   [tue apr  4 19:06:26 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements dependent code for the events on ia32 with paging
 * architecture.
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * XXX EVENT assignments have to be written.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*								    [cut] k3 */

/*
 * ---------- extern ---------------------------------------------------------
 */

/*
 * the kernel address space.
 */

extern t_asid			kasid;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the event manager dispatch.
 */

d_event				event_dispatch =
  {
    NULL,
    NULL,
    ia32_event_reserve,
    ia32_event_release,
    ia32_event_init,
    ia32_event_clean
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * reserve an event on the ia32 architecture.
 *
 * steps:
 *
 * 1) check id bounds.
 * 2) associate the handler to the given eventid.
 * 3) unmask event if eventid is an irq.
 */

t_error			ia32_event_reserve(i_event		id,
					   t_uint32		type,
					   u_event_handler	handler)
{
  t_uint32		eventid = id;

  /*
   * 1)
   */

  if (eventid >= EXCEPTION_NR + IRQ_NR)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if (type == EVENT_MESSAGE)
    {
      if (interrupt_set_handler(eventid, (t_event_handler)event_notify)
	  != ERROR_NONE)
	return ERROR_UNKNOWN;
    }
  else
    {
      if (interrupt_set_handler(eventid, handler.function) != ERROR_NONE)
	return ERROR_UNKNOWN;
    }

  /*
   * 3)
   */

  if ((eventid >= IDT_IRQ_BASE) && (eventid < IDT_IRQ_BASE + IRQ_NR))
    if (pic_enable_irq(eventid - IDT_IRQ_BASE) != ERROR_NONE)
      return ERROR_UNKNOWN;

  return ERROR_NONE;
}

/*
 * release an event on the ia32 architecture.
 *
 * steps:
 *
 * 1) check id bounds.
 * 2) clear the eventid handler.
 * 3) mask event if eventid is an irq.
 */

t_error			ia32_event_release(i_event		id)
{
  t_uint32		eventid = id;

  /*
   * 1)
   */

  if (eventid >= EXCEPTION_NR + IRQ_NR)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if (interrupt_set_handler(eventid, ia32_event_handler) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  if ((eventid >= IDT_IRQ_BASE) && (eventid < IDT_IRQ_BASE + IRQ_NR))
    if (pic_disable_irq(eventid - IDT_IRQ_BASE) != ERROR_NONE)
      return ERROR_UNKNOWN;

  return ERROR_NONE;
}

/*
 * initialize events on the ia32 architecture.
 *
 * steps:
 *
 * 1) allocate space for the idt.
 * 2( build the idt and activate it.
 * 3) init events.
 * 4) set default handler for every exception.
 * 5) set default handler for every irq.
 * 6) enable external interrupts.
 */

t_error			ia32_event_init(void)
{
  int			id;
  t_vaddr		vaddr;
  t_ia32_idt		new_idt;

  /*
   * 1)
   */

  if (map_reserve(kasid, MAP_OPT_NONE,
		  PAGESZ,
		  PERM_READ | PERM_WRITE,
		  &vaddr)
      != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if (idt_build(IDT_MAX_ENTRIES, vaddr, 1, &new_idt) != ERROR_NONE)
    return ERROR_UNKNOWN;

  if (idt_activate(&new_idt) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  if (interrupt_init() != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 4)
   */

  for (id = IDT_EXCEPTION_BASE; id < IDT_EXCEPTION_BASE + EXCEPTION_NR; id++)
    if (interrupt_set_handler(id, ia32_event_handler) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 5)
   */

  for (id = IDT_IRQ_BASE; id < IDT_IRQ_BASE + IRQ_NR; id++)
    if (interrupt_set_handler(id, ia32_event_handler) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 6)
   */

  STI();

  return ERROR_NONE;
}

/*
 * this function cleans events on ia32 architecture.
 *
 * 1) ignore external interrupts.
 * 2) disable exceptions.
 * 3) disable irq's.
 */

t_error			ia32_event_clean(void)
{
  int			id;

  /*
   * 1)
   */

  CLI();

  /*
   * 2)
   */

  for (id = IDT_EXCEPTION_BASE; id < IDT_EXCEPTION_BASE + EXCEPTION_NR; id++)
    if (event_release(id) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 3)
   */

  for (id = IDT_IRQ_BASE; id < IDT_IRQ_BASE + IRQ_NR; id++)
    if (event_release(id) != ERROR_NONE)
      return ERROR_UNKNOWN;

  return ERROR_NONE;
}


/*
 * the default handler for ia32 architecture.
 *
 * do nothing.
 * used before the event has been reserved,
 * or after the event has been released.
 */

void			ia32_event_handler(t_uint32		id)
{


}

/*
 * XXX EVENT remove me !
 * just for testing.
 */

static const char       scancodes[] =
  {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', 0, 0, 'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l' , ';',
    '\'', '`', '-', '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', 0, 0, 0, ' ', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };


void                    ia32_kbd_handler(t_uint32		id)
{
  t_uint8               scancode;

  INB(0x60, scancode);

  if (scancode < 70)
    printf("%c", scancodes[scancode]);
}

void                    ia32_pf_handler(t_uint32		error_code)
{
  t_uint32              addr;

  SCR2(addr);

  printf("error: page fault !\n"
         "  trying to %s at the address 0x%x requires %s\n",
         (error_code & 2) ? "write" : "read",
         addr,
         (error_code & 1) ? "a lower DPL" : "the page to be present");

  t_ia32_directory pd;
  t_ia32_table pt;
  t_ia32_page pg;

  pd = ENTRY_ADDR(PD_MIRROR, PD_MIRROR);

  if (pd_get_table(&pd, PDE_ENTRY(addr), &pt) != ERROR_NONE)
    printf("no pde\n");
  else
    {
      printf("pde: %p\n", pt.entries);
      pt.entries = ENTRY_ADDR(PD_MIRROR, PDE_ENTRY(addr));

      if (pt_get_page(&pt, PTE_ENTRY(addr), &pg) != ERROR_NONE)
	printf("no pte\n");
      else
	{
	  printf("pte: %p\n", pg.addr);
	}
    }

  while (1);
}

/*								[cut] /k3 */
