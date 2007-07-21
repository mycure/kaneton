/*
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

#include <core/id.h>
#include <core/types.h>

#include <machine/machine.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define INTERFACE_AS_GIVE 0
#define INTERFACE_AS_VADDR 1
#define INTERFACE_AS_PADDR 2
#define INTERFACE_AS_COPY 3
#define INTERFACE_AS_CLONE 4
#define INTERFACE_AS_RESERVE 5
#define INTERFACE_AS_RELEASE 6
#define INTERFACE_AS_GET 7
#define INTERFACE_CAPABILITY_RESERVE 8
#define INTERFACE_CAPABILITY_RELEASE 9
#define INTERFACE_CAPABILITY_RESTRICT 10
#define INTERFACE_CAPABILITY_INVALIDATE 11
#define INTERFACE_CAPABILITY_GET 12
#define INTERFACE_CAPABILITY_GIVE 13
#define INTERFACE_CAPABILITY_VERIFY 14
#define INTERFACE_EVENT_RESERVE 15
#define INTERFACE_EVENT_RELEASE 16
#define INTERFACE_EVENT_GET 17
#define INTERFACE_IO_GRANT 18
#define INTERFACE_IO_DENY 19
#define INTERFACE_IO_READ_8 20
#define INTERFACE_IO_READ_16 21
#define INTERFACE_IO_READ_32 22
#define INTERFACE_IO_READ_64 23
#define INTERFACE_IO_WRITE_8 24
#define INTERFACE_IO_WRITE_16 25
#define INTERFACE_IO_WRITE_32 26
#define INTERFACE_IO_WRITE_64 27
#define INTERFACE_MAP_RESERVE 28
#define INTERFACE_MAP_RELEASE 29
#define INTERFACE_MAP_RESIZE 30
#define INTERFACE_MESSAGE_REGISTER 31
#define INTERFACE_REGION_RESERVE 32
#define INTERFACE_REGION_RELEASE 33
#define INTERFACE_REGION_GET 34
#define INTERFACE_SCHEDULER_QUANTUM 35
#define INTERFACE_SCHEDULER_YIELD 36
#define INTERFACE_SCHEDULER_CURRENT 37
#define INTERFACE_SCHEDULER_ADD 38
#define INTERFACE_SCHEDULER_REMOVE 39
#define INTERFACE_SCHEDULER_UPDATE 40
#define INTERFACE_SEGMENT_CLONE 41
#define INTERFACE_SEGMENT_GIVE 42
#define INTERFACE_SEGMENT_COPY 43
#define INTERFACE_SEGMENT_RESERVE 44
#define INTERFACE_SEGMENT_RELEASE 45
#define INTERFACE_SEGMENT_CATCH 46
#define INTERFACE_SEGMENT_PERMS 47
#define INTERFACE_SEGMENT_TYPE 48
#define INTERFACE_SEGMENT_GET 49
#define INTERFACE_TASK_CURRENT 50
#define INTERFACE_TASK_CLONE 51
#define INTERFACE_TASK_RESERVE 52
#define INTERFACE_TASK_RELEASE 53
#define INTERFACE_TASK_PRIORITY 54
#define INTERFACE_TASK_STATE 55
#define INTERFACE_TASK_WAIT 56
#define INTERFACE_TASK_GET 57
#define INTERFACE_THREAD_GIVE 58
#define INTERFACE_THREAD_CLONE 59
#define INTERFACE_THREAD_RESERVE 60
#define INTERFACE_THREAD_RELEASE 61
#define INTERFACE_THREAD_PRIORITY 62
#define INTERFACE_THREAD_STATE 63
#define INTERFACE_THREAD_STACK 64
#define INTERFACE_THREAD_LOAD 65
#define INTERFACE_THREAD_STORE 66
#define INTERFACE_THREAD_GET 67
#define INTERFACE_TIMER_RESERVE 68
#define INTERFACE_TIMER_RELEASE 69
#define INTERFACE_TIMER_DELAY 70
#define INTERFACE_TIMER_REPEAT 71
#define INTERFACE_TIMER_MODIFY 72
#define INTERFACE_TIMER_GET 73


#define INTERFACE_NSYSCALLS 74

/*
 * ---------- types -----------------------------------------------------------
 */

typedef t_error (*t_interface_dispatch)(void*);

typedef struct
{
  t_id			id;
  i_node		node;
  union
  {
    struct
    {
      t_operations	operation;
      t_capability	capability;
      union
      {
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	}		as_give;
	struct
	{
	  t_id	arg1;
	  t_paddr	arg2;
	}		as_vaddr;
	struct
	{
	  t_id	arg1;
	  t_vaddr	arg2;
	}		as_paddr;
	struct
	{
	  t_id	arg1;
	  t_vaddr	arg2;
	  i_as	arg3;
	  t_vaddr	arg4;
	  t_vsize	arg5;
	}		as_copy;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	}		as_clone;
	struct
	{
	  i_task	arg1;
	}		as_reserve;
	struct
	{
	  t_id	arg1;
	}		as_release;
	struct
	{
	  t_id	arg1;
	}		as_get;
	struct
	{
	  t_id	arg1;
	  t_operations	arg2;
	}		capability_reserve;
	struct
	{
	  t_id	arg1;
	}		capability_release;
	struct
	{
	  t_id	arg1;
	  t_operations	arg2;
	}		capability_restrict;
	struct
	{
	  t_id	arg1;
	  t_id	arg2;
	}		capability_invalidate;
	struct
	{
	  t_id	arg1;
	}		capability_get;
	struct
	{
	  t_id	arg1;
	  i_node	arg2;
	}		capability_give;
	struct
	{
	  t_capability	arg1;
	}		capability_verify;
	struct
	{
	  t_id	arg1;
	  t_type	arg2;
	  u_event_handler	arg3;
	}		event_reserve;
	struct
	{
	  t_id	arg1;
	}		event_release;
	struct
	{
	  t_id	arg1;
	}		event_get;
	struct
	{
	  t_id	arg1;
	  i_task	arg2;
	  t_uint8	arg3;
	}		io_grant;
	struct
	{
	  t_id	arg1;
	  i_task	arg2;
	  t_uint8	arg3;
	}		io_deny;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	}		io_read_8;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	}		io_read_16;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	}		io_read_32;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	}		io_read_64;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	  t_uint8	arg3;
	}		io_write_8;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	  t_uint16	arg3;
	}		io_write_16;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	  t_uint32	arg3;
	}		io_write_32;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	  t_uint64	arg3;
	}		io_write_64;
	struct
	{
	  t_id	arg1;
	  t_opts	arg2;
	  t_vsize	arg3;
	  t_perms	arg4;
	}		map_reserve;
	struct
	{
	  t_id	arg1;
	  t_vaddr	arg2;
	}		map_release;
	struct
	{
	  t_id	arg1;
	  t_vaddr	arg2;
	  t_vsize	arg3;
	}		map_resize;
	struct
	{
	  i_task	arg1;
	  t_tag	arg2;
	}		message_register;
	struct
	{
	  t_id	arg1;
	  i_segment	arg2;
	  t_paddr	arg3;
	  t_opts	arg4;
	  t_vaddr	arg5;
	  t_vsize	arg6;
	}		region_reserve;
	struct
	{
	  i_as	arg1;
	  t_id	arg2;
	}		region_release;
	struct
	{
	  i_as	arg1;
	  t_id	arg2;
	}		region_get;
	struct
	{
	  t_quantum	arg1;
	}		scheduler_quantum;
	struct
	{
	  i_cpu	arg1;
	}		scheduler_yield;
	struct
	{
	  i_thread	arg1;
	}		scheduler_add;
	struct
	{
	  i_thread	arg1;
	}		scheduler_remove;
	struct
	{
	  i_thread	arg1;
	}		scheduler_update;
	struct
	{
	  i_as	arg1;
	  t_id	arg2;
	}		segment_clone;
	struct
	{
	  i_as	arg1;
	  t_id	arg2;
	}		segment_give;
	struct
	{
	  t_id	arg1;
	  t_paddr	arg2;
	  i_segment	arg3;
	  t_paddr	arg4;
	  t_psize	arg5;
	}		segment_copy;
	struct
	{
	  i_as	arg1;
	  t_psize	arg2;
	  t_perms	arg3;
	}		segment_reserve;
	struct
	{
	  i_segment	arg1;
	}		segment_release;
	struct
	{
	  i_as	arg1;
	  t_id	arg2;
	}		segment_catch;
	struct
	{
	  t_id	arg1;
	  t_perms	arg2;
	}		segment_perms;
	struct
	{
	  t_id	arg1;
	  t_type	arg2;
	}		segment_type;
	struct
	{
	  t_id	arg1;
	}		segment_get;
	struct
	{
	  t_id	arg1;
	}		task_clone;
	struct
	{
	  t_class	arg1;
	  t_behav	arg2;
	  t_prior	arg3;
	}		task_reserve;
	struct
	{
	  t_id	arg1;
	}		task_release;
	struct
	{
	  t_id	arg1;
	  t_prior	arg2;
	}		task_priority;
	struct
	{
	  t_id	arg1;
	  t_state	arg2;
	}		task_state;
	struct
	{
	  t_id	arg1;
	  t_opts	arg2;
	}		task_wait;
	struct
	{
	  t_id	arg1;
	}		task_get;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	}		thread_give;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	}		thread_clone;
	struct
	{
	  i_task	arg1;
	  t_prior	arg2;
	}		thread_reserve;
	struct
	{
	  t_id	arg1;
	}		thread_release;
	struct
	{
	  t_id	arg1;
	  t_prior	arg2;
	}		thread_priority;
	struct
	{
	  t_id	arg1;
	  t_state	arg2;
	}		thread_state;
	struct
	{
	  t_id	arg1;
	  t_stack	arg2;
	}		thread_stack;
	struct
	{
	  t_id	arg1;
	  t_thread_context	arg2;
	}		thread_load;
	struct
	{
	  t_id	arg1;
	}		thread_store;
	struct
	{
	  t_id	arg1;
	}		thread_get;
	struct
	{
	  t_type	arg1;
	  u_timer_handler	arg2;
	  t_uint32	arg3;
	  t_uint32	arg4;
	}		timer_reserve;
	struct
	{
	  t_id	arg1;
	}		timer_release;
	struct
	{
	  t_id	arg1;
	  t_uint32	arg2;
	}		timer_delay;
	struct
	{
	  t_id	arg1;
	  t_uint32	arg2;
	}		timer_repeat;
	struct
	{
	  t_id	arg1;
	  t_uint32	arg2;
	  t_uint32	arg3;
	}		timer_modify;
	struct
	{
	  t_id	arg1;
	}		timer_get;
      }	u;
    } request;
    struct
    {
      t_error		error;
      union
      {
	struct
	{
	  t_vaddr	result1;
	}		as_vaddr;
	struct
	{
	  t_paddr	result1;
	}		as_paddr;
	struct
	{
	  i_as	result1;
	}		as_clone;
	struct
	{
	  i_as	result1;
	}		as_reserve;
	struct
	{
	  o_as	result1;
	}		as_get;
	struct
	{
	  t_capability	result1;
	}		capability_reserve;
	struct
	{
	  t_capability	result1;
	}		capability_restrict;
	struct
	{
	  t_capability_descriptor	result1;
	}		capability_get;
	struct
	{
	  o_event	result1;
	}		event_get;
	struct
	{
	  t_uint8	result1;
	}		io_read_8;
	struct
	{
	  t_uint16	result1;
	}		io_read_16;
	struct
	{
	  t_uint32	result1;
	}		io_read_32;
	struct
	{
	  t_uint64	result1;
	}		io_read_64;
	struct
	{
	  t_vaddr	result1;
	}		map_reserve;
	struct
	{
	  t_vaddr	result1;
	}		map_resize;
	struct
	{
	  i_region	result1;
	}		region_reserve;
	struct
	{
	  o_region	result1;
	}		region_get;
	struct
	{
	  i_thread	result1;
	}		scheduler_current;
	struct
	{
	  i_segment	result1;
	}		segment_clone;
	struct
	{
	  i_segment	result1;
	}		segment_reserve;
	struct
	{
	  o_segment	result1;
	}		segment_get;
	struct
	{
	  i_task	result1;
	}		task_current;
	struct
	{
	  i_task	result1;
	}		task_clone;
	struct
	{
	  i_task	result1;
	}		task_reserve;
	struct
	{
	  t_wait	result1;
	}		task_wait;
	struct
	{
	  o_task	result1;
	}		task_get;
	struct
	{
	  i_thread	result1;
	}		thread_clone;
	struct
	{
	  i_thread	result1;
	}		thread_reserve;
	struct
	{
	  t_thread_context	result1;
	}		thread_store;
	struct
	{
	  o_thread	result1;
	}		thread_get;
	struct
	{
	  i_timer	result1;
	}		timer_reserve;
	struct
	{
	  o_timer	result1;
	}		timer_get;
      } u;
    } reply;
  } u;
}			o_syscall;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/interface/interface.c
 */

/*
 * ../../core/interface/interface.c
 */

t_error		interface_as_give(o_syscall*	message);

t_error		interface_as_vaddr(o_syscall*	message);

t_error		interface_as_paddr(o_syscall*	message);

t_error		interface_as_copy(o_syscall*	message);

t_error		interface_as_clone(o_syscall*	message);

t_error		interface_as_reserve(o_syscall*	message);

t_error		interface_as_release(o_syscall*	message);

t_error		interface_as_get(o_syscall*	message);

t_error		interface_capability_reserve(o_syscall*	message);

t_error		interface_capability_release(o_syscall*	message);

t_error		interface_capability_restrict(o_syscall*	message);

t_error		interface_capability_invalidate(o_syscall*	message);

t_error		interface_capability_get(o_syscall*	message);

t_error		interface_capability_give(o_syscall*	message);

t_error		interface_capability_verify(o_syscall*	message);

t_error		interface_event_reserve(o_syscall*	message);

t_error		interface_event_release(o_syscall*	message);

t_error		interface_event_get(o_syscall*	message);

t_error		interface_io_grant(o_syscall*	message);

t_error		interface_io_deny(o_syscall*	message);

t_error		interface_io_read_8(o_syscall*	message);

t_error		interface_io_read_16(o_syscall*	message);

t_error		interface_io_read_32(o_syscall*	message);

t_error		interface_io_read_64(o_syscall*	message);

t_error		interface_io_write_8(o_syscall*	message);

t_error		interface_io_write_16(o_syscall*	message);

t_error		interface_io_write_32(o_syscall*	message);

t_error		interface_io_write_64(o_syscall*	message);

t_error		interface_map_reserve(o_syscall*	message);

t_error		interface_map_release(o_syscall*	message);

t_error		interface_map_resize(o_syscall*	message);

t_error		interface_message_register(o_syscall*	message);

t_error		interface_region_reserve(o_syscall*	message);

t_error		interface_region_release(o_syscall*	message);

t_error		interface_region_get(o_syscall*	message);

t_error		interface_scheduler_quantum(o_syscall*	message);

t_error		interface_scheduler_yield(o_syscall*	message);

t_error		interface_scheduler_current(o_syscall*	message);

t_error		interface_scheduler_add(o_syscall*	message);

t_error		interface_scheduler_remove(o_syscall*	message);

t_error		interface_scheduler_update(o_syscall*	message);

t_error		interface_segment_clone(o_syscall*	message);

t_error		interface_segment_give(o_syscall*	message);

t_error		interface_segment_copy(o_syscall*	message);

t_error		interface_segment_reserve(o_syscall*	message);

t_error		interface_segment_release(o_syscall*	message);

t_error		interface_segment_catch(o_syscall*	message);

t_error		interface_segment_perms(o_syscall*	message);

t_error		interface_segment_type(o_syscall*	message);

t_error		interface_segment_get(o_syscall*	message);

t_error		interface_task_current(o_syscall*	message);

t_error		interface_task_clone(o_syscall*	message);

t_error		interface_task_reserve(o_syscall*	message);

t_error		interface_task_release(o_syscall*	message);

t_error		interface_task_priority(o_syscall*	message);

t_error		interface_task_state(o_syscall*	message);

t_error		interface_task_wait(o_syscall*	message);

t_error		interface_task_get(o_syscall*	message);

t_error		interface_thread_give(o_syscall*	message);

t_error		interface_thread_clone(o_syscall*	message);

t_error		interface_thread_reserve(o_syscall*	message);

t_error		interface_thread_release(o_syscall*	message);

t_error		interface_thread_priority(o_syscall*	message);

t_error		interface_thread_state(o_syscall*	message);

t_error		interface_thread_stack(o_syscall*	message);

t_error		interface_thread_load(o_syscall*	message);

t_error		interface_thread_store(o_syscall*	message);

t_error		interface_thread_get(o_syscall*	message);

t_error		interface_timer_reserve(o_syscall*	message);

t_error		interface_timer_release(o_syscall*	message);

t_error		interface_timer_delay(o_syscall*	message);

t_error		interface_timer_repeat(o_syscall*	message);

t_error		interface_timer_modify(o_syscall*	message);

t_error		interface_timer_get(o_syscall*	message);

void			interface_loop(void);


/*
 * eop
 */

#endif
