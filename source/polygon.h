
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

#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "surface.h"

typedef struct
{
  LONG x;
  LONG y;
  LONG z;
  LONG u;
  LONG v;
  LONG w;
} VERTEX;

#define MAX_POLYGON 60

typedef VERTEX POLYGON[MAX_POLYGON];

int polygon_clip(POLYGON dst, POLYGON src, int n, int dist[]);
int polygon_clip_to_frustum(POLYGON dst, POLYGON src, int n);
void polygon_project(POLYGON dst, POLYGON src, int n, int width, int height, SURFACE* surface);

#endif

