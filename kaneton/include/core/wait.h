/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/wait.h
 *
 * created       julien quintard   [wed apr 12 05:17:42 2006]
 * updated       matthieu bucchianeri   [wed apr 12 11:53:36 2006]
 */

#ifndef CORE_WAIT_H
#define CORE_WAIT_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/* XXX */

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * wait type
 */

typedef struct
{
  union
  {
    i_task			task;
    t_thrid			thread;
  }				id;

  t_status			status;

  t_uint32			error;
}				t_wait;

/*
 * ---------- macro functions -------------------------------------------------
 */

/* XXX */

/*
  WAIT_TASK() // task_wait
  WAIT_THREAD() // thread_wait
  WAIT_CAUSE() // death, start, stop
  WAIT_STATUS() // if death, return value
 */

#endif