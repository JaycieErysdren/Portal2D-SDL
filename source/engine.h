
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

#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "defs.h"
#include "picture.h"
#include "matrix.h"

#define WORLD_UNIT 64

#define SIN_90    512
#define SIN_180   1024
#define SIN_360   2048
#define SIN_UNIT  i2f(1)
  
#define NEXT_WALL(A) (walls[A].next)
#define FIRST_WALL(A) (sectors[A].first_wall)

#ifndef PI
#define PI (3.14159265358979323846)
#endif

/*
** Standard DOS Colors
*/
#define BLACK         0x0
#define BLUE          0x1
#define GREEN         0x2
#define CYAN          0x3
#define RED           0x4
#define MAGENTA       0x5
#define BROWN         0x6
#define WHITE         0x7
#define GRAY          0x8
#define LIGHTBLUE     0x9
#define LIGHTGREEN    0xA
#define LIGHTCYAN     0xB
#define LIGHTRED      0xC
#define LIGHTMAGENTA  0xD
#define YELLOW        0xE
#define BRIGHTWHITE   0xF      
/*
** Engine Datatypes
*/
typedef struct
{
  int width;
  int height;
  int zoom;
  int zoom_div;
  int grid; 
  int key;
  PALLETE palette;
} VIEW;

extern VIEW view;

int sector_make(VECTOR pts[], int n);
int world_split(int x1, int y1, int x2, int y2);

void editor_execute();
  
#endif



