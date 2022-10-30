// ========================================================
//
// FILE:			/source/dev_io.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Device I/O layer.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

// Include global header
#include "portal2d.h"

int input_mouse_x, input_mouse_y;
	
char input_keys[512];
char input_keys_prev[512];
char input_key_last;

//
// Global registers
//

static union REGS regs;

//
// Input routines
//

void DevicesRead(void)
{
	MouseRead(&input_mouse_x, &input_mouse_y);
}

//
// Mouse routines
//

void MouseInstall(void)
{
	regs.x.eax = 0;
	int386(0x33, &regs, &regs);
}

void MouseShow(int on)
{
	regs.x.eax = on ? 1 : 2;
	int386(0x33, &regs, &regs);
}

int MouseRead(int *x, int *y)
{
	regs.x.eax = 3;
	int386(0x33, &regs, &regs);

	if (x) *x = regs.x.ecx;
	if (y) *y = regs.x.edx;

	return regs.x.ebx;
}

void MouseSet(int* x, int* y)
{
	regs.x.eax = 4;
	regs.x.ecx = *x;
	regs.x.edx = *y;
	int386(0x33, &regs, &regs);
}

int MouseButtonPress(int* x, int* y)
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

void TimerInstall(int target_speed)
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

void TimerRemove(void)
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

static void (__interrupt __far* __keyboard_interrupt)();

void __interrupt __far keyboard_interrupt(void)
{
	int c = inp(0x60);
	input_keys[input_key_last = c & 0x7F] = !(c & 0x80);

	outp(0x20, 0x20);
}

void KeyboardRemove(void)
{
	_disable();
	if (__keyboard_interrupt)
	{
		_dos_setvect(9, __keyboard_interrupt);
		__keyboard_interrupt = 0;
	}
	_enable();
}

void KeyboardInstall(void)
{
	_disable();
	if (!__keyboard_interrupt)
	{
		memset(input_keys, 0, sizeof(input_keys));
		memset(input_keys_prev, 0, sizeof(input_keys_prev));
		__keyboard_interrupt = _dos_getvect(9);
		_dos_setvect(9, keyboard_interrupt);
	}
	_enable();
}

//
// Graphics routines
//

void DoubleBuffer(void)
{
	PictureCopy(&pic_fbuffer, &pic_bbuffer);
}
	
void GraphicsInstall(const char *title, int width, int height)
{
	_setvideomode(_MRES256COLOR);
	//regs.x.eax = mode;
	//int386(0x10, &regs, &regs);
}

void GraphicsRemove(void)
{
	_setvideomode(_DEFAULTMODE);
}

void PaletteLoad(char *filename)
{
	FILE *palette_file;
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

void PaletteInstall(PALETTE palette)
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
