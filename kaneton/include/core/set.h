/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/set.h
 *
 * created       julien quintard   [sun jun 19 14:51:33 2005]
 * updated       julien quintard   [sun apr  2 13:50:41 2006]
 */

#ifndef CORE_SET_H
#define CORE_SET_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/id.h>
#include <core/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * iterator's state
 */

#define ITERATOR_STATE_USED	0x01
#define ITERATOR_STATE_UNUSED	0x02

/*
 * types
 */

#define SET_TYPE_ARRAY		0x01
#define SET_TYPE_BPT		0x02
#define SET_TYPE_LL		0x03
#define SET_TYPE_PIPE		0x04
#define SET_TYPE_STACK		0x05

/*
 * options
 */

#define SET_OPT_NONE		(0 << 0)
#define SET_OPT_FORWARD		(1 << 0)
#define SET_OPT_BACKWARD	(1 << 1)
#define SET_OPT_CONTAINER	(1 << 2)
#define SET_OPT_ALLOC		(1 << 3)
#define SET_OPT_FREE		(1 << 4)
#define SET_OPT_SORT		(1 << 5)
#define SET_OPT_ORGANISE	(1 << 6)

/*
 * trap debug
 */

#undef SET_DEBUG_TRAP

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * iterator's forward declaration
 */

typedef struct s_iterator	t_iterator;

/*
 * set size
 */

typedef t_sint64		t_setsz;

/*
 * set object's forward declaration
 */

typedef struct s_set		o_set;

/*
 * ---------- includes --------------------------------------------------------
 */

#include <core/set_array.h>
#include <core/set_bpt.h>
#include <core/set_ll.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * iterator
 */

struct				s_iterator
{
  union
  {
    t_iterator_array		array;
/*                                                                  [cut] k2 */
    t_iterator_bpt		bpt;
/*                                                                 [cut] /k2 */
    t_iterator_ll		ll;
  }				u;
};

/*
 * set object
 *
 * the setid field represent the set identifier used to retrieve the set
 * data structure. the union is composed of subparts dependending of the
 * set type.
 */

struct				s_set
{
  t_setid			setid;

  t_setsz			size;

  t_type			type;
  t_opts			opts;

  t_size			datasz;

  union
  {
    t_set_array			array;
/*                                                                  [cut] k2 */
    t_set_bpt			bpt;
/*                                                                 [cut] /k2 */
    t_set_ll			ll;
  }				u;
};

/*
 * set manager
 */

typedef struct
{
  o_id				id;

  t_staid			stats;

  t_id				sets;
  o_set*			container;
}				m_set;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define SET_CHECK(_set_)						\
  {									\
    if ((_set_) == NULL)						\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define SET_ENTER(_set_)						\
  {									\
    SET_CHECK((_set_));							\
									\
    STATS_BEGIN((_set_)->stats);					\
  }

/*
 * leave
 */

#define SET_LEAVE(_set_, _error_)					\
  {									\
    STATS_END((_set_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * debug
 */

#if (DEBUG & DEBUG_SET) && defined(SET_DEBUG_TRAP)

# define set_debug(_func_, _setid_, _args_...)				\
  fprintf(stderr, "[setd] trap: %s(%qu, %s)\n",				\
          #_func_,							\
          _setid_,							\
          #_args_);

#else

# define set_debug(_func_, _setid_, _args...)

#endif

/*
 * traps
 */

#define set_trap(_func_, _setid_, _args_...)				\
  (									\
    {									\
      t_error		_r_ = ERROR_UNKNOWN;				\
      o_set*		_set_;						\
									\
      set_debug(_func_, _setid_, _args_);				\
									\
      if (set_descriptor((_setid_), &_set_) == ERROR_NONE)		\
        {								\
          switch (_set_->type)						\
            {								\
/*                                                          [cut] k2 */	\
              case SET_TYPE_ARRAY:					\
                _r_ = _func_##_array((_setid_), ##_args_);		\
                break;							\
              case SET_TYPE_BPT:					\
                _r_ = _func_##_bpt((_setid_), ##_args_);		\
                break;							\
              case SET_TYPE_LL:						\
                _r_ = _func_##_ll((_setid_), ##_args_);			\
                break;							\
              case SET_TYPE_PIPE:					\
                _r_ = _func_##_pipe((_setid_), ##_args_);		\
                break;							\
              case SET_TYPE_STACK:					\
                _r_ = _func_##_stack((_setid_), ##_args_);		\
                break;							\
/*                                                         [cut] /k2 */	\
            }								\
        }								\
      _r_;								\
    }									\
  )

#define set_type(_type_, _setid_)					\
  set_type_##_type_(_setid_)

#define set_reserve(_type_, _args_...)					\
  set_reserve_##_type_(_args_)

#define set_release(_setid_, _args_...)					\
  set_trap(set_release, _setid_, ##_args_)

#define set_head(_setid_, _args_...)					\
  set_trap(set_head, _setid_, ##_args_)

#define set_tail(_setid_, _args_...)					\
  set_trap(set_tail, _setid_, ##_args_)

#define set_previous(_setid_, _args_...)				\
  set_trap(set_prev, _setid_, ##_args_)

#define set_next(_setid_, _args_...)					\
  set_trap(set_next, _setid_, ##_args_)

#define set_add(_setid_, _args_...)					\
  set_trap(set_add, _setid_, ##_args_)

#define set_remove(_setid_, _args_...)					\
  set_trap(set_remove, _setid_, ##_args_)

#define set_delete(_setid_, _args_...)					\
  set_trap(set_delete, _setid_, ##_args_)

#define set_flush(_setid_, _args_...)					\
  set_trap(set_flush, _setid_, ##_args_)

#define set_insert_head(_setid_, _args_...)				\
  set_trap(set_insert_head, _setid_, ##_args_)

#define set_insert_tail(_setid_, _args_...)				\
  set_trap(set_insert_tail, _setid_, ##_args_)

#define set_insert_before(_setid_, _args_...)				\
  set_trap(set_insert_before, _setid_, ##_args_)

#define set_insert_after(_setid_, _args_...)				\
  set_trap(set_insert_after, _setid_, ##_args_)

#define set_locate(_setid_, _args_...)					\
  set_trap(set_locate, _setid_, ##_args_)

#define set_object(_setid_, _args_...)					\
  set_trap(set_object, _setid_, ##_args_)

#define set_show(_setid_, _args_...)					\
  set_trap(set_show, _setid_, ##_args_)

#define set_push(_setid_, _args_...)					\
  set_trap(set_push, _setid_, ##_args_)

#define set_pop(_setid_, _args_...)					\
  set_trap(set_pop, _setid_, ##_args_)

#define set_pick(_setid_, _args_...)					\
  set_trap(set_pick, _setid_, ##_args_)


/*
 * foreach
 */

#define set_foreach(_opt_, _setid_, _iterator_, _state_)		\
  for ((_state_) = ITERATOR_STATE_UNUSED;				\
        (((_state_) == ITERATOR_STATE_UNUSED) ?				\
          ((_opt_) == SET_OPT_FORWARD ?					\
            set_head((_setid_), (_iterator_)) == ERROR_NONE :		\
            set_tail((_setid_), (_iterator_)) == ERROR_NONE) :		\
          ((_opt_) == SET_OPT_FORWARD ?					\
            set_next((_setid_), *(_iterator_), (_iterator_)) ==		\
              ERROR_NONE :						\
            set_previous((_setid_), *(_iterator_), (_iterator_)) ==	\
              ERROR_NONE));						\
	 (_state_) = ITERATOR_STATE_USED				\
       )

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/set/set.c
 *      ../../core/set/set_array.c
 *      ../../core/set/set_ll.c
 *	../../core/set/set_bpt.c
 *	../../core/set/set_pipe.c
 *	../../core/set/set_stack.c

 */

/*
 * ../../core/set/set.c
 */

t_error			set_dump(void);

t_error			set_size(t_setid			setid,
				 t_setsz*			size);

t_error			set_new(o_set*				o);

t_error			set_destroy(t_setid			setid);

t_error			set_descriptor(t_setid			setid,
				       o_set**			o);

t_error			set_get(t_setid				setid,
				t_id				id,
				void**				o);

t_error			set_init(void);

t_error			set_clean(void);


/*
 * ../../core/set/set_array.c
 */

t_error			set_type_array(t_setid			setid);

t_error			set_show_array(t_setid			setid);

t_error			set_head_array(t_setid			setid,
				       t_iterator*		iterator);

t_error			set_tail_array(t_setid			setid,
				       t_iterator*		iterator);

t_error			set_prev_array(t_setid			setid,
				       t_iterator		current,
				       t_iterator*		previous);

t_error			set_next_array(t_setid			setid,
				       t_iterator		current,
				       t_iterator*		next);

t_error			set_insert_head_array(t_setid		setid,
					      void*		data);

t_error			set_insert_tail_array(t_setid		setid,
					      void*		data);

t_error			set_insert_before_array(t_setid		setid,
						t_iterator	iterator,
						void*		data);

t_error			set_insert_after_array(t_setid		setid,
					       t_iterator	iterator,
					       void*		data);

t_error			set_add_array(t_setid			setid,
				      void*			data);

t_error			set_remove_array(t_setid		setid,
					 t_id			id);

t_error			set_delete_array(t_setid		setid,
					 t_iterator		iterator);

t_error			set_flush_array(t_setid			setid);

t_error			set_locate_array(t_setid		setid,
					 t_id			id,
					 t_iterator*		iterator);

t_error			set_object_array(t_setid		setid,
					 t_iterator		iterator,
					 void**			data);

t_error			set_reserve_array(t_opts		opts,
					  t_setsz		initsz,
					  t_size		datasz,
					  t_setid*		setid);

t_error			set_release_array(t_setid		setid);

t_error			set_push_array(t_setid			setid,
				       void*			data);

t_error			set_pop_array(t_setid			setid);

t_error			set_pick_array(t_setid			setid,
				       void**			data);


/*
 * ../../core/set/set_ll.c
 */

t_error			set_type_ll(t_setid			setid);

t_error			set_show_ll(t_setid			setid);

t_error			set_head_ll(t_setid			setid,
				    t_iterator*			iterator);

t_error			set_tail_ll(t_setid			setid,
				    t_iterator*			iterator);

t_error			set_prev_ll(t_setid			setid,
				    t_iterator			current,
				    t_iterator*			previous);

t_error			set_next_ll(t_setid			setid,
				    t_iterator			current,
				    t_iterator*			next);

t_error			set_insert_head_ll(t_setid		setid,
					   void*		data);

t_error			set_insert_tail_ll(t_setid		setid,
					   void*		data);

t_error			set_insert_before_ll(t_setid		setid,
					     t_iterator		iterator,
					     void*		data);

t_error			set_insert_after_ll(t_setid		setid,
					    t_iterator		iterator,
					    void*		data);

t_error			set_add_ll(t_setid			setid,
				   void*			data);

t_error			set_remove_ll(t_setid			setid,
				      t_id			id);

t_error			set_delete_ll(t_setid			setid,
				      t_iterator		iterator);

t_error			set_flush_ll(t_setid			setid);

t_error			set_locate_ll(t_setid			setid,
				      t_id			id,
				      t_iterator*		iterator);

t_error			set_object_ll(t_setid			setid,
				      t_iterator		iterator,
				      void**			data);

t_error			set_reserve_ll(t_opts			opts,
				       t_size			datasz,
				       t_setid*			setid);

t_error			set_release_ll(t_setid			setid);

t_error			set_push_ll(t_setid			setid,
				    void*			data);

t_error			set_pop_ll(t_setid			setid);

t_error			set_pick_ll(t_setid			setid,
				    void**			data);


/*
 * ../../core/set/set_bpt.c
 */

void			set_load_bpt(t_bpt(set)*		bpt,
				     t_bpt_imm(set)*		node,
				     t_bpt_node(set)		addr);

void			set_unload_bpt(t_bpt(set)*		bpt,
				       t_bpt_imm(set)*		node);

int			set_addrcmp_bpt(t_bpt(set)*		bpt,
					t_bpt_addr(set)		addr1,
					t_bpt_addr(set)		addr2);

int			set_keycmp_bpt(t_bpt(set)*		bpt,
				       t_bpt_key(set)		key1,
				       t_bpt_key(set)		key2);

int			set_valcmp_bpt(t_bpt(set)*		bpt,
				       t_bpt_value(set)		value1,
				       t_bpt_value(set)		value2);

t_error			set_show_unused_bpt(o_set*		o);

t_error			set_type_bpt(t_setid			setid);

t_error			set_build_bpt(o_set*			o,
				      BPT_NODESZ_T		nodesz);

t_error			set_adjust_bpt(o_set*			o,
				       t_bpt_uni(set)		alloc,
				       t_bpt_uni(set)		size);

t_error			set_destroy_bpt(o_set*			o);

t_error			set_show_bpt(t_setid			setid);

t_error			set_head_bpt(t_setid			setid,
				     t_iterator*		iterator);

t_error			set_tail_bpt(t_setid			setid,
				     t_iterator*		iterator);

t_error			set_prev_bpt(t_setid			setid,
				     t_iterator			current,
				     t_iterator*		previous);

t_error			set_next_bpt(t_setid			setid,
				     t_iterator			current,
				     t_iterator*		next);

t_error			set_insert_head_bpt(t_setid		setid,
					    void*		data);

t_error			set_insert_tail_bpt(t_setid		setid,
					    void*		data);

t_error			set_insert_before_bpt(t_setid		setid,
					      t_iterator	iterator,
					      void*		data);

t_error			set_insert_after_bpt(t_setid		setid,
					     t_iterator		iterator,
					     void*		data);

t_error			set_add_bpt(t_setid			setid,
				    void*			data);

t_error			set_remove_bpt(t_setid			setid,
				       t_id			id);

t_error			set_delete_bpt(t_setid			setid,
				       t_iterator		iterator);

t_error			set_flush_bpt(t_setid			setid);

t_error			set_locate_bpt(t_setid			setid,
				       t_id			id,
				       t_iterator*		iterator);

t_error			set_object_bpt(t_setid			setid,
				       t_iterator		iterator,
				       void**			data);

t_error			set_reserve_bpt(t_opts			opts,
					t_size			datasz,
					t_bpt_nodesz(set)	nodesz,
					t_setid*		setid);

t_error			set_release_bpt(t_setid			setid);

t_error			set_push_bpt(t_setid			setid,
				     void*			data);

t_error			set_pop_bpt(t_setid			setid);

t_error			set_pick_bpt(t_setid			setid,
				     void**			data);


/*
 * ../../core/set/set_pipe.c
 */

t_error			set_type_pipe(t_setid			setid);

t_error			set_reserve_pipe(t_opts			opts,
					 t_size			datasz,
					 t_setid*		setid);

t_error			set_show_pipe(t_setid			setid);

t_error			set_release_pipe(t_setid		setid);

t_error			set_flush_pipe(t_setid			setid);

t_error			set_push_pipe(t_setid			setid,
				      void*			data);

t_error			set_pick_pipe(t_setid			setid,
				      void**			data);

t_error			set_pop_pipe(t_setid			setid);

t_error			set_head_pipe(t_setid			setid,
				      t_iterator*		iterator);

t_error			set_tail_pipe(t_setid			setid,
				      t_iterator*		iterator);

t_error			set_prev_pipe(t_setid			setid,
				      t_iterator		current,
				      t_iterator*		previous);

t_error			set_next_pipe(t_setid			setid,
				      t_iterator		current,
				      t_iterator*		next);

t_error			set_insert_head_pipe(t_setid		setid,
					     void*		data);

t_error			set_insert_tail_pipe(t_setid		setid,
					     void*		data);

t_error			set_insert_before_pipe(t_setid		setid,
					       t_iterator	iterator,
					       void*		data);

t_error			set_insert_after_pipe(t_setid		setid,
					      t_iterator	iterator,
					      void*		data);

t_error			set_add_pipe(t_setid			setid,
				     void*			data);

t_error			set_remove_pipe(t_setid			setid,
					t_id			id);

t_error			set_delete_pipe(t_setid			setid,
					t_iterator		iterator);

t_error			set_locate_pipe(t_setid			setid,
					t_id			id,
					t_iterator*		iterator);

t_error			set_object_pipe(t_setid			setid,
					t_iterator		iterator,
					void**			data);


/*
 * ../../core/set/set_stack.c
 */

t_error			set_type_stack(t_setid			setid);

t_error			set_reserve_stack(t_opts		opts,
					  t_size		datasz,
					  t_setid*		setid);

t_error			set_show_stack(t_setid			setid);

t_error			set_release_stack(t_setid		setid);

t_error			set_flush_stack(t_setid			setid);

t_error			set_push_stack(t_setid			setid,
				       void*			data);

t_error			set_pick_stack(t_setid			setid,
				       void**			data);

t_error			set_pop_stack(t_setid			setid);

t_error			set_head_stack(t_setid			setid,
				       t_iterator*		iterator);

t_error			set_tail_stack(t_setid			setid,
				       t_iterator*		iterator);

t_error			set_prev_stack(t_setid			setid,
				       t_iterator		current,
				       t_iterator*		previous);

t_error			set_next_stack(t_setid			setid,
				       t_iterator		current,
				       t_iterator*		next);

t_error			set_insert_head_stack(t_setid		setid,
					      void*		data);

t_error			set_insert_tail_stack(t_setid		setid,
					      void*		data);

t_error			set_insert_before_stack(t_setid		setid,
						t_iterator	iterator,
						void*		data);

t_error			set_insert_after_stack(t_setid		setid,
					       t_iterator	iterator,
					       void*		data);

t_error			set_add_stack(t_setid			setid,
				      void*			data);

t_error			set_remove_stack(t_setid		setid,
					 t_id			id);

t_error			set_delete_stack(t_setid		setid,
					 t_iterator		iterator);

t_error			set_locate_stack(t_setid		setid,
					 t_id			id,
					 t_iterator*		iterator);

t_error			set_object_stack(t_setid		setid,
					 t_iterator		iterator,
					 void**			data);


/*
 * eop
 */

#endif