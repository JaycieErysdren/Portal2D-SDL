
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

#ifndef __SURFACE_H__
#define __SURFACE_H__

#include "defs.h"

#define SURFACE_OPAQUE   1
#define SURFACE_COLORKEY 2
#define SURFACE_LIGHTING 4

typedef struct 
{
  BYTE texture;
  BYTE overlay;               
  WORD flags;
  BYTE light, red, green, blue; // RGB is _not_ currently used.
  LONG slopex;
  LONG slopey;
  LONG slopez;
  BYTE repeatx, _pad4, _pad5, _pad6;
  BYTE repeaty, _pad7, _pad8, _pad9;
  LONG panningx;                           // fixed point texture u offset
  LONG panningy;                           // fixed point texture v offset
  LONG reserved;  
} SURFACE;

#endif

