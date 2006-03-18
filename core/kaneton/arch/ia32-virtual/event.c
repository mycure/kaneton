/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/core/kaneton/arch/ia32-virtual/event.c
 *
 * created       renaud voltz   [mon feb 13 01:05:52 2006]
 * updated       renaud voltz   [mon feb 13 01:05:52 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements dependant code for the events on ia32 with paging
 * architecture.
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * XXX EVENT
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*								[cut] k3 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the event manager interface.
 */

i_event				event_interface =
  {

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

t_error			ia32_event_reserve(t_eventid		eventid,
					   e_event_type		type,
					   u_event_handler	handler)
{
  t_uint32		id = eventid;

  /*
   * 1)
   */

  if (id >= EXCEPTION_NR + IRQ_NR)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if (type == EVENT_MESSAGE)
    {
      if (interrupt_set_handler(id, (t_event_handler)event_notify)
	  != ERROR_NONE)
	return ERROR_UNKNOWN;
    }
  else
    {
      if (interrupt_set_handler(id, handler.function) != ERROR_NONE)
	return ERROR_UNKNOWN;
    }

  /*
   * 3)
   */

  if ((id >= IDT_IRQ_BASE) && (id < IDT_IRQ_BASE + IRQ_NR))
    if (pic_enable_irq(id - IDT_IRQ_BASE) != ERROR_NONE)
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

t_error			ia32_event_release(t_eventid		eventid)
{
  t_uint32		id = eventid;

  /*
   * 1)
   */

  if (id >= EXCEPTION_NR + IRQ_NR)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if (interrupt_set_handler(id, ia32_event_handler) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  if ((id >= IDT_IRQ_BASE) && (id < IDT_IRQ_BASE + IRQ_NR))
    if (pic_disable_irq(id - IDT_IRQ_BASE) != ERROR_NONE)
      return ERROR_UNKNOWN;

  return ERROR_NONE;
}

/*
 * initialize events on the ia32 architecture.
 *
 * steps:
 *
 * 1) init events.
 * 2) set default handler for every exception.
 * 3) set default handler for every irq.
 * 4) XXX
 */

t_error			ia32_event_init(void)
{
  int			id;

  /*
   * 1)
   */

  if (interrupt_init() != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  for (id = IDT_EXCEPTION_BASE; id < IDT_EXCEPTION_BASE + EXCEPTION_NR; id++)
    if (interrupt_set_handler(id, ia32_event_handler) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 3)
   */

  for (id = IDT_IRQ_BASE; id < IDT_IRQ_BASE + IRQ_NR; id++)
    if (interrupt_set_handler(id, ia32_event_handler) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 4)
   */

  STI();

  return ERROR_NONE;
}

/*
 * this function cleans events on ia32 architecture.
 *
 * 1) XXX
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

/*								[cut] /k3 */
