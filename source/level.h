
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

#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "engine.h"
#include "wall.h"
#include "sector.h"
#include "object.h"

extern int tick;

/*
** Engine Limits
*/
#define MAX_POINT 1000
#define MAX_WALL 5000
#define MAX_SECTOR 1000
#define MAX_OBJECT 1000
#define MAX_TEXTURE 256

extern WALL    walls[MAX_WALL];
extern SECTOR  sectors[MAX_SECTOR];
extern OBJECT  objects[MAX_OBJECT];
extern PICTURE textures[MAX_TEXTURE];
extern WORD    sector_list[MAX_SECTOR];
extern int    sector_list_count;

void level_save_to_file(PATH fn);
void level_load_from_file(PATH fn);

#endif

