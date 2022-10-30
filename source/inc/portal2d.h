// ========================================================
//
// FILE:			/source/inc/portal2d.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Global Portal2D header.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

// Standard headers
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <assert.h>
#include <memory.h>
#include <math.h>

// DOS headers
#include <io.h>
#include <i86.h>
#include <dos.h>
#include <graph.h>
#include <conio.h>

// DIV0 header
#include "div0.h"

// Datatypes
#ifndef __TYPES_H__
#define __TYPES_H__
#include "types.h"
#endif

// Utility functions
#ifndef __UTILS_H__
#define __UTILS_H__
#include "utils.h"
#endif

// Fixed-point math
#ifndef __MATH_X_H__
#define __MATH_X_H__
#include "math_x.h"
#endif

// Integer math
#ifndef __MATH_I_H__
#define __MATH_I_H__
#include "math_i.h"
#endif

// Other math
#ifndef __MATH_H__
#define __MATH_H__
#include "math.h"
#endif

// Assembly math header
#ifndef __PRAGMAS_H__
#define __PRAGMAS_H__
#include "pragmas.h"
#endif

// Matrix header
#ifndef __MATRIX_H__
#define __MATRIX_H__
#include "matrix.h"
#endif

// Main engine header
#ifndef __ENGINE_H__
#define __ENGINE_H__
#include "engine.h"
#endif

// Rendering header
#ifndef __RENDER_H__
#define __RENDER_H__
#include "render.h"
#endif

// Console header
#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#include "console.h"
#endif

// Device I/O header
#ifndef __DEV_IO_H__
#define __DEV_IO_H__
#include "dev_io.h"
#endif

// File I/O header
#ifndef __FILE_IO_H__
#define __FILE_IO_H__
#include "file_io.h"
#endif

// Polygon header
#ifndef __POLYGON_H__
#define __POLYGON_H__
#include "polygon.h"
#endif

// Wall header
#ifndef __WALL_H__
#define __WALL_H__
#include "wall.h"
#endif

// Sector header
#ifndef __SECTOR_H__
#define __SECTOR_H__
#include "sector.h"
#endif

// Picture header
#ifndef __PICTURE_H__
#define __PICTURE_H__
#include "picture.h"
#endif

// Object header
#ifndef __OBJECT_H__
#define __OBJECT_H__
#include "object.h"
#endif

// Spanner header
#ifndef __SPANNER_H__
#define __SPANNER_H__
#include "spanner.h"
#endif
