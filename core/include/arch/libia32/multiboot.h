/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/include/arch/libia32/multiboot.h
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [tue dec 20 14:09:52 2005]
 */

#ifndef IA32_IA32_MULTIBOOT_H
#define IA32_IA32_MULTIBOOT_H	1

/*
 * ---------- defines ---------------------------------------------------------
 */

#define MULTIBOOT_BOOTLOADER_MAGIC	0x2BADB002

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  unsigned int			magic;
  unsigned int			flags;
  unsigned int			checksum;
  unsigned int			header_addr;
  unsigned int			load_addr;
  unsigned int			load_end_addr;
  unsigned int			bss_end_addr;
  unsigned int			entry_addr;
}				multiboot_header_t;

typedef struct
{
  unsigned int			tabsize;
  unsigned int			strsize;
  unsigned int			addr;
  unsigned int			reserved;
}				aout_symbol_table_t;

typedef struct
{
  unsigned int			num;
  unsigned int			size;
  unsigned int			addr;
  unsigned int			shndx;
}				elf_section_header_table_t;

typedef struct
{
  unsigned int			flags;
  unsigned int			mem_lower;
  unsigned int			mem_upper;
  unsigned int			boot_device;
  unsigned int			cmdline;
  unsigned int			mods_count;
  unsigned int			mods_addr;

  union
  {
    aout_symbol_table_t		aout_sym;
    elf_section_header_table_t	elf_sec;
  }				u;

  unsigned int			mmap_length;
  unsigned int			mmap_addr;

  /* XXX */
  unsigned long drives_length;
  unsigned long drives_addr;
  unsigned long config_table;
  unsigned long boot_loader_name;
  unsigned long apm_table;
  unsigned long vbe_control_info;
  unsigned long vbe_mode_info;
  unsigned short
  vbe_mode, vbe_interface_seg, vbe_interface_off, vbe_interface_len;
  /* XXX */
}				multiboot_info_t;

typedef struct
{
  unsigned int			mod_start;
  unsigned int			mod_end;
  unsigned int			string;
  unsigned int			reserved;
}				module_t;

typedef struct
{
  unsigned int			size;
  unsigned int			base_addr_low;
  unsigned int			base_addr_high;
  unsigned int			length_low;
  unsigned int			length_high;
  unsigned int			type;
}				memory_map_t;

#endif
