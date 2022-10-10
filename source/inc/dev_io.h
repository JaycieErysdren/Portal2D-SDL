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
// LAST EDITED:		October 9th, 2022
//
// ========================================================

extern int mouse_x, mouse_y;

// Input routines
void RexDevicesRead(void);

// SDL target
#ifdef REX_SDL

	// Keyboard scancodes
	#define KB_ESC				SDL_SCANCODE_ESCAPE
	#define KB_DNARROW			SDL_SCANCODE_DOWN
	#define KB_LTARROW			SDL_SCANCODE_LEFT
	#define KB_RTARROW			SDL_SCANCODE_RIGHT
	#define KB_UPARROW			SDL_SCANCODE_UP
	#define KB_ENTER			SDL_SCANCODE_RETURN
	#define KB_CTRL				SDL_SCANCODE_LCTRL
	#define KB_LTSHIFT			SDL_SCANCODE_LSHIFT
	#define KB_RTSHIFT			SDL_SCANCODE_RSHIFT
	#define KB_HOME				SDL_SCANCODE_HOME
	#define KB_PAGEUP			SDL_SCANCODE_PAGEUP
	#define KB_END				SDL_SCANCODE_END
	#define KB_PAGEDN			SDL_SCANCODE_PAGEDOWN
	#define KB_INSERT			SDL_SCANCODE_INSERT
	#define KB_DELETE			SDL_SCANCODE_DELETE
	#define KB_W				SDL_SCANCODE_W
	#define KB_A				SDL_SCANCODE_A
	#define KB_S				SDL_SCANCODE_S
	#define KB_D				SDL_SCANCODE_D
	#define KB_SPACE			SDL_SCANCODE_SPACE
	#define KB_C				SDL_SCANCODE_C

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

	// Graphics functions
	void RexDoubleBuffer(void);
	void RexGraphicsInstall(const char *title, int width, int height);
	void RexGraphicsRemove(void);

	// Palette functions
	void RexPaletteLoad(char *filename);
	void RexPaletteInstall(PALETTE palette);

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

	// Graphics functions
	void RexDoubleBuffer(void);
	void RexGraphicsInstall(const char *title, int width, int height);
	void RexGraphicsRemove(void);

	// Palette functions
	void RexPaletteLoad(char *filename);
	void RexPaletteInstall(PALETTE palette);

#endif
