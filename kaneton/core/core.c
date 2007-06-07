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

#include <klibc.h>
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

t_init*			init;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function simply initialises and cleans the kernel manager.
 *
 * steps:
 *
 * 1) sets the init variable from the bootloader argument.
 * 2) initialises the console manager.
 * 3) displays the current kaneton version.
 * 4) if needed, displays the kernel parameters.
 * 5) initialises the fine grained allocator.
 * 6) initializes the kernel manager.
 * 7) performs the kernel work...
 * 8) cleans the kernel manager.
 * 9) shutdown the system.
 */

void			kaneton(t_init*				bootloader)
{
  /*
   * 1)
   */

#ifdef O2
  io_cons_init();

  printf("\n");
  printf("%s\n", version);
  printf("\n");
  mipsr10k_event_init();
  while(1);
#endif

  init = bootloader;

  /*
   * 2)
   */

  if (cons_init() != ERROR_NONE)
    core_error("cannot initialise the console manager\n");

  /*
   * 3)
   */

  printf("\n");
  cons_msg('+', "%s\n", version);
  printf("\n");

  /*
   * 4)
   */

#if (DEBUG & DEBUG_PARAMS)
  kaneton_dump();
#endif

  /*
   * 5)
   */

  cons_msg('+', "starting malloc\n");

  alloc_init(init->alloc, init->allocsz);

  /*
   * 6)
   */

  cons_msg('+', "starting kernel manager\n");

  kernel_init();

  /*
   * 7)
   */

  cons_msg('+', "kaneton started\n");


#ifdef CONF_ENABLE_CHECK
  cons_msg('+', "running manual tests\n");
  check_tests();
  while(1)
    ;
#endif

#ifdef SERIAL
  cons_msg('+', "starting debug manager\n");
  debug_init();
  while(1)
    ;
#endif

#ifdef GDB_STUB
  cons_msg('+', "stating gdb manager\n");
  gdb_init();
#endif

#if (DEBUG & DEBUG_CPU)
  smp_test();
#endif

  cons_msg('+', "starting modules\n");
  if (mod_launch() != ERROR_NONE)
    cons_msg('!', "failed to start initial modules\n");

#if (DEBUG & DEBUG_MESSAGE)
  message_test();
#endif

  while(1)
    interface_loop();

  cons_msg('#', "kaneton is stopping...\n");

  /*
   * 8)
   */

  kernel_clean();

  /*
   * 9)
   */

  cons_msg('+', "system shutdown\n");

  while (1)
    ;
}

/*
 * this function displays the kaneton parameters.
 */

void			kaneton_dump(void)
{
  cons_msg('#', "dumping kernel parameters:\n");

#if defined(___bootloader)
  cons_msg('#', " mode: bootloader\n");
  core_error("kernel compiled in a bad way where ___bootloader"
		"is defined\n");
#elif defined(___kernel)
  cons_msg('#', " mode: kernel\n");
#else
  core_error("no mode defined\n");
#endif

#if defined(___kaneton)
  cons_msg('#', " name: kaneton\n");
#else
  core_error("___kaneton not defined\n");
#endif

#if (___wordsz == WORDSZ_8)
  cons_msg('#', " wordsize: 8-bit\n");
#elif (___wordsz == WORDSZ_16)
  cons_msg('#', " wordsize: 16-bit\n");
#elif (___wordsz == WORDSZ_32)
  cons_msg('#', " wordsize: 32-bit\n");
#elif (___wordsz == WORDSZ_64)
  cons_msg('#', " wordsize: 64-bit\n");
#elif (___wordsz == WORDSZ_128)
  cons_msg('#', " wordsize: 128-bit\n");
#else
  core_error("no word size defined\n");
#endif

#if (___endian == ENDIAN_LITTLE)
  cons_msg('#', " endian: little\n");
#elif (___endian == ENDIAN_BIG)
  cons_msg('#', " endian: big\n");
#else
  core_error("no endian defined\n");
#endif
}
