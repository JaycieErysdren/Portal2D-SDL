// ========================================================
//
// FILE:			/source/dev_io.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Device IO abstractions.
//
// LAST EDITED:		October 8th, 2022
//
// ========================================================

#include "rex.h"

#ifdef REX_SDL

	void RexMouseInstall(void)
	{
		return;
	}

	void RexMouseShow(int on)
	{
		return;
	}

	int RexMouseRead(int* x, int* y)
	{
		return 0;
	}

	void RexMouseSet(int* x, int* y)
	{
		return;
	}

	int RexMouseButtonPress(int* x, int* y)
	{
		return 0;
	}

	//
	// Timer routines
	//

	unsigned timer;

	void RexTimerInstall(int target_speed)
	{
		return;
	}

	void RexTimerRemove(void)
	{
		return;
	}

	//
	// Keyboard routines
	//

	char keydown[128];
	char keyprev[128];
	char last_key;

	void RexKeyboardRemove(void)
	{
		return;
	}

	void RexKeyboardInstall(void)
	{
		return;
	}

	//
	// Graphics routines
	//

	void RexGraphicsInstall(char *title, int width, int height)
	{
		return;
	}

	void RexGraphicsRemove(void)
	{
		return;
	}

	void RexPaletteLoad(char *filename)
	{
		return;
	}

	void RexPaletteInstall(PALETTE palette)
	{
		return;
	}

#endif

#ifdef REX_DOS

	//
	// Global registers
	//

	static union REGS regs;

	//
	// Mouse routines
	//

	void RexMouseInstall(void)
	{
		regs.x.eax = 0;
		int386(0x33, &regs, &regs);
	}

	void RexMouseShow(int on)
	{
		regs.x.eax = on ? 1 : 2;
		int386(0x33, &regs, &regs);
	}

	int RexMouseRead(int* x, int* y)
	{
		regs.x.eax = 3;
		int386(0x33, &regs, &regs);

		if (x) *x = regs.x.ecx;
		if (y) *y = regs.x.edx;

		return regs.x.ebx;
	}

	void RexMouseSet(int* x, int* y)
	{
		regs.x.eax = 4;
		regs.x.ecx = *x;
		regs.x.edx = *y;
		int386(0x33, &regs, &regs);
	}

	int RexMouseButtonPress(int* x, int* y)
	{
		regs.x.eax = 5;
		regs.x.ebx = 0;
		int386(0x33, &regs, &regs);

		if (x) *x = regs.x.ecx;
		if (y) *y = regs.x.edx;

		return regs.x.ebx;
	}

	//
	// Timer routines
	//

	unsigned timer;

	static void (__interrupt __far* __timer_interupt)();

	void __interrupt __far timer_interupt(void)
	{
		timer++;

		outp(0x20, 0x20);
	}

	void RexTimerInstall(int target_speed)
	{
		int speed = DOS_CLOCK_SPEED / target_speed;

		_disable();

		if (!__timer_interupt)
		{
			__timer_interupt = _dos_getvect(8);

			_dos_setvect(8, timer_interupt);
			outp(0x43, 0x34);
			outp(0x40, speed);
			outp(0x40, speed >> 8);
		}

		_enable();
	}

	void RexTimerRemove(void)
	{
		_disable();

		if (__timer_interupt)
		{
			_dos_setvect(8, __timer_interupt);
			outp(0x43, 0x34);
			outp(0x40, 0x00);
			outp(0x40, 0x00);
			__timer_interupt = 0;
		}

		_enable();
	}

	//
	// Keyboard routines
	//

	char keydown[128];
	char keyprev[128];
	char last_key;

	static void (__interrupt __far* __keyboard_interrupt)();

	void __interrupt __far keyboard_interrupt(void)
	{
		int c = inp(0x60);
		keydown[last_key = c & 0x7F] = !(c & 0x80);

		outp(0x20, 0x20);
	}

	void RexKeyboardRemove(void)
	{
		_disable();
		if (__keyboard_interrupt)
		{
			_dos_setvect(9, __keyboard_interrupt);
			__keyboard_interrupt = 0;
		}
		_enable();
	}

	void RexKeyboardInstall(void)
	{
		_disable();
		if (!__keyboard_interrupt)
		{
			memset(keydown, 0, sizeof(keydown));
			memset(keyprev, 0, sizeof(keyprev));
			__keyboard_interrupt = _dos_getvect(9);
			_dos_setvect(9, keyboard_interrupt);
		}
		_enable();
	}

	//
	// Graphics routines
	//

	void RexGraphicsInstall(char *title, int width, int height)
	{
		_setvideomode(_MRES256COLOR);
		//regs.x.eax = mode;
		//int386(0x10, &regs, &regs);
	}

	void RexGraphicsRemove(void)
	{
		_setvideomode(_DEFAULTMODE);
	}

	void RexPaletteLoad(char *filename)
	{
		FILE *palette_file;
		long setpal[256];
		int i;

		palette_file = fopen(filename, "rb");

		if (palette_file == NULL)
			fail("Failed to load palette file %s", filename);

		for (i = 0; i < 256; i++)
		{
			unsigned char r = getc(palette_file);
			unsigned char g = getc(palette_file);
			unsigned char b = getc(palette_file);

			palette[i][0] = r;
			palette[i][1] = g;
			palette[i][2] = b;
		}

		fclose(palette_file);
	}

	void RexPaletteInstall(PALETTE palette)
	{
		int i;
		outp(0x3c8,0);
		for(i = 0; i < 256; i++)
		{
			outp(0x3c9, imuldiv(palette[i][0], 63, 255));
			outp(0x3c9, imuldiv(palette[i][1], 63, 255));
			outp(0x3c9, imuldiv(palette[i][2], 63, 255));
		}
	}

#endif
