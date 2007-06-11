/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/task/task.c
 *
 * created       julien quintard   [sat dec 10 13:56:00 2005]
 * updated       julian pidancet   [fri apr 13 03:04:55 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the task manager manages tasks.
 *
 * a task is  composed of the process control  block (task id, parent,
 * children...), a priority used  for the scheduling, an address space
 * and a set of waits.
 *
 * XXX adapt for cpu use.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

machine_include(task);

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * the init variable, filled by the bootloader, containing in this case
 * the list of segments to mark used.
 */

extern t_init*		init;

/*
 * XXX
 */

extern m_sched*		sched;

/*
 * the identifier of the pre-reserved segment containing the mod service
 * code.
 */

extern i_segment	mod;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the task manager variable.
 */

m_task*			task = NULL;

/*
 * the kernel task.
 */

i_task			ktask = ID_UNUSED;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function returns the current task.
 */

t_error			task_current(i_task*			tsk)
{
  i_thread		current;
  o_thread*		o;

  TASK_ENTER(task);

  if (!sched)
    {
      *tsk = ktask;

      TASK_LEAVE(task, ERROR_NONE);
    }

  if (sched_current(&current) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  if (thread_get(current, &o) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  *tsk = o->taskid;

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * this function shows a precise task.
 */

t_error			task_show(i_task			id)
{
  o_task*		o;
  char*			state;

  TASK_ENTER(task);

  if (task_get(id, &o) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  switch (o->sched)
    {
      case SCHED_STATE_STOP:
	state = "stopped";
	break;
      case SCHED_STATE_RUN:
	state = "running";
	break;
      default:
	state = "other";
	break;
    }

  cons_msg('#', "  task %qd: %s on cpu %qd\n", id, state, o->cpuid);

  if (machine_call(task, task_show, id) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * this function dumps the tasks managed by the kernel.
 *
 * steps:
 *
 * 1) gets the set's size.
 * 2) for each task, show it.
 */

t_error			task_dump(void)
{
  t_state		state;
  o_task*		data;
  t_setsz		size;
  t_iterator		i;

  TASK_ENTER(task);

  /*
   * 1)
   */

  if (set_size(task->tasks, &size) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "dumping %qu task(s):\n", size);

  set_foreach(SET_OPT_FORWARD, task->tasks, &i, state)
    {
      if (set_object(task->tasks, i, (void**)&data) != ERROR_NONE)
	TASK_LEAVE(task, ERROR_UNKNOWN);

      if (task_show(data->tskid) != ERROR_NONE)
	TASK_LEAVE(task, ERROR_UNKNOWN);
    }

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * this function clones a task.
 *
 * this function so takes care of cloning the address space and every thread.
 *
 * steps:
 *
 * 1) get the source task object given its identifier.
 * 2) reserve the cloned task object.
 * 3) get the destination task object previously reserved.
 * 4) clone the address space from the source task object.
 * 5) clone the thread set from the source task object.
 * 6) clone the waits.
 * 7) set the new task state.
 * 8) call the machine-dependent code.
 */

t_error			task_clone(i_task			old,
				   i_task*			new)
{
  t_state		state;
  o_task*		from;
  o_task*		to;
  t_iterator		i;

  TASK_ENTER(task);

  /*
   * 1)
   */

  if (task_get(old, &from) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_reserve(from->class, from->behav, from->prior, new) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (task_get(*new, &to) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (from->asid != ID_UNUSED)
    {
      if (as_clone(to->tskid, from->asid, &to->asid) != ERROR_NONE)
	TASK_LEAVE(task, ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  set_foreach(SET_OPT_FORWARD, from->threads, &i, state)
    {
      i_thread		needless;
      i_thread*		data;

      if (set_object(from->threads, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "task: cannot find the object "
		   "corresponding to its identifier\n");

	  TASK_LEAVE(task, ERROR_UNKNOWN);
	}

      if (thread_clone(to->tskid, *data, &needless) != ERROR_NONE)
	TASK_LEAVE(task, ERROR_UNKNOWN);
    }

  /*
   * 6)
   */

  /* XXX waits */

  /*
   * 7)
   */

  to->sched = from->sched;

  /*
   * 8)
   */

  if (machine_call(task, task_clone, old, new) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * this function reserves a task object.
 *
 * steps:
 *
 * 1) checks the validity of arguments.
 * 2) initialises the task object.
 * 3) reserves an identifier for the task object.
 * 4) get the parent task and add the created task to its children set.
 * 5) reserves the set of threads for the new task object.
 * 6) reserves the set of waits for the new task object.
 * 7) reserves the set of children for the new task object.
 * 8) register a message box into the message manager.
 * 9) adds the new task object in the task set.
 * 10) calls the machine-dependent code.
 */

t_error			task_reserve(t_class			class,
				     t_behav			behav,
				     t_prior			prior,
				     i_task*			id)
{
  o_task		o;
  o_task*		parent;

  TASK_ENTER(task);

  /*
   * 1)
   */

  if ((class != TASK_CLASS_CORE) &&
      (class != TASK_CLASS_DRIVER) &&
      (class != TASK_CLASS_SERVICE) &&
      (class != TASK_CLASS_PROGRAM))
    TASK_LEAVE(task, ERROR_UNKNOWN);

  if ((behav != TASK_BEHAV_CORE) &&
      (behav != TASK_BEHAV_REALTIME) &&
      (behav != TASK_BEHAV_INTERACTIVE) &&
      (behav != TASK_BEHAV_TIMESHARING) &&
      (behav != TASK_BEHAV_BACKGROUND))
    TASK_LEAVE(task, ERROR_UNKNOWN);

/* XXX bogus */
  if (0 && (prior != TASK_PRIOR_CORE) &&
      (prior != TASK_PRIOR_REALTIME) &&
      (prior != TASK_PRIOR_INTERACTIVE) &&
      (prior != TASK_PRIOR_TIMESHARING) &&
      (prior != TASK_PRIOR_BACKGROUND))
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 2)
   */

  memset(&o, 0x0, sizeof(o_task));

  o.class = class;
  o.behav = behav;
  o.prior = prior;

  o.asid = ID_UNUSED;
  o.threads = ID_UNUSED;
  o.waits = ID_UNUSED;

  if (ktask != ID_UNUSED)
    {
      if (cpu_select(&o.cpuid) != ERROR_NONE)
	TASK_LEAVE(task, ERROR_UNKNOWN);
    }
  else
    o.cpuid = init->bsp;

  o.sched = SCHED_STATE_STOP;

  /*
   * 3)
   */

  if (id_reserve(&task->id, &o.tskid) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (ktask != ID_UNUSED)
    {
      if (task_current(&o.parent) != ERROR_NONE)
	TASK_LEAVE(task, ERROR_UNKNOWN);

      if (task_get(o.parent, &parent) != ERROR_NONE)
	TASK_LEAVE(task, ERROR_UNKNOWN);

      if (set_add(parent->children, &o) != ERROR_NONE)
	TASK_LEAVE(task, ERROR_UNKNOWN);
    }
  else
    o.parent = ID_UNUSED;

  *id = o.tskid;

  /*
   * 5)
   */

  if (set_reserve(array, SET_OPT_SORT | SET_OPT_ALLOC, TASK_THREADS_INITSZ,
		  sizeof(i_thread), &o.threads) != ERROR_NONE)
    {
      id_release(&task->id, o.tskid);

      TASK_LEAVE(task, ERROR_UNKNOWN);
    }

  /*
   * 6)
   */

  if (set_reserve(array, SET_OPT_ALLOC, TASK_WAITS_INITSZ,
		  sizeof(o_waitfor), &o.waits) != ERROR_NONE)
    {
      id_release(&task->id, o.tskid);

      set_release(o.threads);

      TASK_LEAVE(task, ERROR_UNKNOWN);
    }

  /*
   * 7)
   */

  if (set_reserve(ll, SET_OPT_SORT | SET_OPT_ALLOC,
		  sizeof(i_task), &o.children) != ERROR_NONE)
    {
      id_release(&task->id, o.tskid);

      set_release(o.threads);
      set_release(o.waits);

      TASK_LEAVE(task, ERROR_UNKNOWN);
    }

  /*
   * 8)
   */

  if (o.tskid != 0)
  {
    if (message_register(o.tskid, 0) != ERROR_NONE)
    {
      id_release(&task->id, o.tskid);

      set_release(o.threads);
      set_release(o.waits);
      set_release(o.children);

      TASK_LEAVE(task, ERROR_UNKNOWN);
    }
  }

  /*
   * 9)
   */

  if (set_add(task->tasks, &o) != ERROR_NONE)
    {
      id_release(&task->id, o.tskid);

      set_release(o.threads);
      set_release(o.waits);
      set_release(o.children);

      TASK_LEAVE(task, ERROR_UNKNOWN);
    }

  /*
   * 10)
   */

  if (machine_call(task, task_reserve, class,
		   behav, prior, id) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * this function releases a task.
 *
 * steps:
 *
 * 1) calls the machine-dependent code.
 * 2) gets the task object given its identifier.
 * 3) releases the task's object identifier.
 * 4) releases the task object's set of threads.
 * 5) releases the task object's set of waits.
 * 6) removes the task object from the task set.
 */

t_error			task_release(i_task			id)
{
  o_task*		o;

  TASK_ENTER(task);

  /*
   * 1)
   */

  if (machine_call(task, task_release, id) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(id, &o) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (id_release(&task->id, o->tskid) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (o->asid != ID_UNUSED && id != ktask)
    {
      if (as_release(o->asid) != ERROR_NONE)
	TASK_LEAVE(task, ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  if (thread_flush(id) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  if (set_release(o->threads) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (set_release(o->waits) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 6)
   */

  if (set_remove(task->tasks, o->tskid) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * this function updates the task's priority to "prior".
 *
 * steps:
 *
 * 1) get the task object.
 * 2) check the priority range using the behaviour.
 * 3) update the task priority.
 * 4) call the machine dependent code.
 * 5) update all the threads priorities in the scheduler.
 */

t_error			task_priority(i_task			id,
				      t_prior			prior)
{
  o_task*		o;
  t_iterator		i;
  t_state		state;

  TASK_ENTER(task);

  /*
   * 1)
   */

  if (task_get(id, &o) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 2)
   */

  switch (o->behav)
    {
      case TASK_BEHAV_CORE:
	if (prior < TASK_LPRIOR_CORE || prior > TASK_HPRIOR_CORE)
	  TASK_LEAVE(task, ERROR_UNKNOWN);
	break;
      case TASK_BEHAV_REALTIME:
	if (prior < TASK_LPRIOR_REALTIME || prior > TASK_HPRIOR_REALTIME)
	  TASK_LEAVE(task, ERROR_UNKNOWN);
	break;
      case TASK_BEHAV_INTERACTIVE:
	if (prior < TASK_LPRIOR_INTERACTIVE || prior > TASK_HPRIOR_INTERACTIVE)
	  TASK_LEAVE(task, ERROR_UNKNOWN);
	break;
      case TASK_BEHAV_TIMESHARING:
	if (prior < TASK_LPRIOR_TIMESHARING || prior > TASK_HPRIOR_TIMESHARING)
	  TASK_LEAVE(task, ERROR_UNKNOWN);
	break;
      case TASK_BEHAV_BACKGROUND:
	if (prior < TASK_LPRIOR_BACKGROUND || prior > TASK_HPRIOR_BACKGROUND)
	  TASK_LEAVE(task, ERROR_UNKNOWN);
	break;
      default:
	TASK_LEAVE(task, ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  o->prior = prior;

  /*
   * 4)
   */

  if (machine_call(task, task_priority, id, prior) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 5)
   */

  set_foreach(SET_OPT_FORWARD, o->threads, &i, state)
    {
      i_thread*		th;

      if (set_object(o->threads, i, (void**)&th) != ERROR_NONE)
	{
	  cons_msg('!', "task: cannot find the object "
		   "corresponding to its identifier\n");

	  TASK_LEAVE(task, ERROR_UNKNOWN);
	}

      if (sched_update(*th) != ERROR_NONE)
	TASK_LEAVE(task, ERROR_UNKNOWN);
    }

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * this function marks the task as runnable.
 *
 * steps:
 *
 * 1) get the task object.
 * 2) set the new state.
 * 3) wakeup the waiting tasks.
 * 4) call the machine dependent code.
 * 5) start the threads.
 */

t_error			task_state(i_task			id,
				   t_state			sched)
{
  o_task*		o;
  t_iterator		i;
  t_state		state;
  t_state		wakeup;

  TASK_ENTER(task);

  /*
   *
   */

  switch(sched)
    {
      case SCHED_STATE_RUN:
        wakeup = WAIT_START;
        break;
      case SCHED_STATE_STOP:
        wakeup = WAIT_STOP;
        break;
      case SCHED_STATE_ZOMBIE:
        wakeup = WAIT_DEATH;
        break;
      case SCHED_STATE_BLOCK:
	wakeup = 0;
	break;
      default:
        TASK_LEAVE(task, ERROR_UNKNOWN);
    }

  /*
   * 1)
   */

  if (task_get(id, &o) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (o->sched == sched)
    TASK_LEAVE(task, ERROR_NONE);

  o->sched = sched;

  /*
   * 3)
   */

  set_foreach(SET_OPT_FORWARD, o->waits, &i, state)
    {
      o_waitfor*	w;

      if (set_object(o->waits, i, (void**)&w) != ERROR_NONE)
	{
	  cons_msg('!', "task: cannot find the object "
		   "corresponding to its identifier\n");

	  TASK_LEAVE(task, ERROR_UNKNOWN);
	}

      if (w->opts & wakeup)
	task_state(w->u.task, SCHED_STATE_RUN);

      /* XXX remove */
    }

  /*
   * 4)
   */

  if (machine_call(task, task_state, id, sched) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 5)
   */

  set_foreach(SET_OPT_FORWARD, o->threads, &i, state)
    {
      i_thread*		th;

      if (set_object(o->threads, i, (void**)&th) != ERROR_NONE)
	{
	  cons_msg('!', "task: cannot find the object "
		   "corresponding to its identifier\n");

	  TASK_LEAVE(task, ERROR_UNKNOWN);
	}

      if (sched != SCHED_STATE_BLOCK &&
	  thread_state(*th, sched) != ERROR_NONE)
	TASK_LEAVE(task, ERROR_UNKNOWN);
    }

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * this function waits for state change.
 */

t_error			task_wait(i_task			id,
				  t_opts			opts,
				  t_wait*			wait)
{
  o_task*		o;
  o_waitfor		w;

  TASK_ENTER(task);

  /* XXX */

  if (opts & WAIT_ID)
    {
      /*
       * 1)
       */

      /*
       * a)
       */

      if (task_get(id, &o) != ERROR_NONE)
	TASK_LEAVE(task, ERROR_UNKNOWN);

      /*
       * b)
       */

      if ((opts & WAIT_START) && o->sched == SCHED_STATE_RUN)
	TASK_LEAVE(task, ERROR_NONE);

      if ((opts & WAIT_STOP) && o->sched == SCHED_STATE_STOP)
	TASK_LEAVE(task, ERROR_NONE);

      if ((opts & WAIT_DEATH) && o->sched == SCHED_STATE_ZOMBIE)
	TASK_LEAVE(task, ERROR_NONE);

      /*
       * c)
       */

      if (!(opts & WAIT_NOHANG))
	{
	  /*
	   * d)
	   */

	  if (task_current(&w.u.task) != ERROR_NONE)
	    TASK_LEAVE(task, ERROR_UNKNOWN);;

	  w.opts = opts;

	  if (set_add(o->waits, &w) != ERROR_NONE)
	    TASK_LEAVE(task, ERROR_UNKNOWN);

	  /*
	   * e)
	   */

	  if (task_state(w.u.task, SCHED_STATE_STOP) != ERROR_NONE)
	    TASK_LEAVE(task, ERROR_UNKNOWN);
	}
    }
  else
    {
      /*
       * 2)
       */

    }

  /*
   * 3)
   */

  if (wait != NULL)
    {

    }

  /*
   * 4)
   * XXX ou mettre ca !?
   */

  if (machine_call(task, task_wait, id, opts, wait) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  TASK_LEAVE(task, ERROR_UNKNOWN);
}

/*
 * this function gets a task object from the task set.
 */

t_error			task_get(i_task				id,
				 o_task**			o)
{
  TASK_ENTER(task);

  if (set_get(task->tasks, id, (void**)o) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  TASK_LEAVE(task, ERROR_NONE);
}

/*
 * this functions initialises the task manager.
 *
 * this function takes care of initialising and builing the kernel task.
 *
 * steps:
 *
 * 1) allocate and initialises the task manager structure.
 * 2) initialise the identifier object to be able to generate
 *    the task identifiers.
 * 3) reserve the task set which will contain the tasks built later.
 * 4) reserve the kernel task and its address space.
 * 5) add the segments to the kernel address space.
 *    note that for the segment corresponding to the mod service code, the
 *    segment identifier is saved in the mod global variable so that the
 *    core can retrieve it, build a task, map the segment, and launch it.
 * 6) add the regions to the kernel address space.
 * 7) call the machine-dependent code.
 * 8) if asked, dumps the task manager.
 */

t_error			task_init(void)
{
  i_segment		segments[INIT_SEGMENTS];
  i_thread		needless;
  i_region		useless;
  o_segment*		segment;
  o_region*		region;
  i_as			asid;
  t_uint32		i;

  /*
   * 1)
   */

  if ((task = malloc(sizeof(m_task))) == NULL)
    {
      cons_msg('!', "task: cannot allocate memory for the task manager "
	       "structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(task, 0x0, sizeof(m_task));

  /*
   * 2)
   */

  if (id_build(&task->id) != ERROR_NONE)
    {
      cons_msg('!', "task: unable to initialise the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  if (set_reserve(ll, SET_OPT_ALLOC | SET_OPT_SORT,
		  sizeof(o_task), &task->tasks) != ERROR_NONE)
    {
      cons_msg('!', "task: unable to reserve the task set\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  if (task_reserve(TASK_CLASS_CORE, TASK_BEHAV_CORE,
		   TASK_PRIOR_CORE, &ktask) != ERROR_NONE)
    {
      cons_msg('!', "task: unable to reserve the kernel task\n");

      return (ERROR_UNKNOWN);
    }

  if (as_reserve(ktask, &asid) != ERROR_NONE)
    {
      cons_msg('!', "task: unable to reserve the kernel address space\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  for (i = 0; i < init->nsegments; i++)
    {
      if ((segment = malloc(sizeof(o_segment))) == NULL)
	return (ERROR_UNKNOWN);

      segment->type = SEGMENT_TYPE_MEMORY;
      segment->address = init->segments[i].address;
      segment->size = init->segments[i].size;
      segment->perms = init->segments[i].perms;

      if (segment_inject(asid, segment, &segments[i]) != ERROR_NONE)
	{
	  cons_msg('!', "segment: cannot add a pre-reserved segment in "
		   "the segment set\n");

	  return (ERROR_UNKNOWN);
	}

      if (init->mcode == init->segments[i].address)
	mod = segments[i];
    }

  /*
   * 6)
   */

  for (i = 0; i < init->nregions; i++)
    {
      if ((region = malloc(sizeof(o_region))) == NULL)
	return (ERROR_UNKNOWN);

      region->segid = segments[init->regions[i].segment];
      region->address = init->regions[i].address;
      region->offset = init->regions[i].offset;
      region->size = init->regions[i].size;
      region->opts = init->regions[i].opts;

      if (region_inject(asid, region, &useless) != ERROR_NONE)
	{
	  cons_msg('!', "region: cannot map a region to a pre-reserved "
		   "region\n");

	  return (ERROR_UNKNOWN);
	}
    }

  /*
   * 7)
   */

  if (machine_call(task, task_init) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 8)
   */

#if (DEBUG & DEBUG_TASK)
  task_dump();
#endif

  return (ERROR_NONE);
}

/*
 * this function just reinitialises the task manager.
 *
 * steps:
 *
 * 1) call the machine-dependent code.
 * 2) release the task's set.
 * 3) destroy the id object.
 * 4) free the task manager structure's memory.
 */

t_error			task_clean(void)
{
  t_iterator		i;
  i_task		*data;

  /*
   * 1)
   */

  if (machine_call(task, task_clean) != ERROR_NONE)
    TASK_LEAVE(task, ERROR_UNKNOWN);

  /*
   * 2)
   */

  while (set_head(task->tasks, &i) == ERROR_NONE)
    {
      if (set_object(task->tasks, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "task: cannot find the task object "
		   "corresponding to its identifier\n");

	  return (ERROR_UNKNOWN);
	}

      if (task_release(*data) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }

  if (set_release(task->tasks) != ERROR_NONE)
    {
      cons_msg('!', "task: unable to release the task set\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  if (id_destroy(&task->id) != ERROR_NONE)
    {
      cons_msg('!', "task: unable to destroy the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  free(task);

  return (ERROR_NONE);
}
