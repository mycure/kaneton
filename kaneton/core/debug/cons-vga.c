/*
 * licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/debug/cons-vga.c
 *
 * created       matthieu bucchianeri   [fri dec  2 20:14:33 2005]
 * updated       matthieu bucchianeri   [fri dec  8 02:02:17 2006]
 */

/*
 * http://my.execpc.com/~geezer/osd/graphics/modes.c
 *
 * an example from which this file was written.
 */

/*
 * mode selection (MODE2 is not working)
 */
#define MODE3		// 1: 640x480x2, 2: 640x480x16, 3: 320x200x256

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>


/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * vga registers
 */

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ		0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9
#define	VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
#define VGA_CRTC_INDEX		0x3D4		/* 0x3B4 */
#define VGA_CRTC_DATA		0x3D5		/* 0x3B5 */
#define	VGA_INSTAT_READ		0x3DA
#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS		(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + \
				VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the console variable.
 */

t_cons			cons;

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
}			t_fbcons;

t_fbcons		fbcons;

/*
 * modes
 */

static unsigned char g_640x480x2[] =
{
	0xE3,
	0x03, 0x01, 0x0F, 0x00, 0x06,
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
	0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
	0xFF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0F,
	0xFF,
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x01, 0x00, 0x0F, 0x00, 0x00
};

static unsigned char g_640x480x16[] =
{
	0xE3,
	0x03, 0x01, 0x08, 0x00, 0x06,
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
	0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
	0xFF,
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
	0xFF,
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x01, 0x00, 0x0F, 0x00, 0x00
};

static unsigned char g_320x200x256[] =
{
	0x63,
	0x03, 0x01, 0x0F, 0x00, 0x0E,
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
	0xFF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00,	0x00
};

/*
 * font
 */

static const unsigned char g_8x8_font[2048] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x7E, 0x81, 0xA5, 0x81, 0xBD, 0x99, 0x81, 0x7E,
	0x7E, 0xFF, 0xDB, 0xFF, 0xC3, 0xE7, 0xFF, 0x7E,
	0x6C, 0xFE, 0xFE, 0xFE, 0x7C, 0x38, 0x10, 0x00,
	0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x10, 0x00,
	0x38, 0x7C, 0x38, 0xFE, 0xFE, 0x92, 0x10, 0x7C,
	0x00, 0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x7C,
	0x00, 0x00, 0x18, 0x3C, 0x3C, 0x18, 0x00, 0x00,
	0xFF, 0xFF, 0xE7, 0xC3, 0xC3, 0xE7, 0xFF, 0xFF,
	0x00, 0x3C, 0x66, 0x42, 0x42, 0x66, 0x3C, 0x00,
	0xFF, 0xC3, 0x99, 0xBD, 0xBD, 0x99, 0xC3, 0xFF,
	0x0F, 0x07, 0x0F, 0x7D, 0xCC, 0xCC, 0xCC, 0x78,
	0x3C, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x7E, 0x18,
	0x3F, 0x33, 0x3F, 0x30, 0x30, 0x70, 0xF0, 0xE0,
	0x7F, 0x63, 0x7F, 0x63, 0x63, 0x67, 0xE6, 0xC0,
	0x99, 0x5A, 0x3C, 0xE7, 0xE7, 0x3C, 0x5A, 0x99,
	0x80, 0xE0, 0xF8, 0xFE, 0xF8, 0xE0, 0x80, 0x00,
	0x02, 0x0E, 0x3E, 0xFE, 0x3E, 0x0E, 0x02, 0x00,
	0x18, 0x3C, 0x7E, 0x18, 0x18, 0x7E, 0x3C, 0x18,
	0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x66, 0x00,
	0x7F, 0xDB, 0xDB, 0x7B, 0x1B, 0x1B, 0x1B, 0x00,
	0x3E, 0x63, 0x38, 0x6C, 0x6C, 0x38, 0x86, 0xFC,
	0x00, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x7E, 0x00,
	0x18, 0x3C, 0x7E, 0x18, 0x7E, 0x3C, 0x18, 0xFF,
	0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x00,
	0x18, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x00,
	0x00, 0x18, 0x0C, 0xFE, 0x0C, 0x18, 0x00, 0x00,
	0x00, 0x30, 0x60, 0xFE, 0x60, 0x30, 0x00, 0x00,
	0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xFE, 0x00, 0x00,
	0x00, 0x24, 0x66, 0xFF, 0x66, 0x24, 0x00, 0x00,
	0x00, 0x18, 0x3C, 0x7E, 0xFF, 0xFF, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00,
	0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x6C, 0x6C, 0xFE, 0x6C, 0xFE, 0x6C, 0x6C, 0x00,
	0x18, 0x7E, 0xC0, 0x7C, 0x06, 0xFC, 0x18, 0x00,
	0x00, 0xC6, 0xCC, 0x18, 0x30, 0x66, 0xC6, 0x00,
	0x38, 0x6C, 0x38, 0x76, 0xDC, 0xCC, 0x76, 0x00,
	0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00,
	0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00,
	0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00,
	0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30,
	0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00,
	0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00,
	0x7C, 0xCE, 0xDE, 0xF6, 0xE6, 0xC6, 0x7C, 0x00,
	0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xFC, 0x00,
	0x78, 0xCC, 0x0C, 0x38, 0x60, 0xCC, 0xFC, 0x00,
	0x78, 0xCC, 0x0C, 0x38, 0x0C, 0xCC, 0x78, 0x00,
	0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x1E, 0x00,
	0xFC, 0xC0, 0xF8, 0x0C, 0x0C, 0xCC, 0x78, 0x00,
	0x38, 0x60, 0xC0, 0xF8, 0xCC, 0xCC, 0x78, 0x00,
	0xFC, 0xCC, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x00,
	0x78, 0xCC, 0xCC, 0x78, 0xCC, 0xCC, 0x78, 0x00,
	0x78, 0xCC, 0xCC, 0x7C, 0x0C, 0x18, 0x70, 0x00,
	0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00,
	0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x30,
	0x18, 0x30, 0x60, 0xC0, 0x60, 0x30, 0x18, 0x00,
	0x00, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00, 0x00,
	0x60, 0x30, 0x18, 0x0C, 0x18, 0x30, 0x60, 0x00,
	0x3C, 0x66, 0x0C, 0x18, 0x18, 0x00, 0x18, 0x00,
	0x7C, 0xC6, 0xDE, 0xDE, 0xDC, 0xC0, 0x7C, 0x00,
	0x30, 0x78, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0x00,
	0xFC, 0x66, 0x66, 0x7C, 0x66, 0x66, 0xFC, 0x00,
	0x3C, 0x66, 0xC0, 0xC0, 0xC0, 0x66, 0x3C, 0x00,
	0xF8, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00,
	0xFE, 0x62, 0x68, 0x78, 0x68, 0x62, 0xFE, 0x00,
	0xFE, 0x62, 0x68, 0x78, 0x68, 0x60, 0xF0, 0x00,
	0x3C, 0x66, 0xC0, 0xC0, 0xCE, 0x66, 0x3A, 0x00,
	0xCC, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0xCC, 0x00,
	0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,
	0x1E, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, 0x00,
	0xE6, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0xE6, 0x00,
	0xF0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xFE, 0x00,
	0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0x00,
	0xC6, 0xE6, 0xF6, 0xDE, 0xCE, 0xC6, 0xC6, 0x00,
	0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x00,
	0xFC, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xF0, 0x00,
	0x7C, 0xC6, 0xC6, 0xC6, 0xD6, 0x7C, 0x0E, 0x00,
	0xFC, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0xE6, 0x00,
	0x7C, 0xC6, 0xE0, 0x78, 0x0E, 0xC6, 0x7C, 0x00,
	0xFC, 0xB4, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,
	0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0x00,
	0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00,
	0xC6, 0xC6, 0xC6, 0xC6, 0xD6, 0xFE, 0x6C, 0x00,
	0xC6, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0xC6, 0x00,
	0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x30, 0x78, 0x00,
	0xFE, 0xC6, 0x8C, 0x18, 0x32, 0x66, 0xFE, 0x00,
	0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x00,
	0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02, 0x00,
	0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00,
	0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
	0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,
	0xE0, 0x60, 0x60, 0x7C, 0x66, 0x66, 0xDC, 0x00,
	0x00, 0x00, 0x78, 0xCC, 0xC0, 0xCC, 0x78, 0x00,
	0x1C, 0x0C, 0x0C, 0x7C, 0xCC, 0xCC, 0x76, 0x00,
	0x00, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00,
	0x38, 0x6C, 0x64, 0xF0, 0x60, 0x60, 0xF0, 0x00,
	0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8,
	0xE0, 0x60, 0x6C, 0x76, 0x66, 0x66, 0xE6, 0x00,
	0x30, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,
	0x0C, 0x00, 0x1C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78,
	0xE0, 0x60, 0x66, 0x6C, 0x78, 0x6C, 0xE6, 0x00,
	0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,
	0x00, 0x00, 0xCC, 0xFE, 0xFE, 0xD6, 0xD6, 0x00,
	0x00, 0x00, 0xB8, 0xCC, 0xCC, 0xCC, 0xCC, 0x00,
	0x00, 0x00, 0x78, 0xCC, 0xCC, 0xCC, 0x78, 0x00,
	0x00, 0x00, 0xDC, 0x66, 0x66, 0x7C, 0x60, 0xF0,
	0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0x1E,
	0x00, 0x00, 0xDC, 0x76, 0x62, 0x60, 0xF0, 0x00,
	0x00, 0x00, 0x7C, 0xC0, 0x70, 0x1C, 0xF8, 0x00,
	0x10, 0x30, 0xFC, 0x30, 0x30, 0x34, 0x18, 0x00,
	0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00,
	0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00,
	0x00, 0x00, 0xC6, 0xC6, 0xD6, 0xFE, 0x6C, 0x00,
	0x00, 0x00, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0x00,
	0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8,
	0x00, 0x00, 0xFC, 0x98, 0x30, 0x64, 0xFC, 0x00,
	0x1C, 0x30, 0x30, 0xE0, 0x30, 0x30, 0x1C, 0x00,
	0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00,
	0xE0, 0x30, 0x30, 0x1C, 0x30, 0x30, 0xE0, 0x00,
	0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0x00,
	0x7C, 0xC6, 0xC0, 0xC6, 0x7C, 0x0C, 0x06, 0x7C,
	0x00, 0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00,
	0x1C, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00,
	0x7E, 0x81, 0x3C, 0x06, 0x3E, 0x66, 0x3B, 0x00,
	0xCC, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,
	0xE0, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,
	0x30, 0x30, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,
	0x00, 0x00, 0x7C, 0xC6, 0xC0, 0x78, 0x0C, 0x38,
	0x7E, 0x81, 0x3C, 0x66, 0x7E, 0x60, 0x3C, 0x00,
	0xCC, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00,
	0xE0, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00,
	0xCC, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,
	0x7C, 0x82, 0x38, 0x18, 0x18, 0x18, 0x3C, 0x00,
	0xE0, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,
	0xC6, 0x10, 0x7C, 0xC6, 0xFE, 0xC6, 0xC6, 0x00,
	0x30, 0x30, 0x00, 0x78, 0xCC, 0xFC, 0xCC, 0x00,
	0x1C, 0x00, 0xFC, 0x60, 0x78, 0x60, 0xFC, 0x00,
	0x00, 0x00, 0x7F, 0x0C, 0x7F, 0xCC, 0x7F, 0x00,
	0x3E, 0x6C, 0xCC, 0xFE, 0xCC, 0xCC, 0xCE, 0x00,
	0x78, 0x84, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00,
	0x00, 0xCC, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00,
	0x00, 0xE0, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00,
	0x78, 0x84, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00,
	0x00, 0xE0, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00,
	0x00, 0xCC, 0x00, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8,
	0xC3, 0x18, 0x3C, 0x66, 0x66, 0x3C, 0x18, 0x00,
	0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x00,
	0x18, 0x18, 0x7E, 0xC0, 0xC0, 0x7E, 0x18, 0x18,
	0x38, 0x6C, 0x64, 0xF0, 0x60, 0xE6, 0xFC, 0x00,
	0xCC, 0xCC, 0x78, 0x30, 0xFC, 0x30, 0xFC, 0x30,
	0xF8, 0xCC, 0xCC, 0xFA, 0xC6, 0xCF, 0xC6, 0xC3,
	0x0E, 0x1B, 0x18, 0x3C, 0x18, 0x18, 0xD8, 0x70,
	0x1C, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,
	0x38, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,
	0x00, 0x1C, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00,
	0x00, 0x1C, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00,
	0x00, 0xF8, 0x00, 0xB8, 0xCC, 0xCC, 0xCC, 0x00,
	0xFC, 0x00, 0xCC, 0xEC, 0xFC, 0xDC, 0xCC, 0x00,
	0x3C, 0x6C, 0x6C, 0x3E, 0x00, 0x7E, 0x00, 0x00,
	0x38, 0x6C, 0x6C, 0x38, 0x00, 0x7C, 0x00, 0x00,
	0x18, 0x00, 0x18, 0x18, 0x30, 0x66, 0x3C, 0x00,
	0x00, 0x00, 0x00, 0xFC, 0xC0, 0xC0, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFC, 0x0C, 0x0C, 0x00, 0x00,
	0xC6, 0xCC, 0xD8, 0x36, 0x6B, 0xC2, 0x84, 0x0F,
	0xC3, 0xC6, 0xCC, 0xDB, 0x37, 0x6D, 0xCF, 0x03,
	0x18, 0x00, 0x18, 0x18, 0x3C, 0x3C, 0x18, 0x00,
	0x00, 0x33, 0x66, 0xCC, 0x66, 0x33, 0x00, 0x00,
	0x00, 0xCC, 0x66, 0x33, 0x66, 0xCC, 0x00, 0x00,
	0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88,
	0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA,
	0xDB, 0xF6, 0xDB, 0x6F, 0xDB, 0x7E, 0xD7, 0xED,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0xF8, 0x18, 0x18, 0x18,
	0x18, 0x18, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18,
	0x36, 0x36, 0x36, 0x36, 0xF6, 0x36, 0x36, 0x36,
	0x00, 0x00, 0x00, 0x00, 0xFE, 0x36, 0x36, 0x36,
	0x00, 0x00, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18,
	0x36, 0x36, 0xF6, 0x06, 0xF6, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x00, 0x00, 0xFE, 0x06, 0xF6, 0x36, 0x36, 0x36,
	0x36, 0x36, 0xF6, 0x06, 0xFE, 0x00, 0x00, 0x00,
	0x36, 0x36, 0x36, 0x36, 0xFE, 0x00, 0x00, 0x00,
	0x18, 0x18, 0xF8, 0x18, 0xF8, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xF8, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x1F, 0x00, 0x00, 0x00,
	0x18, 0x18, 0x18, 0x18, 0xFF, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xFF, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x1F, 0x18, 0x18, 0x18,
	0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
	0x18, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18,
	0x36, 0x36, 0x36, 0x36, 0x37, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x37, 0x30, 0x3F, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3F, 0x30, 0x37, 0x36, 0x36, 0x36,
	0x36, 0x36, 0xF7, 0x00, 0xFF, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xFF, 0x00, 0xF7, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x37, 0x30, 0x37, 0x36, 0x36, 0x36,
	0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00,
	0x36, 0x36, 0xF7, 0x00, 0xF7, 0x36, 0x36, 0x36,
	0x18, 0x18, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00,
	0x36, 0x36, 0x36, 0x36, 0xFF, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xFF, 0x00, 0xFF, 0x18, 0x18, 0x18,
	0x00, 0x00, 0x00, 0x00, 0xFF, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x36, 0x36, 0x3F, 0x00, 0x00, 0x00,
	0x18, 0x18, 0x1F, 0x18, 0x1F, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18,
	0x00, 0x00, 0x00, 0x00, 0x3F, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x36, 0x36, 0xFF, 0x36, 0x36, 0x36,
	0x18, 0x18, 0xFF, 0x18, 0xFF, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0xF8, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x1F, 0x18, 0x18, 0x18,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
	0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
	0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x76, 0xDC, 0xC8, 0xDC, 0x76, 0x00,
	0x00, 0x78, 0xCC, 0xF8, 0xCC, 0xF8, 0xC0, 0xC0,
	0x00, 0xFC, 0xCC, 0xC0, 0xC0, 0xC0, 0xC0, 0x00,
	0x00, 0x00, 0xFE, 0x6C, 0x6C, 0x6C, 0x6C, 0x00,
	0xFC, 0xCC, 0x60, 0x30, 0x60, 0xCC, 0xFC, 0x00,
	0x00, 0x00, 0x7E, 0xD8, 0xD8, 0xD8, 0x70, 0x00,
	0x00, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0xC0,
	0x00, 0x76, 0xDC, 0x18, 0x18, 0x18, 0x18, 0x00,
	0xFC, 0x30, 0x78, 0xCC, 0xCC, 0x78, 0x30, 0xFC,
	0x38, 0x6C, 0xC6, 0xFE, 0xC6, 0x6C, 0x38, 0x00,
	0x38, 0x6C, 0xC6, 0xC6, 0x6C, 0x6C, 0xEE, 0x00,
	0x1C, 0x30, 0x18, 0x7C, 0xCC, 0xCC, 0x78, 0x00,
	0x00, 0x00, 0x7E, 0xDB, 0xDB, 0x7E, 0x00, 0x00,
	0x06, 0x0C, 0x7E, 0xDB, 0xDB, 0x7E, 0x60, 0xC0,
	0x38, 0x60, 0xC0, 0xF8, 0xC0, 0x60, 0x38, 0x00,
	0x78, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x00,
	0x00, 0x7E, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00,
	0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x7E, 0x00,
	0x60, 0x30, 0x18, 0x30, 0x60, 0x00, 0xFC, 0x00,
	0x18, 0x30, 0x60, 0x30, 0x18, 0x00, 0xFC, 0x00,
	0x0E, 0x1B, 0x1B, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0xD8, 0xD8, 0x70,
	0x18, 0x18, 0x00, 0x7E, 0x00, 0x18, 0x18, 0x00,
	0x00, 0x76, 0xDC, 0x00, 0x76, 0xDC, 0x00, 0x00,
	0x38, 0x6C, 0x6C, 0x38, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
	0x0F, 0x0C, 0x0C, 0x0C, 0xEC, 0x6C, 0x3C, 0x1C,
	0x58, 0x6C, 0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00,
	0x70, 0x98, 0x30, 0x60, 0xF8, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * write vga register dump to the card.
 *
 */

void		write_regs(unsigned char *regs)
{
  unsigned int	i, a;

  OUTB(VGA_MISC_WRITE, *regs);
  regs++;
  for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
    {
      OUTB(VGA_SEQ_INDEX, i);
      OUTB(VGA_SEQ_DATA, *regs);
      regs++;
    }
  OUTB(VGA_CRTC_INDEX, 0x03);
  INB(VGA_CRTC_DATA, a);
  OUTB(VGA_CRTC_DATA, a | 0x80);
  OUTB(VGA_CRTC_INDEX, 0x11);
  INB(VGA_CRTC_DATA, a);
  OUTB(VGA_CRTC_DATA, a & ~0x80);
  regs[0x03] |= 0x80;
  regs[0x11] &= ~0x80;
  for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
    {
      OUTB(VGA_CRTC_INDEX, i);
      OUTB(VGA_CRTC_DATA, *regs);
      regs++;
    }
  for(i = 0; i < VGA_NUM_GC_REGS; i++)
    {
      OUTB(VGA_GC_INDEX, i);
      OUTB(VGA_GC_DATA, *regs);
      regs++;
    }
  for(i = 0; i < VGA_NUM_AC_REGS; i++)
    {
      INB(VGA_INSTAT_READ, a);
      OUTB(VGA_AC_INDEX, i);
      OUTB(VGA_AC_WRITE, *regs);
      regs++;
    }
  INB(VGA_INSTAT_READ, a);
  OUTB(VGA_AC_INDEX, 0x20);
}

/*
 * get base vga segment.
 *
 */

static char*	get_fb_seg(void)
{
  unsigned int	seg;

  OUTB(VGA_GC_INDEX, 6);
  INB(VGA_GC_DATA, seg);
  seg >>= 2;
  seg &= 3;
  switch(seg)
    {
      case 0:
      case 1:
	seg = 0xA0000;
		break;
      case 2:
	seg = 0xB0000;
	break;
      case 3:
	seg = 0xB8000;
	break;
    }
  return (char *)seg;
}

/*
 * this function just clears the console.
 */

void			cons_clear(void)
{
  t_uint16		i;

  for (i = 0; i < fbcons.size; i++)
    {
      fbcons.vga[i] = 0x0;
    }

  cons.line = 0;
  cons.column = 0;
}

/*
 * this function scrolls the screen.
 */

void			cons_scroll(t_uint16			lines)
{
  unsigned int		ln;

  ln = (fbcons.width * 8) / (8 / fbcons.bpp);
  memcpy(fbcons.vga, fbcons.vga + ln, fbcons.size - ln);
  memset(fbcons.vga + fbcons.size - ln, 0, ln);

  cons.column = 0;
  cons.line -= lines;
}

/*
 * this function is called by the printf function to change the
 * current console attributes with the sequence %#.
 */

void			cons_attr(t_uint8			attr)
{
  cons.attr = attr;
}

/*
 * used to decompose a byte
 */
static int bit_on(char c, int n)
{
  int	mask;

  mask = 1 << (7 - n);
  return c & mask;
}

/*
 * this function is called by the printf function to print a
 * character.
 */

int			cons_print_char(char			c)
{
  int			pos;
  int			i, j;
  int			fg;
  int			bg;
  char			ch, p;

  if (c == '\n')
    {
      cons.line++;
      cons.column = 0;

      return 1;
    }

  if (c == '\r')
    {
      cons.column = 0;

      return 1;
    }

  if (cons.column == fbcons.columns)
    {
      ++cons.line;
      cons.column = 0;
    }

  if (cons.line == fbcons.lines)
    {
      cons_scroll(1);
    }

  if (fbcons.bpp == 8)
    {
      fg = cons.attr & 0xF;
      bg = (cons.attr >> 4) & 0xF;

      for (i = 0; i < 8; ++i)
	for (j = 0; j < 8; ++j)
	  {
	    ch = g_8x8_font[c * 8 + i];
	    p = bit_on(ch, j) ? fg : bg;

	    pos = ((cons.line * 8 + i) * fbcons.width) +
	      (cons.column * 8) + j;
	    fbcons.vga[pos] = p;
	  }
    }
  else if (fbcons.bpp == 4)
    {
      fg = cons.attr & 0xF;
      bg = (cons.attr >> 4) & 0xF;

      for (i = 0; i < 8; ++i)
	for (j = 0; j < 4; ++j)
	  {
	    ch = g_8x8_font[c * 8 + i];
	    p = bit_on(ch, j) ? fg : bg;
	    p |= ((bit_on(ch, j + 1) ? fg : bg) << 4);

	    pos = ((cons.line * 8 + i) * fbcons.width) / 2 +
	      (cons.column * 8) / 2 + j;
	    fbcons.vga[pos] = p;
	  }
    }
  else if (fbcons.bpp == 1)
    {
      for (i = 0; i < 8; ++i)
	{
	  fbcons.vga[(cons.line * 8 + i) * fbcons.width / 8 +
		     cons.column] = g_8x8_font[c * 8 + i];
	}
    }
  cons.column++;

  return (1);
}

/*
 * this function just prints a string.
 */

void			cons_print_string(char*			string)
{
  t_uint32		i;

  for (i = 0; string[i]; i++)
    cons_print_char(string[i]);
}

/*
 * this function prints a status message.
 *
 * '+' is used for printing information about the execution.
 * '#' is used for printing debug information.
 * '!' is used for printing warning and error messages.
 */

void			cons_msg(char				indicator,
				 char*				fmt,
				 ...)
{
  t_uint8		attr = cons.attr;
  va_list		args;

  cons.attr = CONS_FRONT(CONS_BLUE) | CONS_BACK(CONS_BLACK) | CONS_INT;
  cons_print_char('[');

  switch (indicator)
    {
    case '+':
      cons.attr = CONS_FRONT(CONS_GREEN) | CONS_BACK(CONS_BLACK) | CONS_INT;
      break;
    case '#':
      cons.attr = CONS_FRONT(CONS_MAGENTA) | CONS_BACK(CONS_BLACK) | CONS_INT;
      break;
    case '!':
      cons.attr = CONS_FRONT(CONS_RED) | CONS_BACK(CONS_BLACK) | CONS_INT;
      break;
    }

  cons_print_char(indicator);

  cons.attr = CONS_FRONT(CONS_BLUE) | CONS_BACK(CONS_BLACK) | CONS_INT;
  cons_print_string("] ");

  cons.attr = attr;

  va_start(args, fmt);

  vprintf(fmt, args);

  va_end(args);
}

/*
 * this function just initialises the bootloader console.
 *
 * this function also initialises the printf function to work with the
 * console.
 */

t_error			cons_init(void)
{
  char			buff[10 * CONS_COLUMNS * CONS_BPC];
  int			prevx, prevy, i, j;
  int			pos;
  int			attr;

  cons.attr = CONS_FRONT(CONS_WHITE) | CONS_BACK(CONS_BLACK) | CONS_INT;
  cons.vga = (char*)CONS_ADDR;
  memcpy(buff, cons.vga, 10 * CONS_COLUMNS * CONS_BPC);

#ifdef MODE1
  write_regs(g_640x480x2);
  fbcons.width = 640;
  fbcons.columns = 80;
  fbcons.height = 480;
  fbcons.lines = 60;
  fbcons.bpp = 1;
#endif
#ifdef MODE2
  write_regs(g_640x480x16);
  fbcons.width = 640;
  fbcons.columns = 80;
  fbcons.height = 480;
  fbcons.lines = 60;
  fbcons.bpp = 4;
#endif
#ifdef MODE3
  write_regs(g_320x200x256);
  fbcons.width = 320;
  fbcons.columns = 40;
  fbcons.height = 200;
  fbcons.lines = 25;
  fbcons.bpp = 8;
#endif
  fbcons.size = (fbcons.width * fbcons.height) / (8 / fbcons.bpp);
  fbcons.vga = get_fb_seg();
  prevx = cons.column;
  prevy = cons.line;
  cons_clear();
  attr = cons.attr;
  for (i = 0; i < prevy; ++i)
    for (j = 0; j < CONS_COLUMNS; ++j)
      {
	pos = i * CONS_COLUMNS * CONS_BPC + j * CONS_BPC;
	cons_attr(buff[pos + 1]);
	cons_print_char(buff[pos]);
      }
  cons.attr = attr;
  printf_init(cons_print_char, cons_attr);

  return (ERROR_NONE);
}

/*
 * this function just reinitialises the bootloader console.
 *
 * there is nothing special to do.
 */

t_error			cons_clean(void)
{
  return (ERROR_NONE);
}

