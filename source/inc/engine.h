// ========================================================
//
// FILE:			/source/inc/engine.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Main engine functions header.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

// Engine limits
#define MAX_POINT				1024
#define MAX_WALL				4192
#define MAX_SECTOR				1024
#define MAX_OBJECT				1024
#define MAX_TEXTURE				256
#define MAX_POLYGON				64

// Rendering datatypes
typedef BYTE CLUT[256][256];
typedef BYTE PALETTE[256][3];
typedef BYTE LIGHTMAP[32][256];
typedef BYTE TEXTURE66[64][64];

// Camera view datatype
typedef struct
{
	int width;
	int height;
	int zoom;
	int zoom_div;
	int grid;
	int key;
	PALETTE palette;
} VIEW;

// Vertex datatype
typedef struct
{
	LONG x;
	LONG y;
	LONG z;
	LONG u;
	LONG v;
	LONG w;
} VERTEX;

// Surface datatype
typedef struct
{
	BYTE texture;
	BYTE overlay;
	WORD flags;
	BYTE light, red, green, blue; // RGB is _not_ currently used.
	LONG slopex;
	LONG slopey;
	LONG slopez;
	BYTE repeatx, _pad4, _pad5, _pad6;
	BYTE repeaty, _pad7, _pad8, _pad9;
	LONG panningx; 				// fixed point texture u offset
	LONG panningy; 				// fixed point texture v offset
	LONG reserved;
} SURFACE;

// Sector datatype
typedef struct
{
	// Persistent Variables. Moving these variables will break the file format!
	WORD lid;
	WORD padding;
	WORD first_wall;
	WORD flags;
	SURFACE top;
	SURFACE bot;
	SURFACE mid;

	int reserved;

	// Calculated variables. These variables are not saved out to a file.
	LONG vis_id;
	RECT bounds;
	POINT center;
	LONG locked;
} SECTOR;

// Wall datatype
typedef struct
{
	// Persistent Variables. Moving these variables will break the file format!
	WORD sid , _sid;
	WORD next, _next;
	WORD port, _port;
	LONG x, y;
	SURFACE surface;

	int reserved;

	// Calculated variables. These variables are not saved out to a file.
	VERTEX top;
	VERTEX bot;
	VERTEX mid;
	VERTEX poly[4];
	LONG dx, dy;
	LONG visible;
	LONG frame;
	LONG length;
	LONG distance;
} WALL;

// Object datatype
typedef struct
{
	LONG x, xx, xxx;
	LONG y, yy, yyy;
	LONG z, zz, zzz;
	VECTOR rot;
	LONG sid;
	LONG flags;
	SURFACE front;
	SURFACE back;
} OBJECT;

// Scanlines union
typedef union
{
	BYTE** b;
	WORD** w;
	DWORD** l;
} SCANLINES;

// Picture datatype
typedef struct
{
	LONG width;					// width of picture in pixels
	LONG height;				// height of picture in pixels
	LONG bpp;					// bits per pixel
	LONG bytes_per_row;			// bytes per row. rows are 4 byte aligned
	LONG shared;				// false if buffer is owned. TODO: reference counting
	VOID* buffer;				// allocated pixel buffer
	SCANLINES scanlines;		// pre-calculated scanline pointers
} PICTURE;

// Polygon datatype
typedef VERTEX POLYGON[MAX_POLYGON];

// Span datatype
typedef struct
{
	int x1, x2;
} SPAN;

// Span array datatype
typedef struct
{
	int nspans;
	SPAN spans[100];
} SPAN_ARRAY;

// Spanner datatype
typedef struct
{
	int width;
	int height;
	int ngaps;
	SPAN_ARRAY* lines;
} SPANNER;

// Miscellaneous
#define TEXEL66(A,X,Y) *((BYTE*)A+((Y>>10)&(63<<6))+((X>>16)&63))

// Global variables
extern WALL						walls[MAX_WALL];
extern SECTOR					sectors[MAX_SECTOR];
extern OBJECT					objects[MAX_OBJECT];
extern PICTURE					textures[MAX_TEXTURE];
extern WORD						sector_list[MAX_SECTOR];

extern int						sector_list_count;
extern int						tick;

extern CLUT						blender;
extern LIGHTMAP					lightmaps[32];

extern VIEW						view;

extern OBJECT					*camera;
extern OBJECT					*thing;

extern PICTURE					pic_stencil;
extern PICTURE					pic_fbuffer;
extern PICTURE					pic_bbuffer;
extern PICTURE					pic_zbuffer;

extern PICTURE					pic_sky;
extern PICTURE					pic_console;
extern PICTURE					pic_font;

extern PALETTE					palette;

// Definitions
#define RENDER_RECIPROCAL

#define SURFACE_OPAQUE			1
#define SURFACE_COLORKEY		2
#define SURFACE_LIGHTING		4

#define PICTURE_MODE_COPY		0
#define PICTURE_MODE_COLORKEY	1

#define WORLD_UNIT				64

#define NEXT_WALL(A)			(walls[A].next)
#define FIRST_WALL(A)			(sectors[A].first_wall)

#define SECTOR_RENDER_MIDDLE	1

// Function prototypes

void RexLightmapsCreate(LIGHTMAP lightmap, PALETTE palette, int brightness);
void RexTablesCreate(void);

void RexEngineDestroy(void);
void RexEngineExecute(void);
void RexEngineCreate(void);
