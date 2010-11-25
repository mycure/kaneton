/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/core.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [sun may  6 18:44:10 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file is the entry point of the kaneton microkernel.
 *
 * from the kaneton() function, the kernel manager will be initialized
 * and then cleaned.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * this variable is generated when the kernel is compiled, indicating
 * the current kernel version.
 */

extern const char	version[];

/*
 * init variable received from the bootloader specifying segments, regions,
 * physical memory layout etc.
 */

t_init*			_init;

/*
 * this variable is filled by the task manager when it injects the
 * pre-reserved segments. this variable holds the identifier of the
 * segment containing the system service code.
 *
 * indeed, the kernel needs to retrieve this segment to map it and then
 * build a task for this very first service.
 */

i_segment		_system;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function simply initializes and cleans the kernel manager.
 *
 * steps:
 *
 * 1) set the _init variable from the boot loader argument.
 * 2) initialize some fundamental modules.
 * 3) displays the current kaneton version.
 * 4) set up the fine grained allocator.
 * 5) start the kernel.
 * 6) run the test system, should have it be loaded.
 * 7) spawn the 'system' service.
 * 8) stop the kernel.
 * 9) clean the loaded modules.
 * 10) shutdown the system.
 */

void			kaneton(t_init*				bootloader)
{
  /*
   * 1)
   */

  _init = bootloader;

  /*
   * 2)
   */

  module_call(console, console_initialize);
  module_call(forward, forward_initialize);
  module_call(report, report_initialize);

  /*
   * 3)
   */

  module_call(console, console_message,
	      '+', "%s\n", version);

  /*
   * 4)
   */

  module_call(console, console_message,
	      '+', "setting up the fine-graind memeory allocator\n");

  alloc_init(_init->alloc, _init->allocsz);

  /*
   * 5)
   */

  module_call(console, console_message,
	      '+', "starting the kernel\n");

  kernel_initialize();

  /*
   * 6)
   */

  module_call(test, test_run);

  /*
   * 7)
   */

  module_call(console, console_message,
	      '+', "spawning the 'system' server\n");

  kaneton_spawn();

  /*
   * 8)
   */

  module_call(console, console_message,
	      '+', "stopping the kernel\n");

  kernel_clean();

  /*
   * 9)
   */

  module_call(console, console_clean);
  module_call(forward, forward_clean);
  module_call(report, report_clean);

  /*
   * 10)
   */

  while (1)
    ;
}

/*
 * this function launches the very first server by creating a new
 * task.
 *
 * the new task's address space is only composed of a stack and the
 * code which is a mapping of the code provided by the booloader:
 * init->scode.
 *
 * this first server is referred to as the 'system' server.
 *
 * steps:
 *
 * 1) check that the boot loader provided a system to spawn through
 *    the _slocation_ and _scodesz_ attributes of the init structure.
 * 2) reserve a task and addresse space for the server.
 * 3) map the server's code segment, i.e _system (c.f. initialize() in task.c)
 *    at a very precise location.
 * 4) map the array of inputs (servers to be launch at boot time by the
 *    'system' service) so that the server can access and spawn them.
 * 5) reserve a thread.
 * 6) set up the thread stack.
 * 7) initialize the thread context, especially the entry point.
 * 8) builds an arguments array containing the address of the inputs array.
 *    place these arguments so that the thread can retrieve them in the
 *    main() function.
 * 9) set the task as running.
 */

void			kaneton_spawn(void)
{
  i_thread		thread;
  i_region		region;
  t_stack		stack;
  i_task		task;
  t_thread_context	ctx;
  i_as			as;
  o_thread*		o;
  struct
  {
    i_task		task;
    i_as		as;
    int			argc;
    char*		argv;
    char*		envp;
  }			args;

  /*
   * 1)
   */

  assert(_init->scodesz != 0);

  /*
   * 2)
   */

  assert(task_reserve(TASK_CLASS_SERVICE,
		      TASK_BEHAVIOUR_TIMESHARING,
		      TASK_PRIORITY_TIMESHARING,
		      &task) == ERROR_OK);

  assert(as_reserve(task, &as) == ERROR_OK);

  /*
   * 3)
   */

  assert(region_reserve(as,
			_system,
			0,
			REGION_OPTION_FORCE,
			_init->slocation,
			_init->scodesz,
			&region) == ERROR_OK);

  /*
   * 4)
   */

  assert(region_reserve(as,
			(i_segment)(t_vaddr)_init->inputs,
			0,
			REGION_OPTION_FORCE,
			(t_vaddr)_init->inputs,
			_init->inputssz,
			&region) == ERROR_OK);

  /*
   * 5)
   */

  assert(thread_reserve(task, THREAD_PRIORITY, &thread) == ERROR_OK);

  /*
   * 6)
   */

  stack.base = 0;
  stack.size = THREAD_STACKSZ;

  assert(thread_stack(thread, stack) == ERROR_OK);

  /*
   * 7)
   */

  assert(thread_get(thread, &o) == ERROR_OK);

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = _init->sentry;

  assert(thread_load(thread, ctx) == ERROR_OK);

  /*
   * 8)
   */

  args.task = task;
  args.as = as;
  args.argc = 1;
  args.argv = (char*)_init->inputs;
  args.envp = NULL;

  assert(thread_args(thread, &args, sizeof (args)) == ERROR_OK);

  /*
   * 9)
   */

  assert(task_run(task) == ERROR_OK);
}
