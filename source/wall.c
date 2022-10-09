// ========================================================
//
// FILE:			/source/wall.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Wall handling functions.
//
// LAST EDITED:		October 8th, 2022
//
// ========================================================

#include "rex.h"

int RexWallCreate(int sid)
{
	int wid;

	assert(sid != 0);

	for (wid = 1; wid < MAX_WALL && walls[wid].sid; wid++);

	assert(wid < MAX_WALL);

	memset(&walls[wid], 0, sizeof(WALL));

	walls[wid].sid = sid;
	walls[wid].surface.texture = 0;

	return wid;
}

// extrude a wall into a polygon. prepare u, v texture coordinates.
void RexWallExtrude(int wid, POLYGON polygon)
{
	// Construct a polygon from a wall. We also assign texture UV's.

	WALL* a = &walls[wid];
	WALL* b = &walls[a->next];

	polygon[0].x = a->top.x;
	polygon[0].y = a->top.y;
	polygon[0].z = a->top.z;
	polygon[0].u = 0;
	polygon[0].v = a->top.v;
	polygon[0].w = a->top.w;

	polygon[1].x = b->top.x;
	polygon[1].y = b->top.y;
	polygon[1].z = b->top.z;
	polygon[1].u = a->top.u;
	polygon[1].v = b->top.v;
	polygon[1].w = b->top.w;

	polygon[2].x = b->bot.x;
	polygon[2].y = b->bot.y;
	polygon[2].z = b->bot.z;
	polygon[2].u = a->bot.u;
	polygon[2].v = b->bot.v;
	polygon[2].w = b->bot.w;

	polygon[3].x = a->bot.x;
	polygon[3].y = a->bot.y;
	polygon[3].z = a->bot.z;
	polygon[3].u = 0;
	polygon[3].v = a->bot.v;
	polygon[3].w = a->bot.w;
}

int RexLightCalc(int x, int y, int z)
{
	int t = 0xFFFF;

	x = (x & t) - (t >> 1);
	y = (y & t) - (t >> 1);
	z = (z & t) - (t >> 1);

	#ifdef REX_DOS
	return imax(imin(tmulscale10(x, x, y, y, z, z), i2f(30)), 0);
	#endif

	#ifdef REX_SDL
	return imax(imin(tmulscale(x, x, y, y, z, z, 10), i2f(30)), 0);
	#endif
}

// transform a wall into camera space.
void RexWallTransform(int wid, MATRIX matrix)
{
	int top, bot, mid, x, y;

	WALL* a = &walls[wid];
	WALL* b = &walls[a->next];

	int dx = b->x - a->x;
	int dy = b->y - a->y;

	// We only need to recalc the length of the wall if it has moved relative to the next wall.
	// This prevents a square root for each wall.
	if (dx != a->dx || dy != a->dy)
	{
		a->dx = dx;
		a->dy = dy;
		a->length = isqrt(isqr(dx) + isqr(dy));
	}

	RexSectorZ(a->sid, a->x, a->y, &bot, &top, &mid);

	a->top.u = i2f(a->length); a->top.v = top << 10;
	a->bot.u = i2f(a->length); a->bot.v = bot << 10;

	x = a->x << 6;
	y = a->y << 6;

	a->top.x = fixdot3(x, matrix[0][0], top, matrix[0][1], y, matrix[0][2]) + matrix[0][3];
	a->top.y = fixdot3(x, matrix[1][0], top, matrix[1][1], y, matrix[1][2]) + matrix[1][3];
	a->top.z = fixdot3(x, matrix[2][0], top, matrix[2][1], y, matrix[2][2]) + matrix[2][3];
	a->top.w = RexLightCalc(x, y, top);

	a->bot.x = fixdot3(x, matrix[0][0], bot, matrix[0][1], y, matrix[0][2]) + matrix[0][3];
	a->bot.y = fixdot3(x, matrix[1][0], bot, matrix[1][1], y, matrix[1][2]) + matrix[1][3];
	a->bot.z = fixdot3(x, matrix[2][0], bot, matrix[2][1], y, matrix[2][2]) + matrix[2][3];
	a->bot.w = RexLightCalc(x, y, bot);

	a->mid.x = fixdot3(x, matrix[0][0], mid, matrix[0][1], y, matrix[0][2]) + matrix[0][3];
	a->mid.y = fixdot3(x, matrix[1][0], mid, matrix[1][1], y, matrix[1][2]) + matrix[1][3];
	a->mid.z = fixdot3(x, matrix[2][0], mid, matrix[2][1], y, matrix[2][2]) + matrix[2][3];
	a->mid.w = RexLightCalc(x, y, mid);
}

// returns true if two walls are linked.
int RexWallsLinked(int w1, int w2)
{
	return w1 == NEXT_WALL(w2) || w2 == NEXT_WALL(w1);
}

// returns true if two walls touch. therefore they are portals.
int RexWallsTouch(int w1, int w2)
{
	WALL* a = &walls[w1];
	WALL* b = &walls[w2];
	return
		(a->x == walls[b->next].x) && (a->y == walls[b->next].y) &&
		(b->x == walls[a->next].x) && (b->y == walls[a->next].y);
}

// calculate the normal of a wall. the normal has the length of the wall, therefore you should adjust for the scale.
void RexWallNormal(int wid, int* x, int* y)
{
	*x = walls[walls[wid].next].y - walls[wid].y;
	*y = walls[wid].x - walls[walls[wid].next].x;
}

// returns the points of a wall. not really useful. this is for reference.
void RexWallLine(int wid, int* x1, int* y1, int* x2, int* y2)
{
	*x1 = walls[wid].x;
	*y1 = walls[wid].y;
	*x2 = walls[walls[wid].next].x;
	*y2 = walls[walls[wid].next].y;
}

// returns true if a wall is visible from a given point. (based on a dot product)
int RexWallIsVisible(int wid, int x, int y)
{
	int nx = walls[walls[wid].next].y - walls[wid].y;
	int ny = walls[wid].x - walls[walls[wid].next].x;

	#ifdef REX_DOS
	return dmulscale25(nx, x - walls[wid].x, ny, y - walls[wid].y) >= 0;
	#endif

	#ifdef REX_SDL
	return dmulscale(nx, x - walls[wid].x, ny, y - walls[wid].y, 25) >= 0;
	#endif
}

// extract a wall segment.
int RexWallSegment(VECTOR poly[], int w1, int w2)
{
	int n = 0, wid;

	for (wid = w1; ;wid = NEXT_WALL(wid))
	{
		poly[n].x = walls[wid].x;
		poly[n].y = walls[wid].y;
		poly[n].z = wid;
		n++;
		if (wid == w2) break;
	}
	return n;
}

// Returns the closest point on a wall to a given point.
POINT RexWallClosestPoint(int wid, int x, int y)
{
	POINT result;
	WALL* a = &walls[wid];
	WALL* b = &walls[a->next];

	int dax = b->x - a->x;
	int day = b->y - a->y;
	int daz = dax * (x - a->x) + day * (y - a->y);

	if (daz > 0)
	{
		int daz2 = isqr(dax) + isqr(day);

		if (daz >= daz2)
		{
			result.x = b->x;
			result.y = b->y;
		}
		else
		{
			result.x = a->x + imuldiv(dax, daz, daz2);
			result.y = a->y + imuldiv(day, daz, daz2);
		}
	}
	else
	{
		result.x = a->x;
		result.y = a->y;
	}
	return result;
}
