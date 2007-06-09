
#
# core interface.c header
#

def header_kinterface(f)
  f.puts "/*
 * project       kaneton
 *
 * generated by gen_interface.rb
 *
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file describes the core's interface.
 *
 * in other words, this file describes the services provided by the kernel
 * so the core functions a task is allowed to use.
 *
 * each time a message is destinated to the core, the corresponding
 * function of the interface is launched.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_task		ktask;

/*
 * ---------- functions -------------------------------------------------------
 */

"
end

#
# core interface.c footer
#

def footer_kinterface(f)
  f.puts "/*
 * this function receives, dispatch and reply incoming system calls.
 */

void			interface_loop(void)
{
  o_syscall		message;

  if (message_async_recv(ktask,
			 0,
			 &message,
			 sizeof (message)) == ERROR_NONE)
    {
      if (message.u.request.operation < INTERFACE_NSYSCALLS)
        {
          dispatch[message.u.request.operation](&message);

          if (message_async_send(ktask,
			         message.node,
			         0,
			         &message,
			         sizeof (message)) != ERROR_NONE)
	    {
	      cons_msg('!', \"error in interface, async_send\\n\");
	    }
        }
    }
}"
end


#
# core interface.h header
#

def header_hinterface(f)
  f.puts "/*
 * project       kaneton
 *
 * generated by gen_interface.rb
 *
 */

#ifndef CORE_INTERFACE_H
#define CORE_INTERFACE_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/id.h>
#include <core/types.h>

"
end

#
# core interface.h footer
#

def footer_hinterface(f)
  f.puts "/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/interface/interface.c
 */


/*
 * eop
 */

#endif"
end

#
# user interface.c header
#

def header_uinterface(f)
  f.puts "/*
 * project       kaneton
 *
 * generated by gen_interface.rb
 *
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements user side of system calls.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

"
end

#
# user interface.c footer
#

def footer_uinterface(f)
  f.puts ""
end
