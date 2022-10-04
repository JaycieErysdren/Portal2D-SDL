
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

#include "defs.h"
#include "polygon.h"
#include "pragmas.h"
#include "math.h"
#include "fix.h"

/*
** Name: polygon_clip 
** Desc: clip a polygon to a pre-calculated distance table.
*/
int polygon_clip(POLYGON dst, POLYGON src, int n, int dist[])
{
  int r = 0, a, b, t;

  if (n >= 3)
  {
    for (a = n - 1, b = 0; b < n; a = b++)
    {
      if (dist[a] >= 0) dst[r++] = src[a];

      if ((dist[a] ^ dist[b]) < 0)
      {
        VERTEX v1 = src[a], v2 = src[b];               
                   
        dst[r].x = v1.x + fixmul(v2.x - v1.x, t = fixdiv(dist[a], dist[a] - dist[b]));  
        dst[r].y = v1.y + fixmul(v2.y - v1.y, t);  
        dst[r].z = v1.z + fixmul(v2.z - v1.z, t);  
        dst[r].u = v1.u + fixmul(v2.u - v1.u, t);  
        dst[r].v = v1.v + fixmul(v2.v - v1.v, t);
        dst[r].w = v1.w + fixmul(v2.w - v1.w, t);  
                                 
        r++;
      }
    }
  }
  return r;
}

/*
** Name: polygon_clip_to_frustum
** Desc: clip a polygon to the frustrum.
*/
int polygon_clip_to_frustum(POLYGON dst, POLYGON src, int n)
{
  int i;
  int dist[MAX_POLYGON];
  POLYGON tmp;  
  
  //for (i = n; i--; dist[i] = src[i].z - 100   ); n = polygon_clip(dst, src, n, dist); // Front
  //for (i = n; i--; dist[i] = 0x8000000 - tmp[i].z); n = polygon_clip(dst, tmp, n, dist); // Back
  
  for (i = n; i--; dist[i] = src[i].x  + src[i].z); n = polygon_clip(tmp, src, n, dist); // Left
  for (i = n; i--; dist[i] = tmp[i].z  - tmp[i].x); n = polygon_clip(dst, tmp, n, dist); // Right
  for (i = n; i--; dist[i] = dst[i].y  + dst[i].z); n = polygon_clip(tmp, dst, n, dist); // Top
  for (i = n; i--; dist[i] = tmp[i].z  - tmp[i].y); n = polygon_clip(dst, tmp, n, dist); // Bottom
  
  return n;
}

/*
** Name: polygon_project
** Desc: project a polygon into screen space. prepare z, u & v for linear interpolation.
*/
void polygon_project(POLYGON dst, POLYGON src, int n, int width, int height, SURFACE* surface)
{
  int cx = i2f(width ) >> 1;
  int cy = i2f(height) >> 1;
  
  while (n--)
  {
    VERTEX p = src[n];   

    p.z += 6; // 6 == 1 in view space.
            
    dst[n].x =   imuldiv(p.x, cx - i2f(0), p.z) + cx - (FRACCEIL >> 1);
    dst[n].y = - imuldiv(p.y, cy - i2f(0), p.z) + cy - (FRACCEIL >> 1);
    dst[n].z =   fixinv(p.z); 
    dst[n].u =   mulscale(p.u + surface->panningx, dst[n].z, surface->repeatx + 18); 
    dst[n].v =   mulscale(p.v + surface->panningy, dst[n].z, surface->repeaty + 18);
    dst[n].w =   i2f(31) - imin((abs(p.x) << 1) + (abs(p.y) << 1) + (p.z << 2), i2f(30));
  }
}

