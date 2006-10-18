/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/core/arch/sgi-octane.mips64/event.c
 *
 * created       Enguerrand RAYMOND   [tue oct 17 13:38:14 2006]
 * updated       Enguerrand RAYMOND   [tue oct 17 22:41:12 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements dependent code for the events on octane with paging
 * architecture.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <liboctane.h>
#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ---------------------------------------------------------
 */

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
    NULL,
    NULL,
    NULL,
    NULL,
  };

/*
 * ---------- functions -------------------------------------------------------
 */
