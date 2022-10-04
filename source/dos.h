
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

#ifndef __DOS_H__
#define __DOS_H__

#include "defs.h"

#include <graph.h>
#include <conio.h>

#define  KB_ESC       0x01
#define  KB_DNARROW   0x50
#define  KB_LTARROW   0x4B
#define  KB_RTARROW   0x4D
#define  KB_UPARROW   0x48

#define KB_ENTER      28
#define KB_CTRL       29
#define KB_LTSHIFT    42
#define KB_RTSHIFT    54

#define KB_HOME       71
#define KB_PAGEUP     73
#define KB_END        79
#define KB_PAGEDN     81
#define KB_INSERT     82
#define KB_DELETE     83
      
extern char keydown[128];
extern char keyprev[128];
extern char last_key;

#define KEY_PRESSED(A) (keydown[A] != keyprev[A] && keydown[A])
#define KEY_DOWN(A) keydown[A]

extern unsigned timer;

void mouse_install(void);
void mouse_show(int);
int mouse_read(int* x, int* y);
void mouse_set(int* x, int* y);
int mouse_button_press(int* x, int* y);

void keyboard_install(void);
void keyboard_remove(void);
void timer_install(void);
void timer_remove(void);

void vga_install(int mode);
void palette_install(PALLETE palette);

#endif

