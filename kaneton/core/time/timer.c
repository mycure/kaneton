/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/time/timer.c
 *
 * created       renaud voltz   [sun feb 12 23:04:54 2006]
 * updated       julien quintard   [sat jul  8 02:27:47 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the timer manager.
 * a timer is parameterised around three properties: the task it belongs to,
 * its delay and its repeat mode. When its delay (in 1/100 of sec) has expired,
 * a message is sent to the task taskid before behaving as follow:
 * a) if the timer repeat is set to TIMER_REPEAT_ENABLE, the timer is
 *    automaticaly reset to its original delay.
 * b) if the timer repeat is set to TIMER_REPEAT_DISABLE, then the timer is
 *    released.
 */

/*								    [cut] k3 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

machdep_include(timer);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the timer manager variable.
 */

m_timer*		timer = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows a given timer.
 *
 * steps:
 *
 * 1) get the timer object from its identifier.
 * 2) display its delay.
 */

t_error			timer_show(i_timer			id)
{
  o_timer*		o;

  TIMER_ENTER(timer);

  /*
   * 1)
   */

  if (timer_get(id, &o) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "  timer %qd: delay = %u\n",
	   o->timerid, o->delay - timer->timeref);

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * this function dumps the timers managed by the kernel.
 *
 * steps:
 *
 * 1) get the size of the timer set.
 * 2) display information about every timer.
 */

t_error			timer_dump(void)
{
  t_state		state;
  o_timer*		data;
  t_setsz		size;
  t_iterator		i;

  TIMER_ENTER(timer);

  /*
   * 1)
   */

  if (set_size(timer->timers, &size) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  cons_msg('#', "dumping %qd timer(s):\n", size);

  /*
   *  2)
   */

  set_foreach(SET_OPT_FORWARD, timer->timers, &i, state)
    {
      if (set_object(timer->timers, i, (void**)&data) != ERROR_NONE)
	TIMER_LEAVE(timer, ERROR_UNKNOWN);

      if (timer_show(data->timerid) != ERROR_NONE)
	TIMER_LEAVE(timer, ERROR_UNKNOWN);
    }

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * notify a task that one of its timers expired.
 *
 * steps:
 *
 * 1) get the timer object from the set.
 * 2) call the machine-dependent code.
 * 3) notify the task for its timer expiration.
 */

t_error			timer_notify(i_timer			id)
{
  o_timer*		o;

  TIMER_ENTER(timer);

  /*
   * 1)
   */

  if (timer_get(id, &o) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (machdep_call(timer, timer_notify, id) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 3)
   */

  /* XXX TIMER send a message to o->taskid */
  printf("timer_notify: id = %qd, taskid = %qd\n", id, o->handler.taskid);

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * insert a timer in the timer set.
 *
 * steps:
 *
 * 1) insert the timer before the first which has a greater expiration date.
 * 2) if there is no timer or every timer expire sooner, insert it at the end.
 */

t_error			timer_insert(o_timer*			o)
{
  t_state		state;
  o_timer*		o_tmp;
  t_iterator		i;

  TIMER_ENTER(timer);

  /*
   * 1)
   */

  set_foreach(SET_OPT_FORWARD, timer->timers, &i, state)
    {
      if (set_object(timer->timers, i, (void**)&o_tmp) != ERROR_NONE)
	TIMER_LEAVE(timer, ERROR_UNKNOWN);

      if (o_tmp->delay >= o->delay)
	{
	  if (set_before(timer->timers, i, o) != ERROR_NONE)
	    TIMER_LEAVE(timer, ERROR_UNKNOWN);

	  TIMER_LEAVE(timer, ERROR_NONE);
	}
    }

  /*
   * 2)
   */

  if (set_append(timer->timers, o) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * reserve a timer.
 *
 * steps:
 *
 * 1) call the machine-dependent code.
 * 2) create a new timer object and give it an identifier.
 * 3) setup delay and repeat mode.
 * 4) add the new timer to the timer manager.
 */

t_error			timer_reserve(t_type			type,
				      u_timer_handler		handler,
				      t_uint32			delay,
				      t_uint32			repeat,
				      i_timer*			id)
{
  o_timer		o;

  TIMER_ENTER(timer);

  /*
   * 1)
   */

  if (machdep_call(timer, timer_reserve, type, handler, delay, repeat, id)
      != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (id_reserve(&timer->id, id) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  memset(&o, 0x0, sizeof(o_timer));

  o.timerid = *id;
  o.type = type;
  o.handler = handler;

  /*
   * 3)
   */

  if (repeat)
    o.repeat = delay;

  o.delay = timer->timeref + delay;

  /*
   * 4)
   */

  if (timer_insert(&o) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * this function releases a timer.
 *
 * steps:
 *
 * 1) check that the timer exists.
 * 2) call the machine dependant code.
 * 3) destroy the identifier object.
 * 4) remove the timer object from the timer set.
 */

t_error			timer_release(i_timer			id)
{
  o_timer*		o;

  TIMER_ENTER(timer);

  /*
   * 1)
   */

  if (timer_get(id, &o) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (machdep_call(timer, timer_release, id) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (id_release(&timer->id, id) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (set_remove(timer->timers, id) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * set the timer delay.
 *
 * steps:
 *
 * 1) get the timer object.
 * 2) set the delay.
 # 3) call the machine-dependent code.
 */

t_error			timer_delay(i_timer			id,
				    t_uint32			delay)
{
  o_timer*		o;

  TIMER_ENTER(timer);

  /*
   * 1)
   */

  if (timer_get(id, &o) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 2)
   */

  o->delay = delay;

  /*
   * 3)
   */

  if (machdep_call(timer, timer_delay, id, delay) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * set the timer repeat mode.
 *
 * steps:
 *
 * 1) get the timer object.
 * 2) set the repeat mode.
 * 3) call the machine-dependent code.
 */

t_error			timer_repeat(i_timer			id,
				     t_uint32			repeat)
{
  o_timer*		o;

  TIMER_ENTER(timer);

  /*
   * 1)
   */

  if (timer_get(id, &o) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 2)
   */

  o->repeat = repeat;

  /*
   * 3)
   */

  if (machdep_call(timer, timer_repeat, id, repeat) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * modify a timer attributes.
 *
 * steps:
 *
 * 1) check if the timer exists and get it.
 * 2) call the machdep code.
 * 3) clone the timer and update its delay and repeat mode.
 * 4) reorganize the timer position within the set.
 */

t_error			timer_modify(i_timer			id,
				     t_uint32			delay,
				     t_uint32			repeat)
{
  o_timer		o;
  o_timer*		old;

  TIMER_ENTER(timer);

  /*
   * 1)
   */

  if (timer_get(id, &old) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (machdep_call(timer, timer_modify, id, delay, repeat) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /*
   * 3)
   */

  memcpy(&o, old, sizeof(o_timer));

  if (repeat)
    o.repeat = delay;

  o.delay = timer->timeref + delay;

  /*
   * 4)
   */

  if (timer_release(id) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  if (timer_insert(&o) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  TIMER_LEAVE(timer, ERROR_NONE);
}



/*
 * this function finds a timer object in the timer set.
 */

t_error			timer_get(i_timer			id,
				  o_timer**			o)
{
  TIMER_ENTER(timer);

  if (set_get(timer->timers, id, (void**)o) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*
 * initialize the timer manager.
 *
 * steps:
 *
 * 1) allocate and initialize the timer manager.
 * 2) initialize the object identifier.
 * 3) reserve the timer set.
 * 4) try to reserve a statistic object.
 * 5) call the machine dependent code.
 * 6) dump the timer manager if debug is enabled.
 */

t_error			timer_init(void)
{

  /*
   * 1)
   */

  if ((timer = malloc(sizeof(m_timer))) == NULL)
    {
      cons_msg('!', "timer: cannot allocate memory for the timer manager "
               "structure\n");

      return ERROR_UNKNOWN;
    }

  memset(timer, 0x0, sizeof(m_timer));

  /*
   * 2)
   */

  if (id_build(&timer->id) != ERROR_NONE)
    {
      cons_msg('!', "timer: unable to initialize the identifier object\n");

      return ERROR_UNKNOWN;
    }

  timer->timeref = 0;

  /*
   * 3)
   */

  if (set_reserve(ll, SET_OPT_ALLOC,
                  sizeof(o_timer), &timer->timers) != ERROR_NONE)
    {
      cons_msg('!', "timer: unable to reserve the timer set\n");

      return ERROR_UNKNOWN;
    }

  /*
   * 4)
   */

  STATS_RESERVE("timer", &timer->stats);

  /*
   * 5)
   */

  if (machdep_call(timer, timer_init) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 6)
   */

#if (DEBUG & DEBUG_TIMER)
  if (timer_dump() != ERROR_UNKNOWN)
    return ERROR_UNKNOWN;
#endif

  return ERROR_NONE;
}

/*
 * destroy the timer manager.
 *
 * steps:
 *
 * 1) call the machine dependent code.
 * 2) release the statistics object.
 * 3) release the timer set.
 * 4) destroy the identifier object.
 * 5) free the timer manager's structure memory.
 */

t_error			timer_clean(void)
{
  /*
   * 1)
   */

  if (machdep_call(timer, timer_clean) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  STATS_RELEASE(timer->stats);

  /*
   * 3)
   */

  if (set_release(timer->timers) != ERROR_NONE)
    {
      cons_msg('!', "timer: unable to release the timer set\n");

      return ERROR_UNKNOWN;
    }

  /*
   * 4)
   */

  if (id_destroy(&timer->id) != ERROR_NONE)
    {
      cons_msg('!', "timer: unable to destroy the identifier object\n");

      return ERROR_UNKNOWN;
    }

  /*
   * 5)
   */

  free(timer);

  return ERROR_NONE;
}

/*
 * check timer expiration.
 *
 * steps:
 *
 * 1) get the timer with the closest expiration date.
 * 2) return if this timer has not expired yet.
 * 3) otherwise, notify the task its timer has expired.
 * 4) release the timer, or re-initialize it if repeat mode is enabled.
 */

t_error			timer_check(void)
{
  o_timer*		o;
  t_iterator		i;

  TIMER_ENTER(timer);

  while (set_head(timer->timers, &i) == ERROR_NONE)
    {

      /*
       * 1)
       */

      if (set_object(timer->timers, i, (void**)&o) != ERROR_NONE)
        TIMER_LEAVE(timer, ERROR_UNKNOWN);

      /*
       * 2)
       */

      if (timer->timeref < o->delay)
        break;

      /*
       * 3)
       */

      if (o->type == EVENT_MESSAGE)
	{
	  if (timer_notify(o->timerid) != ERROR_NONE)
	    TIMER_LEAVE(timer, ERROR_UNKNOWN);
	}
      else
	{
	  o->handler.function();
	}

      /*
       * 4)
       */

      if (o->repeat)
        {
	  if (timer_modify(o->timerid, o->repeat, TIMER_REPEAT_ENABLE)
	      != ERROR_NONE)
            TIMER_LEAVE(timer, ERROR_UNKNOWN);
        }
      else
        {
          if (timer_release(o->timerid) != ERROR_NONE)
            TIMER_LEAVE(timer, ERROR_UNKNOWN);
        }
    }

  TIMER_LEAVE(timer, ERROR_NONE);
}

void			timer_handler(t_uint32			id)
{
  /*
   *
   */

  timer->timeref++;

  /*
   *
   */

  timer_check();
}


/*
 * ---------- testing ---------------------------------------------------------
 */

/*
 * XXX TIMER: remove !
 * timer handler 1
 */

t_error			timer_hdl1(void)
{
  printf("timer handler 1\n");

  return ERROR_NONE;
}

/*
 * XXX TIMER: remove !
 * timer handler 2
 */

t_error			timer_hdl2(void)
{
  printf("timer handler 2\n");

  return ERROR_NONE;
}

/*
 * XXX TIMER: remove me !
 * just for testing
 */

t_error			timer_test(void)
{
  i_timer		id;

  if (timer_reserve(EVENT_FUNCTION,
		    (u_timer_handler)((t_timer_handler)timer_hdl1), 500,
		    TIMER_REPEAT_ENABLE, &id) != ERROR_NONE)
    {
      printf("error: reserving a new timer !\n");
      return ERROR_UNKNOWN;
    }

  if (timer_reserve(EVENT_MESSAGE,
		    (u_timer_handler)((i_task)42), 100,
		    TIMER_REPEAT_ENABLE, &id) != ERROR_NONE)
    {
      printf("error: reserving a new timer !\n");
      return ERROR_UNKNOWN;
    }

  if (timer_dump() != ERROR_NONE)
    {
      printf("error: dumping timers set !/n");
      return ERROR_UNKNOWN;
    }

  return ERROR_NONE;
}

/*								   [cut] /k3 */
