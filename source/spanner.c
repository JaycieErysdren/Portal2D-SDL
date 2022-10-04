
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

#include "spanner.h"

/*
** 2D Span Clipping Routines
*/
void span_array_invalidate(span_array_t* spanline, int x1, int x2)
{
   spanline->nspans = 1;
   spanline->spans[0].x1 = x1;
   spanline->spans[0].x2 = x2;
}

void span_array_validate(span_array_t* array, int x1, int x2)
{
  int n;
  span_t* span    = array->spans;
  span_t* newspan = &array->spans[array->nspans];

  for (n = array->nspans; n--; span++)
  {
    if (x1 < span->x2 && x2 > span->x1)
    {
      if (x1 > span->x1)
      {
        if (x2 < span->x2)
        {
          newspan->x1 = x2;
          newspan->x2 = span->x2;
          newspan++;
          array->nspans++;
        }
        span->x2 = x1;
      }
      else
      {
        if (x2 < span->x2)
        {
          span->x1 = x2;
        }
        else
        {
          span_t* s;
          for (s = span; s != newspan; s[0] = s[1], s++);

          newspan--;
          span--;
          array->nspans--;            
        }
      }
    }
  }
  assert(array->nspans >= 0 && array->nspans < 60);
}

void spanner_invalidate(spanner_t* spanner)
{
  int i;
  
  spanner->ngaps = spanner->height;
   
  for (i = 0; i < spanner->height; i++)
  {
    span_array_invalidate(&spanner->lines[i], 0, spanner->width);
  }
}

void spanner_validate(spanner_t* spanner, int x1, int y1, int x2, int y2)
{
  int i;
  
  for (i = y1; i < y2; i++)
  {
    span_array_validate(&spanner->lines[i], x1, x2);

    if (spanner->lines[i].nspans == 0) spanner->ngaps--;
  }
}

void spanner_create(spanner_t* spanner, int width, int height)
{
  memset(spanner, 0, sizeof(spanner_t));

  spanner->width    = width;
  spanner->height   = height;   
  spanner->lines    = (span_array_t*) calloc(height, sizeof(span_array_t));
}

void spanner_delete(spanner_t* spanner)
{
  free(spanner->lines);
}

