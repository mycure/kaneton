/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ton/machine/glue/ibm-pc.ia32/time/time.c
 *
 * created       julien quintard   [thu nov 18 21:50:35 2010]
 * updated       julien quintard   [sat nov 20 23:15:59 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "time.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

static t_uint64		test_ibmpc_ia32_time_ticks;

/*
 * ---------- functions -------------------------------------------------------
 */

t_uint8			test_ibmpc_ia32_time_cmos(void)
{
  unsigned char		c;
  unsigned char		r;

  OUTB(0x70, 0);
  INB(0x71, c);

  r = 10 * ((c & 0xf0) >> 4) + (c & 0x0f);

  return r;
}

t_uint64		test_ibmpc_ia32_time_rtc(void)
{
  return (test_ibmpc_ia32_time_ticks / 100);
}

unsigned char		test_ibmpc_ia32_time_read(unsigned char	reg)
{
  t_uint8		val;

  OUTB(0x70, reg);
  INB(0x71, val);

  return (val);
}

void			test_ibmpc_ia32_time_write(unsigned char	reg,
						    unsigned char	val)
{
  OUTB(0x70, reg);
  OUTB(0x71, val);
}

void			test_ibmpc_ia32_time_rtc_handler(void)
{
  t_uint8		mask;

  test_ibmpc_ia32_time_ticks += 3125;

  asm volatile ("movb $0xc, %al\n\t"
                "outb %al,$0x70\n\t"
                "outb %al,$0x80\n\t"
                "inb  $0x71,%al\n\t");

  INB(TEST_SLAVE_PORT_B, mask);
  OUTB(TEST_SLAVE_PORT_B, mask);
  OUTB(TEST_SLAVE_PORT_A, 0x60 + (8 & 0x7));
  OUTB(TEST_MASTER_PORT_A, 0x60 + 2);
}

void			test_ibmpc_ia32_time_rtc_setup(void)
{
  t_uint8		mask;

  if (event_reserve(32 + 8,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_ibmpc_ia32_time_rtc_handler),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  /* enable IRQ 8 */
  INB(TEST_SLAVE_PORT_B, mask);
  OUTB(TEST_SLAVE_PORT_B, mask & ~(1 << 0));

  while (test_ibmpc_ia32_time_read(TEST_RTC_STATUSA) & TEST_RTCSA_TUP)
    ; /* wait until is RTC ready */

  test_ibmpc_ia32_time_ticks = 0;

  /* set divider to 1024 Hz */
  test_ibmpc_ia32_time_write(TEST_RTC_STATUSA,
			      TEST_RTCSA_DIVIDER | TEST_RTCSA_32);

  /* set up interrupt */
  test_ibmpc_ia32_time_write(TEST_RTC_STATUSB,
			      test_ibmpc_ia32_time_read(TEST_RTC_STATUSB) |
			      TEST_RTCSB_PINTR |
			      TEST_RTCSB_SQWE);

  /* reset */
  test_ibmpc_ia32_time_read(TEST_RTC_INTR);
}
