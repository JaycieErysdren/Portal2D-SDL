
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

#include "matrix.h"
#include "fix.h"
#include <string.h>
/*
** Name: matrix_copy
** Desc: Copies the contents of one matrix to another.
*/
void matrix_copy(MATRIX dst, MATRIX src)
{
  memcpy(dst, src, sizeof(MATRIX));
}

/*
** Name: matrix_identity
** Desc: Returns a identity matrix.
*/
void matrix_identity(MATRIX dst)
{
  MATRIX src = 
  {
    {i2f(1), i2f(0), i2f(0), i2f(0)},
    {i2f(0), i2f(1), i2f(0), i2f(0)},
    {i2f(0), i2f(0), i2f(1), i2f(0)},
    {i2f(0), i2f(0), i2f(0), i2f(1)}
  };
  matrix_copy(dst, src);
}

void matrix_position(MATRIX dst, int x, int y, int z)
{
  matrix_identity(dst);
  dst[0][3] = x;
  dst[1][3] = y;
  dst[2][3] = z;
}

void matrix_scale(MATRIX dst, int x, int y, int z)
{
  matrix_identity(dst);
  dst[0][0] = x;
  dst[1][1] = y;
  dst[2][2] = z;  
}

void matrix_rotate_x(MATRIX dst, int angle)
{
  matrix_identity(dst);
  dst[1][1] =   (dst[2][2] = fixcos(angle));
  dst[1][2] = - (dst[2][1] = fixsin(angle));  
}

void matrix_rotate_y(MATRIX dst, int angle)
{
  matrix_identity(dst);
  dst[0][0] =   (dst[2][2] = fixcos(angle));
  dst[0][2] = - (dst[2][0] = fixsin(angle));
}

void matrix_rotate_z(MATRIX dst, int angle)
{
   matrix_identity(dst);
   dst[0][0] =   (dst[1][1] = fixcos(angle));
   dst[0][1] = - (dst[1][0] = fixsin(angle));  
}

void matrix_multiply(MATRIX dst, MATRIX ma1, MATRIX ma2)
{
  int i, j;
  MATRIX tmp;
   
  for (i = 4; i--;)
  {
    for (j = 4; j--;)
    {
      tmp[i][j] =
        fixmul(ma2[i][0], ma1[0][j]) +
        fixmul(ma2[i][1], ma1[1][j]) +
        fixmul(ma2[i][2], ma1[2][j]) +
        fixmul(ma2[i][3], ma1[3][j]); 
    }
  }
  matrix_copy(dst, tmp);
}

void matrix_transpose(MATRIX dst, MATRIX src)
{
  int x, y;
  
  for (y = 4; y--;) for (x = 4; x--;) dst[x][y] = src[y][x];
}
