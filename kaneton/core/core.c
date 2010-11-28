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
 * ---------- XXX -------------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- macro-functions -------------------------------------------------
 */

#define TEST_ENTER()

#define TEST_ERROR(_format_, _arguments_...)				\
  do									\
    {									\
      module_call(report, report_dump);					\
									\
      printf(_format_ "\n", ##_arguments_);				\
									\
      TEST_LEAVE();							\
    } while (0)

#define TEST_LEAVE()							\
  return;

void			thread1(void)
{
  t_clock		clock;
  t_uint64		past;
  t_uint64		current;

  clock_current(&clock);
  past = CLOCK_UNIQUE(&clock);

  while (1)
    {
      printf("[thread1]\n");

      int i;
      for (i = 0; i < 2222222; i++)
	;

      /*
      if (thread_sleep(3000) != ERROR_OK)
	{
	  printf("ERROR\n");
	  module_report_dump();
	}

      clock_current(&clock);
      current = CLOCK_UNIQUE(&clock);

      printf("[difference] %d\n", current - past);

      past = current;
      */

      //thread_wait(
    }
}

void			thread2(void)
{
  t_clock		clock;
  t_uint64		past;
  t_uint64		current;

  clock_current(&clock);
  past = CLOCK_UNIQUE(&clock);

  while (1)
    {
      printf("[thread2]\n");

      int i;
      for (i = 0; i < 4444444; i++)
	;
      for (i = 0; i < 4444444; i++)
	;

      i_thread id;
      scheduler_current(&id);
      printf("DYING\n");
      thread_die(id);
    }
}

void			THREAD(void* p)
{
  i_thread		thread;
  o_thread*		o;
  t_thread_context	ctx;
  t_stack		stack;
  t_ia32_context	ia32_ctx;

  TEST_ENTER();

  if (thread_reserve(_kernel->task, THREAD_PRIORITY, &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error\n");

  stack.base = 0;
  stack.size = THREAD_LSTACKSZ;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error\n");

  if (thread_get(thread, &o) != ERROR_OK)
    TEST_ERROR("[thread_get] error\n");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)p;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error\n");

  if (ia32_get_context(thread, &ia32_ctx) != ERROR_OK)
    TEST_ERROR("[ia32_get_context] error\n");

  ia32_ctx.eflags |= (1 << 12);
  ia32_ctx.eflags |= (1 << 13);

  if (ia32_set_context(thread, &ia32_ctx, IA32_CONTEXT_EFLAGS) != ERROR_OK)
    TEST_ERROR("[ia32_set_context] error\n");

  if (thread_run(thread) != ERROR_OK)
    TEST_ERROR("[thread_run] error\n");

  TEST_LEAVE();
}

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

  assert(kernel_initialize() == ERROR_OK);

  // XXX
  THREAD(thread1);
  THREAD(thread2);

  if (scheduler_start() != ERROR_OK)
    TEST_ERROR("[scheduler_start] error\n");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error\n");
  // XXX

  /*
   * 6)
   */

  module_call(test, test_run);

  /*
   * 7)
   */

  module_call(console, console_message,
	      '+', "spawning the 'system' server\n");

  assert(kaneton_spawn() == ERROR_OK);

  /*
   * 8)
   */

  module_call(console, console_message,
	      '+', "stopping the kernel\n");

  assert(kernel_clean() == ERROR_OK);

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

t_error			kaneton_spawn(void)
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

  if (_init->scodesz == 0)
    CORE_ESCAPE("there is no 'system' server to launch");

  /*
   * 2)
   */

  if (task_reserve(TASK_CLASS_SERVICE,
		   TASK_BEHAVIOUR_TIMESHARING,
		   TASK_PRIORITY_TIMESHARING,
		   &task) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a task");

  if (as_reserve(task, &as) != ERROR_OK)
    CORE_ESCAPE("unable to reserve an address space");

  /*
   * 3)
   */

  if (region_reserve(as,
		     _system,
		     0,
		     REGION_OPTION_FORCE,
		     _init->slocation,
		     _init->scodesz,
		     &region) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the region for the server code");

  /*
   * 4)
   */

  if (region_reserve(as,
		     (i_segment)(t_vaddr)_init->inputs,
		     0,
		     REGION_OPTION_FORCE,
		     (t_vaddr)_init->inputs,
		     _init->inputssz,
		     &region) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the region for the inputs");

  /*
   * 5)
   */

  if (thread_reserve(task, THREAD_PRIORITY, &thread) != ERROR_OK)
    CORE_ESCAPE("unable to reserve a thread");

  /*
   * 6)
   */

  stack.base = 0;
  stack.size = THREAD_STACKSZ;

  if (thread_stack(thread, stack) != ERROR_OK)
    CORE_ESCAPE("unable to set up the thread's stack");

  /*
   * 7)
   */

  if (thread_get(thread, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the thread object");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = _init->sentry;

  if (thread_load(thread, ctx) != ERROR_OK)
    CORE_ESCAPE("unable to load the initial thread's context");

  /*
   * 8)
   */

  args.task = task;
  args.as = as;
  args.argc = 1;
  args.argv = (char*)_init->inputs;
  args.envp = NULL;

  if (thread_args(thread, &args, sizeof (args)) != ERROR_OK)
    CORE_ESCAPE("unable to pass arguments to the thread");

  /*
   * 9)
   */

  if (task_run(task) != ERROR_OK)
    CORE_ESCAPE("unable to start the task");

  CORE_LEAVE();
}
