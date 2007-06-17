/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...on/machine/platform/ibm-pc/include/vga.h
 *
 * created       matthieu bucchianeri   [sun jun 17 17:07:35 2007]
 * updated       matthieu bucchianeri   [sun jun 17 17:20:05 2007]
 */

#ifndef PLATFORM_VGA_H
#define PLATFORM_VGA_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * vga registers
 */

#define	IBMPC_VGA_AC_INDEX		0x3C0
#define	IBMPC_VGA_AC_WRITE		0x3C0
#define	IBMPC_VGA_AC_READ		0x3C1
#define	IBMPC_VGA_MISC_WRITE		0x3C2
#define IBMPC_VGA_SEQ_INDEX		0x3C4
#define IBMPC_VGA_SEQ_DATA		0x3C5
#define	IBMPC_VGA_DAC_READ_INDEX	0x3C7
#define	IBMPC_VGA_DAC_WRITE_INDEX	0x3C8
#define	IBMPC_VGA_DAC_DATA		0x3C9
#define	IBMPC_VGA_MISC_READ		0x3CC
#define IBMPC_VGA_GC_INDEX 		0x3CE
#define IBMPC_VGA_GC_DATA 		0x3CF
#define IBMPC_VGA_CRTC_INDEX		0x3D4		/* 0x3B4 */
#define IBMPC_VGA_CRTC_DATA		0x3D5		/* 0x3B5 */
#define	IBMPC_VGA_INSTAT_READ		0x3DA
#define	IBMPC_VGA_NUM_SEQ_REGS		5
#define	IBMPC_VGA_NUM_CRTC_REGS		25
#define	IBMPC_VGA_NUM_GC_REGS		9
#define	IBMPC_VGA_NUM_AC_REGS		21
#define	IBMPC_VGA_NUM_REGS		(1 + IBMPC_VGA_NUM_SEQ_REGS +	\
					 IBMPC_VGA_NUM_CRTC_REGS +	\
					 IBMPC_VGA_NUM_GC_REGS +	\
					 IBMPC_VGA_NUM_AC_REGS)

/*
 * ---------- types ---------------------------------------------------------
 */

/*
 * fb console variable.
 */

typedef struct
{
  char			*vga;
  int			width;
  int			columns;
  int			height;
  int			lines;
  int			size;
  int			bpp;
}			t_ibmpc_fbcons;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * modes
 */

#define IBMPC_VGA_DUMP_640_480_2(_name_)				\
  static unsigned char (_name_)[] =					\
  {									\
    0xE3,								\
    0x03, 0x01, 0x0F, 0x00, 0x06,					\
    0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,			\
    0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			\
    0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,			\
    0xFF,								\
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0F,			\
    0xFF,								\
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,			\
    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,			\
    0x01, 0x00, 0x0F, 0x00, 0x00					\
  };

#define IBMPC_VGA_DUMP_640_480_16(_name_)				\
  static unsigned char (_name_)[] =					\
  {									\
    0xE3,								\
    0x03, 0x01, 0x08, 0x00, 0x06,					\
    0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,			\
    0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			\
    0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,			\
    0xFF,								\
    0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,			\
    0xFF,								\
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,			\
    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,			\
    0x01, 0x00, 0x0F, 0x00, 0x00					\
  };

#define IBMPC_VGA_DUMP_320_200_256(_name_)				\
  static unsigned char (_name_)[] =					\
  {									\
    0x63,								\
    0x03, 0x01, 0x0F, 0x00, 0x0E,					\
    0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,			\
    0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			\
    0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,			\
    0xFF,								\
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,			\
    0xFF,								\
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,			\
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,			\
    0x41, 0x00, 0x0F, 0x00,	0x00					\
  };

/*
 * font
 */

#define IBMPC_VGA_FONT_8_8(_name_)			\
  static const unsigned char (_name_)[2048] =		\
  {							\
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	\
    0x7E, 0x81, 0xA5, 0x81, 0xBD, 0x99, 0x81, 0x7E,	\
    0x7E, 0xFF, 0xDB, 0xFF, 0xC3, 0xE7, 0xFF, 0x7E,	\
    0x6C, 0xFE, 0xFE, 0xFE, 0x7C, 0x38, 0x10, 0x00,	\
    0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x10, 0x00,	\
    0x38, 0x7C, 0x38, 0xFE, 0xFE, 0x92, 0x10, 0x7C,	\
    0x00, 0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x7C,	\
    0x00, 0x00, 0x18, 0x3C, 0x3C, 0x18, 0x00, 0x00,	\
    0xFF, 0xFF, 0xE7, 0xC3, 0xC3, 0xE7, 0xFF, 0xFF,	\
    0x00, 0x3C, 0x66, 0x42, 0x42, 0x66, 0x3C, 0x00,	\
    0xFF, 0xC3, 0x99, 0xBD, 0xBD, 0x99, 0xC3, 0xFF,	\
    0x0F, 0x07, 0x0F, 0x7D, 0xCC, 0xCC, 0xCC, 0x78,	\
    0x3C, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x7E, 0x18,	\
    0x3F, 0x33, 0x3F, 0x30, 0x30, 0x70, 0xF0, 0xE0,	\
    0x7F, 0x63, 0x7F, 0x63, 0x63, 0x67, 0xE6, 0xC0,	\
    0x99, 0x5A, 0x3C, 0xE7, 0xE7, 0x3C, 0x5A, 0x99,	\
    0x80, 0xE0, 0xF8, 0xFE, 0xF8, 0xE0, 0x80, 0x00,	\
    0x02, 0x0E, 0x3E, 0xFE, 0x3E, 0x0E, 0x02, 0x00,	\
    0x18, 0x3C, 0x7E, 0x18, 0x18, 0x7E, 0x3C, 0x18,	\
    0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x66, 0x00,	\
    0x7F, 0xDB, 0xDB, 0x7B, 0x1B, 0x1B, 0x1B, 0x00,	\
    0x3E, 0x63, 0x38, 0x6C, 0x6C, 0x38, 0x86, 0xFC,	\
    0x00, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x7E, 0x00,	\
    0x18, 0x3C, 0x7E, 0x18, 0x7E, 0x3C, 0x18, 0xFF,	\
    0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x00,	\
    0x18, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x00,	\
    0x00, 0x18, 0x0C, 0xFE, 0x0C, 0x18, 0x00, 0x00,	\
    0x00, 0x30, 0x60, 0xFE, 0x60, 0x30, 0x00, 0x00,	\
    0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xFE, 0x00, 0x00,	\
    0x00, 0x24, 0x66, 0xFF, 0x66, 0x24, 0x00, 0x00,	\
    0x00, 0x18, 0x3C, 0x7E, 0xFF, 0xFF, 0x00, 0x00,	\
    0x00, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00, 0x00,	\
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	\
    0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00,	\
    0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00,	\
    0x6C, 0x6C, 0xFE, 0x6C, 0xFE, 0x6C, 0x6C, 0x00,	\
    0x18, 0x7E, 0xC0, 0x7C, 0x06, 0xFC, 0x18, 0x00,	\
    0x00, 0xC6, 0xCC, 0x18, 0x30, 0x66, 0xC6, 0x00,	\
    0x38, 0x6C, 0x38, 0x76, 0xDC, 0xCC, 0x76, 0x00,	\
    0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,	\
    0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00,	\
    0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00,	\
    0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00,	\
    0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00,	\
    0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30,	\
    0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00,	\
    0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00,	\
    0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00,	\
    0x7C, 0xCE, 0xDE, 0xF6, 0xE6, 0xC6, 0x7C, 0x00,	\
    0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xFC, 0x00,	\
    0x78, 0xCC, 0x0C, 0x38, 0x60, 0xCC, 0xFC, 0x00,	\
    0x78, 0xCC, 0x0C, 0x38, 0x0C, 0xCC, 0x78, 0x00,	\
    0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x1E, 0x00,	\
    0xFC, 0xC0, 0xF8, 0x0C, 0x0C, 0xCC, 0x78, 0x00,	\
    0x38, 0x60, 0xC0, 0xF8, 0xCC, 0xCC, 0x78, 0x00,	\
    0xFC, 0xCC, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x00,	\
    0x78, 0xCC, 0xCC, 0x78, 0xCC, 0xCC, 0x78, 0x00,	\
    0x78, 0xCC, 0xCC, 0x7C, 0x0C, 0x18, 0x70, 0x00,	\
    0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00,	\
    0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x30,	\
    0x18, 0x30, 0x60, 0xC0, 0x60, 0x30, 0x18, 0x00,	\
    0x00, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00, 0x00,	\
    0x60, 0x30, 0x18, 0x0C, 0x18, 0x30, 0x60, 0x00,	\
    0x3C, 0x66, 0x0C, 0x18, 0x18, 0x00, 0x18, 0x00,	\
    0x7C, 0xC6, 0xDE, 0xDE, 0xDC, 0xC0, 0x7C, 0x00,	\
    0x30, 0x78, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0x00,	\
    0xFC, 0x66, 0x66, 0x7C, 0x66, 0x66, 0xFC, 0x00,	\
    0x3C, 0x66, 0xC0, 0xC0, 0xC0, 0x66, 0x3C, 0x00,	\
    0xF8, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00,	\
    0xFE, 0x62, 0x68, 0x78, 0x68, 0x62, 0xFE, 0x00,	\
    0xFE, 0x62, 0x68, 0x78, 0x68, 0x60, 0xF0, 0x00,	\
    0x3C, 0x66, 0xC0, 0xC0, 0xCE, 0x66, 0x3A, 0x00,	\
    0xCC, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0xCC, 0x00,	\
    0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,	\
    0x1E, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, 0x00,	\
    0xE6, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0xE6, 0x00,	\
    0xF0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xFE, 0x00,	\
    0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0x00,	\
    0xC6, 0xE6, 0xF6, 0xDE, 0xCE, 0xC6, 0xC6, 0x00,	\
    0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x00,	\
    0xFC, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xF0, 0x00,	\
    0x7C, 0xC6, 0xC6, 0xC6, 0xD6, 0x7C, 0x0E, 0x00,	\
    0xFC, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0xE6, 0x00,	\
    0x7C, 0xC6, 0xE0, 0x78, 0x0E, 0xC6, 0x7C, 0x00,	\
    0xFC, 0xB4, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,	\
    0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0x00,	\
    0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00,	\
    0xC6, 0xC6, 0xC6, 0xC6, 0xD6, 0xFE, 0x6C, 0x00,	\
    0xC6, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0xC6, 0x00,	\
    0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x30, 0x78, 0x00,	\
    0xFE, 0xC6, 0x8C, 0x18, 0x32, 0x66, 0xFE, 0x00,	\
    0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x00,	\
    0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02, 0x00,	\
    0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00,	\
    0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00,	\
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,	\
    0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,	\
    0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,	\
    0xE0, 0x60, 0x60, 0x7C, 0x66, 0x66, 0xDC, 0x00,	\
    0x00, 0x00, 0x78, 0xCC, 0xC0, 0xCC, 0x78, 0x00,	\
    0x1C, 0x0C, 0x0C, 0x7C, 0xCC, 0xCC, 0x76, 0x00,	\
    0x00, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00,	\
    0x38, 0x6C, 0x64, 0xF0, 0x60, 0x60, 0xF0, 0x00,	\
    0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8,	\
    0xE0, 0x60, 0x6C, 0x76, 0x66, 0x66, 0xE6, 0x00,	\
    0x30, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,	\
    0x0C, 0x00, 0x1C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78,	\
    0xE0, 0x60, 0x66, 0x6C, 0x78, 0x6C, 0xE6, 0x00,	\
    0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,	\
    0x00, 0x00, 0xCC, 0xFE, 0xFE, 0xD6, 0xD6, 0x00,	\
    0x00, 0x00, 0xB8, 0xCC, 0xCC, 0xCC, 0xCC, 0x00,	\
    0x00, 0x00, 0x78, 0xCC, 0xCC, 0xCC, 0x78, 0x00,	\
    0x00, 0x00, 0xDC, 0x66, 0x66, 0x7C, 0x60, 0xF0,	\
    0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0x1E,	\
    0x00, 0x00, 0xDC, 0x76, 0x62, 0x60, 0xF0, 0x00,	\
    0x00, 0x00, 0x7C, 0xC0, 0x70, 0x1C, 0xF8, 0x00,	\
    0x10, 0x30, 0xFC, 0x30, 0x30, 0x34, 0x18, 0x00,	\
    0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00,	\
    0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00,	\
    0x00, 0x00, 0xC6, 0xC6, 0xD6, 0xFE, 0x6C, 0x00,	\
    0x00, 0x00, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0x00,	\
    0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8,	\
    0x00, 0x00, 0xFC, 0x98, 0x30, 0x64, 0xFC, 0x00,	\
    0x1C, 0x30, 0x30, 0xE0, 0x30, 0x30, 0x1C, 0x00,	\
    0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00,	\
    0xE0, 0x30, 0x30, 0x1C, 0x30, 0x30, 0xE0, 0x00,	\
    0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	\
    0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0x00,	\
    0x7C, 0xC6, 0xC0, 0xC6, 0x7C, 0x0C, 0x06, 0x7C,	\
    0x00, 0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00,	\
    0x1C, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00,	\
    0x7E, 0x81, 0x3C, 0x06, 0x3E, 0x66, 0x3B, 0x00,	\
    0xCC, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,	\
    0xE0, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,	\
    0x30, 0x30, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,	\
    0x00, 0x00, 0x7C, 0xC6, 0xC0, 0x78, 0x0C, 0x38,	\
    0x7E, 0x81, 0x3C, 0x66, 0x7E, 0x60, 0x3C, 0x00,	\
    0xCC, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00,	\
    0xE0, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00,	\
    0xCC, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,	\
    0x7C, 0x82, 0x38, 0x18, 0x18, 0x18, 0x3C, 0x00,	\
    0xE0, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,	\
    0xC6, 0x10, 0x7C, 0xC6, 0xFE, 0xC6, 0xC6, 0x00,	\
    0x30, 0x30, 0x00, 0x78, 0xCC, 0xFC, 0xCC, 0x00,	\
    0x1C, 0x00, 0xFC, 0x60, 0x78, 0x60, 0xFC, 0x00,	\
    0x00, 0x00, 0x7F, 0x0C, 0x7F, 0xCC, 0x7F, 0x00,	\
    0x3E, 0x6C, 0xCC, 0xFE, 0xCC, 0xCC, 0xCE, 0x00,	\
    0x78, 0x84, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00,	\
    0x00, 0xCC, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00,	\
    0x00, 0xE0, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00,	\
    0x78, 0x84, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00,	\
    0x00, 0xE0, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00,	\
    0x00, 0xCC, 0x00, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8,	\
    0xC3, 0x18, 0x3C, 0x66, 0x66, 0x3C, 0x18, 0x00,	\
    0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x00,	\
    0x18, 0x18, 0x7E, 0xC0, 0xC0, 0x7E, 0x18, 0x18,	\
    0x38, 0x6C, 0x64, 0xF0, 0x60, 0xE6, 0xFC, 0x00,	\
    0xCC, 0xCC, 0x78, 0x30, 0xFC, 0x30, 0xFC, 0x30,	\
    0xF8, 0xCC, 0xCC, 0xFA, 0xC6, 0xCF, 0xC6, 0xC3,	\
    0x0E, 0x1B, 0x18, 0x3C, 0x18, 0x18, 0xD8, 0x70,	\
    0x1C, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,	\
    0x38, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,	\
    0x00, 0x1C, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00,	\
    0x00, 0x1C, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00,	\
    0x00, 0xF8, 0x00, 0xB8, 0xCC, 0xCC, 0xCC, 0x00,	\
    0xFC, 0x00, 0xCC, 0xEC, 0xFC, 0xDC, 0xCC, 0x00,	\
    0x3C, 0x6C, 0x6C, 0x3E, 0x00, 0x7E, 0x00, 0x00,	\
    0x38, 0x6C, 0x6C, 0x38, 0x00, 0x7C, 0x00, 0x00,	\
    0x18, 0x00, 0x18, 0x18, 0x30, 0x66, 0x3C, 0x00,	\
    0x00, 0x00, 0x00, 0xFC, 0xC0, 0xC0, 0x00, 0x00,	\
    0x00, 0x00, 0x00, 0xFC, 0x0C, 0x0C, 0x00, 0x00,	\
    0xC6, 0xCC, 0xD8, 0x36, 0x6B, 0xC2, 0x84, 0x0F,	\
    0xC3, 0xC6, 0xCC, 0xDB, 0x37, 0x6D, 0xCF, 0x03,	\
    0x18, 0x00, 0x18, 0x18, 0x3C, 0x3C, 0x18, 0x00,	\
    0x00, 0x33, 0x66, 0xCC, 0x66, 0x33, 0x00, 0x00,	\
    0x00, 0xCC, 0x66, 0x33, 0x66, 0xCC, 0x00, 0x00,	\
    0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88,	\
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA,	\
    0xDB, 0xF6, 0xDB, 0x6F, 0xDB, 0x7E, 0xD7, 0xED,	\
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,	\
    0x18, 0x18, 0x18, 0x18, 0xF8, 0x18, 0x18, 0x18,	\
    0x18, 0x18, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18,	\
    0x36, 0x36, 0x36, 0x36, 0xF6, 0x36, 0x36, 0x36,	\
    0x00, 0x00, 0x00, 0x00, 0xFE, 0x36, 0x36, 0x36,	\
    0x00, 0x00, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18,	\
    0x36, 0x36, 0xF6, 0x06, 0xF6, 0x36, 0x36, 0x36,	\
    0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,	\
    0x00, 0x00, 0xFE, 0x06, 0xF6, 0x36, 0x36, 0x36,	\
    0x36, 0x36, 0xF6, 0x06, 0xFE, 0x00, 0x00, 0x00,	\
    0x36, 0x36, 0x36, 0x36, 0xFE, 0x00, 0x00, 0x00,	\
    0x18, 0x18, 0xF8, 0x18, 0xF8, 0x00, 0x00, 0x00,	\
    0x00, 0x00, 0x00, 0x00, 0xF8, 0x18, 0x18, 0x18,	\
    0x18, 0x18, 0x18, 0x18, 0x1F, 0x00, 0x00, 0x00,	\
    0x18, 0x18, 0x18, 0x18, 0xFF, 0x00, 0x00, 0x00,	\
    0x00, 0x00, 0x00, 0x00, 0xFF, 0x18, 0x18, 0x18,	\
    0x18, 0x18, 0x18, 0x18, 0x1F, 0x18, 0x18, 0x18,	\
    0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,	\
    0x18, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18,	\
    0x18, 0x18, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18,	\
    0x36, 0x36, 0x36, 0x36, 0x37, 0x36, 0x36, 0x36,	\
    0x36, 0x36, 0x37, 0x30, 0x3F, 0x00, 0x00, 0x00,	\
    0x00, 0x00, 0x3F, 0x30, 0x37, 0x36, 0x36, 0x36,	\
    0x36, 0x36, 0xF7, 0x00, 0xFF, 0x00, 0x00, 0x00,	\
    0x00, 0x00, 0xFF, 0x00, 0xF7, 0x36, 0x36, 0x36,	\
    0x36, 0x36, 0x37, 0x30, 0x37, 0x36, 0x36, 0x36,	\
    0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00,	\
    0x36, 0x36, 0xF7, 0x00, 0xF7, 0x36, 0x36, 0x36,	\
    0x18, 0x18, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00,	\
    0x36, 0x36, 0x36, 0x36, 0xFF, 0x00, 0x00, 0x00,	\
    0x00, 0x00, 0xFF, 0x00, 0xFF, 0x18, 0x18, 0x18,	\
    0x00, 0x00, 0x00, 0x00, 0xFF, 0x36, 0x36, 0x36,	\
    0x36, 0x36, 0x36, 0x36, 0x3F, 0x00, 0x00, 0x00,	\
    0x18, 0x18, 0x1F, 0x18, 0x1F, 0x00, 0x00, 0x00,	\
    0x00, 0x00, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18,	\
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x36, 0x36, 0x36,	\
    0x36, 0x36, 0x36, 0x36, 0xFF, 0x36, 0x36, 0x36,	\
    0x18, 0x18, 0xFF, 0x18, 0xFF, 0x18, 0x18, 0x18,	\
    0x18, 0x18, 0x18, 0x18, 0xF8, 0x00, 0x00, 0x00,	\
    0x00, 0x00, 0x00, 0x00, 0x1F, 0x18, 0x18, 0x18,	\
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,	\
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,	\
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,	\
    0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,	\
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,	\
    0x00, 0x00, 0x76, 0xDC, 0xC8, 0xDC, 0x76, 0x00,	\
    0x00, 0x78, 0xCC, 0xF8, 0xCC, 0xF8, 0xC0, 0xC0,	\
    0x00, 0xFC, 0xCC, 0xC0, 0xC0, 0xC0, 0xC0, 0x00,	\
    0x00, 0x00, 0xFE, 0x6C, 0x6C, 0x6C, 0x6C, 0x00,	\
    0xFC, 0xCC, 0x60, 0x30, 0x60, 0xCC, 0xFC, 0x00,	\
    0x00, 0x00, 0x7E, 0xD8, 0xD8, 0xD8, 0x70, 0x00,	\
    0x00, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0xC0,	\
    0x00, 0x76, 0xDC, 0x18, 0x18, 0x18, 0x18, 0x00,	\
    0xFC, 0x30, 0x78, 0xCC, 0xCC, 0x78, 0x30, 0xFC,	\
    0x38, 0x6C, 0xC6, 0xFE, 0xC6, 0x6C, 0x38, 0x00,	\
    0x38, 0x6C, 0xC6, 0xC6, 0x6C, 0x6C, 0xEE, 0x00,	\
    0x1C, 0x30, 0x18, 0x7C, 0xCC, 0xCC, 0x78, 0x00,	\
    0x00, 0x00, 0x7E, 0xDB, 0xDB, 0x7E, 0x00, 0x00,	\
    0x06, 0x0C, 0x7E, 0xDB, 0xDB, 0x7E, 0x60, 0xC0,	\
    0x38, 0x60, 0xC0, 0xF8, 0xC0, 0x60, 0x38, 0x00,	\
    0x78, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x00,	\
    0x00, 0x7E, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00,	\
    0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x7E, 0x00,	\
    0x60, 0x30, 0x18, 0x30, 0x60, 0x00, 0xFC, 0x00,	\
    0x18, 0x30, 0x60, 0x30, 0x18, 0x00, 0xFC, 0x00,	\
    0x0E, 0x1B, 0x1B, 0x18, 0x18, 0x18, 0x18, 0x18,	\
    0x18, 0x18, 0x18, 0x18, 0x18, 0xD8, 0xD8, 0x70,	\
    0x18, 0x18, 0x00, 0x7E, 0x00, 0x18, 0x18, 0x00,	\
    0x00, 0x76, 0xDC, 0x00, 0x76, 0xDC, 0x00, 0x00,	\
    0x38, 0x6C, 0x6C, 0x38, 0x00, 0x00, 0x00, 0x00,	\
    0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,	\
    0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,	\
    0x0F, 0x0C, 0x0C, 0x0C, 0xEC, 0x6C, 0x3C, 0x1C,	\
    0x58, 0x6C, 0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00,	\
    0x70, 0x98, 0x30, 0x60, 0xF8, 0x00, 0x00, 0x00,	\
    0x00, 0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x00, 0x00,	\
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00	\
  };

#endif
