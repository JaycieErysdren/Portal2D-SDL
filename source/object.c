
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

#include "object.h"
#include "level.h"
#include "sector.h"
#include "math.h"
#include "fix.h"

void object_local_space(OBJECT* object, MATRIX matrix)
{
  MATRIX m;
  
  matrix_identity(matrix);
  matrix_rotate_x(m, -object->rot.x);
  matrix_multiply(matrix, matrix, m);    
  matrix_rotate_y(m, -object->rot.y);
  matrix_multiply(matrix, matrix, m);
  matrix_rotate_z(m, -object->rot.z);
  matrix_multiply(matrix, matrix, m);  
  matrix_position(m, object->x << 6, object->z << 6, object->y << 6);
  matrix_multiply(matrix, matrix, m);  
}

void object_matrix(OBJECT* object, MATRIX matrix, int width, int height)
{
  MATRIX m;  
  
  matrix_identity(matrix);

  matrix_position(m, - object->x << 6, - object->z << 6, - object->y << 6);
  matrix_multiply(matrix, matrix, m);
  
  matrix_rotate_z(m, object->rot.z);
  matrix_multiply(matrix, matrix, m);
  
  matrix_rotate_y(m, object->rot.y);
  matrix_multiply(matrix, matrix, m);
  
  matrix_rotate_x(m, object->rot.x);
  matrix_multiply(matrix, matrix, m);

  matrix_scale(m, i2f(1), fixdiv(i2f(width) / height, i2f(640) / 480), i2f(1));
  matrix_multiply(matrix, matrix, m);
}

void object_update(OBJECT* object)
{
  object->xxx += object->xx;
  object->yyy += object->yy;
  object->zzz += object->zz;
    
  object->x += f2i(object->xxx);
  object->y += f2i(object->yyy);
  object->z += f2i(object->zzz);

  object->xxx -= i2f(f2i(object->xxx));
  object->yyy -= i2f(f2i(object->yyy));
  object->zzz -= i2f(f2i(object->zzz));
        
  if (!sector_point_inside(object->sid, object->x, object->y))
  {
    int first_wall = FIRST_WALL(object->sid), wid = first_wall;
    do
    {
      if (walls[wid].port && sector_point_inside(walls[walls[wid].port].sid, object->x, object->y))
      {
        object->sid = walls[walls[wid].port].sid;
        return;
      }
    }
    while ((wid = NEXT_WALL(wid)) != first_wall);
    
    {
    int sid = sector_from_point(object->x, object->y);
          
    if (sid) object->sid = sid;
    }
  }
}

void object_sector_collision(OBJECT* object, int sid);

/*
** Name: object_wall_collision
** Desc: Handle the collision between an object and a wall. Traverses though portals.
*/
void object_wall_collision(OBJECT* object, int wid)
{
  if (wall_is_visible(wid, object->x, object->y))
  {    
    POINT p = wall_closest_point(wid, object->x, object->y);

    int nx = object->x - p.x;
    int ny = object->y - p.y;

    int len = isqr(nx) + isqr(ny);

    if (len <= isqr(50))
    {
      if (walls[wid].port)
      {
        int bot, top;
        
        sector_z(walls[walls[wid].port].sid, p.x, p.y, &bot, &top, 0);
      
        if ((object->z << 6) > bot && (object->z << 6) < top)
        {
          object_sector_collision(object, walls[walls[wid].port].sid);
          return;
        }
      }
      len = isqrt(len);
    
      object->x += imuldiv(nx, 50 - len, len);
      object->y += imuldiv(ny, 50 - len, len);
    }
  }
}

/*
** Name: object_sector_collision
** Desc: Handle the collision between an object and a sector. We iterate the walls and
**       call object_wall_collision() for each. A list of sectors that have been
**       visited is recorder to prevent a endless loop.
*/
void object_sector_collision(OBJECT* object, int sid)
{
  int i, first_wall, wid;
  
  for (i = sector_list_count; i--;) if (sector_list[i] == sid) return;

  sector_list[sector_list_count++] = sid;
  
  first_wall = FIRST_WALL(sid), wid = first_wall;
    
  do object_wall_collision(object, wid); while ((wid = NEXT_WALL(wid)) != first_wall);
}

/*
** Name: object_collision
** Desc: Handle object to world collisions.
*/
void object_collision(OBJECT* object)
{
  sector_list_count = 0;
  object_sector_collision(object, object->sid);
}

