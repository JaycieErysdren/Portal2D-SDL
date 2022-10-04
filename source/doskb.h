
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

#ifndef __DOSKB_H
#define __DOSKB_H

#define  KB_ESC      0x01
#define  KB_DNARROW  0x50
#define  KB_LTARROW  0x4B
#define  KB_RTARROW  0x4D
#define  KB_UPARROW  0x48

extern char keyMap[128];

void keyboard_install(void);
void keyboard_remove(void);

#endif

