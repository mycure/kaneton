/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/libs/libia32/interrupt/interrupt.c
 *
 * created       renaud voltz   [thu feb 23 10:49:43 2006]
 * updated       renaud voltz   [thu feb 23 10:49:43 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements exception and irq handling on ia32 architecture.
 * both exceptions and irq's are called interrupts in kaneton.
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

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * global interrupt handler table
 */

t_interrupt_hdl			interrupt_handler;

/*
 * ---------- functions -------------------------------------------------------
 */

static t_interrupt_pre_hdl	prehandlers[EXCEPTION_NR + IRQ_NR] =
  {
    prehandler_exception0,
    prehandler_exception1,
    prehandler_exception2,
    prehandler_exception3,
    prehandler_exception4,
    prehandler_exception5,
    prehandler_exception6,
    prehandler_exception7,
    prehandler_exception8,
    prehandler_exception9,
    prehandler_exception10,
    prehandler_exception11,
    prehandler_exception12,
    prehandler_exception13,
    prehandler_exception14,
    prehandler_exception15,
    prehandler_exception16,
    prehandler_exception17,
    prehandler_exception18,
    prehandler_exception19,
    prehandler_exception20,
    prehandler_exception21,
    prehandler_exception22,
    prehandler_exception23,
    prehandler_exception24,
    prehandler_exception25,
    prehandler_exception26,
    prehandler_exception27,
    prehandler_exception28,
    prehandler_exception29,
    prehandler_exception30,
    prehandler_exception31,
    prehandler_irq0,
    prehandler_irq1,
    prehandler_irq2,
    prehandler_irq3,
    prehandler_irq4,
    prehandler_irq5,
    prehandler_irq6,
    prehandler_irq7,
    prehandler_irq8,
    prehandler_irq9,
    prehandler_irq10,
    prehandler_irq11,
    prehandler_irq12,
    prehandler_irq13,
    prehandler_irq14,
    prehandler_irq15
  };

/*
 * add an idt entry.
 *
 * steps:
 *
 * 1) check interrupt identifier.
 * 2) set interrupt gate and add it into the idt.
 */

t_error			interrupt_add(t_uint8			nr,
				      t_prvl			privilege,
				      t_interrupt_pre_hdl	handler)
{
  t_gate		gate;

  /*
   * 1)
   */

  if (nr >= EXCEPTION_NR + IRQ_NR)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  gate.offset = (t_uint32)prehandlers[nr];
  gate.segsel = PMODE_GDT_CORE_CS << 3;
  gate.privilege = privilege;
  gate.type = type_gate_interrupt;

  idt_add_gate(NULL, nr, gate);

  return ERROR_NONE;
}

/*
 * set the interrupt generic handler.
 */

t_error			interrupt_set_handler(t_interrupt_hdl	handler)
{
  interrupt_handler = handler;

  return ERROR_NONE;
}

/*
 * init interrupts..
 *
 * steps:
 *
 * 1) set default generic handler.
 * 2) add an interrupt gate descriptor in the idt for each exception.
 * 3) add an interrupt gate descriptor in the idt for each irq.
 * 4) initialize the pic 8259A.
 */

t_error			interrupt_init(void)
{
  int			i;

  /*
   * 1)
   */

  interrupt_set_handler(interrupt_default_handler);

  /*
   * 2)
   */

  for (i = 0; i < EXCEPTION_NR; i++)
    if (interrupt_add(IDT_EXCEPTION_BASE + i, 0,
		      prehandlers[IDT_EXCEPTION_BASE + i]) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 3)
   */

  for (i = 0; i < IRQ_NR; i++)
    if (interrupt_add(IDT_IRQ_BASE + i, 0,
		      prehandlers[IDT_IRQ_BASE + i]) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 4)
   */

  pic_init();

  return ERROR_NONE;
}

/*
 * handle an interrupt.
 *
 * steps:
 *
 * for an exception:
 * 1) just call the generic handler
 *
 * for an irq:
 * 1) disable maskable interrupts.
 * 2) acknowledge the pic.
 * 3) call the generic handler.
 * 4) enable maskable interrupts.
 */

void			interrupt_wrapper(t_uint32		nr)
{
  if (nr < EXCEPTION_NR)
    {
      interrupt_handler(nr);
    }
  else
    {
      CLI();

      pic_acknowledge(nr - IDT_IRQ_BASE);

      interrupt_handler(nr);

      STI();
    }
}

/*
 * do nothing.
 */

void			interrupt_default_handler(t_uint32	nr)
{

}

/*
 * exception pre-handlers definitions
 */

EXCEPTION_PREHANDLER(0)
EXCEPTION_PREHANDLER(1)
EXCEPTION_PREHANDLER(2)
EXCEPTION_PREHANDLER(3)
EXCEPTION_PREHANDLER(4)
EXCEPTION_PREHANDLER(5)
EXCEPTION_PREHANDLER(6)
EXCEPTION_PREHANDLER(7)
EXCEPTION_PREHANDLER(8)
EXCEPTION_PREHANDLER(9)
EXCEPTION_PREHANDLER(10)
EXCEPTION_PREHANDLER(11)
EXCEPTION_PREHANDLER(12)
EXCEPTION_PREHANDLER(13)
EXCEPTION_PREHANDLER(14)
EXCEPTION_PREHANDLER(15)
EXCEPTION_PREHANDLER(16)
EXCEPTION_PREHANDLER(17)
EXCEPTION_PREHANDLER(18)
EXCEPTION_PREHANDLER(19)
EXCEPTION_PREHANDLER(20)
EXCEPTION_PREHANDLER(21)
EXCEPTION_PREHANDLER(22)
EXCEPTION_PREHANDLER(23)
EXCEPTION_PREHANDLER(24)
EXCEPTION_PREHANDLER(25)
EXCEPTION_PREHANDLER(26)
EXCEPTION_PREHANDLER(27)
EXCEPTION_PREHANDLER(28)
EXCEPTION_PREHANDLER(29)
EXCEPTION_PREHANDLER(30)
EXCEPTION_PREHANDLER(31)

/*
 * irq pre-handlers definitions
 */

IRQ_PREHANDLER(0)
IRQ_PREHANDLER(1)
IRQ_PREHANDLER(2)
IRQ_PREHANDLER(3)
IRQ_PREHANDLER(4)
IRQ_PREHANDLER(5)
IRQ_PREHANDLER(6)
IRQ_PREHANDLER(7)
IRQ_PREHANDLER(8)
IRQ_PREHANDLER(9)
IRQ_PREHANDLER(10)
IRQ_PREHANDLER(11)
IRQ_PREHANDLER(12)
IRQ_PREHANDLER(13)
IRQ_PREHANDLER(14)
IRQ_PREHANDLER(15)

