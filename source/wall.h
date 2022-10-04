
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

#ifndef __WALL_H__
#define __WALL_H__

#include "polygon.h"
#include "object.h"
#include "matrix.h"
#include "defs.h"

typedef struct
{
  // Persistent Variables. Moving these variables will break the file format!
  WORD sid , _sid;
  WORD next, _next;
  WORD port, _port;
  LONG x, y;   
  SURFACE surface;
  
  int reserved;
    
  // Calculated variables. These variables are not saved out to a file.
  VERTEX top;
  VERTEX bot;
  VERTEX mid;
  VERTEX poly[4];
  LONG dx, dy;
  LONG visible;
  LONG frame;
  LONG length;
  LONG distance;
} WALL;

int wall_create(int sid);
void wall_extrude(int wid, POLYGON polygon);
void wall_transform(int wid, MATRIX matrix);
int walls_linked(int w1, int w2);
int walls_touch(int w1, int w2);
void wall_normal(int wid, int* x, int* y);
int wall_segment(VECTOR poly[], int w1, int w2);
void wall_line(int wid, int* x1, int* y1, int* x2, int* y2);
int wall_is_visible(int wid, int x, int y);
POINT wall_closest_point(int wid, int x, int y);

#endif

