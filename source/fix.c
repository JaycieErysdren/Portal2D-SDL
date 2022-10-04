
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

#include "fix.h"

int fixinv(int a)
{
  return fixdiv(i2f(1), a); 
}

int fixmul(int a, int b)
{
  return ((__int64)a * b) >> 16;
}

int fixdiv(int a, int b)
{
  return ((__int64)a << 16) / b;
}

int fixdot3(int a1, int b1, int a2, int b2, int a3, int b3)
{
  return ((__int64)a1 * b1 + (__int64)a2 * b2 + (__int64)a3 * b3) >> 16;
}

