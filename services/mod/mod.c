/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/services/mod/mod.c
 *
 * created       matthieu bucchianeri   [sat may  5 18:43:57 2007]
 * updated       matthieu bucchianeri   [mon aug  6 00:10:46 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the module service load modules and act as a modules directory.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <libkaneton.h>
#include <crt.h>

#include "mod.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * ---------- macro-functions -------------------------------------------------
 */

#define PAGED_SIZE(_size_)						\
  ((_size_) % PAGESZ ?							\
   (_size_) + PAGESZ - (_size_) % PAGESZ :				\
   (_size_))

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function loads a input (32 bits).
 *
 * steps:
 *
 * 1) reserve the task.
 * 2) loop through the ELF segments.
 * 3) reserve the segment and map in the input's address space.
 * 4) copy from the file to the memory.
 * 5) create the thread.
 * 6) pushes arguments onto the stack.
 * 7) run the input!
 */

t_error			mod_load_32(Elf32_Ehdr*		header,
				    char*		name)
{
  Elf32_Phdr*		phdr;
  int			i;
  i_task		tsk;
  i_as			as;
  i_segment		seg;
  i_region		reg;
  t_perms		perms;
  i_thread		thr;
  t_thread_context	ctx;
  t_stack		stack;
  struct
  {
    i_task		tskid;
    i_as		asid;
    i_task		mod;
  }			args;

  /*
   * 1)
   */

  if (task_reserve(TASK_CLASS_PROGRAM,
		   TASK_BEHAV_TIMESHARING,
		   TASK_PRIOR_TIMESHARING,
		   &tsk) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (as_reserve(tsk, &as) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  for (i = 0, phdr = (Elf32_Phdr*)((char*)header + header->e_phoff);
       i < header->e_phnum;
       i++, phdr++)
    {
      if (phdr->p_type != PT_LOAD)
	{
	  printf(" -- mod: Unsupported program header, skipping.\n");
	  continue;
	}

      printf(" -- mod: Phdr LOAD at 0x%p (%c%c%c) of size %u\n",
	     phdr->p_vaddr,
	     phdr->p_flags & PF_R ? 'r' : '-',
	     phdr->p_flags & PF_W ? 'w' : '-',
	     phdr->p_flags & PF_X ? 'x' : '-',
	     phdr->p_memsz);

      if (phdr->p_memsz == 0)
	continue;

      /*
       * 3)
       */

      perms = 0;

      if (phdr->p_flags & PF_R)
	perms |= PERM_READ;
      if (phdr->p_flags & PF_W)
	perms |= PERM_WRITE;
      if (phdr->p_flags & PF_X)
	perms |= PERM_EXEC;

      if (segment_reserve(as,
			  PAGED_SIZE(phdr->p_memsz),
			  PERM_WRITE,
			  &seg) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      if (region_reserve(as,
			 seg,
			 0,
			 REGION_OPT_FORCE,
			 phdr->p_vaddr,
			 PAGED_SIZE(phdr->p_memsz),
			 &reg) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      /*
       * 4)
       */

      if (segment_write(seg,
			0,
			(char*)header + phdr->p_offset,
			phdr->p_filesz) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      if (segment_perms(seg, perms) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  if (thread_reserve(tsk, THREAD_PRIOR, &thr) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  stack.base = 0;
  stack.size = 4 * PAGESZ;

  if (thread_stack(thr, stack) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (thread_attribute_stack(thr, &stack.base) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  ctx.sp = stack.base + stack.size - 16;
  ctx.pc = (t_vaddr)header->e_entry;

  if (thread_load(thr, ctx) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 6)
   */

  args.tskid = tsk;
  args.asid = as;
  args.mod = _crt_get_task_id();

  if (thread_args(thr, &args, sizeof (args)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 7)
   */

  if (task_state(tsk, SCHEDULER_STATE_RUN) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

/*
 * this function loads a input (64 bits).
 */

t_error			mod_load_64(Elf64_Ehdr*		header,
				    char*		name)
{
  printf(" -- mod: Elf64 format not supported\n");

  return (ERROR_UNKNOWN);
}

/*
 * this function launches all inputs passed to the kernel.
 *
 * steps:
 *
 * 1) XXX
 * 2) get elf header.
 * 3) load the input.
 * 4) go on to the next input.
 */

void			mod_launch(void)
{
  int			i;
  t_input*		p;
  t_inputs*		inputs;
  char*			name;
  Elf32_Ehdr*		elf;

  /*
   * 1)
   */

  /* XXX inputs = */

  for (i = 0, p = (t_input*)(inputs + 1);
       i < inputs->ninputs;
       i++)
    {
      name = p->name;

      printf(" -- mod: loading input %s ... ", name);

      /*
       * 2)
       */

      elf = (Elf32_Ehdr*)(p + 1);

      if (strncmp(elf->e_ident, ELFMAG, SELFMAG))
	{
	  printf("not an ELF file\n");
	  goto next;
	}

      printf("\n");

      if (elf->e_ident[EI_CLASS] != ELFCLASS32 &&
	  elf->e_ident[EI_CLASS] != ELFCLASS64)
	{
	  printf(" -- mod: invalid ELF class\n");
	  goto next;
	}

      /*
       * 3)
       */

      if (elf->e_ident[EI_CLASS] == ELFCLASS32)
	{
	  if (mod_load_32(elf, name) != ERROR_NONE)
	    {
	      printf(" -- mod: error loading input\n");
	      goto next;
	    }
	}
      else
	{
	  if (mod_load_64((Elf64_Ehdr*)elf, name) != ERROR_NONE)
	    {
	      printf (" -- mod: error loading input\n");
	      goto next;
	    }
	}

      printf(" -- mod: loaded.\n");

      /*
       * 4)
       */

    next:
      p = (t_input*)((char*)p + sizeof (t_input) + p->size + strlen(name) + 1);
    }
}

/*
 * modules directory serving loop.
 */

void			mod_serve(void)
{
  while (1)
    {
      /* XXX */
    }
}

/*
 * service main.
 */

int			main(int argc, char** argv, char** envp)
{
  mod_launch();

  /* hack: call the CRT function that attaches the console to the driver */
  _crt_attach_cons();

  printf(" -- mod: moduler loader service started.\n");

  mod_serve();

  return 0;
}
