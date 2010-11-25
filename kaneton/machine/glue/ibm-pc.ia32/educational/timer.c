/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ine/glue/ibm-pc.ia32/educational/timer.c
 *
 * created       julien quintard   [mon jun 11 05:41:14 2007]
 * updated       julien quintard   [wed nov 24 23:38:36 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements dependant code for the timer managment on a ibm-pc.ia32
 * machine.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <glue/glue.h>
#include <architecture/architecture.h>
#include <platform/platform.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_timer*		_timer;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the ibm-pc.ia32 timer manager dispatch.
 */

d_timer			glue_timer_dispatch =
  {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    glue_timer_initialize,
    glue_timer_clean,
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * init the timer on the platform.
 *
 */

t_error			glue_timer_initialize(void)
{
  TIMER_ENTER(_timer);

  if (platform_pit_initialize() != ERROR_OK)
    TIMER_LEAVE(_timer, ERROR_KO);

  if (event_reserve(32,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(timer_handler),
		    0) != ERROR_OK)
    TIMER_LEAVE(_timer, ERROR_KO);

  TIMER_LEAVE(_timer, ERROR_OK);
}

/*
 * clean the timer.
 */

t_error			glue_timer_clean(void)
{
  TIMER_ENTER(_timer);

  if (platform_pit_clean() != ERROR_OK)
    TIMER_LEAVE(_timer, ERROR_KO);

  TIMER_LEAVE(_timer, ERROR_OK);
}
