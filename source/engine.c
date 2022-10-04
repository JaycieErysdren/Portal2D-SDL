
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

/*
** POSSIBLE GAME TYPES:
**  Carmageddon, Doom, Blood, Duke Nukem, Shadow Warrior, Driver, Tomb Raider, Dark Forces,
**  Jedi Knight, Uprising
**
** DESIGN CRITERIA
**  - Portable ANSIC 32bit code. 
**  - Playable on low spec PC's and game consoles. Playstation, Gameboy Advanced. 
**  - Easy world design & game programming.
**  - Flexable game platform. Should handle different types of games.
**  - Small world download for dynamic internet games.
**  - Inside & outside rendering.
**  - Portable to any video graphics card. (MIN: 320x200 Chunky graphics)
**  - Independant of floating point processor. (Fixed point maths)
**  - Extendable. Static lighting, multi leveled sectors, zbuffered polygon meshes.
**  - Compatible with DOOM & Build engine AI code.
**
** TODO
**  - Bases sector functions. Doors, lifts.
**  - global effects. Lightning, palette effects, screen shakes.
**  - Better collision detection and kinetics.
**  - 2D sprites (Use the current polygon rendering & add color keying)
**  - Correct sector sorting to remove dependancy on zbuffering. (Then add 2D Span clipping)
**  - OpenGL rendering. (Maybe Glide support for 3DFX. DOS & Linux)
**  - Look into MMX. (Not really useful these days)
**  - Sector based terain render. Create mesh from bitmap.
*/

#include "div/div0.h"
#include "engine.h"
#include "wall.h"
#include "sector.h"
#include "level.h"
#include "matrix.h"
#include "polygon.h"
#include "picture.h"
#include "dos.h"
#include "pragmas.h"
#include "fix.h"
#include "math.h"
/*
** Global Variables
*/
CLUT blender;
LIGHTMAP lightmaps[32];

VIEW view;

OBJECT* camera = &objects[1];
OBJECT* thing  = &objects[2];

PICTURE pic_stencil;
PICTURE pic_fbuffer;
PICTURE pic_bbuffer;
PICTURE pic_zbuffer;
PICTURE pic_arrow;
PICTURE pic_sky;
PICTURE pic_lens;
PICTURE pic_console;
PICTURE pic_font;

SURFACE clipboard;

PALLETE palette;

char sbuf[1024]; // General purpose string buffer. Results should be copied after use.

void picture_create_mip(PICTURE* dst, PICTURE* src, CLUT blender)
{
  int x, y;
  
  picture_create(dst, src->width, src->height, src->bpp, 0, 0);
  
  for (y = 0; y < dst->height; y += 2)
  {
    for (x = 0; x < dst->width; x += 2)
    {
      int a = blender[src->scanlines.b[y + 0][x + 0]][src->scanlines.b[y + 0][x + 1]];
      int b = blender[src->scanlines.b[y + 1][x + 0]][src->scanlines.b[y + 1][x + 1]];
      int c = blender[a][b];
      
      dst->scanlines.b[y][x] = c;
      dst->scanlines.b[y][x + 1] = c;
      dst->scanlines.b[y + 1][x] = c;
      dst->scanlines.b[y + 1][x + 1] = c;
    }
  } 
}

void lightmap_create(LIGHTMAP lightmap, PALLETE palette, int brightness)
{
  int fr = 0;
  int fg = 0;
  int fb = 0;
  int i, j;  
      
  for (i = 0; i < 256; i++)
  {
    for (j = 0; j < 32; j++)
    {
      lightmap[j][i] = palette_search(palette,
        imin(imuldiv(fr + imuldiv(palette[i][0] - fr, j, 31), brightness * 4, 31), 255),
        imin(imuldiv(fg + imuldiv(palette[i][1] - fg, j, 31), brightness * 4, 31), 255),
        imin(imuldiv(fb + imuldiv(palette[i][2] - fb, j, 31), brightness * 4, 31), 255));
    }
  }  
}

void say(char* s)
{
  _outtext(s);
  _outtext("\n");
}

void tables_create(void)
{
  int i, x, y, r;
      
  say("Calculating Lightmaps.");  
  for (i = 32; i--;) lightmap_create(lightmaps[i], palette, i);

  say("Calculating Blender LUT.");  
  
  for (x = 255; x--;)
  {
    for (y = 255; y--;)
    {
      r = (palette[x][0] + palette[y][0]) >> 1;
      
      blender[y][x] = palette_search(palette,
        (palette[x][0] + palette[y][0]) >> 1,
        (palette[x][1] + palette[y][1]) >> 1,
        (palette[x][2] + palette[y][2]) >> 1);
    }
  }
}

/*
** name: polygon_render
** desc: render polygon to the back buffer. standard perspective correct dda style rendering.
**       also handles sky rendering & mip mapping.
*/
#define RENDER_RECIPROCAL

void polygon_render(POLYGON src, int n, const SURFACE* surface, const WORD id, const int width, const int height)
{
  int i;
  POLYGON tmp1, tmp2;
  int ys[MAX_POLYGON];
    
  for (i = n; i--; ys[i] = src [i].x + src [i].z); n = polygon_clip(tmp2,  src, n, ys); // Left
  for (i = n; i--; ys[i] = tmp2[i].z - tmp2[i].x); n = polygon_clip(tmp1, tmp2, n, ys); // Right
  for (i = n; i--; ys[i] = tmp1[i].y + tmp1[i].z); n = polygon_clip(tmp2, tmp1, n, ys); // Top
  for (i = n; i--; ys[i] = tmp2[i].z - tmp2[i].y); n = polygon_clip(tmp1, tmp2, n, ys); // Bottom
    
  if (n >= 3)
  {
    int i, j;
    int lt_i, lt_x, lt_xx, lt_z, lt_zz, lt_u, lt_uu, lt_v, lt_vv, lt_w, lt_ww;
    int rt_i, rt_x, rt_xx, rt_z, rt_zz, rt_u, rt_uu, rt_v, rt_vv, rt_w, rt_ww;
    int x, xx;
      
    TEXTURE66* tb = textures[surface->texture].buffer;   
    LIGHTMAP*  lb = lightmaps[(surface->light + 16) & 31];
    
    int lt_length = 0;
    int rt_length = 0;  
    int y1 = INT_MAX;
    int y2 = INT_MIN;
        
    int cx = i2f(width ) >> 1;
    int cy = i2f(height) >> 1;
  
    for (i = n; i--;)
    {
      VERTEX p = tmp1[i];   

      p.z += 6; // 6 == 1 in view space.
            
      tmp2[i].x =   imuldiv(p.x, cx - i2f(0), p.z) + cx - (FRACCEIL >> 1);
      tmp2[i].y = - imuldiv(p.y, cy - i2f(0), p.z) + cy - (FRACCEIL >> 1);
      tmp2[i].z =   j = fixinv(p.z); 
      tmp2[i].u =   mulscale(p.u + surface->panningx, j, surface->repeatx + 18); 
      tmp2[i].v =   mulscale(p.v + surface->panningy, j, surface->repeaty + 18);
      tmp2[i].w =   p.w; 

      ys[i] = fixceil(tmp2[i].y);
    
      if (tmp2[i].y < y1) {y1 = tmp2[i].y; lt_i = rt_i = i;}
      if (tmp2[i].y > y2) {y2 = tmp2[i].y;}
    }        
    for (y1 = fixceil(y1), y2 = fixceil(y2); y1 < y2; y1++)
    {
      if (--lt_length <= 0)
      {        
        do
        {        
          i = lt_i;
          if (--lt_i < 0) lt_i = n - 1;
        }
        while ((lt_length = ys[lt_i] - ys[i]) <= 0);

#ifdef RENDER_RECIPROCAL           
        lt_xx = fixmul(tmp2[lt_i].x - tmp2[i].x, j = fixinv(tmp2[lt_i].y - tmp2[i].y));
        lt_zz = fixmul(tmp2[lt_i].z - tmp2[i].z, j);
        lt_uu = fixmul(tmp2[lt_i].u - tmp2[i].u, j);
        lt_vv = fixmul(tmp2[lt_i].v - tmp2[i].v, j);
        lt_ww = fixmul(tmp2[lt_i].w - tmp2[i].w, j);      
#else
        lt_xx = fixdiv(tmp2[lt_i].x - tmp2[i].x, j = tmp2[lt_i].y - tmp2[i].y);
        lt_zz = fixdiv(tmp2[lt_i].z - tmp2[i].z, j);
        lt_uu = fixdiv(tmp2[lt_i].u - tmp2[i].u, j);
        lt_vv = fixdiv(tmp2[lt_i].v - tmp2[i].v, j);
        lt_ww = fixdiv(tmp2[lt_i].w - tmp2[i].w, j);       
#endif                        
        lt_x  = tmp2[i].x + fixmul(lt_xx, j = i2f(ys[i]) - tmp2[i].y);
        lt_z  = tmp2[i].z + fixmul(lt_zz, j);
        lt_u  = tmp2[i].u + fixmul(lt_uu, j);
        lt_v  = tmp2[i].v + fixmul(lt_vv, j);
        lt_w  = tmp2[i].w + fixmul(lt_ww, j);
                                  
        assert(y1 == ys[i]);      
      }
      if (--rt_length <= 0)
      {        
        do
        {
          i = rt_i;
          if (++rt_i >= n) rt_i = 0;
        }
        while ((rt_length = ys[rt_i] - ys[i]) <= 0);        
        
#ifdef RENDER_RECIPROCAL        
        rt_xx = fixmul(tmp2[rt_i].x - tmp2[i].x, j = fixinv(tmp2[rt_i].y - tmp2[i].y));
        rt_zz = fixmul(tmp2[rt_i].z - tmp2[i].z, j);
        rt_uu = fixmul(tmp2[rt_i].u - tmp2[i].u, j);
        rt_vv = fixmul(tmp2[rt_i].v - tmp2[i].v, j);
        rt_ww = fixmul(tmp2[rt_i].w - tmp2[i].w, j);       
#else
        rt_xx = fixdiv(tmp2[rt_i].x - tmp2[i].x, j = tmp2[rt_i].y - tmp2[i].y);
        rt_zz = fixdiv(tmp2[rt_i].z - tmp2[i].z, j);
        rt_uu = fixdiv(tmp2[rt_i].u - tmp2[i].u, j);
        rt_vv = fixdiv(tmp2[rt_i].v - tmp2[i].v, j);
        rt_ww = fixdiv(tmp2[rt_i].w - tmp2[i].w, j);      
#endif                        
        rt_x  = tmp2[i].x + fixmul(rt_xx, j = i2f(ys[i]) - tmp2[i].y);
        rt_z  = tmp2[i].z + fixmul(rt_zz, j);
        rt_u  = tmp2[i].u + fixmul(rt_uu, j);
        rt_v  = tmp2[i].v + fixmul(rt_vv, j);
        rt_w  = tmp2[i].w + fixmul(rt_ww, j);
                            
        assert(y1 == ys[i]);
      }
      xx = fixceil(rt_x) - (x = fixceil(lt_x));
      
      if (xx > 0)
      {    
        BYTE* fb = &pic_bbuffer.scanlines.b[y1][x];
        WORD* sb = &pic_stencil.scanlines.w[y1][x];
        WORD* zb = &pic_zbuffer.scanlines.w[y1][x];

#ifdef RENDER_RECIPROCAL                
        int zz = fixmul(rt_z - lt_z, j = fixinv(rt_x - lt_x));
        int uu = fixmul(rt_u - lt_u, j);
        int vv = fixmul(rt_v - lt_v, j);
        int ww = fixmul(rt_w - lt_w, j);       
#else
        int zz = fixdiv(rt_z - lt_z, j = rt_x - lt_x);
        int uu = fixdiv(rt_u - lt_u, j);
        int vv = fixdiv(rt_v - lt_v, j);
        int ww = fixdiv(rt_w - lt_w, j);       
#endif                
        int z = lt_z + fixmul(zz, j = i2f(x) - lt_x);
        int u = lt_u + fixmul(uu, j);
        int v = lt_v + fixmul(vv, j);
        int w = lt_w + fixmul(ww, j);

        int zz16 = zz << 4;        
        int uu16 = uu << 4;
        int vv16 = vv << 4;
        int ww16 = ww << 4;
                
#define INNER_LOOP  \
  int a  = fixmul(u, j = fixinv(z)); \
  int b  = fixmul(v, j            ); \
  int aa = (fixmul(u += uu16, j = fixinv(z + zz16)) - a) >> 4; \
  int bb = (fixmul(v += vv16, j                   ) - b) >> 4; \  
  int c = w, cc = ((w += ww16) - c) >> 4; \  
  for (; xx >= 16; xx -= 16) { \                       
    AFFINE( 0) AFFINE( 1) AFFINE( 2) AFFINE( 3) AFFINE( 4) AFFINE( 5) AFFINE( 6) AFFINE( 7) \
    AFFINE( 8) AFFINE( 9) AFFINE(10) AFFINE(11) AFFINE(12) AFFINE(13) AFFINE(14) AFFINE(15) \
    fb += 16; zb += 16; sb += 16; \
    aa = (fixmul(u += uu16, j = fixinv(z + zz16)) - a) >> 4; \
    bb = (fixmul(v += vv16, j                   ) - b) >> 4; \
    cc = ((w += ww16) - c) >> 4; \   
  } for (j = 0; j < xx; j++) {AFFINE(j)} 
                        
        if (tb)
        {                    
          if (surface->flags & SURFACE_OPAQUE)
          {
#undef  AFFINE
#define AFFINE(I) if ((WORD)(z>>8) > zb[I]) {zb[I] = (z>>8); sb[I] = id; fb[I] = (*lb)[f2i(c)][blender[TEXEL66(tb,a,b)][fb[I]]];} z += zz; a += aa; b += bb; c += cc;
//#define AFFINE(I) if ((((int)&fb[I] ^ y1)&1) && (WORD)(z>>8) > zb[I]) {zb[I] = (z>>8); sb[I] = id; fb[I] = (*lb)[f2i(c)][TEXEL66(tb,a,b)];} z += zz; a += aa; b += bb; c += cc;
                          
            INNER_LOOP
          }
          else
          {
#undef  AFFINE
#define AFFINE(I) if ((WORD)(z>>8) > zb[I]) {zb[I] = (z>>8); sb[I] = id; fb[I] = (*lb)[f2i(c)][TEXEL66(tb,a,b)];} z += zz; a += aa; b += bb; c += cc;
                                
            INNER_LOOP;
          }
        }
        else
        {
#undef  AFFINE
#define AFFINE if ((z>>8) > *zb) {*zb = (z>>8); *sb = id; *fb = *sky;} sky++; zb++; fb++; sb++; z += zz;

          int sky_pos = (x + (camera->rot.y >> 1) + (tick >> 1)) & 511;
          int sky_lft = imin(512 - sky_pos, xx);
          int sky_rht = xx - sky_lft;
        
          BYTE* sky = &pic_sky.scanlines.b[y1][sky_pos];

          while (sky_lft--) {AFFINE}

          sky = &pic_sky.scanlines.b[y1][0];
        
          while (sky_rht--) {AFFINE}
        
        }        
      }
      lt_x += lt_xx; lt_z += lt_zz; lt_u += lt_uu; lt_v += lt_vv; lt_w += lt_ww;
      rt_x += rt_xx; rt_z += rt_zz; rt_u += rt_uu; rt_v += rt_vv; rt_w += rt_ww; 
    }
  }
}

void gl_polygon_render(POLYGON polygon, int n, SURFACE* surface, int id, MATRIX matrix)
{
  polygon_render(polygon, n, surface, id, pic_bbuffer.width, pic_bbuffer.height);  
}  

/*
** name: wall_render_texture
** desc: render the polygons of a wall.
*/
void wall_render_texture(int wid, MATRIX matrix)
{
  POLYGON polygon;
  
  assert(wid);    
  
  if (walls[wid].visible)
  {
    if (walls[wid].port)
    {      
      polygon[0] = walls[wid].poly[3]; //wall_polys[walls[wid].port][2];
      polygon[1] = walls[wid].poly[2]; //wall_polys[walls[wid].port][3];  
      polygon[2] = walls[wid].poly[2];
      polygon[3] = walls[wid].poly[3];

      if (walls[walls[wid].port].poly[2].y > polygon[0].y)
      {
        polygon[0]   = walls[walls[wid].port].poly[2];
        polygon[0].u = walls[walls[wid].port].poly[3].u;
      }
      if (walls[walls[wid].port].poly[3].y > polygon[1].y)
      {
        polygon[1]   = walls[walls[wid].port].poly[3];
        polygon[1].u = walls[walls[wid].port].poly[2].u;
      }      
      //poly_normal[0].u = wall_polys[walls[wid].port][3].u;
      //poly_normal[1].u = wall_polys[walls[wid].port][2].u;

      gl_polygon_render(polygon, 4, &walls[wid].surface, 0x0000 | wid, matrix);
   
      polygon[0] = walls[wid].poly[0];
      polygon[1] = walls[wid].poly[1];  
      polygon[2] = walls[walls[wid].port].poly[0];
      polygon[3] = walls[walls[wid].port].poly[1];

      polygon[2].u = walls[walls[wid].port].poly[1].u;
      polygon[3].u = walls[walls[wid].port].poly[0].u;
            
      gl_polygon_render(polygon, 4, &walls[wid].surface, 0x0000 | wid, matrix);
    }
    else
    {
      gl_polygon_render(walls[wid].poly, 4, &walls[wid].surface, 0x0000 | wid, matrix);     
    }
  }
}

/*
** name: sector_render
** desc: render all the components of a sector.
*/
void sector_render(int sid, MATRIX matrix)
{  
  POLYGON polygon;  
  
  int first_wall = FIRST_WALL(sid), wid = first_wall;

  do wall_render_texture(wid, matrix); while ((wid = NEXT_WALL(wid)) != first_wall);

  if (sectors[sid].flags & SECTOR_RENDER_MIDDLE)
  {
    //gl_polygon_render(polygon, sector_extrude_mid(sid, polygon), &sectors[sid].mid, 0x3000 | sid, matrix);
  }
            
  gl_polygon_render(polygon, sector_extrude_floor  (sid, polygon), &sectors[sid].bot, 0x1000 | sid, matrix);  
  gl_polygon_render(polygon, sector_extrude_ceiling(sid, polygon), &sectors[sid].top, 0x2000 | sid, matrix);
}

void sector_render_mid(int sid, MATRIX matrix)
{
  if (sectors[sid].flags & SECTOR_RENDER_MIDDLE)
  {
    POLYGON polygon;  
    gl_polygon_render(polygon, sector_extrude_mid(sid, polygon), &sectors[sid].mid, 0x3000 | sid, matrix);
  }
}

void object_render(int oid, MATRIX matrix)
{
  MATRIX  m;
  POLYGON polygon;

  int w = 500 << 6, h = 500 << 6;  
  
  object_local_space(&objects[oid], m);

  matrix_multiply(m, m, matrix);
  
  polygon[0].x = fixdot3(-w, m[0][0],  h, m[0][1], 0, m[0][2]) + m[0][3]; 
  polygon[0].y = fixdot3(-w, m[1][0],  h, m[1][1], 0, m[1][2]) + m[1][3]; 
  polygon[0].z = fixdot3(-w, m[2][0],  h, m[2][1], 0, m[2][2]) + m[2][3]; 
  polygon[0].u = 0;
  polygon[0].v = 0;

  polygon[1].x = fixdot3( w, m[0][0],  h, m[0][1], 0, m[0][2]) + m[0][3]; 
  polygon[1].y = fixdot3( w, m[1][0],  h, m[1][1], 0, m[1][2]) + m[1][3]; 
  polygon[1].z = fixdot3( w, m[2][0],  h, m[2][1], 0, m[2][2]) + m[2][3]; 
  polygon[1].u = i2f(1024);
  polygon[1].v = 0;

  polygon[2].x = fixdot3( w, m[0][0], -h, m[0][1], 0, m[0][2]) + m[0][3]; 
  polygon[2].y = fixdot3( w, m[1][0], -h, m[1][1], 0, m[1][2]) + m[1][3]; 
  polygon[2].z = fixdot3( w, m[2][0], -h, m[2][1], 0, m[2][2]) + m[2][3]; 
  polygon[2].u = i2f(1024);
  polygon[2].v = i2f(1024);

  polygon[3].x = fixdot3(-w, m[0][0], -h, m[0][1], 0, m[0][2]) + m[0][3]; 
  polygon[3].y = fixdot3(-w, m[1][0], -h, m[1][1], 0, m[1][2]) + m[1][3]; 
  polygon[3].z = fixdot3(-w, m[2][0], -h, m[2][1], 0, m[2][2]) + m[2][3]; 
  polygon[3].u = 0;
  polygon[3].v = i2f(1024);
  
  gl_polygon_render(polygon, 4, &objects[oid].front, 0x8000 | oid, matrix);

  polygon[4] = polygon[3];
  polygon[5] = polygon[2];
  polygon[6] = polygon[1];
  polygon[7] = polygon[0];

  gl_polygon_render(&polygon[4], 4, &objects[oid].back, 0x9000 | oid, matrix);
}

void console_render(void)
{
  int x, y;
  
  for (y = pic_console.height; y--;)
  {
    for (x = pic_console.width; x--;)
    {
      int xx = x << 3, yy = y << 3;
      
      int c = pic_console.scanlines.b[y][x] << 3;

      picture_blit8(&pic_bbuffer, xx, yy, xx + 8, yy + 8, &pic_font, c, 0, c + 8, 8, PICTURE_MODE_COLORKEY);
    }
  }
}

void console_outtext(int x, int y, STRING text)
{
  BYTE* p;
  for (p = &pic_console.scanlines.b[y][x]; (*p++ = *text++) != 0;);
}

void render_view(OBJECT* camera)
{
  int x, y;

  // Clear the zbuffer.
  picture_clear(&pic_zbuffer);
      
  if (camera->sid)
  {
    MATRIX matrix;
    int i;
      
    sector_calc_vis(camera);            

    object_matrix(camera, matrix, pic_bbuffer.width, pic_bbuffer.height);    
         
    for (i = 0; i < sector_list_count; i++) sector_transform (sector_list[i], matrix);
    for (i = 0; i < sector_list_count; i++) sector_render    (sector_list[i], matrix);
    for (i = 0; i < sector_list_count; i++) sector_render_mid(sector_list[i], matrix);

    object_render(2, matrix);
  }
  console_render();

  mouse_read(&x, &y);

  picture_draw8(&pic_bbuffer, &pic_arrow, x, y, PICTURE_MODE_COLORKEY);

  // Copy view buffer to video memory.
  picture_copy(&pic_fbuffer, &pic_bbuffer);  
}

void show_logo(void)
{
  PALLETE palette;
  PICTURE pic_logo;

  picture_load_from_file(&pic_logo, "images/logo.pcx", palette);

  vga_install(0x13);
  palette_install(palette);

  picture_draw8(&pic_fbuffer, &pic_logo, 0, 0, PICTURE_MODE_COPY); 
  
  while (!kbhit());
  getch();
  
  picture_destroy(&pic_logo);
}

void engine_execute(void)
{ 
  int i, x, y;
  int floor_z, ceil_z, under;      
  int frame_count = 0;

  //show_logo();
  
  mouse_show(0);

  vga_install(0x13);  

  picture_create(&pic_fbuffer, 320, 200, 8, 0, (void*) 0xA0000);

  picture_create(&pic_bbuffer, 320, 200, 8, 0, 0);


  picture_create(&pic_zbuffer, pic_fbuffer.width, pic_fbuffer.height, 16, 0, 0);  
  picture_create(&pic_stencil, pic_fbuffer.width, pic_fbuffer.height, 16, 0, 0);

  picture_create(&pic_console, 40, 10, 8, 0, 0);
  
      
  for(i = 0; i < 256; i++)
  {
    x = RGB_BRIGHTNESS(palette[i][0],palette[i][1],palette[i][2]);

    view.palette[i][0] = imin(pow(palette[i][0] / 255.0, 64.0 / 64) * 255.0, 255);
    view.palette[i][1] = imin(pow(palette[i][1] / 255.0, 64.0 / 64) * 255.0, 255);
    view.palette[i][2] = imin(pow(palette[i][2] / 255.0, 64.0 / 64) * 255.0, 255);
        
    view.palette[i][0] = imin(imuldiv(palette[i][0], 128, 255), 255);
    view.palette[i][1] = imin(imuldiv(palette[i][1], 128, 255), 255);
    view.palette[i][2] = imin(imuldiv(palette[i][2], 255, 255), 255);
  }
  
  palette_install(view.palette);
  
  keyboard_install();  
  timer_install();  
  
  thing->sid = 0;
  thing->x   = -900;
  thing->y   = 2000;
  thing->z   = 200;
  thing->front.texture = 124;
  //thing->front.flags |= SURFACE_OPAQUE;
  thing->back.texture = 10;
  //thing->back.flags |= SURFACE_OPAQUE;
    
  camera->sid = 0;
  camera->z   = WORLD_UNIT * 4;
  
  for (tick = timer;;)
  {
    picture_luquid_effect8(&textures[41], &textures[42], tick);
    picture_luquid_effect8(&textures[43], &textures[44], tick);

    picture_blend8(&textures[113], &textures[41], &textures[15], blender);
    picture_blend8(&textures[114], &textures[41], &textures[64], blender);

    render_view(camera);           
    mouse_read(&x, &y);
                        
    sprintf(sbuf, "Key:%3d X:%4d Y:%4d Z:%4d SID:%d",
      last_key,
      camera->x,
      camera->y,
      camera->z,
      camera->sid);
    
    console_outtext(0, 0, sbuf);
    
    sector_z(camera->sid, camera->x, camera->y, &floor_z, &ceil_z, 0);

    sprintf(sbuf, "%d %d FPS: %d", floor_z, ceil_z, imuldiv(frame_count++, 120, timer + 1));

    console_outtext(0, 1, sbuf);    
            
    for (; tick < timer; tick++)
    {
      camera->zz -= fl2f(0.4); // Gravity
      
      object_update(thing);
      thing->rot.y += 2;
      
      object_update(camera);      
      object_collision(camera);
      
      sector_z(camera->sid, camera->x, camera->y, &floor_z, &ceil_z, 0);
    
      under = ((camera->z - 300) << 6) - floor_z;
    
      if (under < 0)
      {
        camera->xx -= camera->xx / 18; 
        camera->yy -= camera->yy / 18; 
        camera->zz -= camera->zz / 18;
                
        camera->zz -= under * 8;

        camera->zz &= 0xFFFFF000;
        
        if (KEY_DOWN(KB_UPARROW))
        {
          camera->xx += imuldiv(fixsin(camera->rot.y), 6, 8);
          camera->yy += imuldiv(fixcos(camera->rot.y), 6, 8);
        }
        if (KEY_DOWN(KB_DNARROW)) 
        {
          camera->xx -= imuldiv(fixsin(camera->rot.y), 6, 8);
          camera->yy -= imuldiv(fixcos(camera->rot.y), 6, 8);
        }
        if (KEY_DOWN(51))
        {
          camera->xx -= imuldiv(fixcos(camera->rot.y), 6, 8);
          camera->yy += imuldiv(fixsin(camera->rot.y), 6, 8);
        }
        if (KEY_DOWN(52))
        {
          camera->xx += imuldiv(fixcos(camera->rot.y), 6, 8);
          camera->yy -= imuldiv(fixsin(camera->rot.y), 6, 8);
        }           
        if (KEY_PRESSED(57)) // Jump
        {
          camera->zz += fl2f(15);
        }
        if (KEY_DOWN(46)) // Crouch
        {
          camera->zz -= fl2f(0.7);
        }                                          
      }                             

      if (KEY_PRESSED(88)) picture_save_to_file(&pic_bbuffer, "screen.pcx", view.palette);
      
   
      if (KEY_DOWN(30) && camera->rot.x <  320) camera->rot.x += 7;
      if (KEY_DOWN(44) && camera->rot.x > -320) camera->rot.x -= 7;      
      if (KEY_DOWN(KB_RTARROW)) camera->rot.y += 7;
      if (KEY_DOWN(KB_LTARROW)) camera->rot.y -= 7;
                          
      if (KEY_DOWN(KB_ESC)) goto stop_game;

#define SURFACE_EDIT(VAR,SUR,A) (clipboard.VAR = (SUR.VAR = KEY_DOWN(KB_CTRL) ? clipboard.VAR : SUR.VAR + (KEY_DOWN(KB_LTSHIFT) ? -(A) : (A))))

      if (KEY_PRESSED(24)) // Opacity
      {
        WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;
        
        if ((code >> 12) == 3) sectors[id].mid  .flags ^= SURFACE_OPAQUE;                
        if ((code >> 12) == 8) objects[id].front.flags ^= SURFACE_OPAQUE;
        if ((code >> 12) == 9) objects[id].back .flags ^= SURFACE_OPAQUE;  
      }
      if (KEY_PRESSED(20)) // Texture
      {
        WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

        if ((code >> 12) == 0) SURFACE_EDIT(texture, walls  [id].surface, 1);
        if ((code >> 12) == 1) SURFACE_EDIT(texture, sectors[id].bot    , 1);
        if ((code >> 12) == 2) SURFACE_EDIT(texture, sectors[id].top    , 1);
        if ((code >> 12) == 3) SURFACE_EDIT(texture, sectors[id].mid    , 1);
                
        if ((code >> 12) == 8) SURFACE_EDIT(texture, objects[id].front  , 1);
        if ((code >> 12) == 9) SURFACE_EDIT(texture, objects[id].back   , 1);
      }
      if (KEY_PRESSED(45)) // X Repeat
      {       
        WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

        if ((code >> 12) == 0) SURFACE_EDIT(repeatx, walls  [id].surface, 1);          
        if ((code >> 12) == 1) SURFACE_EDIT(repeatx, sectors[id].bot    , 1);
        if ((code >> 12) == 2) SURFACE_EDIT(repeatx, sectors[id].top    , 1);        
      }
      if (KEY_PRESSED(21)) // Y Repeat
      {       
        WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

        if ((code >> 12) == 0) SURFACE_EDIT(repeaty, walls  [id].surface, 1);          
        if ((code >> 12) == 1) SURFACE_EDIT(repeaty, sectors[id].bot    , 1);
        if ((code >> 12) == 2) SURFACE_EDIT(repeaty, sectors[id].top    , 1);        
      }
                  
      if (KEY_PRESSED(35)) // X Panning 
      {       
        WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

        if ((code >> 12) == 0) SURFACE_EDIT(panningx, walls  [id].surface, i2f(16));          
        if ((code >> 12) == 1) SURFACE_EDIT(panningx, sectors[id].bot    , i2f(16));
        if ((code >> 12) == 2) SURFACE_EDIT(panningx, sectors[id].top    , i2f(16));        
      }
      if (KEY_PRESSED(47)) // Y Panning 
      {       
        WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

        if ((code >> 12) == 0) SURFACE_EDIT(panningy, walls  [id].surface, i2f(16));          
        if ((code >> 12) == 1) SURFACE_EDIT(panningy, sectors[id].bot    , i2f(16));
        if ((code >> 12) == 2) SURFACE_EDIT(panningy, sectors[id].top    , i2f(16));        
      }           
      if (KEY_PRESSED(38)) // Light (Brightness)
      {       
        WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

        if ((code >> 12) == 0) SURFACE_EDIT(light, walls  [id].surface, 1);          
        if ((code >> 12) == 1) SURFACE_EDIT(light, sectors[id].bot    , 1);
        if ((code >> 12) == 2) SURFACE_EDIT(light, sectors[id].top    , 1);        
      }
            
      if (KEY_PRESSED(KB_INSERT))
      {
        WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;
          
        if ((code >> 12) == 1) sectors[id].bot.slopex += 4;
        if ((code >> 12) == 2) sectors[id].top.slopex += 4;
        if ((code >> 12) == 3) sectors[id].mid.slopex += 4;        
      }
      if (KEY_PRESSED(KB_DELETE))
      {
        WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;
          
        if ((code >> 12) == 1) sectors[id].bot.slopex -= 4;
        if ((code >> 12) == 2) sectors[id].top.slopex -= 4;
        if ((code >> 12) == 3) sectors[id].mid.slopex -= 4;    
      }
      if (KEY_PRESSED(KB_HOME))
      {
        WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;
          
        if ((code >> 12) == 1) sectors[id].bot.slopey += 4;
        if ((code >> 12) == 2) sectors[id].top.slopey += 4;
        if ((code >> 12) == 3) sectors[id].mid.slopey += 4;
      }
      if (KEY_PRESSED(KB_END))
      {
        WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;
          
        if ((code >> 12) == 1) sectors[id].bot.slopey -= 4;
        if ((code >> 12) == 2) sectors[id].top.slopey -= 4;
        if ((code >> 12) == 3) sectors[id].mid.slopey -= 4;      
      }              
      if (KEY_PRESSED(KB_PAGEUP))
      {
        WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;
          
        if ((code >> 12) == 1) SURFACE_EDIT(slopez, sectors[id].bot, -WORLD_UNIT);   
        if ((code >> 12) == 2) SURFACE_EDIT(slopez, sectors[id].top, -WORLD_UNIT);
        if ((code >> 12) == 3) SURFACE_EDIT(slopez, sectors[id].mid, -WORLD_UNIT);         
      }
      if (KEY_PRESSED(KB_PAGEDN))
      {       
        WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;
          
        if ((code >> 12) == 1) SURFACE_EDIT(slopez, sectors[id].bot,  WORLD_UNIT);   
        if ((code >> 12) == 2) SURFACE_EDIT(slopez, sectors[id].top,  WORLD_UNIT);
        if ((code >> 12) == 3) SURFACE_EDIT(slopez, sectors[id].mid,  WORLD_UNIT);    
      }
      if (KEY_PRESSED(50))
      {
        sectors[camera->sid].flags ^= SECTOR_RENDER_MIDDLE;
      }
      memcpy(keyprev, keydown, sizeof(keyprev));
    }
  }
stop_game:
  timer_remove();
  keyboard_remove();

  picture_destroy(&pic_fbuffer);  
  picture_destroy(&pic_bbuffer);
  picture_destroy(&pic_zbuffer);
  picture_destroy(&pic_stencil);
  picture_destroy(&pic_console);
}


void engine_create(void)
{
  int i;  
           
  say("AlienKinetics 3D Engine [DOS] - Copyright (C) 2001-2002 AlienKinetics");
  say("---------------------------------------------------------------------");
  say("");
  
  say("Installing mouse driver.");
  mouse_install();  

  say("Allocating offscreen buffers.");

  picture_load_from_file(&pic_font , "images/font8x8.pcx", palette);    
  picture_load_from_file(&pic_sky  , "images/sky.pcx"    , palette);
  picture_load_from_file(&pic_arrow, "images/arrow.pcx"  , palette);  
  picture_load_from_file(&pic_lens , "images/lens.pcx"   , palette);
    
  say("Loading textures.");

  for (i = 0; i < MAX_TEXTURE; i++)
  {
    sprintf(sbuf, "textures/%d.pcx", i);
    picture_load_from_file(&textures[i], sbuf, palette);
    if (i == 0) tables_create();  
  }  
}

void main(void)
{  
  freopen("stderr.txt", "w", stderr);  
  freopen("stdout.txt", "w", stdout);

  view.width    = 640;
  view.height   = 480;
  view.zoom     = 1;
  view.zoom_div = WORLD_UNIT;
  view.grid     = WORLD_UNIT * 8;
      
  engine_create();
  
  mouse_show(1);

  for (;;)
  {
    editor_execute();
    
    if (view.key == 'q') break;
    
    if (view.key == 13)
    {
      div0_init(DM_SATURATE);
      engine_execute();
      div0_close();
      if (view.key == 'q') break;
    }
  }
  mouse_show(0);
  
  _setvideomode(_DEFAULTMODE);
}



