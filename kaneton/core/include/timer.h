/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/kaneton/core/include/timer.h
 *
 * created       julien quintard   [wed jun  6 15:42:26 2007]
 * updated       julien quintard   [thu nov 25 11:02:21 2010]
 */

#ifndef CORE_TIMER_H
#define CORE_TIMER_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>

#include <machine/machine.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * timer handling way
 */

#define TIMER_FUNCTION		0
#define TIMER_MESSAGE		1

/*
 * timer options.
 */

#define TIMER_OPTION_NONE	0
#define TIMER_OPTION_REPEAT	1

/*
 * number of millisecond between each tick.
 */

#define TIMER_DELAY		25

/*
 * ---------- macro functions -------------------------------------------------
 */

#define TIMER_HANDLER(_function_)					\
  ((u_timer_handler)(t_timer_handler)(_function_))

#define TIMER_TASK(_task_)						\
  ((u_timer_handler)(i_task)(_task_))

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * generic timer handler type
 */

typedef t_error			(*t_timer_handler)(i_timer, t_vaddr);

/*
 * timer handler type
 */

typedef union
{
  t_timer_handler		function;
  i_task			task;
}				u_timer_handler;

/*
 * timer object
 */

typedef struct
{
  i_timer			id;

  t_uint64			delay;

  t_options			options;
  t_uint64			repeat;

  t_type			type;

  u_timer_handler		handler;
  t_vaddr			data;

  machine_data(o_timer);
}				o_timer;

/*
 * message object for event
 */

typedef struct
{
  i_timer			id;

  t_vaddr			data;
}				o_timer_message;

/*
 * timer manager
 */

typedef struct
{
  o_id				id;

  t_uint64			reference;

  i_set				timers;

  machine_data(m_timer);
}				m_timer;

/*
 * the timer architecture dependent interface
 */

typedef struct
{
  t_error			(*timer_show)(i_timer);
  t_error			(*timer_delay)(i_timer,
					       t_uint32);
  t_error			(*timer_repeat)(i_timer,
						t_uint32);
  t_error			(*timer_modify)(i_timer,
						t_uint32,
						t_uint32);
  t_error			(*timer_notify)(i_timer);
  t_error			(*timer_reserve)(t_type,
						 u_timer_handler,
						 t_vaddr,
						 t_uint32,
						 t_uint32,
						 i_timer*);
  t_error			(*timer_release)(i_timer);
  t_error			(*timer_initialize)(void);
  t_error			(*timer_clean)(void);
}				d_timer;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define TIMER_CHECK(_timer_)						\
  {									\
    if ((_timer_) == NULL)						\
      return ERROR_KO;						\
  }

/*
 * enter
 */

#define TIMER_ENTER(_timer_)						\
  {									\
    TIMER_CHECK((_timer_));						\
  }

/*
 * leave
 */

#define TIMER_LEAVE(_timer_, _error_)					\
  {									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/time/timer.c
 */

/*
 * ../../core/time/timer.c
 */

t_error			timer_show(i_timer			id);

t_error			timer_dump(void);

t_error			timer_notify(i_timer			id);

t_error			timer_insert(o_timer*			o);

t_error			timer_reserve(t_type			type,
				      u_timer_handler		handler,
				      t_vaddr			data,
				      t_uint32			delay,
				      t_options			options,
				      i_timer*			id);

t_error			timer_release(i_timer			id);

t_error			timer_delay(i_timer			id,
				    t_uint32			delay);

t_error			timer_repeat(i_timer			id,
				     t_uint64			repeat);

t_error			timer_modify(i_timer			id,
				     t_uint64			delay,
				     t_options			options);

t_error			timer_get(i_timer			id,
				  o_timer**			o);

t_error			timer_initialize(void);

t_error			timer_clean(void);

t_error			timer_check(void);

void			timer_handler(t_id			id);


/*
 * eop
 */

#endif
