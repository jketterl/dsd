/*
 * Copyright (C) 2010 DSD Author
 * GPG Key ID: 0x3F1D7FD0 (74EF 430D F7F2 0A48 FCE6  F630 FAA2 635D 3F1D 7FD0)
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef INCLUDE_DSD_STATE_H_
#define INCLUDE_DSD_STATE_H_

#include <mbelib.h>
#include "p25p1_heuristics.h"

typedef struct
{
  int *dibit_buf;
  int *dibit_buf_p;
  int repeat;
  short *audio_out_buf;
  short *audio_out_buf_p;
  float *audio_out_float_buf;
  float *audio_out_float_buf_p;
  float audio_out_temp_buf[160];
  float *audio_out_temp_buf_p;
  int audio_out_idx;
  int audio_out_idx2;
  //int wav_out_bytes;
  int center;
  int jitter;
  int synctype;
  int min;
  int max;
  int lmid;
  int umid;
  int minref;
  int maxref;
  int lastsample;
  int sbuf[128];
  int sidx;
  int maxbuf[1024];
  int minbuf[1024];
  int midx;
  char err_str[64];
  char fsubtype[16];
  char ftype[16];
  int symbolcnt;
  int rf_mod;
  int numflips;
  int lastsynctype;
  int lastp25type;
  int offset;
  int carrier;
  char tg[25][16];
  int tgcount;
  int lasttg;
  int lastsrc;
  int nac;
  int errs;
  int errs2;
  int mbe_file_type;
  int optind;
  int numtdulc;
  int firstframe;
  char slot0light[8];
  char slot1light[8];
  float aout_gain;
  float aout_max_buf[200];
  float *aout_max_buf_p;
  int aout_max_buf_idx;
  int samplesPerSymbol;
  int symbolCenter;
  char algid[9];
  char keyid[17];
  int currentslot;
  mbe_parms *cur_mp;
  mbe_parms *prev_mp;
  mbe_parms *prev_mp_enhanced;
  int p25kid;

  unsigned int debug_audio_errors;
  unsigned int debug_header_errors;
  unsigned int debug_header_critical_errors;

  // Last dibit read
  int last_dibit;

  // Heuristics state data for +P5 signals
  P25Heuristics p25_heuristics;

  // Heuristics state data for -P5 signals
  P25Heuristics inv_p25_heuristics;

#ifdef TRACE_DSD
  char debug_prefix;
  char debug_prefix_2;
  unsigned int debug_sample_index;
  unsigned int debug_sample_left_edge;
  unsigned int debug_sample_right_edge;
  FILE* debug_label_file;
  FILE* debug_label_dibit_file;
  FILE* debug_label_imbe_file;
#endif

} dsd_state;

void initState (dsd_state * state);

#endif /* INCLUDE_DSD_STATE_H_ */
