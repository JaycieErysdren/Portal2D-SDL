
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

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "matrix.h"
#include "surface.h"

typedef struct
{
  LONG x, xx, xxx;
  LONG y, yy, yyy;
  LONG z, zz, zzz;
  VECTOR rot;
  LONG sid;
  LONG flags;  
  SURFACE front;
  SURFACE back;
} OBJECT;

void object_local_space(OBJECT* object, MATRIX matrix);
void object_matrix(OBJECT* object, MATRIX matrix, int width, int height);
void object_update(OBJECT* object);
void object_collision(OBJECT* object);

#endif

