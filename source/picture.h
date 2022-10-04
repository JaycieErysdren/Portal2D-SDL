
/* 
** The contents of this file are subject to the Mozilla Public License
** Version 1.1 (the "License"); you may not use this file except in compliance
** with the License. You may obtain a copy of the License at
** http://www.mozilla.org/MPL/
** 
** Software distributed under the License is distributed on an "AS IS" basis,
** WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for
** the specific language governing rights and limitations under the License.
*/

#ifndef __PICTURE_H__
#define __PICTURE_H__

#include "defs.h"

#define RGB_BRIGHTNESS(R,G,B) (((R) * 77 + (G) * 150 + (B) * 29) >> 8)

typedef union
{
  BYTE**  b;
  WORD**  w;
  DWORD** l;
} SCANLINES;

typedef struct
{
  LONG  width;              /* width of picture in pixels */
  LONG  height;             /* height of picture in pixels */
  LONG  bpp;                /* bits per pixel */
  LONG  bytes_per_row;      /* bytes per row. rows are 4 byte aligned */
  LONG  shared;             /* false if buffer is owned. TODO: reference counting */  
  VOID* buffer;             /* allocated pixel buffer */
  SCANLINES scanlines;      /* pre-calculated scanline pointers */  
} PICTURE;

void picture_create(PICTURE* picture, int width, int height, int bpp, int bytes_per_row, void* buffer);
void picture_destroy(PICTURE* picture);
void picture_resize(PICTURE* picture, int width, int height);
void picture_clear(PICTURE* picture);
void picture_copy(PICTURE* dst, PICTURE* src);
void picture_load_from_file(PICTURE* picture, PATH filename, PALLETE palette);
void picture_save_to_file(PICTURE* picture, PATH filename, PALLETE palette);
void picture_flip8(PICTURE* dst, PICTURE* src);

void picture_draw8(PICTURE* dst, PICTURE* src, int x, int y, int mode);

#define PICTURE_MODE_COPY     0
#define PICTURE_MODE_COLORKEY 1

void picture_blit8(
   PICTURE* dst, int x1, int y1, int x2, int y2,
   PICTURE* src, int u1, int v1, int u2, int v2, int flags);

void picture_luquid_effect8(PICTURE* dst, PICTURE* src, int tick);
void picture_blend8(PICTURE* dst, PICTURE* src1, PICTURE* src2, CLUT blender);

int palette_search(PALLETE palette, int r, int g, int b);
   
#endif

