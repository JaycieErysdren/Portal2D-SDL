// ========================================================
//
// FILE:			/source/sector.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Sector handling functions.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

#include "rex.h"

static int sector_vis_id;

// returns true if a point is inside a sector. otherwise false.
int RexSectorPointInside(int sid, int x, int y)
{
	int cnt = 0;

	if (sid && sectors[sid].lid && FIRST_WALL(sid))
	{
		int first_wall = FIRST_WALL(sid), wid = first_wall;

		do
		{
			WALL* a = &walls[wid];
			WALL* b = &walls[a->next];

			int y1 = a->y - y, y2 = b->y - y;

			if ((y1 ^ y2) < 0)
			{
				int x1 = a->x - x, x2 = b->x - x;

				cnt ^= (x1 ^ x2) < 0 ? (x1 * y2 - x2 * y1) ^ y2 : x1;
			}
		}
		while ((wid = NEXT_WALL(wid)) != first_wall);
	}

	return cnt < 0;
}

int RexSectorFromPoint(int x, int y)
{
	int i;
	for (i = 1; i < MAX_SECTOR; i++) if (RexSectorPointInside(i, x, y)) return i;
	return 0;
}

// compute the z of a given point of the floor and ceiling.
void RexSectorZ(int sid, int x, int y, int* bot, int* top, int* mid)
{
	SECTOR* sector = &sectors[sid];
	x -= sector->bounds.x1;
	y -= sector->bounds.y1;
	if (bot) *bot = (sector->bot.slopez << 6) + (x * sector->bot.slopex) + (y * sector->bot.slopey);
	if (top) *top = (sector->top.slopez << 6) + (x * sector->top.slopex) + (y * sector->top.slopey);
	if (mid) *mid = (sector->mid.slopez << 6) + (x * sector->mid.slopex) + (y * sector->mid.slopey);
}

// transform a sector into camera space.
void RexSectorTransform(int sid, MATRIX matrix)
{
	int wid, first_wall;

	sectors[sid].center = RexSectorCenter(sid);
	sectors[sid].bounds = RexSectorBounds(sid);

	// Transform all the walls into camera space.
	wid = first_wall = FIRST_WALL(sid);
	do RexWallTransform(wid, matrix); while ((wid = NEXT_WALL(wid)) != first_wall);
	// Extrude all the walls.
	wid = first_wall = FIRST_WALL(sid);
	do RexWallExtrude(wid, walls[wid].poly); while ((wid = NEXT_WALL(wid)) != first_wall);
}

// extrude the ceiling of a sector from its walls. Note that we have to create the ceiling vertices in reverse order.
int RexSectorExtrudeCeiling(int sid, POLYGON poly)
{
	int first_wall = FIRST_WALL(sid), wid = first_wall;

	int n = MAX_POLYGON;

	int ox = walls[wid].x >> 8 << 8;
	int oy = walls[wid].y >> 8 << 8;

	do
	{
		WALL* wall = &walls[wid];

		n--;

		poly[n].x = wall->top.x;
		poly[n].y = wall->top.y;
		poly[n].z = wall->top.z;
		poly[n].u = i2f(wall->x - ox);
		poly[n].v = i2f(wall->y - oy);
		poly[n].w = wall->top.w;
	}
	while ((wid = NEXT_WALL(wid)) != first_wall);

	memmove(poly, &poly[n], (MAX_POLYGON - n) * sizeof(VERTEX));

	return MAX_POLYGON - n;
}

// extrude the floor of a sector from its walls.
int RexSectorExtrudeFloor(int sid, POLYGON poly)
{
	int first_wall = FIRST_WALL(sid), wid = first_wall;

	int n = 0;

	int ox = walls[wid].x >> 8 << 8;
	int oy = walls[wid].y >> 8 << 8;

	do
	{
		WALL* wall = &walls[wid];

		poly[n].x = wall->bot.x;
		poly[n].y = wall->bot.y;
		poly[n].z = wall->bot.z;
		poly[n].u = i2f(wall->x - ox);
		poly[n].v = i2f(wall->y - oy);
		poly[n].w = wall->bot.w;

		n++;
	}
	while ((wid = NEXT_WALL(wid)) != first_wall);

	return n;
}

int RexSectorExtrudeMiddle(int sid, POLYGON poly)
{
	int first_wall = FIRST_WALL(sid), wid = first_wall;

	int n = 0;

	int ox = walls[wid].x >> 8 << 8;
	int oy = walls[wid].y >> 8 << 8;

	do
	{
		WALL* wall = &walls[wid];

		poly[n].x = wall->mid.x;
		poly[n].y = wall->mid.y;
		poly[n].z = wall->mid.z;
		poly[n].u = i2f(wall->x - ox);
		poly[n].v = i2f(wall->y - oy);
		poly[n].w = wall->mid.w;

		n++;
	}
	while ((wid = NEXT_WALL(wid)) != first_wall);

	return n;
}

// calculate the center of a sector.
POINT RexSectorCenter(int sid)
{
	int n = 0, wid, first_wall;
	POINT result;

	result.x = 0;
	result.y = 0;

	wid = first_wall = FIRST_WALL(sid);

	do
	{
		result.x += walls[wid].x;
		result.y += walls[wid].y;
		n++;
	}
	while ((wid = NEXT_WALL(wid)) != first_wall);

	result.x /= n;
	result.y /= n;

	return result;
}

// calculate the bounding box of a sector.
RECT RexSectorBounds(int sid)
{
	RECT r;
	int first_wall = FIRST_WALL(sid), wid = first_wall;

	do
	{
		int x = walls[wid].x, y = walls[wid].y;

		if (wid == first_wall)
		{
			r.x1 = x; r.y1 = y; r.x2 = x; r.y2 = y;
		}
		else
		{
			if (x < r.x1) r.x1 = x;
			if (y < r.y1) r.y1 = y;
			if (x > r.x2) r.x2 = x;
			if (y > r.y2) r.y2 = y;
		}
	}
	while ((wid = NEXT_WALL(wid)) != first_wall);

	return r;
}

int RexSectorsLinked(int s1, int s2)
{
	int first_wall = FIRST_WALL(s1), wid = first_wall;

	do
	{
		if (walls[wid].port && walls[walls[wid].port].sid == s2) return 1;
	}
	while ((wid = NEXT_WALL(wid)) != first_wall);

	return 0;
}

int RexSectorWallList(int sid, int list[])
{
	int wid = FIRST_WALL(sid), first_wall = wid, n = 0;
	do list[n++] = wid; while ((wid = NEXT_WALL(wid)) != first_wall);
	return n;
}

int RexLineClip(int x1, int y1, int x2, int y2, int *u1, int *v1, int *u2, int *v2)
{
	// Calculate the normal of the clipping line. Note: The length does not have to be 1.
	int nx = y2 - y1;
	int ny = x1 - x2;
	// Calculate the distance of each point to the clipping line.
	int d1 = (*u1 - x1) * nx + (*v1 - y1) * ny;
	int d2 = (*u2 - x1) * nx + (*v2 - y1) * ny;

	// Is the first point clipped?
	if (d1 < 0)
	{
		if (d2 < 0) return 3; // Both points have been clipped.
		// Calcutate the new point.
		nx = *u1 + imuldiv(*u2 - *u1, d1, d1 - d2);
		ny = *v1 + imuldiv(*v2 - *v1, d1, d1 - d2);
		// Update point.
		*u1 = nx;
		*v1 = ny;
		return 1;
	}
	// Is the second point clipped?
	if (d2 < 0)
	{
		if (d1 < 0) return 3; // Both points have been clipped.
		// Calcutate the new point.
		nx = *u1 + imuldiv(*u2 - *u1, d1, d1 - d2);
		ny = *v1 + imuldiv(*v2 - *v1, d1, d1 - d2);
		// Update point.
		*u2 = nx;
		*v2 = ny;
		return 2;
	}
	// Neither point has been clipped. The line therefore remains intact.
	return 0;
}

void __RexSectorCalcVis(int sid, int mx, int my, int lx, int ly, int rx, int ry)
{
	if (!sectors[sid].locked) // Dont enter sector if its locked.
	{
		int wid, first_wall;

		sectors[sid].locked = 1; // Lock the sector.

		if (sectors[sid].vis_id != sector_vis_id) // Is this the first visit to the sector?
		{
			sectors[sid].vis_id = sector_vis_id;

			sector_list[sector_list_count++] = sid;

			wid = first_wall = FIRST_WALL(sid);

			do
			{
				// Calculate the visablity of each wall. Portals are assumed to be not visible until
				// we prove otherwise.
				walls[wid].visible = !walls[wid].port && RexWallIsVisible(wid, mx, my);
			}
			while ((wid = NEXT_WALL(wid)) != first_wall);
		}

		wid = first_wall = FIRST_WALL(sid);

		do
		{
			// Is this a visible portal.
			if (walls[wid].port && !sectors[walls[walls[wid].port].sid].locked && (walls[wid].visible || RexWallIsVisible(wid, mx, my)))
			{
				int x1 = walls[wid].x;
				int y1 = walls[wid].y;
				int x2 = walls[walls[wid].next].x;
				int y2 = walls[walls[wid].next].y;

				// Clip the portal to the frustum.
				if (RexLineClip(mx, my, lx, ly, &x1, &y1, &x2, &y2) != 3 && RexLineClip(rx, ry, mx, my, &x1, &y1, &x2, &y2) != 3)
				{
					// The portal is visible. Therefore, traverse though the portal using the new frustum.
					walls[wid].visible = 1;
					__RexSectorCalcVis(walls[walls[wid].port].sid, mx, my, x1, y1, x2, y2);
				}
			}
		}
		while ((wid = NEXT_WALL(wid)) != first_wall);

		sectors[sid].locked = 0;
	}
}

// calculate the sector vis from the given camera object.
void RexSectorCalcVis(OBJECT* camera)
{
	sector_vis_id++;

	sector_list_count = 0;

	__RexSectorCalcVis(
		camera->sid,
		camera->x,
		camera->y,
		camera->x + (fixsin(camera->rot.y - 400) >> 4),
		camera->y + (fixcos(camera->rot.y - 400) >> 4),
		camera->x + (fixsin(camera->rot.y + 400) >> 4),
		camera->y + (fixcos(camera->rot.y + 400) >> 4));
}
