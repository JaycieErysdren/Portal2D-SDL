
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

#include "engine.h"
#include "wall.h"
#include "polygon.h"
#include "level.h"
#include "dos.h"
#include "math.h"
#include "fix.h"

int number_of_new_walls;
VECTOR new_walls[200];

void _drawline(int x1, int y1, int x2, int y2)
{
  _moveto(x1, y1); _lineto(x2, y2);
}
    
void handle_render(int x, int y, int color)
{
  _setcolor(BLACK); _rectangle(_GBORDER, x - 2, y - 2, x + 2, y + 2);  
  _setcolor(color); _rectangle(_GFILLINTERIOR, x - 1, y - 1, x + 1, y + 1);
}

void view_to_client(int* x, int* y)
{
  float scale = view.zoom * view.grid / view.zoom_div;
  *x = (int)floor( (*x - 320.0) / scale + 0.5) * view.grid + objects[1].x;
  *y = (int)floor(-(*y - 240.0) / scale + 0.5) * view.grid + objects[1].y;
}

void client_to_screen(int* x, int* y)
{
  *x =   imuldiv(*x - objects[1].x, view.zoom, view.zoom_div) + 320;
  *y = - imuldiv(*y - objects[1].y, view.zoom, view.zoom_div) + 240;  
}

void find_portals(void)
{
  int i, j;
  
  for (i = 1; i < MAX_WALL; i++)
  {
    if (walls[i].sid)
    {
      for (j = 1; j < MAX_WALL; j++)
      {
        if (walls[j].sid && walls_touch(i, j))
        {
          walls[i].port = j;
          walls[j].port = i;          
        }
      }
    }
  }
}

int sector_alloc(int lid)
{
  int sid;
  
  assert(lid != 0);
  
  for (sid = 1; sid < MAX_SECTOR && sectors[sid].lid; sid++);
  
  assert(sid < MAX_SECTOR);
  
  memset(&sectors[sid], 0, sizeof(SECTOR));
  
  sectors[sid].lid = lid;

  sectors[sid].top.texture = 2;  
  sectors[sid].top.slopez  = WORLD_UNIT * 8;
      
  sectors[sid].bot.texture = 1;  
  
  return sid;
}

void sector_free(int sid)
{
  int wid, first_wall, next_wall;
  
  assert(sid && FIRST_WALL(sid));
    
  wid = first_wall = FIRST_WALL(sid);

  do
  {
    next_wall = NEXT_WALL(wid);
    memset(&walls[wid], 0, sizeof(WALL));
  }
  while ((wid = next_wall) != first_wall);
  
  memset(&sectors[sid], 0, sizeof(SECTOR));

  find_portals();
}

int sector_find_wall(int sid, int x, int y)
{
  int wid, first_wall;
  
  assert(sid && FIRST_WALL(sid));
  
  wid = first_wall = FIRST_WALL(sid);
  do
  {
    if (walls[wid].x == x && walls[wid].y == y) return wid;
  }
  while ((wid = NEXT_WALL(wid)) != first_wall);

  return 0;
}

int sector_make(VECTOR pts[], int n)
{
  int i, j, sid;
  WALL* wall;
  
  if (n < 3) return 0;
  
  for (i = n; i--;) for (j = n; j--;) if (i != j && pts[i].x == pts[j].x && pts[i].y == pts[j].y) return 0;
  
  sid = sector_alloc(1);
  
  wall = 0;
          
  for (i = 0; i < n; i++)
  {
    int wid = wall_create(sid);
                        
    if (i == 0) sectors[sid].first_wall = wid;

    if (wall) wall->next = wid;
            
    wall    = &walls[wid];
    wall->x = pts[i].x;
    wall->y = pts[i].y;

    if (pts[i].z)
    {
      wall->surface.light = walls[pts[i].z].surface.light;
    }
  }
  assert(wall);
          
  wall->next = sectors[sid].first_wall;

  find_portals();

  return sid;
}

int world_split(int x1, int y1, int x2, int y2)
{
  int w1;
  
  for (w1 = 1; w1 < MAX_WALL; w1++)
  {
    if (walls[w1].sid && walls[w1].x == x1 && walls[w1].y == y1)
    {      
      int w2 = sector_find_wall(walls[w1].sid, x2, y2);
      
      if (w2 && !walls_linked(w1, w2))
      {
        VECTOR poly[MAX_POLYGON];
        
        int s1 = sector_make(poly, wall_segment(poly, w1, w2));        
        int s2 = sector_make(poly, wall_segment(poly, w2, w1));

        //assert(s1 && s2);
        
        sectors[s1].bot = sectors[walls[w1].sid].bot;
        sectors[s1].top = sectors[walls[w1].sid].top;
        sectors[s2].bot = sectors[walls[w1].sid].bot;
        sectors[s2].top = sectors[walls[w1].sid].top;
                        
        sector_free(walls[w1].sid);
        
        return 1;    
      }    
    }
  }
  return 0;
}

void editor_render(int erase)
{
  int i, x1, x, xx, y1, y, yy, sid;
  
  mouse_show(0);  
  
  if (erase)
  {       
    _setcolor(GRAY);
    _rectangle(_GFILLINTERIOR, 0, 0, 640, 480);
  
    _setcolor(BLACK);

    x1 = 0; xx = imuldiv(view.zoom, view.grid, view.zoom_div);
    y1 = 0; yy = imuldiv(view.zoom, view.grid, view.zoom_div);
    
    view_to_client(&x1, &y1);
    client_to_screen(&x1, &y1);
    
    for (y = y1; y < view.height; y += yy)
      for (x = x1; x < view.width; x += xx)
        _setpixel(x, y);
   
    x = y = 0;
    client_to_screen(&x, &y);
    
    _setcolor(WHITE);
    _drawline(0, y, 640, y);
    _drawline(x, 0, x, 480);
  }
  _setcolor(1);
  
  for (sid = 0; sid < MAX_SECTOR; sid++)
  {
    SECTOR* sector = &sectors[sid];    
    
    if (sector->lid)
    {
      int wid = sector->first_wall;
      do 
      {
        int x = walls[wid].x;
        int y = walls[wid].y;
        client_to_screen(&x, &y);
        
        if (wid == sector->first_wall) _moveto(x, y); else _lineto(x, y);
        _setcolor(walls[wid].port ? RED : GREEN);
      }
      while ((wid = NEXT_WALL(wid)) != sector->first_wall);
            
      wid = sector->first_wall;
      do 
      {
        int x = walls[wid].x;
        int y = walls[wid].y;
        client_to_screen(&x, &y);
        
        if (wid == sector->first_wall) _lineto(x, y);
        
        handle_render(x, y, GREEN);
      }
      while ((wid = NEXT_WALL(wid)) != sector->first_wall);
    }
  }
  _setcolor(BRIGHTWHITE);
  
  for (i = 0; i < number_of_new_walls; i++)
  {
    int x = new_walls[i].x;
    int y = new_walls[i].y;
    client_to_screen(&x, &y);
    if (i) _lineto(x, y); else _moveto(x, y);    
  }
  for (i = 0; i < number_of_new_walls; i++)
  {
    int x = new_walls[i].x;
    int y = new_walls[i].y;
    client_to_screen(&x, &y);
    handle_render(x, y, BRIGHTWHITE); 
  }    
  mouse_show(1);
}

void editor_execute(void)
{
  STRING s;
  
  mouse_show(0);
  _setvideomode(_VRES16COLOR);
  mouse_show(1);

  _setviewport(0, 0, 640, 416);
  _settextwindow(27, 1, 30, 80);  
  _outtext("Editor Mode Started\n");
  
  objects[1].x = 0;
  objects[1].y = 0;
  objects[1].z = i2f(WORLD_UNIT * 4);
    
  editor_render(1);

  for (;;)
  {    
    while (!kbhit())
    {
      int x, y;     
      
      if (mouse_button_press(&x, &y))
      {
        view_to_client(&x, &y);       

        if (number_of_new_walls > 0 && new_walls[0].x == x && new_walls[0].y == y)
        {
          sector_make(new_walls, number_of_new_walls);
  
          number_of_new_walls = 0;
          editor_render(1);
        }
        else
        {
          new_walls[number_of_new_walls].x = x;
          new_walls[number_of_new_walls].y = y;
        
          number_of_new_walls++;

          if (number_of_new_walls == 2)
          {
            if (world_split(new_walls[0].x, new_walls[0].y, new_walls[1].x, new_walls[1].y))
            {
              number_of_new_walls = 0;
              editor_render(1);  
            }
          }
      
          editor_render(0);
        }
      }
    }
    if (kbhit())
    {
      if ((view.key = getch()) == 0) view.key = -getch();

      //_outtext(format("%d", view.key));
      
      if (view.key == 27 && number_of_new_walls > 0)
      {
        number_of_new_walls = 0; editor_render(1);
        _outtext("Operation Canceled\n");
      }
      if (view.key == 90 && view.zoom > 1)
      { 
        view.zoom--; editor_render(1);
        _outtext((sprintf(s, "Zoom Level = %d:%d\n", view.zoom, view.zoom_div), s));
      }       
      if (view.key == 122 && view.zoom < 100)
      {
        view.zoom++; editor_render(1);
        _outtext((sprintf(s, "Zoom Level = %d:%d\n", view.zoom, view.zoom_div), s));
      }
      if (view.key == 103 && view.grid > 16)
      {
        view.grid >>= 1; editor_render(1);
        _outtext((sprintf(s, "Grid Size = %d\n", view.grid), s));
      }
      if (view.key == 71 && view.grid < 1024)
      {
        view.grid <<= 1; editor_render(1);
        _outtext((sprintf(s, "Grid Size = %d\n", view.grid), s));
      }
            
      if (view.key == -77) {objects[1].x += view.grid; editor_render(1);}
      if (view.key == -75) {objects[1].x -= view.grid; editor_render(1);}        
      if (view.key == -72) {objects[1].y += view.grid; editor_render(1);}
      if (view.key == -80) {objects[1].y -= view.grid; editor_render(1);}

      if (view.key == 's')
      {
        PATH fn;
        
        _outtext("Enter map name to save: ");
                   
        gets(fn);

        if (fn[0]) level_save_to_file(fn);   
      }
      if (view.key == 'l')
      {
        PATH fn;        
        _outtext("Enter map name to load: ");
        gets(fn);
        if (fn[0]) level_load_from_file(fn);   
      }
           
      if (view.key ==  13) break;
      if (view.key == 'q') break;       
      if (view.key == 'l') break;      
      //printf("%d", view.key);
      //exit(-1);
    }
  }
}

