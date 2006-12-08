/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/timer.h
 *
 * created       renaud voltz   [sun feb 12 22:26:04 2006]
 * updated       matthieu bucchianeri   [fri dec  8 02:15:21 2006]
 */

#ifndef CORE_TIMER_H
#define CORE_TIMER_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/id.h>
#include <core/types.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * timer repeat mode.
 */

#define TIMER_REPEAT_DISABLE    0
#define TIMER_REPEAT_ENABLE     1

/*
 * number of millisecond between each tick.
 */

#define TIMER_MS_PER_TICK	5

/*
 * ---------- macro functions -------------------------------------------------
 */

#define TIMER_HANDLER(_function_)					\
  ((u_timer_handler)(t_timer_handler)(_function_))

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * generic timer handler type.
 */

typedef t_error                 (*t_timer_handler)(void);

/*
 * timer handler type
 */

union				u_timer_handler
{
  t_timer_handler		function;
  i_task			taskid;
};

/*
 * timer object
 */

typedef struct
{
  i_timer			timerid;

  t_uint32			delay;

  t_uint32			repeat;

  t_type			type;

  u_timer_handler		handler;

  machdep_data(o_timer);
}				o_timer;


/*
 * timer manager
 */

typedef struct
{
  o_id				id;

  i_stats			stats;

  t_uint32			timeref;

  i_set				timers;

  machdep_data(m_timer);
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
						 t_uint32,
						 t_uint32,
						 i_timer*);
  t_error			(*timer_release)(i_timer);
  t_error			(*timer_init)(void);
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
      return ERROR_UNKNOWN;						\
  }

/*
 * enter
 */

#define TIMER_ENTER(_timer_)						\
  {									\
    TIMER_CHECK((_timer_));						\
									\
    STATS_BEGIN((_timer_)->stats);					\
  }

/*
 * leave
 */

#define TIMER_LEAVE(_timer_, _error_)					\
  {									\
    STATS_END((_timer_)->stats, (_error_));				\
									\
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
				      t_uint32			delay,
				      t_uint32			repeat,
				      i_timer*			id);

t_error			timer_release(i_timer			id);

t_error			timer_delay(i_timer			id,
				    t_uint32			delay);

t_error			timer_repeat(i_timer			id,
				     t_uint32			repeat);

t_error			timer_modify(i_timer			id,
				     t_uint32			delay,
				     t_uint32			repeat);

t_error			timer_get(i_timer			id,
				  o_timer**			o);

t_error			timer_init(void);

t_error			timer_clean(void);

t_error			timer_check(void);

void			timer_handler(t_id			id);

t_error			timer_hdl1(void);

t_error			timer_hdl2(void);

t_error			timer_test(void);


/*
 * eop
 */

#endif
