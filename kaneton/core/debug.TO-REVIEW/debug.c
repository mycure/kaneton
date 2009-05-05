/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/debug/debug.c
 *
 * created       julien quintard   [sat may 28 18:23:13 2005]
 * updated       matthieu bucchianeri   [sun may 27 21:14:51 2007]
 */

/*
 *  this file must not be used by students.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>
#include <core/debug.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function executes the right function.
 *
 * steps:
 *
 * 1) parse the function address.
 * 2) setup serial output.
 * 3) execute the function.
 * 4) set back the console output.
 */

static t_error		debug_exec_cmd_tab(t_serial_data *cmd)
{
  int (*func)(void);

  /*
   * 1)
   */

  *strchr((char*)cmd->data, '/') = 0;
  func = (int (*)(void))strtol((char*)cmd->data + strlen((char*)cmd->data) + 1,
			       0, 16);
  cons_msg('+', "running test %s @ 0x%p\n", (char*)cmd->data, func);

  /*
   * 2)
   */

#ifdef IA32_DEPENDENT
  printf_init(serial_put, 0);
#endif

  /*
   * 3)
   */

#ifdef IA32_DEPENDENT
  STI(); /* XXX this is machdep */
#endif

  func();

  /*
   * 4)
   */

#ifndef MIPS64_DEPENDENT

  printf_init(cons_print_char, cons_attr);

#endif

  return (ERROR_NONE);
}

/*
 * this function receives and parses the commands.
 *
 * steps:
 *
 * 1) receive the buffer.
 * 2) execute the command.
 * 3) release memory.
 */

static t_error		debug_recv(void)
{
  t_serial_data		recv_type;

  /*
   * 1)
   */

#ifdef IA32_DEPENDENT
  serial_recv(SERIAL_PRIMARY, &recv_type);
#endif

  if (!strcmp((char*)recv_type.data, "command"))
    {
#ifdef IA32_DEPENDENT
      t_serial_data		cmd;

      /*
       * 2)
       */
      serial_recv(SERIAL_PRIMARY, (t_serial_data  *) &cmd);
      debug_exec_cmd_tab(&cmd);
      free(cmd.data);
      serial_put(-1);
      serial_send(SERIAL_PRIMARY, (t_uint8*)"endprintf", 9);
#endif
    }
  else
    printf("debug receive unknown type\n");

  /*
   * 3)
   */

  free(recv_type.data);

  return (ERROR_NONE);
}

/*
 * this function just initializes the debug manager.
 *
 * steps:
 *
 * 1) initialize the serial port.
 * 2) send the ready command.
 * 3) receive and parse commands.
 */

t_error			debug_initialize(void)
{
  /*
   * 1)
   */

#ifdef IA32_DEPENDENT
  serial_init(SERIAL_PRIMARY, SERIAL_BR57600, SERIAL_8N1);
#endif
  printf("serial port initialized\n");

  /*
   * 2)
   */

#ifdef IA32_DEPENDENT
  serial_send(SERIAL_PRIMARY, (t_uint8*)"Ready!", 6);
#endif

  /*
   * 3)
   */

  while(1)
    debug_recv();

  return (ERROR_NONE);
}

/*
 * this function cleans the debug manager.
 */

t_error			debug_clean(void)
{
  return (ERROR_NONE);
}