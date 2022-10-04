
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

#ifndef __MATH_H__
#define __MATH_H__

#include <math.h>

int isqrt(int value);

int imin(int, int);
#pragma aux imin =\
  "cmp eax, ebx",\
  "jl skipit",\
  "mov eax, ebx",\
  "skipit:",\
  parm nomemory [eax][ebx]\
  modify exact [eax];

int imax(int, int);
#pragma aux imax =\
  "cmp eax, ebx",\
  "jg skipit",\
  "mov eax, ebx",\
  "skipit:",\
  parm nomemory [eax][ebx]\
  modify exact [eax];

int isqr(int);
#pragma aux isqr =\
  "imul eax, eax",\
  parm nomemory [eax]\
  modify exact [eax]\
  value [eax];
  
int imuldiv(int, int, int);
#pragma aux imuldiv parm [eax] [edx] [ebx] modify exact [eax edx] = \
  "imul edx"  \
  "idiv ebx";

void memcpy32(long *Source, long *Destination, int Length);
#pragma aux memcpy32 = " rep movsd " parm [EDI] [ESI] [ECX] modify [EDI ESI ECX];

      
#endif



