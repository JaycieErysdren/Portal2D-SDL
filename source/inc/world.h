// ========================================================
//
// FILE:			/source/inc/world.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		World interfacing.
//
// LAST EDITED:		October 31st, 2022
//
// ========================================================

// Palette type
typedef ubyte_t palette_t[256][3];

// CLUT type
typedef ubyte_t clut_t[256][256];

// Lightmap type
typedef ubyte_t lightmap_t[32][256];

// 2D coordinate type
typedef struct
{
	long_t x, y;
} coord_t;

// Rectangle type
typedef struct
{
	long_t x1, y1, x2, y2;
} rectangle_t;

// 3-point vector type
typedef struct
{
	long_t x, y, z;
} vec3_t;

// Vertex type
typedef struct
{
	long_t x, y, z;
	long_t u, v;
	long_t w;
} vertex_t;

// Surface type
typedef struct
{
	ubyte_t texture;
	ubyte_t overlay;
	ushort_t flags;
	ubyte_t light, red, green, blue;		// RGB is _not_ currently used.
	long_t slopex;
	long_t slopey;
	long_t slopez;
	ubyte_t repeatx, _pad4, _pad5, _pad6;
	ubyte_t repeaty, _pad7, _pad8, _pad9;
	long_t panningx; 						// fixed point texture u offset
	long_t panningy; 						// fixed point texture v offset
	long_t reserved;
} surface_t;

// Sector type
typedef struct
{
	// Persistent Variables. Moving these variables will break the file format!
	ushort_t lid;
	ushort_t padding;
	ushort_t first_wall;
	ushort_t flags;
	surface_t *top;
	surface_t *bot;
	surface_t *mid;

	int_t reserved;

	// Calculated variables. These variables are not saved out to a file.
	long_t vis_id;
	rectangle_t *bounds;
	coord_t *center;
	long_t locked;
} sector_t;

// Wall type
typedef struct
{
	// Persistent Variables. Moving these variables will break the file format!
	ushort_t sid , _sid;
	ushort_t next, _next;
	ushort_t port, _port;
	long_t x, y;
	surface_t *surface;

	int_t reserved;

	// Calculated variables. These variables are not saved out to a file.
	vertex_t *top;
	vertex_t *bot;
	vertex_t *mid;
	vertex_t *poly[4];
	long_t dx, dy;
	long_t visible;
	long_t frame;
	long_t length;
	long_t distance;
} wall_t;

// Object type
typedef struct
{
	long_t x, xx, xxx;
	long_t y, yy, yyy;
	long_t z, zz, zzz;
	vec3_t *rotation;
	long_t side_id;
	long_t flags;
	surface_t *front;
	surface_t *back;
} object_t;

// Scanlines union
typedef union
{
	ubyte_t **b;
	ushort_t **w;
	ulong_t **l;
} scanlines_t;

// Texture type
typedef struct
{
	long_t width;				// width of picture in pixels
	long_t height;				// height of picture in pixels
	long_t bpp;					// bits per pixel
	long_t bytes_per_row;		// bytes per row. rows are 4 byte aligned
	long_t shared;				// false if buffer is owned. TODO: reference counting
	void_t *buffer;				// allocated pixel buffer
	scanlines_t scanlines;		// pre-calculated scanline pointers
} texture_t;

// World type
typedef struct
{
	// metadata
	byte_t *name;

	// walls
	wall_t *walls;
	ushort_t num_walls;

	// sectors
	sector_t *sectors;
	ushort_t num_sectors;

	// sector list
	ushort_t *sector_list;
	ushort_t num_sector_list;

	// objects
	object_t *objects;
	ushort_t num_objects;

	// textures
	texture_t *textures;
	ushort_t num_textures;
} world_t;

// View type
typedef struct
{
	int_t width, height;
	int_t zoom;
	int_t zoom_div;
	int_t grid;
	int_t key;
	palette_t *palette;
	texture_t *stencil_buffer;
	texture_t *screen_buffer;
	texture_t *color_buffer;
	texture_t *z_buffer;
} view_t;
