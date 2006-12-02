
/**
 ** Auto-generated file, dont modify directly
 ** your changes will be lost !
 **
 ** Generated by DSX on 2006-04-12 16:13:48.077966
 ** by unknown@callisto
 **/

#include <klibc.h>

#include <dsx.h>
#define DSX_INMAIN
#include "tg_proto.h"
#include "demux_proto.h"
#include "vld_proto.h"
#include "iqzz_proto.h"
#include "idct_proto.h"
#include "libu_proto.h"
#include "ramdac_proto.h"


int dsx_log_open();
void endianness_init();

void mjpeg(void)
{
  dsx_mwmr_t tg_demux;
  dsx_mwmr_t quanti;
  dsx_mwmr_t vld_iqzz;
  dsx_mwmr_t demux_vld;
  dsx_mwmr_t huffman;
  dsx_mwmr_t iqzz_idct;
  dsx_mwmr_t idct_libu;
  dsx_mwmr_t libu_ramdac;

  dsx_mwmr_alloc(&tg_demux, 8, 2, "tg_demux");
  dsx_mwmr_alloc(&quanti, 8, 4, "quanti");
  dsx_mwmr_alloc(&vld_iqzz, 32, 2, "vld_iqzz");
  dsx_mwmr_alloc(&demux_vld, 8, 2, "demux_vld");
  dsx_mwmr_alloc(&huffman, 8, 6, "huffman");
  dsx_mwmr_alloc(&iqzz_idct, 64, 2, "iqzz_idct");
  dsx_mwmr_alloc(&idct_libu, 16, 2, "idct_libu");
  dsx_mwmr_alloc(&libu_ramdac, 96, 2, "libu_ramdac");

  tg_args_t tg_args = {
    .output = tg_demux,
    .is_unused = 42
  };
  tg_launcher( &tg_args );


  demux_args_t demux_args = {
    .input = tg_demux,
    .quanti = quanti,
    .huffman = huffman,
    .output = demux_vld,
    .is_unused = 42
  };
  demux_launcher( &demux_args );


  vld_args_t vld_args = {
    .input = demux_vld,
    .huffman = huffman,
    .output = vld_iqzz,
    .is_unused = 42
  };
  vld_launcher( &vld_args );


  iqzz_args_t iqzz_args = {
    .input = vld_iqzz,
    .quanti = quanti,
    .output = iqzz_idct,
    .is_unused = 42
  };
  iqzz_launcher( &iqzz_args );


  idct_args_t idct_args = {
    .input = iqzz_idct,
    .output = idct_libu,
    .is_unused = 42
  };
  idct_launcher( &idct_args );


  libu_args_t libu_args = {
    .input = idct_libu,
    .output = libu_ramdac,
    .is_unused = 42
  };
  libu_launcher( &libu_args );


  ramdac_args_t ramdac_args = {
    .input = libu_ramdac,
    .is_unused = 42
  };
  ramdac_launcher( &ramdac_args );


  while (1)
    pthread_yield();
}
