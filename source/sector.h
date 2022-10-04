
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

#ifndef __SECTOR_H__
#define __SECTOR_H__

#include "defs.h"
#include "polygon.h"

#define SECTOR_RENDER_MIDDLE 1

typedef struct
{
  // Persistent Variables. Moving these variables will break the file format!
  WORD    lid;
  WORD    padding;
  WORD    first_wall;
  WORD    flags;
  SURFACE top;
  SURFACE bot;
  SURFACE mid;
  
  int reserved;
  
  // Calculated variables. These variables are not saved out to a file.
    
  LONG  vis_id;
  RECT  bounds;
  POINT center;
  LONG  locked;
} SECTOR;

int sector_point_inside(int sid, int x, int y);
int sector_from_point(int x, int y);
void sector_z(int sid, int x, int y, int* bot, int* top, int* mid);
void sector_transform(int sid, MATRIX matrix);
int sector_extrude_ceiling(int sid, POLYGON poly);
int sector_extrude_floor(int sid, POLYGON poly);
int sector_extrude_mid(int sid, POLYGON poly);
POINT sector_center(int sid);
RECT sector_bounds(int sid);
int sectors_are_linked(int s1, int s2);
void sector_calc_vis(OBJECT* camera);

#endif

