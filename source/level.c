
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

#include "level.h"

int tick;

WALL    walls[MAX_WALL];
SECTOR  sectors[MAX_SECTOR];
OBJECT  objects[MAX_OBJECT];
PICTURE textures[MAX_TEXTURE];
WORD    sector_list[MAX_SECTOR];

int sector_list_count;

int chunk_write(FILE* fp, int type, int id, int flag, void* buff, int size)
{
  return
    (fwrite(&type, sizeof(int), 1, fp) == 1) &&
    (fwrite(&id  , sizeof(int), 1, fp) == 1) &&
    (fwrite(&flag, sizeof(int), 1, fp) == 1) &&
    (fwrite(&size, sizeof(int), 1, fp) == 1) &&
    (fwrite(buff , size       , 1, fp) == 1);
}

int chunk_read(FILE* fp, int* type, int* id, int* flag, void* buff, int* size)
{
  return
    (fread(type, sizeof(int), 1, fp) == 1) &&
    (fread(id  , sizeof(int), 1, fp) == 1) &&
    (fread(flag, sizeof(int), 1, fp) == 1) &&        
    (fread(size, sizeof(int), 1, fp) == 1) &&
    (fread(buff, *size      , 1, fp) == 1);
}

void wall_write(int wid, FILE* fp)
{    
  chunk_write(fp, 'WALL', wid, 0, &walls[wid], offsetof(WALL, reserved));
}

void sector_write(int sid, FILE* fp)
{
  chunk_write(fp, 'SECT', sid, 0, &sectors[sid], offsetof(SECTOR, reserved));
}

void level_write(FILE* fp)
{
  int i;
  for (i = 0; i < MAX_WALL  ; i++) if (walls  [i].sid) wall_write(i, fp);
  for (i = 0; i < MAX_SECTOR; i++) if (sectors[i].lid) sector_write(i, fp);  
}

void level_clear(void)
{
  memset(walls, 0, sizeof(walls));
  memset(sectors, 0, sizeof(sectors));  
}

void level_read(FILE* fp)
{
  char buffer[1024];
  int type, id, flag, size;
  
  level_clear();

  while (chunk_read(fp, &type, &id, &flag, buffer, &size))
  {   
    if (type == 'WALL')
    {
      assert(id < MAX_WALL);
      memcpy(&walls[id], buffer, size);
    }
    if (type == 'SECT')
    {
      assert(id < MAX_SECTOR);
      memcpy(&sectors[id], buffer, size);
    }
  }
}

void level_save_to_file(PATH fn)
{
  FILE* fp = fopen(fn, "wb");
  
  if (fp)
  {
    level_write(fp);
    fclose(fp);
  }
}

void level_load_from_file(PATH fn)
{
  FILE* fp = fopen(fn, "rb");
  
  if (fp)
  {
    level_read(fp);
    fclose(fp);
  }
}

