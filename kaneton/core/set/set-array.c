/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/set/set-array.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [tue feb  6 22:58:44 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this  subpart  of the  set  manager is  used  to  build array  data
 * structures.
 *
 * there  are two  way  to use  the  arrays :  with  pointers or  with
 * objects.    the  first   method   is  the   default  one   (without
 * SET_OPTION_ALLOC) : it only  stores pointers refering to objects.  The
 * second method (used with  SET_OPTION_ALLOC) makes copies of objects in
 * the set,  using memcpy. with  SET_OPTION_FREE, objects are  freed when
 * they   are  removed   from  the   array  or   when  the   array  is
 * flushed/released.
 *
 * the option ORGANISE is used to keep the array as small as possible,
 * but  some operation  requires to  shift entire  parts of  the array
 * (loss of performances).
 *
 * options:    SET_OPTION_CONTAINER,    SET_OPTION_SORT,   SET_OPTION_ORGANISE,
 *   SET_OPTION_ALLOC, SET_OPTION_FREE
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_set*		_set;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function tells if the set object is an array set.
 *
 * steps:
 *
 * 1) gets the descriptor associated with the id.
 * 2) checks the type field.
 */

t_error			set_type_array(i_set			setid)
{
  o_set*		o;

  SET_ENTER(_set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 2)
   */

  if (o->type != SET_TYPE_ARRAY)
    SET_LEAVE(_set, ERROR_KO);

  SET_LEAVE(_set, ERROR_OK);
}

/*
 * this function shows set objects contained in a set.
 *
 * steps:
 *
 * 1) retrieves the set associated with the setid.
 * 2) loops through the slots and displays elements.
 */

t_error			set_show_array(i_set			setid)
{
  o_set*		o;
  t_setsz		i;
  t_setsz		pos;
  t_id			id;

  SET_ENTER(_set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 2)
   */

  module_call(console, console_message,
	      '#', "  %qd node(s) from the array set %qu:\n",
	      o->size,
	      setid);

  for (i = 0, pos = 0; i < o->u.array.arraysz; ++i)
    {
      if (o->u.array.array[i])
	id = *((t_id*)(o->u.array.array[i]));
      else
	continue;

      if (id == ID_UNUSED)
	module_call(console, console_message,
		    '!', "warning: unused object detected !\n");

      module_call(console, console_message,
		  '#', "  array[%qd] = %qd\n", pos, id);

      ++pos;
    }

  SET_LEAVE(_set, ERROR_OK);
}

/*
 * this function returns an iterator on the first entry on the array.
 * if there is no element in the array, the function returns ERROR_KO.
 *
 * steps:
 *
 * 1) gets descriptor.
 * 2) looks for the first element of the list.
 * 3) updates the iterator.
 */

t_error			set_head_array(i_set			setid,
				       t_iterator*		iterator)
{
  o_set*		o;
  t_setsz		i;

  SET_ENTER(_set);

  assert(iterator != NULL);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 2)
   */

  if (o->size == 0)
    SET_LEAVE(_set, ERROR_KO);

  for (i = 0; i < o->u.array.arraysz; ++i)
    {
      if (o->u.array.array[i])
	break;
    }

  /*
   * 3)
   */

  memset(iterator, 0x0, sizeof(t_iterator));

  iterator->u.array.i = i;

  SET_LEAVE(_set, ERROR_OK);
}

/*
 * this function returns an iterator on the last entry of the array.
 * if there is no node in the list, the function returns ERROR_KO.
 *
 * steps:
 *
 * 1) gets descriptor.
 * 2) looks for the last element of the list.
 * 3) updates the iterator.
 */

t_error			set_tail_array(i_set			setid,
				       t_iterator*		iterator)
{
  o_set*		o;
  t_setsz		i;

  SET_ENTER(_set);

  assert(iterator != NULL);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 2)
   */

  if (o->size == 0)
    SET_LEAVE(_set, ERROR_KO);

  for (i = o->u.array.arraysz - 1; i >= 0; --i)
    {
      if (o->u.array.array[i])
	break;
    }

  /*
   * 3)
   */

  memset(iterator, 0x0, sizeof(t_iterator));

  iterator->u.array.i = i;

  SET_LEAVE(_set, ERROR_OK);
}

/*
 * this function returns an iterator on the previous entry.
 *
 * steps:
 *
 * 1) gets the descriptor.
 * 2) moves to the previous non-empty slot.
 * 3) updates the iterator.
 */

t_error			set_previous_array(i_set		setid,
					   t_iterator		current,
					   t_iterator*		previous)
{
  o_set*		o;
  t_setsz		i;

  SET_ENTER(_set);

  assert(previous != NULL);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 2)
   */

  for (i = current.u.array.i - 1; i >= 0; --i)
    {
      if (o->u.array.array[i])
	break;
    }

  if (i == -1)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 3)
   */

  previous->u.array.i = i;

  SET_LEAVE(_set, ERROR_OK);
}

/*
 * this function returns an iterator on the next entry of the array.
 *
 * steps:
 *
 * 1) gets the descriptor.
 * 2) moves to the next non-empty slot.
 * 3) updates the iterator.
 */

t_error			set_next_array(i_set			setid,
				       t_iterator		current,
				       t_iterator*		next)
{
  o_set*		o;
  t_setsz		i;

  SET_ENTER(_set);

  assert(next != NULL);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 2)
   */

  for (i = current.u.array.i + 1; i < o->u.array.arraysz; ++i)
    {
      if (o->u.array.array[i])
	break;
    }

  if (i >= o->u.array.arraysz)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 3)
   */

  next->u.array.i = i;

  SET_LEAVE(_set, ERROR_OK);
}

/*
 * this  function is  used when  no room  is found  in the  array. it
 * expands the array and copy needed data.
 *
 * steps:
 *
 * 1) computes the new size for the array
 * 2) expands the array
 * 3) initializes created slots
 */

static t_error		set_expand_array(o_set			*o,
					 void			*data)
{
  t_setsz		i;
  t_setsz		sz;

  assert(o != NULL);

  i = o->u.array.arraysz;

  /*
   * 1)
   */

  if (o->options & SET_OPTION_ORGANISE)
    sz = o->u.array.arraysz + 1;
  else
    sz = o->u.array.arraysz * 2;

  /*
   * 2)
   */

  if ((o->u.array.array = realloc(o->u.array.array,
				  sz * sizeof(void*))) == NULL)
    return ERROR_KO;

  o->u.array.arraysz = sz;

  /*
   * 3)
   */

  o->u.array.array[i] = data;
  for (++i; i < o->u.array.arraysz; ++i)
    o->u.array.array[i] = NULL;

  return ERROR_OK;
}

/*
 * this function inserts an element at the specified place.
 *
 * steps:
 *
 * 1) expands the array if necessary.
 * 2) inserts in its place if this one is empty.
 * 3) inserts just before if possible.
 * 4) shifts right to make some room and inserts.
 */

static t_error		set_insert_at(o_set			*o,
				      t_setsz			pos,
				      void			*data)
{
  t_setsz		limit;

  assert(o != NULL);

  /*
   * 1)
   */

  if (o->u.array.arraysz == pos)
    {
      return set_expand_array(o, data);
    }

  /*
   * 2)
   */

  if (o->u.array.array[pos] == NULL)
    {
      o->u.array.array[pos] = data;
      return ERROR_OK;
    }

  /*
   * 3)
   */

  if (pos > 0 && !(o->options & SET_OPTION_ORGANISE))
    {
      if (o->u.array.array[pos - 1] == NULL)
	{
	  o->u.array.array[pos - 1] = data;
	  return ERROR_OK;
	}
    }

  /*
   * 4)
   */

  for (limit = pos; limit < o->u.array.arraysz; ++limit)
    {
      if (o->u.array.array[limit] == NULL)
	break;
    }
  if (limit == o->u.array.arraysz)
    {
      if (set_expand_array(o, o->u.array.array[limit - 1]) !=
	  ERROR_OK)
	return ERROR_KO;
    }
  for (; limit > pos; --limit)
    {
      o->u.array.array[limit] = o->u.array.array[limit - 1];
    }
  o->u.array.array[pos] = data;

  return ERROR_OK;
}

/*
 * this function inserts a new entry at the head of the array.
 *
 * steps:
 * 1) gets the descriptor.
 * 2) checks options.
 * 3) copies the element if necessary.
 * 4) inserts the element in its place.
 * 5) updates count.
 */

t_error			set_insert_array(i_set			setid,
					 void*			data)
{
  o_set*		o;
  void*			cpy;

  SET_ENTER(_set);

  assert(data != NULL);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 2)
   */

  if (o->options & SET_OPTION_SORT)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 3)
   */

  if (o->options & SET_OPTION_ALLOC)
    {
      if (!(cpy = malloc(o->datasz)))
	SET_LEAVE(_set, ERROR_KO);
      memcpy(cpy, data, o->datasz);
      data = cpy;
    }

  /*
   * 4)
   */

  if (set_insert_at(o, 0, data) != ERROR_OK)
    {
      if (o->options & SET_OPTION_ALLOC)
	free(data);
      SET_LEAVE(_set, ERROR_KO);
    }

  /*
   * 5)
   */

  ++o->size;

  SET_LEAVE(_set, ERROR_OK);
}

/*
 * this function inserts a new entry at the tail of the array.
 *
 * steps:
 * 1) gets the descriptor.
 * 2) checks options.
 * 3) copies the element if necessary.
 * 4) inserts the element in its place.
 * 5) updates count.
 */

t_error			set_append_array(i_set			setid,
					 void*			data)
{
  o_set*		o;
  t_setsz		i;
  void*			cpy;

  SET_ENTER(_set);

  assert(data != NULL);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 2)
   */

  if (o->options & SET_OPTION_SORT)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 3)
   */

  if (o->options & SET_OPTION_ALLOC)
    {
      if (!(cpy = malloc(o->datasz)))
	SET_LEAVE(_set, ERROR_KO);
      memcpy(cpy, data, o->datasz);
      data = cpy;
    }

  /*
   * 4)
   */

  if (o->size)
    {
      for (i = o->u.array.arraysz; i > 0; --i)
	{
	  if (o->u.array.array[i - 1])
	    break;
	}
    }
  else
    i = 0;

  if (set_insert_at(o, i, data) != ERROR_OK)
    {
      if (o->options & SET_OPTION_ALLOC)
	free(data);
      SET_LEAVE(_set, ERROR_KO);
    }

  /*
   * 5)
   */

  ++o->size;

  SET_LEAVE(_set, ERROR_OK);
}

/*
 * this function inserts an object before the one referenced by the iterator.
 *
 * steps:
 * 1) gets the descriptor.
 * 2) checks options.
 * 3) copies the element if necessary.
 * 4) inserts the element in its place.
 * 5) updates item count.
 */

t_error			set_before_array(i_set			setid,
					 t_iterator		iterator,
					 void*			data)
{
  o_set*		o;
  t_setsz		i;
  void*			cpy;

  SET_ENTER(_set);

  assert(data != NULL);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 2)
   */

  if (o->options & SET_OPTION_SORT)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 3)
   */

  if (o->options & SET_OPTION_ALLOC)
    {
      if (!(cpy = malloc(o->datasz)))
	SET_LEAVE(_set, ERROR_KO);
      memcpy(cpy, data, o->datasz);
      data = cpy;
    }

  /*
   * 4)
   */

  i = iterator.u.array.i;

  if (i < 0)
    i = 0;

  if (set_insert_at(o, i, data) != ERROR_OK)
    {
      if (o->options & SET_OPTION_ALLOC)
	free(data);
      SET_LEAVE(_set, ERROR_KO);
    }

  /*
   * 5)
   */

  ++o->size;

  SET_LEAVE(_set, ERROR_OK);
}

/*
 * this function inserts a new object after a existing one referenced
 * by the iterator.
 *
 * steps:
 * 1) gets the descriptor.
 * 2) checks options.
 * 3) copies the element if necessary.
 * 4) inserts the element in its place.
 * 5) updates count.
 */

t_error			set_after_array(i_set			setid,
					t_iterator		iterator,
					void*			data)
{
  o_set*       		o;
  t_setsz		i;
  void*			cpy;

  SET_ENTER(_set);

  assert(data != NULL);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 2)
   */

  if (o->options & SET_OPTION_SORT)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 3)
   */

  if (o->options & SET_OPTION_ALLOC)
    {
      if (!(cpy = malloc(o->datasz)))
	SET_LEAVE(_set, ERROR_KO);
      memcpy(cpy, data, o->datasz);
      data = cpy;
    }

  /*
   * 4)
   */

  i = iterator.u.array.i + 1;

  if (i > o->u.array.arraysz)
    i = o->u.array.arraysz;

  if (set_insert_at(o, i, data) != ERROR_OK)
    {
      if (o->options & SET_OPTION_ALLOC)
	free(data);
      SET_LEAVE(_set, ERROR_KO);
    }

  /*
   * 5)
   */

  ++o->size;

  SET_LEAVE(_set, ERROR_OK);
}

/*
 * this function adds an object in the array.
 *
 * steps:
 *
 * 1) checks the data.
 * 2) get the object associated with the id.
 * 3) copies the object if required.
 * 4) inserts the element.
 *   A) (sorted array)
 *    i) finds the place of the new element.
 *    ii) copies in the right place.
 *    iii) expands the array if necessary.
 *   B) (unsorted array)
 *    i) tries to find a free place.
 *    ii) expands the array if necessary.
 * 5) updates size.
 */

t_error			set_add_array(i_set			setid,
				      void*			data)
{
  o_set*		o;
  t_setsz		i;
  t_id			id;
  t_id			current;
  void*			cpy;
  t_uint8		empty;
  t_setsz		last;

  SET_ENTER(_set);

  assert(data != NULL);

  /*
   * 1)
   */

  if (!data || *((t_id*)data) == ID_UNUSED)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 3)
   */

  if (o->options & SET_OPTION_ALLOC)
    {
      if (!(cpy = malloc(o->datasz)))
	SET_LEAVE(_set, ERROR_KO);
      memcpy(cpy, data, o->datasz);
      data = cpy;
    }

  /*
   * 4)
   */

  if (o->options & SET_OPTION_SORT)
    {
      /*
       * A)
       */

      /*
       * i)
       */

      empty = 1;
      last = -1;
      id = *((t_id*)data);
      for (i = 0; i < o->u.array.arraysz; ++i)
	{
	  if (o->u.array.array[i] != NULL)
	    {
	      current = *((t_id*)o->u.array.array[i]);
	      empty = 0;
	      if (current == id)
		{
		  module_call(console, console_message,
			      '!', "set: identifier collision detected "
			      "in the set %qu on the object identifier %qu\n",
			      o->id, id);

		  if (o->options & SET_OPTION_ALLOC)
		    free(data);

		  SET_LEAVE(_set, ERROR_KO);
		}
	      if (current > id)
		break;
	      last = i;
	    }
	}

      /*
       * ii)
       */

      ++last;
      if (empty)
	last = i = o->u.array.arraysz / 2;
      if (last < o->u.array.arraysz)
	{
	  if (set_insert_at(o, last, data) != ERROR_OK)
	    {
	      if (o->options & SET_OPTION_ALLOC)
		free(data);
	      SET_LEAVE(_set, ERROR_KO);
	    }
	}

      /*
       * iii)
       */

      if (last >= o->u.array.arraysz)
	{
	  if (set_expand_array(o, data) != ERROR_OK)
	    {
	      if (o->options & SET_OPTION_ALLOC)
		free(data);
	      SET_LEAVE(_set, ERROR_KO);
	    }
	}
    }
  else
    {

      /*
       * B)
       */

      /*
       * i)
       */

      for (i = 0; i < o->u.array.arraysz; ++i)
	{
	  if (o->u.array.array[i] == NULL)
	    {
	      o->u.array.array[i] = data;
	      break;
	    }
	}

      /*
       * ii)
       */

      if (i == o->u.array.arraysz)
	{
	  if (set_expand_array(o, data) != ERROR_OK)
	    {
	      if (o->options & SET_OPTION_ALLOC)
		free(data);
	      SET_LEAVE(_set, ERROR_KO);
	    }
	}
    }

  /*
   * 5)
   */

  ++o->size;

  SET_LEAVE(_set, ERROR_OK);
}

/*
 * this function removes a object from the array, setting the identifier
 * field of the entry as 'unused'.
 *
 * steps:
 *
 * 1) checks the given id.
 * 2) gets object associated to setid.
 * 3) looks for the element, then delete it.
 * 4) re-organises the set if needed.
 * 5) decrements counter.
 */

t_error			set_remove_array(i_set			setid,
					 t_id			id)
{
  o_set*       		o;
  t_setsz		i;
  t_id			current;

  SET_ENTER(_set);

  /*
   * 1)
   */

  if (id == ID_UNUSED)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 3)
   */

  if (o->size == 0)
    SET_LEAVE(_set, ERROR_KO);

  for (i = 0; i < o->u.array.arraysz; ++i)
    {
      if (o->u.array.array[i])
	{
	  current = *((t_id*)(o->u.array.array[i]));

	  if ((o->options & SET_OPTION_SORT) && current > id)
	    SET_LEAVE(_set, ERROR_KO);

	  if (current == id)
	    {
	      if ((o->options & SET_OPTION_FREE) ||
		  (o->options & SET_OPTION_ALLOC))
		free(o->u.array.array[i]);
	      o->u.array.array[i] = NULL;
	      break;
	    }
	}
    }

  if (i == o->u.array.arraysz)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 4)
   */

  if (o->options & SET_OPTION_ORGANISE)
    {
      for (; i < o->u.array.arraysz - 1; ++i)
	{
	  o->u.array.array[i] = o->u.array.array[i + 1];
	}

      --o->u.array.arraysz;
      o->u.array.array = realloc(o->u.array.array,
				 o->u.array.arraysz *
				 sizeof(void*));
    }

  /*
   * 5)
   */

  --o->size;

  SET_LEAVE(_set, ERROR_OK);
}

/*
 * this function deletes an element given an iterator.
 *
 * steps:
 *
 * 1) gets the array.
 * 2) checks bounds.
 * 3) checks if the place isn't already empty.
 * 4) frees the element if necessary, then removes it.
 * 5) organises the set if necessary.
 * 6) updates counter.
 */

t_error			set_delete_array(i_set			setid,
					 t_iterator		iterator)
{
  o_set*		o;
  t_setsz		i;

  SET_ENTER(_set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 2)
   */

  i = iterator.u.array.i;
  if (i >= o->u.array.arraysz)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 3)
   */

  if (o->u.array.array[i] == NULL)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 4)
   */

  if (o->options & SET_OPTION_ALLOC ||
      o->options & SET_OPTION_FREE)
    free(o->u.array.array[i]);
  o->u.array.array[i] = NULL;

  /*
   * 5)
   */

  if (o->options & SET_OPTION_ORGANISE)
    {
      for (; i < o->u.array.arraysz - 1; ++i)
	{
	  o->u.array.array[i] = o->u.array.array[i + 1];
	}

      --o->u.array.arraysz;
      o->u.array.array = realloc(o->u.array.array,
				 o->u.array.arraysz *
				 sizeof(void*));
    }

  /*
   * 6)
   */

  o->size--;

  SET_LEAVE(_set, ERROR_OK);
}

/*
 * this function flushes the set, freeing each element.
 *
 * steps:
 *
 * 1) gets descriptor.
 * 2) frees elements if needed.
 * 3) resets the array.
 */

t_error			set_flush_array(i_set			setid)
{
  o_set*       		o;
  t_setsz		i;

  SET_ENTER(_set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 2)
   */

  if ((o->options & SET_OPTION_FREE) ||
      (o->options & SET_OPTION_ALLOC))
    {
      for (i = 0; i < o->u.array.arraysz; ++i)
	{
	  if (o->u.array.array[i])
	    {
	      free(o->u.array.array[i]);
	      o->u.array.array[i] = NULL;
	    }
	}
      o->size = 0;
    }

  /*
   * 3)
   */

  if ((o->u.array.array =
       realloc(o->u.array.array,
	       o->u.array.initsz * sizeof(void*))) == NULL)
    SET_LEAVE(_set, ERROR_KO);
  for (i = 0; i < o->u.array.initsz; ++i)
    {
      o->u.array.array[i] = NULL;
    }

  o->u.array.arraysz = o->u.array.initsz;
  o->size = 0;

  SET_LEAVE(_set, ERROR_OK);
}

/*
 * this function tries to find an object with its identifier and build
 * a corresponding identifier.
 *
 * steps:
 *
 * 1) checks the given id.
 * 2) gets the descriptor.
 * 3) search for the element.
 *   A) dichotomy lookup. (on sorted array)
 *   B) sequentially searchs for the needed element. (unsorted)
 */

t_error			set_locate_array(i_set			setid,
					 t_id			id,
					 t_iterator*		iterator)
{
  o_set*       		o;
  i_set			i;
  t_setsz		left;
  t_setsz		right;
  t_setsz		dicho;

  SET_ENTER(_set);

  assert(iterator != NULL);

  /*
   * 1)
   */

  if (id == ID_UNUSED)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 2)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 3)
   */
/* XXX la dicho marche mal */
  if (0 && (o->options & SET_OPTION_SORT))
    {

      /*
       * A)
       */

      left = 0;
      right = o->u.array.arraysz - 1;

      while (left <= right)
	{
	  dicho = left + (right - left) / 2;
	  for (i = dicho; i < right; ++i)
	    if (o->u.array.array[i])
	      break;
	  if (*((t_id*)(o->u.array.array[i])) == id)
	    break;
	  if (i == right || *((t_id*)(o->u.array.array[i])) > id)
	    {
	      right = dicho - 1;
	    }
	  else
	    {
	      left = dicho + 1;
	    }
	}

      if (left <= right)
	{
	  iterator->u.array.i = i;
	  SET_LEAVE(_set, ERROR_OK);
	}
    }
  else
    {

      /*
       * B)
       */

      for (i = 0; i < o->u.array.arraysz; ++i)
	{
	  if (o->u.array.array[i] &&
	      *((t_id*)(o->u.array.array[i])) == id)
	    {
	      iterator->u.array.i = i;
	      SET_LEAVE(_set, ERROR_OK);
	    }
	}
    }

  SET_LEAVE(_set, ERROR_KO);
}

/*
 * this function returns an object given its iterator.
 *
 * steps:
 *
 * 1) gets the set object.
 * 2) checks bounds.
 * 3) gets data for given position.
 */

t_error			set_object_array(i_set			setid,
					 t_iterator		iterator,
					 void**			data)
{
  o_set*       		o;

  SET_ENTER(_set);

  assert(data != NULL);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 2)
   */

  if (iterator.u.array.i >= o->u.array.arraysz)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 3)
   */

  if (o->u.array.array[iterator.u.array.i])
    *data = o->u.array.array[iterator.u.array.i];
  else
    SET_LEAVE(_set, ERROR_KO);

  SET_LEAVE(_set, ERROR_OK);
}

/*
 * this function reserves a set.
 *
 * steps:
 *
 * 1) checks options for the array.
 * 2) initializes the set descriptor.
 * 3) if necessary, reserves an unused identifier for this new set.
 * 4) initializes the set fields allocating the array.
 * 5) adds the set descriptor in the set container.
 */

t_error			set_reserve_array(t_options		options,
					  t_setsz		initsz,
					  t_size		datasz,
					  i_set*		setid)
{
  o_set			o;
  t_setsz		i;

  SET_ENTER(_set);

  assert(datasz >= sizeof (t_id));
  assert(setid != NULL);

  /*
   * 1)
   */

  if ((options & SET_OPTION_ALLOC) && (options & SET_OPTION_FREE))
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 2)
   */

  memset(&o, 0x0, sizeof(o_set));

  /*
   * 3)
   */

  if (options & SET_OPTION_CONTAINER)
    {
      *setid = _set->sets;
    }
  else
    {
      if (id_reserve(&_set->id, setid) != ERROR_OK)
	SET_LEAVE(_set, ERROR_KO);
    }

  /*
   * 4)
   */

  o.id = *setid;
  o.size = 0;
  o.type = SET_TYPE_ARRAY;
  o.options = options;
  o.datasz = datasz;

  o.u.array.arraysz = (initsz == 0 ? 1 : initsz);
  o.u.array.initsz = o.u.array.arraysz;

  if ((o.u.array.array =
       malloc(o.u.array.arraysz * sizeof(void*))) == NULL)
    {
      if (!(options & SET_OPTION_CONTAINER))
	id_release(&_set->id, o.id);

      SET_LEAVE(_set, ERROR_KO);
    }

  for (i = 0; i < o.u.array.arraysz; i++)
    o.u.array.array[i] = NULL;

  /*
   * 5)
   */

  if (set_new(&o) != ERROR_OK)
    {
      free(o.u.array.array);

      if (!(options & SET_OPTION_CONTAINER))
	id_release(&_set->id, o.id);

      SET_LEAVE(_set, ERROR_KO);
    }

  SET_LEAVE(_set, ERROR_OK);
}

/*
 * this function releases a set.
 *
 * steps:
 *
 * 1) gets the set given its set identifier.
 * 2) flushs the set.
 * 3) releases the set identifier.
 * 4) frees the array allocated at the set reservation.
 * 5) then, removes the set from the set container.
 */

t_error			set_release_array(i_set		setid)
{
  o_set*       		o;

  SET_ENTER(_set);

  /*
   * 1)
   */

  if (set_descriptor(setid, &o) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 2)
   */

  if (set_flush(setid) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 3)
   */

  if (id_release(&_set->id, o->id) != ERROR_OK)
    SET_LEAVE(_set, ERROR_KO);

  /*
   * 4)
   */

  free(o->u.array.array);

  /*
   * 5)
   */

  if (!(o->options & SET_OPTION_CONTAINER))
    if (set_destroy(o->id) != ERROR_OK)
      SET_LEAVE(_set, ERROR_KO);

  SET_LEAVE(_set, ERROR_OK);
}

/*
 * this function just returns an error because the array set does not
 * support this operation.
 */

t_error			set_push_array(i_set			setid,
				       void*			data)
{
  SET_ENTER(_set);

  SET_LEAVE(_set, ERROR_KO);
}

/*
 * this function just returns an error because the array set does not
 * support this operation.
 */

t_error			set_pop_array(i_set			setid)
{
  SET_ENTER(_set);

  SET_LEAVE(_set, ERROR_KO);
}

/*
 * this function just returns an error because the array set does not
 * support this operation.
 */

t_error			set_pick_array(i_set			setid,
				       void**			data)
{
  SET_ENTER(_set);

  SET_LEAVE(_set, ERROR_KO);
}
