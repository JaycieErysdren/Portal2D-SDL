
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

#include "math.h"

/*
** Name: isqrt
** Desc: Integer square root. Take the square root of an integer.
*/
int isqrt(int value)
{
  int root = 0;

#define STEP(shift) \
    if((0x40000000 >> shift) + root <= value)          \
    {                                                   \
        value -= (0x40000000 >> shift) + root;          \
        root = (root >> 1) | (0x40000000 >> shift);     \
    }                                                   \
    else                                                \
    {                                                   \
        root >>= 1;                                     \
    }
    
  STEP( 0); STEP( 2); STEP( 4); STEP( 6);
  STEP( 8); STEP(10); STEP(12); STEP(14);
  STEP(16); STEP(18); STEP(20); STEP(22);
  STEP(24); STEP(26); STEP(28); STEP(30);

  // round to the nearest integer, cuts max error in half

  if (root < value) root++; 

  return root;
}

