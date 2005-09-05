/*
 * copyright quintard julien
 *
 * kaneton
 *
 * malloc.h
 *
 * path          /home/mycure/kaneton
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Fri Feb 11 02:32:39 2005   mycure
** Last update Tue Sep  6 00:21:54 2005 Reboot Universe
 */

#ifndef LIBDATA_ALLOC_H
# define LIBDATA_ALLOC_H       	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

# include <klibc/include/libsys/types.h>
# include <arch/machdep/machdep.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * chunck
 */

/*# define ALLOC_FIRST_FIT
  # ifdef ALLOC_FIRST_FIT*/

# define ALLOC_FREE		0x1
# define ALLOC_LAST		0x2

/*
** The big, ugly and apparently useless "align" field is actually necessary to
** ensure that the size of the s_chunk structure is a multiple of the size of a
** pointer.
*/
struct				s_chunk
{
  size_t			size;
  unsigned int			flags;
  struct s_chunck*		prv;
} __attribute__ ((__packed__));

typedef struct s_chunk t_chunk;

/*
 * alloc global information
 */
typedef struct
{
  size_t			size;
  size_t			heap; /* Limit of the heap */
  t_chunk*			first_blk;
  t_vaddr			addr;
}				t_alloc;

/*# endif*/
#endif
