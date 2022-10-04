
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

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "defs.h"

typedef struct
{
  LONG x, y, z;
} VECTOR;

typedef LONG PLANE[4];

typedef PLANE MATRIX[4];

VOID matrix_copy(MATRIX dst, MATRIX src);
VOID matrix_identity(MATRIX dst);
VOID matrix_position(MATRIX dst, int x, int y, int z);
VOID matrix_scale(MATRIX dst, int x, int y, int z);
VOID matrix_rotate_x(MATRIX dst, int angle);
VOID matrix_rotate_y(MATRIX dst, int angle);
VOID matrix_rotate_z(MATRIX dst, int angle);
VOID matrix_transpose(MATRIX dst, MATRIX src);
VOID matrix_multiply(MATRIX dst, MATRIX ma1, MATRIX ma2);

#endif

