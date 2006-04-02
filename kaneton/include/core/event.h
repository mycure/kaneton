/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/event.h
 *
 * created       renaud voltz   [sun feb 12 22:26:04 2006]
 * updated       julien quintard   [sun apr  2 14:05:47 2006]
 */

#ifndef CORE_EVENT_H
#define CORE_EVENT_H

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/id.h>
#include <core/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * event object
 */

typedef struct
{
  t_eventid			eventid;

  e_event_type			type;

  u_event_handler		handler;

  machdep_data(o_event);
}				o_event;


/*
 * event manager
 */

typedef struct
{
  o_id				id;

  t_staid			stats;

  t_setid			events;

  machdep_data(m_event);
}				m_event;

/*
 * the event architecture dependent interface
 */

typedef struct
{
  t_error			(*event_show)(t_eventid);
  t_error			(*event_notify)(t_eventid);
  t_error			(*event_reserve)(t_eventid,
						 e_event_type,
						 u_event_handler);
  t_error			(*event_release)(t_eventid);
  t_error			(*event_init)(void);
  t_error			(*event_clean)(void);
}				d_event;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define EVENT_CHECK(_event_)						\
  {									\
    if ((_event_) == NULL)						\
      return ERROR_UNKNOWN;						\
  }

/*
 * enter
 */

#define EVENT_ENTER(_event_)						\
  {									\
    EVENT_CHECK((_event_));						\
									\
    STATS_BEGIN((_event_)->stats);					\
  }

/*
 * leave
 */

#define EVENT_LEAVE(_event_, _error_)					\
  {									\
    STATS_END((_event_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/event/event.c
 */

/*
 * ../../core/event/event.c
 */

t_error			event_show(t_eventid			eventid);

t_error			event_dump(void);

t_error			event_notify(t_eventid			eventid);

t_error			event_reserve(t_eventid			eventid,
				      e_event_type		type,
				      u_event_handler		handler);

t_error			event_release(t_eventid			eventid);

t_error			event_get(t_eventid			eventid,
				  o_event**			o);

t_error			event_init(void);

t_error			event_clean(void);

t_error			event_test(void);


/*
 * eop
 */

#endif