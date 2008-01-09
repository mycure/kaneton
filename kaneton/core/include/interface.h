/*
 * project       kaneton
 *
 * generated by gen_interface.rb
 *
 */

#ifndef CORE_INTERFACE_H
#define CORE_INTERFACE_H	1

/* [cut] */
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
#define INTERFACE_AS_ATTRIBUTE_TSKID 7
#define INTERFACE_EVENT_RESERVE 8
#define INTERFACE_EVENT_RELEASE 9
#define INTERFACE_EVENT_ATTRIBUTE_TYPE 10
#define INTERFACE_EVENT_ATTRIBUTE_HANDLER 11
#define INTERFACE_EVENT_ATTRIBUTE_DATA 12
#define INTERFACE_IO_GRANT 13
#define INTERFACE_IO_DENY 14
#define INTERFACE_IO_READ_8 15
#define INTERFACE_IO_READ_16 16
#define INTERFACE_IO_READ_32 17
#define INTERFACE_IO_READ_64 18
#define INTERFACE_IO_WRITE_8 19
#define INTERFACE_IO_WRITE_16 20
#define INTERFACE_IO_WRITE_32 21
#define INTERFACE_IO_WRITE_64 22
#define INTERFACE_MAP_RESERVE 23
#define INTERFACE_MAP_RELEASE 24
#define INTERFACE_MAP_RESIZE 25
#define INTERFACE_REGION_RESERVE 26
#define INTERFACE_REGION_RELEASE 27
#define INTERFACE_SCHEDULER_QUANTUM 28
#define INTERFACE_SCHEDULER_YIELD 29
#define INTERFACE_SCHEDULER_CURRENT 30
#define INTERFACE_SCHEDULER_ADD 31
#define INTERFACE_SCHEDULER_REMOVE 32
#define INTERFACE_SCHEDULER_UPDATE 33
#define INTERFACE_SEGMENT_CLONE 34
#define INTERFACE_SEGMENT_GIVE 35
#define INTERFACE_SEGMENT_COPY 36
#define INTERFACE_SEGMENT_RESERVE 37
#define INTERFACE_SEGMENT_RELEASE 38
#define INTERFACE_SEGMENT_CATCH 39
#define INTERFACE_SEGMENT_PERMS 40
#define INTERFACE_SEGMENT_TYPE 41
#define INTERFACE_SEGMENT_ATTRIBUTE_ASID 42
#define INTERFACE_SEGMENT_ATTRIBUTE_TYPE 43
#define INTERFACE_SEGMENT_ATTRIBUTE_ADDRESS 44
#define INTERFACE_SEGMENT_ATTRIBUTE_SIZE 45
#define INTERFACE_SEGMENT_ATTRIBUTE_PERMS 46
#define INTERFACE_TASK_CURRENT 47
#define INTERFACE_TASK_CLONE 48
#define INTERFACE_TASK_RESERVE 49
#define INTERFACE_TASK_RELEASE 50
#define INTERFACE_TASK_PRIORITY 51
#define INTERFACE_TASK_STATE 52
#define INTERFACE_TASK_WAIT 53
#define INTERFACE_TASK_ATTRIBUTE_PARENT 54
#define INTERFACE_TASK_ATTRIBUTE_CLASS 55
#define INTERFACE_TASK_ATTRIBUTE_BEHAV 56
#define INTERFACE_TASK_ATTRIBUTE_PRIOR 57
#define INTERFACE_TASK_ATTRIBUTE_ASID 58
#define INTERFACE_TASK_ATTRIBUTE_SCHED 59
#define INTERFACE_TASK_ATTRIBUTE_WAIT 60
#define INTERFACE_THREAD_GIVE 61
#define INTERFACE_THREAD_CLONE 62
#define INTERFACE_THREAD_RESERVE 63
#define INTERFACE_THREAD_RELEASE 64
#define INTERFACE_THREAD_PRIORITY 65
#define INTERFACE_THREAD_STATE 66
#define INTERFACE_THREAD_STACK 67
#define INTERFACE_THREAD_LOAD 68
#define INTERFACE_THREAD_STORE 69
#define INTERFACE_THREAD_ATTRIBUTE_TASKID 70
#define INTERFACE_THREAD_ATTRIBUTE_PRIOR 71
#define INTERFACE_THREAD_ATTRIBUTE_SCHED 72
#define INTERFACE_THREAD_ATTRIBUTE_WAIT 73
#define INTERFACE_THREAD_ATTRIBUTE_STACK 74
#define INTERFACE_THREAD_ATTRIBUTE_STACKSZ 75
#define INTERFACE_TIMER_RESERVE 76
#define INTERFACE_TIMER_RELEASE 77
#define INTERFACE_TIMER_DELAY 78
#define INTERFACE_TIMER_REPEAT 79
#define INTERFACE_TIMER_MODIFY 80
#define INTERFACE_TIMER_ATTRIBUTE_DELAY 81
#define INTERFACE_TIMER_ATTRIBUTE_REPEAT 82
#define INTERFACE_TIMER_ATTRIBUTE_TYPE 83
#define INTERFACE_TIMER_ATTRIBUTE_HANDLER 84
#define INTERFACE_TIMER_ATTRIBUTE_DATA 85


#define INTERFACE_NSYSCALLS 86

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_id			id;
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
	}		as_attribute_tskid;
	struct
	{
	  t_id	arg1;
	  t_type	arg2;
	  u_event_handler	arg3;
	  t_vaddr	arg4;
	}		event_reserve;
	struct
	{
	  t_id	arg1;
	}		event_release;
	struct
	{
	  t_id	arg1;
	}		event_attribute_type;
	struct
	{
	  t_id	arg1;
	}		event_attribute_handler;
	struct
	{
	  t_id	arg1;
	}		event_attribute_data;
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
	}		segment_attribute_asid;
	struct
	{
	  t_id	arg1;
	}		segment_attribute_type;
	struct
	{
	  t_id	arg1;
	}		segment_attribute_address;
	struct
	{
	  t_id	arg1;
	}		segment_attribute_size;
	struct
	{
	  t_id	arg1;
	}		segment_attribute_perms;
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
	}		task_attribute_parent;
	struct
	{
	  t_id	arg1;
	}		task_attribute_class;
	struct
	{
	  t_id	arg1;
	}		task_attribute_behav;
	struct
	{
	  t_id	arg1;
	}		task_attribute_prior;
	struct
	{
	  t_id	arg1;
	}		task_attribute_asid;
	struct
	{
	  t_id	arg1;
	}		task_attribute_sched;
	struct
	{
	  t_id	arg1;
	}		task_attribute_wait;
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
	}		thread_attribute_taskid;
	struct
	{
	  t_id	arg1;
	}		thread_attribute_prior;
	struct
	{
	  t_id	arg1;
	}		thread_attribute_sched;
	struct
	{
	  t_id	arg1;
	}		thread_attribute_wait;
	struct
	{
	  t_id	arg1;
	}		thread_attribute_stack;
	struct
	{
	  t_id	arg1;
	}		thread_attribute_stacksz;
	struct
	{
	  t_type	arg1;
	  u_timer_handler	arg2;
	  t_vaddr	arg3;
	  t_uint32	arg4;
	  t_uint32	arg5;
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
	}		timer_attribute_delay;
	struct
	{
	  t_id	arg1;
	}		timer_attribute_repeat;
	struct
	{
	  t_id	arg1;
	}		timer_attribute_type;
	struct
	{
	  t_id	arg1;
	}		timer_attribute_handler;
	struct
	{
	  t_id	arg1;
	}		timer_attribute_data;
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
	  i_task	result1;
	}		as_attribute_tskid;
	struct
	{
	  t_type	result1;
	}		event_attribute_type;
	struct
	{
	  u_event_handler	result1;
	}		event_attribute_handler;
	struct
	{
	  t_vaddr	result1;
	}		event_attribute_data;
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
	  i_as	result1;
	}		segment_attribute_asid;
	struct
	{
	  t_type	result1;
	}		segment_attribute_type;
	struct
	{
	  t_paddr	result1;
	}		segment_attribute_address;
	struct
	{
	  t_psize	result1;
	}		segment_attribute_size;
	struct
	{
	  t_perms	result1;
	}		segment_attribute_perms;
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
	  i_task	result1;
	}		task_attribute_parent;
	struct
	{
	  t_class	result1;
	}		task_attribute_class;
	struct
	{
	  t_behav	result1;
	}		task_attribute_behav;
	struct
	{
	  t_prior	result1;
	}		task_attribute_prior;
	struct
	{
	  i_as	result1;
	}		task_attribute_asid;
	struct
	{
	  t_state	result1;
	}		task_attribute_sched;
	struct
	{
	  t_wait	result1;
	}		task_attribute_wait;
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
	  i_task	result1;
	}		thread_attribute_taskid;
	struct
	{
	  t_prior	result1;
	}		thread_attribute_prior;
	struct
	{
	  t_state	result1;
	}		thread_attribute_sched;
	struct
	{
	  t_wait	result1;
	}		thread_attribute_wait;
	struct
	{
	  t_vaddr	result1;
	}		thread_attribute_stack;
	struct
	{
	  t_vsize	result1;
	}		thread_attribute_stacksz;
	struct
	{
	  i_timer	result1;
	}		timer_reserve;
	struct
	{
	  t_uint32	result1;
	}		timer_attribute_delay;
	struct
	{
	  t_uint32	result1;
	}		timer_attribute_repeat;
	struct
	{
	  t_type	result1;
	}		timer_attribute_type;
	struct
	{
	  u_timer_handler	result1;
	}		timer_attribute_handler;
	struct
	{
	  t_vaddr	result1;
	}		timer_attribute_data;
      } u;
    } reply;
  } u;
}			o_syscall;

typedef t_error (*t_interface_dispatch)(o_syscall*);

/* [/cut] */

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

t_error		interface_as_attribute_tskid(o_syscall*	message);

t_error		interface_event_reserve(o_syscall*	message);

t_error		interface_event_release(o_syscall*	message);

t_error		interface_event_attribute_type(o_syscall*	message);

t_error		interface_event_attribute_handler(o_syscall*	message);

t_error		interface_event_attribute_data(o_syscall*	message);

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

t_error		interface_region_reserve(o_syscall*	message);

t_error		interface_region_release(o_syscall*	message);

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

t_error		interface_segment_attribute_asid(o_syscall*	message);

t_error		interface_segment_attribute_type(o_syscall*	message);

t_error		interface_segment_attribute_address(o_syscall*	message);

t_error		interface_segment_attribute_size(o_syscall*	message);

t_error		interface_segment_attribute_perms(o_syscall*	message);

t_error		interface_task_current(o_syscall*	message);

t_error		interface_task_clone(o_syscall*	message);

t_error		interface_task_reserve(o_syscall*	message);

t_error		interface_task_release(o_syscall*	message);

t_error		interface_task_priority(o_syscall*	message);

t_error		interface_task_state(o_syscall*	message);

t_error		interface_task_wait(o_syscall*	message);

t_error		interface_task_attribute_parent(o_syscall*	message);

t_error		interface_task_attribute_class(o_syscall*	message);

t_error		interface_task_attribute_behav(o_syscall*	message);

t_error		interface_task_attribute_prior(o_syscall*	message);

t_error		interface_task_attribute_asid(o_syscall*	message);

t_error		interface_task_attribute_sched(o_syscall*	message);

t_error		interface_task_attribute_wait(o_syscall*	message);

t_error		interface_thread_give(o_syscall*	message);

t_error		interface_thread_clone(o_syscall*	message);

t_error		interface_thread_reserve(o_syscall*	message);

t_error		interface_thread_release(o_syscall*	message);

t_error		interface_thread_priority(o_syscall*	message);

t_error		interface_thread_state(o_syscall*	message);

t_error		interface_thread_stack(o_syscall*	message);

t_error		interface_thread_load(o_syscall*	message);

t_error		interface_thread_store(o_syscall*	message);

t_error		interface_thread_attribute_taskid(o_syscall*	message);

t_error		interface_thread_attribute_prior(o_syscall*	message);

t_error		interface_thread_attribute_sched(o_syscall*	message);

t_error		interface_thread_attribute_wait(o_syscall*	message);

t_error		interface_thread_attribute_stack(o_syscall*	message);

t_error		interface_thread_attribute_stacksz(o_syscall*	message);

t_error		interface_timer_reserve(o_syscall*	message);

t_error		interface_timer_release(o_syscall*	message);

t_error		interface_timer_delay(o_syscall*	message);

t_error		interface_timer_repeat(o_syscall*	message);

t_error		interface_timer_modify(o_syscall*	message);

t_error		interface_timer_attribute_delay(o_syscall*	message);

t_error		interface_timer_attribute_repeat(o_syscall*	message);

t_error		interface_timer_attribute_type(o_syscall*	message);

t_error		interface_timer_attribute_handler(o_syscall*	message);

t_error		interface_timer_attribute_data(o_syscall*	message);

t_error			interface_notify(t_uint8*		buffer,
					 t_vsize		size,
					 i_node			source);

t_error			interface_initialize(void);

t_error			interface_clean(void);


/*
 * eop
 */

#endif
