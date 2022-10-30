// ========================================================
//
// FILE:			/source/inc/dev_io.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Generic Device I/O layer.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

extern int input_mouse_x, input_mouse_y;

extern char input_keys[512];
extern char input_keys_prev[512];
extern char input_key_last;

#define KEY_PRESSED(A) (input_keys[A] != input_keys_prev[A] && input_keys[A])
#define KEY_DOWN(A) input_keys[A]

// Input routines
void DevicesRead(void);

// DOS clock speed
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

// Keyboard functions
void KeyboardInstall(void);
void KeyboardRemove(void);

// Mouse functions
void MouseInstall(void);
void MouseShow(int);
int MouseRead(int *x, int *y);
void MouseSet(int *x, int *y);
int MousePress(int *x, int *y);

// Timer variables
extern unsigned timer;

// Timer functions
void TimerInstall(int target_speed);
void TimerRemove(void);

// Graphics functions
void DoubleBuffer(void);
void GraphicsInstall(const char *title, int width, int height);
void GraphicsRemove(void);

// Palette functions
void PaletteLoad(char *filename);
void PaletteInstall(PALETTE palette);
