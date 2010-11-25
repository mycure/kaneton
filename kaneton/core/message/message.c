/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/kaneton/core/message/message.c
 *
 * created       matthieu bucchianeri   [mon jul 23 11:37:30 2007]
 * updated       julien quintard   [thu nov 25 12:10:42 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the message manager provides a complete set of functions to send, receive,
 * request and reply messages in different passing modes.
 *
 * the message manager interface is equivalent in many ways to the
 * well-known parallel and distributed computing libraries like MPI, PVM etc.
 *
 * indeed, functions to send synchronously and asynchronously are provided
 * as well as blocking and non-blocking functions.
 *
 * sending an asynchronous message means the kaneton microkernel will have
 * to copy the message into an internal buffer and then to copy it out in
 * the destination buffer of the receiver.
 *
 * in the other hand, sending a synchronous message means that nothing is
 * done until the two parts, sender and receiver, are ready to exchange the
 * message. then, the kernel just copies the message from the source buffer
 * directly into the destination buffer.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

machine_include(message);

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * kernel manager structure
 */

extern m_kernel*	_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * message manager variable.
 */

m_message*		_message = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */


/*
 * this function retrieves a message box of a task by its type.
 */

static t_error		message_box(i_task			task,
				    t_type			type,
				    o_message_type**		o)
{
  t_id			typeid = type;
  o_task*		otask;

  MESSAGE_ENTER(_message);

  assert(o != NULL);

  if (task_get(task, &otask) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  if (set_get(otask->messages, typeid, (void**)o) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  MESSAGE_LEAVE(_message, ERROR_OK);
}

/*
 * this function registers a new message type with its maximum length.
 *
 * only registered types of messages can be sent through the kernel. unknown
 * types will be rejected.
 *
 * steps:
 *
 * 1) get the message box from the task.
 * 2) check if this type is already registered.
 * 3) build the message type structure.
 * 4) add it to the task messages set.
 * 5) call the machine dependent code.
 *
 */

t_error			message_register(i_task			task,
					 t_type			type,
					 t_vsize		size)
{
  t_id			typeid = type;
  o_task*		o;
  void*			needless;
  o_message_type	msgtype;

  MESSAGE_ENTER(_message);

  /*
   * 1)
   */

  if (task_get(task, &o) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  /*
   * 2)
   */

  if (set_get(o->messages, typeid, (void**)&needless) == ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  /*
   * 3)
   */

  msgtype.id = 0;
  msgtype.type = typeid;
  msgtype.size = size;

  if (set_reserve(pipe, SET_OPTION_ALLOC, sizeof (o_message),
		  &msgtype.queue) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  if (set_reserve(pipe, SET_OPTION_ALLOC, sizeof (o_message),
		  &msgtype.waiters) != ERROR_OK)
    {
      set_release(msgtype.queue);

      MESSAGE_LEAVE(_message, ERROR_KO);
    }

  /*
   * 4)
   */

  if (set_add(o->messages, &msgtype) != ERROR_OK)
    {
      set_release(msgtype.waiters),
      set_release(msgtype.queue);

      MESSAGE_LEAVE(_message, ERROR_KO);
    }

  /*
   * 5)
   */

  if (machine_call(message, message_register, task, type, size) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  MESSAGE_LEAVE(_message, ERROR_OK);
}

/*
 * this function returns the maximum size of a message in a message box.
 *
 */

t_error			message_size(i_task			task,
				     t_type			type,
				     t_vsize*			size)
{
  o_message_type*	o;

  MESSAGE_ENTER(_message);

  if (message_box(task, type, &o) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  *size = o->size;

  MESSAGE_LEAVE(_message, ERROR_OK);
}

/*
 * this function free all the message types for one task.
 *
 * steps:
 *
 * 1) loop though the message types.
 * 2) unblock waiters.
 * 3) free outgoing messages.
 * 4) release the sets.
 * 5) call machine.
 *
 */

t_error			message_flush(i_task			task)
{
  o_task*		o;
  o_message_type*	otype;
  o_message*		omsg;
  t_iterator		it;

  MESSAGE_ENTER(_message);

  if (task_get(task, &o) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  /*
   * 1)
   */

  while (set_head(o->messages, &it) != ERROR_OK)
    {
      if (set_object(o->messages, it, (void**)&otype) != ERROR_OK)
	MESSAGE_LEAVE(_message, ERROR_KO);

      /*
       * 2)
       */

      while (set_pick(otype->waiters, (void**)&omsg) != ERROR_OK)
	{
	  if (message_return(omsg->blocked, ERROR_KO) != ERROR_OK)
	    MESSAGE_LEAVE(_message, ERROR_KO);

	  if (set_pop(otype->waiters) != ERROR_OK)
	    MESSAGE_LEAVE(_message, ERROR_KO);
	}

      /*
       * 3)
       */

      while (set_pick(otype->queue, (void**)&omsg) != ERROR_OK)
	{
	  if (set_pop(otype->waiters) != ERROR_OK)
	    MESSAGE_LEAVE(_message, ERROR_KO);

	  if (omsg->as == ID_UNUSED)
	    {
	      if (message_return(omsg->blocked, ERROR_KO) != ERROR_OK)
		MESSAGE_LEAVE(_message, ERROR_KO);
	    }
	  else
	    {
	      if (omsg->size != 0)
		free(omsg->data);
	    }
	}

      /*
       * 4)
       */

      if (set_release(otype->waiters) != ERROR_OK)
	MESSAGE_LEAVE(_message, ERROR_KO);

      if (set_release(otype->queue) != ERROR_OK)
	MESSAGE_LEAVE(_message, ERROR_KO);
    }

  /*
   * 5)
   */

  if (machine_call(message, message_flush, task) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  MESSAGE_LEAVE(_message, ERROR_OK);
}

/*
 * this function sends a message to a task. the message is delivered
 * asynchronously and the function returns only when the buffer was safely
 * copied.
 *
 * steps:
 *
 * 1) check message destination.
 * 2) get the destination message box.
 * 3) check the size.
 * 4) system call special case.
 * 5) if a thread is waiting for a message, then send it synchronously.
 * 6) build the message.
 *  a) fill the buffer, intra-kernel case.
 *  b) fill the buffer, inter-as case.
 * 7) push the message into the message box.
 * 8) call the machine dependent code.
 *
 */

t_error			message_send(i_task			task,
				     i_node			destination,
				     t_type			type,
				     t_vaddr			data,
				     t_vsize			size)
{
  t_id			typeid = type;
  o_message_type*	o;
  o_task*		otsk;
  o_message		msg;
  t_setsz		setsz;

  MESSAGE_ENTER(_message);

  /*
   * 1)
   */

  if (destination.machine != _kernel->machine)
    {
      /* XXX distr */
      MESSAGE_LEAVE(_message, ERROR_KO);
    }

  /*
   * 2)
   */

  if (message_box(destination.task, typeid, &o) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  /*
   * 3)
   */

  if (size > o->size)
    MESSAGE_LEAVE(_message, ERROR_KO);

  /*
   * 4)
   */

  if (destination.task == _kernel->task && type == 0)
    {
      t_error		res;
      void*		buffer;
      i_node		source;

      assert(task != _kernel->task);
      assert(size != 0);

      source.machine = _kernel->machine;
      source.task = task;

      if (task_get(task, &otsk) != ERROR_OK)
	MESSAGE_LEAVE(_message, ERROR_KO);

      if ((buffer = malloc(size)) == NULL)
	MESSAGE_LEAVE(_message, ERROR_KO);

      if (as_read(otsk->as, data, size, buffer) != ERROR_OK)
	{
	  free(buffer);

	  MESSAGE_LEAVE(_message, ERROR_KO);
	}

      res = interface_notify(buffer, size, source);

      free(buffer);

      MESSAGE_LEAVE(_message, res);
    }

  /*
   * 5)
   */

  if (set_size(o->waiters, &setsz) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  if (setsz != 0)
    {
      t_error		res;

      res = message_transmit(task, destination, type, data, size);

      MESSAGE_LEAVE(_message, res);
    }

  /*
   * 6)
   */

  if (size)
    {
      if ((msg.data = malloc(size)) == NULL)
	MESSAGE_LEAVE(_message, ERROR_KO);
    }
  else
    msg.data = NULL;

  if (size)
    {
      if (task == _kernel->task)
	{
	  /*
	   * a)
	   */

	  memcpy(msg.data, (void*)data, size);
	}
      else
	{
	  /*
	   * b)
	   */

	  if (task_get(task, &otsk) != ERROR_OK)
	    {
	      free(msg.data);

	      MESSAGE_LEAVE(_message, ERROR_KO);
	    }

	  if (as_read(otsk->as, data, size, msg.data) != ERROR_OK)
	    {
	      free(msg.data);

	      MESSAGE_LEAVE(_message, ERROR_KO);
	    }
	}
    }

  msg.as = ID_UNUSED;
  msg.size = size;
  msg.sender.machine = _kernel->machine;
  msg.sender.task = task;

  /*
   * 7)
   */

  msg.id = o->id++;

  if (set_push(o->queue, &msg) != ERROR_OK)
    {
      if (msg.data != NULL)
	free(msg.data);

      MESSAGE_LEAVE(_message, ERROR_KO);
    }

  /*
   * 8)
   */

  if (machine_call(message, message_send, task, destination, type,
		   data, size) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  MESSAGE_LEAVE(_message, ERROR_OK);
}

/*
 * this function sends a message to a task. the message is delivered
 * synchronously and the function returns only when the message is delivered
 * correctly.
 *
 * there is no kernel bufferization.
 *
 * steps:
 *
 * 1) check for message destination.
 * 2) get message box and task object.
 * 3) check size.
 * 4) do the transmission.
 *  a) immediately if a message is being waited.
 *  b) later if no rendezvous has been established.
 * 5) call machine dependent code.
 *
 */

t_error			message_transmit(i_task			task,
					 i_node			destination,
					 t_type			type,
					 t_vaddr		data,
					 t_vsize		size)
{
  t_id			typeid = type;
  o_message_type*	o;
  o_task*		otsk;
  o_message		msg;
  o_message*		pmsg;
  t_setsz		setsz;
  i_thread		thread;

  MESSAGE_ENTER(_message);

  /*
   * 1)
   */

  if (destination.machine != _kernel->machine)
    {
      /* XXX distr */
      MESSAGE_LEAVE(_message, ERROR_KO);
    }

  /*
   * 2)
   */

  if (message_box(destination.task, typeid, &o) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  if (task_get(task, &otsk) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  /*
   * 3)
   */

  if (size > o->size)
    MESSAGE_LEAVE(_message, ERROR_KO);

  /*
   * 4)
   */

  if (set_size(o->waiters, &setsz) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  if (setsz != 0)
    {
      i_node		sender;

      /*
       * a)
       */

      if (set_pick(o->waiters, (void*)&pmsg) != ERROR_OK)
	MESSAGE_LEAVE(_message, ERROR_KO);

      if (size)
	if (as_copy(otsk->as, data, pmsg->as, (t_vaddr)pmsg->data,
		    size) != ERROR_OK)
	  MESSAGE_LEAVE(_message, ERROR_KO);

      sender.machine = _kernel->machine;
      sender.task = task;

      if (message_return_info(pmsg->blocked, ERROR_OK, size,
			      sender) != ERROR_OK)
	MESSAGE_LEAVE(_message, ERROR_KO);

      if (set_pop(o->waiters) != ERROR_OK)
	MESSAGE_LEAVE(_message, ERROR_KO);
    }
  else
    {
      /*
       * b)
       */

      if (scheduler_current(&thread) != ERROR_OK)
	MESSAGE_LEAVE(_message, ERROR_KO);

      msg.as = otsk->as;
      msg.data = (void*)data;
      msg.size = size;
      msg.sender.machine = _kernel->machine;
      msg.sender.task = task;
      msg.blocked = thread;

      msg.id = o->id++;

      if (set_push(o->queue, &msg) != ERROR_OK)
	MESSAGE_LEAVE(_message, ERROR_KO);

      if (thread_block(thread) != ERROR_OK)
	MESSAGE_LEAVE(_message, ERROR_KO);
    }

  /*
   * 5)
   */

  if (machine_call(message, message_transmit, task, destination, type,
		   data, size) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  MESSAGE_LEAVE(_message, ERROR_OK);
}

/*
 * this function sends a message to a task. the message is delivered
 * asynchronously and the function returns immediately. the request parameter
 * is used to poll or wait until the buffer is safely copied.
 *
 * on non-multiprocessor systems, this function is totally equivalent to
 * message_send.
 *
 */

t_error			message_throw(i_task			task,
				      i_node			destination,
				      t_type			type,
				      t_vaddr			data,
				      t_vsize			size,
				      t_message_request*	request)
{
  MESSAGE_ENTER(_message);

  assert(request != NULL);

  if (cpu_multiprocessor() == ERROR_OK)
    {
      /* XXX smp */

      MESSAGE_LEAVE(_message, ERROR_KO);
    }
  else
    {
      t_error		res;

      res = message_send(task, destination, type, data, size);

      if (res != ERROR_OK)
	MESSAGE_LEAVE(_message, ERROR_KO);

      request->completed = MESSAGE_REQUEST_COMPLETED;
    }

  MESSAGE_LEAVE(_message, ERROR_OK);
}

/*
 * this function waits and receives a message, whether it is delivered
 * asynchronously or synchronously.
 *
 * steps:
 *
 * 1) get the message box.
 * 2) look for pending message.
 *  a) no pending message, block.
 *  b) incoming message, get it.
 * 3) call machine code.
 *
 */

t_error			message_receive(i_task			task,
					t_type			type,
					t_vaddr			data,
					t_vsize*		size,
					i_node*			sender)
{
  t_id			typeid = type;
  o_message_type*	o;
  t_setsz		setsz;
  i_thread		thread;
  o_message		msg;
  o_task*		otsk;

  MESSAGE_ENTER(_message);

  /*
   * 1)
   */

  if (message_box(task, typeid, &o) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  /*
   * 2)
   */

  if (set_size(o->queue, &setsz) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  if (setsz == 0)
    {
      /*
       * a)
       */

      if (task_get(task, &otsk) != ERROR_OK)
	MESSAGE_LEAVE(_message, ERROR_KO);

      if (scheduler_current(&thread) != ERROR_OK)
	MESSAGE_LEAVE(_message, ERROR_KO);

      msg.id = o->id++;
      msg.as = otsk->as;
      msg.data = (void*)data;
      msg.blocked = thread;

      if (set_push(o->waiters, &msg) != ERROR_OK)
	MESSAGE_LEAVE(_message, ERROR_KO);

      if (thread_block(thread) != ERROR_OK)
	MESSAGE_LEAVE(_message, ERROR_KO);
    }
  else
    {
      t_error		res;

      /*
       * b)
       */

      res = message_poll(task, type, data, size, sender);

      MESSAGE_LEAVE(_message, res);
    }

  /*
   * 3)
   */

  if (machine_call(message, message_poll, task, type, data,
		   size, sender) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  MESSAGE_LEAVE(_message, ERROR_OK);
}

/*
 * this function starts receiving asynchronous messages. the function returns
 * immediately, so it is not safe to use the buffer until the request has been
 * completely executed.
 *
 * on non-multiprocessor systems, this function is stricly equivalent to
 * message_receive.
 *
 */

t_error			message_grab(i_task			task,
				     t_type			type,
				     t_vaddr			data,
				     t_message_request*		request)
{
  MESSAGE_ENTER(_message);

  if (cpu_multiprocessor() == ERROR_OK)
    {
      /* XXX smp */

      MESSAGE_LEAVE(_message, ERROR_KO);
    }
  else
    {
      t_error		res;
      t_vsize		size;
      i_node		sender;

      res = message_receive(task, type, data, &size, &sender);

      if (res != ERROR_OK)
	MESSAGE_LEAVE(_message, ERROR_KO);

      request->completed = MESSAGE_REQUEST_COMPLETED;
      /*      request->size = size;
	      request->sender = sender;*/
    }

  MESSAGE_LEAVE(_message, ERROR_OK);
}

/*
 * this function checks for an incoming message, receives it or returns an
 * error immediately. this function can be used with asynchronous ou
 * synchronous messages.
 *
 * steps:
 *
 * 1) get the destination message box.
 * 2) poll for incoming message.
 * 3) treat the message.
 *  a) synchronous case.
 *  b) asynchronous, intra-kernel case.
 *  c) asynchronous, inter-address space case.
 * 4) call the machine dependent code.
 *
 */

t_error			message_poll(i_task			task,
				     t_type			type,
				     t_vaddr			data,
				     t_vsize*			size,
				     i_node*			sender)
{
  t_id			typeid = type;
  o_message_type*	o;
  o_task*		otsk;
  o_message*		msg;

  MESSAGE_ENTER(_message);

  /*
   * 1)
   */

  if (message_box(task, typeid, &o) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  /*
   * 2
   */

  if (set_pick(o->queue, (void*)&msg) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  /*
   * 3)
   */

  if (msg->as != ID_UNUSED)
    {
      /*
       * a)
       */

      if (msg->size)
	{
	  if (task_get(task, &otsk) != ERROR_OK)
	    MESSAGE_LEAVE(_message, ERROR_KO);

	  if (as_copy(msg->as, (t_vaddr)msg->data, otsk->as, data,
		      msg->size) != ERROR_OK)
	    MESSAGE_LEAVE(_message, ERROR_KO);
	}

      if (message_return(msg->blocked, ERROR_OK) != ERROR_OK)
	MESSAGE_LEAVE(_message, ERROR_KO);
    }
  else
    {
      if (task == _kernel->task)
	{
	  /*
	   * b)
	   */

	  if (msg->size)
	    memcpy((void*)data, msg->data, msg->size);
	}
      else
	{
	  /*
	   * c)
	   */

	  if (msg->size)
	    {
	      if (task_get(task, &otsk) != ERROR_OK)
		MESSAGE_LEAVE(_message, ERROR_KO);

	      if (as_write(otsk->as, msg->data, msg->size,
			   data) != ERROR_OK)
		MESSAGE_LEAVE(_message, ERROR_KO);
	    }
	}

      if (msg->data != NULL)
	free(msg->data);
    }

  *size = msg->size;
  *sender = msg->sender;

  if (set_pop(o->queue) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  /*
   * 4)
   */

  if (machine_call(message, message_poll, task, type, data,
		   size, sender) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  MESSAGE_LEAVE(_message, ERROR_OK);
}

/*
 * this function checks if a message copy (though message_throw or
 * message_grab) is finished yet.
 *
 */

t_error			message_state(i_task			task,
				      t_message_request		request)
{
  MESSAGE_ENTER(_message);

  /* XXX smp */

  MESSAGE_LEAVE(_message, ERROR_KO);
}

/*
 * this function checks if a message copy (though message_throw or
 * message_grab) is finished yet and waits for the operation to be completed.
 *
 */

t_error			message_wait(i_task			task,
				     t_message_request		request,
				     t_vsize*			size,
				     i_node*			sender)
{
  MESSAGE_ENTER(_message);

  /* XXX smp */

  MESSAGE_LEAVE(_message, ERROR_KO);
}

/*
 * this function resumes a blocked thread on a syscall with given return value.
 *
 */

t_error			message_return(i_thread			thread,
				       t_error			code)
{
  MESSAGE_ENTER(_message);

  if (machine_call(message, message_return, thread, code) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  if (thread_run(thread) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  MESSAGE_LEAVE(_message, ERROR_OK);
}

/*
 * this function resumes a blocked thread on a syscall with given info.
 *
 */

t_error			message_return_info(i_thread		thread,
					    t_error		code,
					    t_vsize		size,
					    i_node		sender)
{
  MESSAGE_ENTER(_message);

  if (machine_call(message, message_return_info, thread, code, size,
		   sender) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  if (thread_run(thread) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  MESSAGE_LEAVE(_message, ERROR_OK);
}


/*
 * this function initializes the message manager.
 *
 * steps:
 *
 * 1) allocate some memory for the manager structure.
 * 2) create message types for the kernel task.
 * 3) call the machine dependent code.
 *
 */

t_error			message_initialize(void)
{
  /*
   * 1)
   */

  if ((_message = malloc(sizeof(m_message))) == NULL)
    {
      module_call(console, console_message,
		  '!', "message: cannot allocate memory for the message "
		  "manager structure\n");

      return (ERROR_KO);
    }

  memset(_message, 0x0, sizeof(m_message));

  /*
   * 2)
   */

  if (message_register(_kernel->task, MESSAGE_TYPE_INTERFACE,
		       sizeof (o_syscall)) != ERROR_OK)
    return (ERROR_KO);

  if (message_register(_kernel->task, MESSAGE_TYPE_EVENT,
		       sizeof (o_event_message)) != ERROR_OK)
    return (ERROR_KO);

  if (message_register(_kernel->task, MESSAGE_TYPE_TIMER,
		       sizeof (o_timer_message)) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 3)
   */

  if (machine_call(message, message_initialize) != ERROR_OK)
    MESSAGE_LEAVE(_message, ERROR_KO);

  MESSAGE_LEAVE(_message, ERROR_OK);
}

/*
 * this function cleans the message manager.
 *
 * steps:
 *
 * 1) call the dependent code.
 * 3) free kernel message types.
 * 2) free the manager structure.
 *
 */

t_error			message_clean(void)
{
  /*
   * 1)
   */

  if (machine_call(message, message_clean) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 2)
   */

  if (message_flush(_kernel->task)  != ERROR_OK)
    return (ERROR_KO);

  /*
   * 3)
   */

  free(_message);

  return (ERROR_OK);
}
