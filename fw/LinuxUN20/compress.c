/* This file exists solely to allow the wsq library to be called from C++ as the WSQ
 * library header files are not C++ safe when included so linkage fails
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "wsq.h"

// WSQ library debug control. Higher numbers give more output

int debug = 0;

extern int un20_wsq_encode_mem(unsigned char **odata, int *olen, const float r_bitrate,
                   unsigned char *idata, const int w, const int h,
                   const int d, const int ppi, char *comment_text)
{
  return wsq_encode_mem(odata, olen, r_bitrate, idata, w, h, d, ppi, comment_text);
}

