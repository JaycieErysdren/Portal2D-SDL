// ========================================================
//
// FILE:			/source/dev_io.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Device IO abstractions.
//
// LAST EDITED:		October 4th, 2022
//
// ========================================================

#ifndef __DEV_IO_H__
#define __DEV_IO_H__

// Global includes
#include "defs.h"
#include "math.h"

// SDL target
#ifdef REX_SDL

	#include <SDL2/SDL.h>

	#define KB_ESC			SDL_SCANCODE_ESCAPE
	#define KB_DNARROW		SDL_SCANCODE_DOWN
	#define KB_LTARROW		SDL_SCANCODE_LEFT
	#define KB_RTARROW		SDL_SCANCODE_RIGHT
	#define KB_UPARROW		SDL_SCANCODE_UP

	#define KB_ENTER		SDL_SCANCODE_RETURN
	#define KB_CTRL			SDL_SCANCODE_LCTRL
	#define KB_LTSHIFT		SDL_SCANCODE_LSHIFT
	#define KB_RTSHIFT		SDL_SCANCODE_RSHIFT

	#define KB_HOME			SDL_SCANCODE_HOME
	#define KB_PAGEUP		SDL_SCANCODE_PAGEUP
	#define KB_END			SDL_SCANCODE_END
	#define KB_PAGEDN		SDL_SCANCODE_PAGEDOWN
	#define KB_INSERT		SDL_SCANCODE_INSERT
	#define KB_DELETE		SDL_SCANCODE_DELETE

	#define KB_W			SDL_SCANCODE_W
	#define KB_A			SDL_SCANCODE_A
	#define KB_S			SDL_SCANCODE_S
	#define KB_D			SDL_SCANCODE_D
	#define KB_SPACE		SDL_SCANCODE_SPACE
	#define KB_C			SDL_SCANCODE_C

	#define GFX_DEFAULT		(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)

	#define KEY_PRESSED(A) 0
	#define KEY_DOWN(A) 0

#endif

// DOS Target
#ifdef REX_DOS

	#include <memory.h>
	#include <io.h>
	#include <i86.h>
	#include <dos.h>
	#include <graph.h>
	#include <fcntl.h>
	#include <conio.h>

	#define KB_ESC			0x01
	#define KB_DNARROW		0x50
	#define KB_LTARROW		0x4B
	#define KB_RTARROW		0x4D
	#define KB_UPARROW		0x48

	#define KB_ENTER		28
	#define KB_CTRL			29
	#define KB_LTSHIFT		42
	#define KB_RTSHIFT		54

	#define KB_HOME			71
	#define KB_PAGEUP		73
	#define KB_END			79
	#define KB_PAGEDN		81
	#define KB_INSERT		82
	#define KB_DELETE		83

	#define KB_W			17
	#define KB_A			30
	#define KB_S			31
	#define KB_D			32
	#define KB_SPACE		57
	#define KB_C			46

	// VGA mode 13 - 320x200, 256 colors
	#define GFX_DEFAULT		0x13
		
	extern char keydown[128];
	extern char keyprev[128];
	extern char last_key;

	#define KEY_PRESSED(A) (keydown[A] != keyprev[A] && keydown[A])
	#define KEY_DOWN(A) keydown[A]

	extern unsigned timer;

	void mouse_install_dos(void);
	void mouse_show_dos(int);
	int mouse_read_dos(int* x, int* y);
	void mouse_set_dos(int* x, int* y);
	int mouse_button_press_dos(int* x, int* y);

	void keyboard_install_dos(void);
	void keyboard_remove_dos(void);
	void timer_install_dos(void);
	void timer_remove_dos(void);

	void graphics_install_dos(int mode);
	void graphics_remove_dos(void);
	void palette_install_dos(PALLETE palette);

	// Abstractions
	#define mouse_install mouse_install_dos
	#define mouse_show mouse_show_dos
	#define mouse_read mouse_read_dos
	#define mouse_set mouse_set_dos
	#define mouse_button_press mouse_button_press_dos

	#define keyboard_install keyboard_install_dos
	#define keyboard_remove keyboard_remove_dos
	#define timer_install timer_install_dos
	#define timer_remove timer_remove_dos

	#define graphics_install graphics_install_dos
	#define graphics_remove graphics_remove_dos
	#define palette_install palette_install_dos

#endif


#endif
