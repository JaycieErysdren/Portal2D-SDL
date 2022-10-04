
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

#include "doskb.h"

#include <stdlib.h>
#include <memory.h>
#include <dos.h>
#include <conio.h>

char keyMap[128];
char last_key;

void (__interrupt __far* __keyboard_interrupt)();

void __interrupt __far keyboard_interrupt(void)
{
  char c = inp(0x60);
  keyMap[last_key = c & 0x7F] = !(c & 0x80);
  outp(0x20, 0x20);
}

void keyboard_remove(void)
{
  _disable();
  if (__keyboard_interrupt)
  {
    _dos_setvect(9, __keyboard_interrupt);
    __keyboard_interrupt = 0;
  }
  _enable();
}


void keyboard_install(void)
{
  _disable();
  if (!__keyboard_interrupt)
  {
    memset(keyMap, 0, sizeof(keyMap));
    __keyboard_interrupt = _dos_getvect(9);
    _dos_setvect(9, keyboard_interrupt);
  }
  _enable();
}


