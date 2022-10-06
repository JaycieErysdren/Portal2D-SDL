// ========================================================
//
// FILE:			/source/inc/dev_io.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Generic Device I/O layer.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

// SDL target
#ifdef REX_SDL



#endif

// DOS target
#ifdef REX_DOS

	#define DOS_CLOCK_SPEED		1193181

	// Keyboard scancodes
	#define KB_ESC				0x01
	#define KB_DNARROW			0x50
	#define KB_LTARROW			0x4B
	#define KB_RTARROW			0x4D
	#define KB_UPARROW			0x48
	#define KB_ENTER			28
	#define KB_CTRL				29
	#define KB_LTSHIFT			42
	#define KB_RTSHIFT			54
	#define KB_HOME				71
	#define KB_PAGEUP			73
	#define KB_END				79
	#define KB_PAGEDN			81
	#define KB_INSERT			82
	#define KB_DELETE			83
	#define KB_W				17
	#define KB_A				30
	#define KB_S				31
	#define KB_D				32
	#define KB_SPACE			57
	#define KB_C				46

	// Keyboard variables
	extern char keydown[128];
	extern char keyprev[128];
	extern char last_key;

	// Keyboard defines
	#define KEY_PRESSED(A) (keydown[A] != keyprev[A] && keydown[A])
	#define KEY_DOWN(A) keydown[A]

	// Keyboard functions
	void RexKeyboardInstall(void);
	void RexKeyboardRemove(void);

	// Mouse functions
	void RexMouseInstall(void);
	void RexMouseShow(int);
	int RexMouseRead(int* x, int* y);
	void RexMouseSet(int* x, int* y);
	int RexMousePress(int* x, int* y);

	// Timer variables
	extern unsigned timer;

	// Timer functions
	void RexTimerInstall(int target_speed);
	void RexTimerRemove(void);

	// Graphics definitions
	#define GFX_DEFAULT			0x13

	// Graphics functions
	void RexGraphicsInstall(int mode);
	void RexGraphicsRemove(void);

	// Palette functions
	void RexPaletteInstall(PALETTE palette);

#endif
