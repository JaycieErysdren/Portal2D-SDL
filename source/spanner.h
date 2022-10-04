
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

#ifndef __SPANNER_H__
#define __SPANNER_H__

#include "defs.h"

typedef struct
{
  int x1, x2;
} span_t;

typedef struct
{
  int nspans;
  span_t spans[100];
} span_array_t;

typedef struct
{
  int width;
  int height;
  int ngaps;
  span_array_t* lines;
} spanner_t;

#endif

