/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/id/id.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [tue feb  6 22:27:40 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the kaneton kernel uses 64-bit identifier.
 *
 * from this fact, we do not care about identifier recycling.
 *
 * the best example of this fact is located in the id_release()
 * function. indeed the function does nothing, meaning that the
 * identifier released will not be recycled.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the id manager structure.
 */

m_id*			_id = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows the id object's state.
 */

t_error			id_show(o_id*				o)
{
  ID_ENTER(_id);

  module_call(console, console_message,
	      '#', "    id object's state: %qu\n", o->id);

  ID_LEAVE(_id, ERROR_OK);
}

/*
 * this function clones an identifier meaning that a new identifier
 * is reserved with same properties.
 */

t_error			id_clone(o_id*				o,
				 t_id				old,
				 t_id*				new)
{
  t_error		r;

  ID_ENTER(_id);

  assert(o != NULL);
  assert(new != NULL);

  r = id_reserve(o, new);

  ID_LEAVE(_id, r);
}

/*
 * this function reserves an identifier in an identifier object
 */

t_error			id_reserve(o_id*			o,
				   t_id*			i)
{
  ID_ENTER(_id);

  assert(o != NULL);
  assert(i != NULL);

  *i = o->id++;

  ID_LEAVE(_id, ERROR_OK);
}

/*
 * this function releases an identifier from an identifier object.
 */

t_error			id_release(o_id*			o,
				   t_id				i)
{
  ID_ENTER(_id);

  assert(o != NULL);

  ID_LEAVE(_id, ERROR_OK);
}

/*
 * this function initializes an id object.
 */

t_error			id_build(o_id*				o)
{
  ID_ENTER(_id);

  assert(o != NULL);

  memset(o, 0x0, sizeof(o_id));

  ID_LEAVE(_id, ERROR_OK);
}

/*
 * this function cleans an id object.
 */

t_error			id_destroy(o_id*			o)
{
  ID_ENTER(_id);

  assert(o != NULL);

  memset(o, 0x0, sizeof(o_id));

  ID_LEAVE(_id, ERROR_OK);
}

/*
 * this function must initialize the id manager.
 */

t_error			id_initialize(void)
{
  if ((_id = malloc(sizeof(m_id))) == NULL)
    {
      module_call(console, console_message,
		  '!', "id: cannot allocate memory for the identifier manager "
		  "structure\n");

      return (ERROR_KO);
    }

  return (ERROR_OK);
}

/*
 * this function cleans the id manager.
 */

t_error			id_clean(void)
{
  free(_id);

  return (ERROR_OK);
}
