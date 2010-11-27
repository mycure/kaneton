/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/event/event.c
 *
 * created       renaud voltz   [sun feb 12 23:04:54 2006]
 * updated       matthieu bucchianeri   [tue feb  6 22:26:00 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the event manager.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

machine_include(event);

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the kernel manager.
 */

extern m_kernel*	_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the event manager variable.
 */

m_event*		_event = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows a given event.
 *
 * steps:
 *
 * 1) get the event object from its identifier.
 * 2) display the tasks which have subscribed to this event.
 * 3) call the machine0dependent code.
 */

t_error			event_show(i_event			id)
{
  o_event*		o;

  /*
   * 1)
   */

  if (event_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the event object");

  /*
   * 2)
   */

  module_call(console, console_message,
	      '#', "  event %qd: task %qd\n", o->id, o->handler.task);

  /*
   * 3)
   */

  if (machine_call(event, event_show, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function dumps the events.
 *
 * steps:
 *
 * 1) get the set size.
 * 2) show each event.
 */

t_error			event_dump(void)
{
  t_state		state;
  o_event*		data;
  t_setsz		size;
  t_iterator		i;

  /*
   * 1)
   */

  if (set_size(_event->events, &size) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of events");

  /*
   * 2)
   */

  module_call(console, console_message,
	      '#', "dumping %qu event(s):\n", size);

  set_foreach(SET_OPTION_FORWARD, _event->events, &i, state)
    {
      if (set_object(_event->events, i, (void**)&data) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the object from the set of events");

      if (event_show(data->id) != ERROR_OK)
	CORE_ESCAPE("unable to show the object");
    }

  CORE_LEAVE();
}

/*
 * notify a task that an event occured.
 *
 * steps:
 *
 * 1) get the event object from the set.
 * 2) notify the task its event has occured.
 * 3) call machine.
 */

t_error			event_notify(i_event			id)
{
  o_event*              o;
  o_event_message	msg;
  i_node		node;

  /*
   * 1)
   */

  if (event_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the event object");

  /*
   * 2)
   */

  msg.id = id;
  msg.data = o->data;

  node.machine = _kernel->machine;
  node.task = o->handler.task;

  if (message_send(_kernel->task, node, MESSAGE_TYPE_EVENT, (t_vaddr)&msg,
		   sizeof (o_event_message)) != ERROR_OK)
    CORE_ESCAPE("unable to send a message to the destination task");

  /*
   * 3)
   */

  if (machine_call(event, event_notify, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function activates the event processing.
 *
 * steps:
 *
 * 1) call the machine.
 */

t_error			event_enable(void)
{
  /*
   * 1)
   */

  if (machine_call(event, event_enable) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function deactivates the event processing.
 *
 * steps:
 *
 * 1) call the machine.
 */

t_error			event_disable(void)
{
  /*
   * 1)
   */

  if (machine_call(event, event_disable) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * reserve an event.
 *
 * steps:
 *
 * 1) check that the event has not been already reserved.
 * 2) create a new event object and give it its arch dependent id.
 * 3) add the new event to the event manager.
 * 4) call the machine dependent code.
 */

t_error			event_reserve(i_event			id,
				      t_type			type,
				      u_event_handler		handler,
				      t_vaddr			data)
{
  o_event*		tmp;
  o_event		o;

  assert(type == EVENT_FUNCTION || type == EVENT_MESSAGE);

  /*
   * 1)
   */

  if (event_exist(id) == ERROR_TRUE)
    CORE_ESCAPE("unable to retrieve the event object");

  /*
   * 2)
   */

  memset(&o, 0x0, sizeof(o_event));

  o.id = id;
  o.type = type;
  o.handler = handler;
  o.data = data;

  /*
   * 3)
   */

  if (set_add(_event->events, &o) != ERROR_OK)
    CORE_ESCAPE("unable to add the object to the set of events");

  /*
   * 4)
   */

  if (machine_call(event, event_reserve, id, type, handler,
		   data) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function releases an event.
 *
 * steps:
 *
 * 1) get the event object from its identifier.
 * 2) remove the object from the event set.
 * 3) call the machine dependent code.
 */

t_error			event_release(i_event			id)
{
  o_event*		o;

  /*
   * 1)
   */

  if (event_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the event object");

  /*
   * 2)
   */

  if (set_remove(_event->events, o->id) != ERROR_OK)
    CORE_ESCAPE("unable to remove the object from the set of events");

  /*
   * 3)
   */

  if (machine_call(event, event_release, o->id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function returns true if the event exists.
 */

t_error			event_exist(i_event			id)
{
  if (set_exist(_event->events, id) != ERROR_TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function gets an event object from the event set.
 */

t_error			event_get(i_event			id,
				  o_event**			o)
{
  assert(o != NULL);

  if (set_get(_event->events, id, (void**)o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the event object from the set");

  CORE_LEAVE();
}

/*
 * initialize the event manager.
 *
 * steps:
 *
 * 1) allocate and initialize the event manager.
 * 2) initialize the object identifier.
 * 3) reserve the event set.
 * 4) call the machine dependent code.
 */

t_error			event_initialize(void)
{
  /*
   * 1)
   */

  if ((_event = malloc(sizeof(m_event))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the event manager's structure");

  memset(_event, 0x0, sizeof(m_event));

  /*
   * 2)
   */

  if (id_build(&_event->id) != ERROR_OK)
    CORE_ESCAPE("unable to initialize the identifier object");

  /*
   * 3)
   */

  if (set_reserve(ll, SET_OPTION_ALLOC | SET_OPTION_SORT,
		  sizeof(o_event), &_event->events) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the set of events");

  /*
   * 4)
   */

  if (machine_call(event, event_initialize) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * destroy the event manager.
 *
 * steps:
 *
 * 1) call the machine-dependent code.
 * 2) release every event object.
 * 3) release the event set.
 * 4) destroy the identifier object.
 * 5) free the event manager's structure memory.
 */

t_error			event_clean(void)
{
  t_iterator		i;
  o_event*		o;

  /*
   * 1)
   */

  if (machine_call(event, event_clean) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  while (set_head(_event->events, &i) == ERROR_TRUE)
    {
      if (set_object(_event->events, i, (void**)&o) != ERROR_OK)
	CORE_ESCAPE("unable to find the event object corresponding to "
		    "its identifier\n");

      if (event_release(o->id) != ERROR_OK)
	CORE_ESCAPE("unable to release the event object");
    }

  /*
   * 3)
   */

  if (set_release(_event->events) != ERROR_OK)
    CORE_ESCAPE("unable to release the set of events");

  /*
   * 4)
   */

  if (id_destroy(&_event->id) != ERROR_OK)
    CORE_ESCAPE("unable to destroy the identifier object");

  /*
   * 5)
   */

  free(_event);

  CORE_LEAVE();
}
