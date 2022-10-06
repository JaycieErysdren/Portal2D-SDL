// ========================================================
//
// FILE:			/source/object.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Object handling functions.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

#include "rex.h"

void RexObjectLocalSpace(OBJECT* object, MATRIX matrix)
{
	MATRIX m;

	RexMatrixIdentity(matrix);
	RexMatrixRotateX(m, -object->rot.x);
	RexMatrixMultiply(matrix, matrix, m);
	RexMatrixRotateY(m, -object->rot.y);
	RexMatrixMultiply(matrix, matrix, m);
	RexMatrixRotateZ(m, -object->rot.z);
	RexMatrixMultiply(matrix, matrix, m);
	RexMatrixPosition(m, object->x << 6, object->z << 6, object->y << 6);
	RexMatrixMultiply(matrix, matrix, m);
}

void RexObjectMatrix(OBJECT* object, MATRIX matrix, int width, int height)
{
	MATRIX m;

	RexMatrixIdentity(matrix);

	RexMatrixPosition(m, - object->x << 6, - object->z << 6, - object->y << 6);
	RexMatrixMultiply(matrix, matrix, m);

	RexMatrixRotateZ(m, object->rot.z);
	RexMatrixMultiply(matrix, matrix, m);

	RexMatrixRotateY(m, object->rot.y);
	RexMatrixMultiply(matrix, matrix, m);

	RexMatrixRotateX(m, object->rot.x);
	RexMatrixMultiply(matrix, matrix, m);

	RexMatrixScale(m, i2f(1), fixdiv(i2f(width) / height, i2f(640) / 480), i2f(1));
	RexMatrixMultiply(matrix, matrix, m);
}

void RexObjectUpdate(OBJECT* object)
{
	object->xxx += object->xx;
	object->yyy += object->yy;
	object->zzz += object->zz;

	object->x += f2i(object->xxx);
	object->y += f2i(object->yyy);
	object->z += f2i(object->zzz);

	object->xxx -= i2f(f2i(object->xxx));
	object->yyy -= i2f(f2i(object->yyy));
	object->zzz -= i2f(f2i(object->zzz));

	if (!RexSectorPointInside(object->sid, object->x, object->y))
	{
		int first_wall = FIRST_WALL(object->sid), wid = first_wall;
		do
		{
			if (walls[wid].port && RexSectorPointInside(walls[walls[wid].port].sid, object->x, object->y))
			{
				object->sid = walls[walls[wid].port].sid;
				return;
			}
		}
		while ((wid = NEXT_WALL(wid)) != first_wall);

		{
		int sid = RexSectorFromPoint(object->x, object->y);

		if (sid) object->sid = sid;
		}
	}
}

// Handle the collision between an object and a wall. Traverses though portals.
void RexObjectWallCollision(OBJECT* object, int wid)
{
	if (RexWallIsVisible(wid, object->x, object->y))
	{
		POINT p = RexWallClosestPoint(wid, object->x, object->y);

		int nx = object->x - p.x;
		int ny = object->y - p.y;

		int len = isqr(nx) + isqr(ny);

		if (len <= isqr(50))
		{
			if (walls[wid].port)
			{
				int bot, top;

				RexSectorZ(walls[walls[wid].port].sid, p.x, p.y, &bot, &top, 0);

				if ((object->z << 6) > bot && (object->z << 6) < top)
				{
					RexObjectSectorCollision(object, walls[walls[wid].port].sid);
					return;
				}
			}
			len = isqrt(len);

			object->x += imuldiv(nx, 50 - len, len);
			object->y += imuldiv(ny, 50 - len, len);
		}
	}
}

// Handle the collision between an object and a sector. We iterate the walls and
// call object_wall_collision() for each. A list of sectors that have been
// visited is recorder to prevent a endless loop.
void RexObjectSectorCollision(OBJECT* object, int sid)
{
	int i, first_wall, wid;

	for (i = sector_list_count; i--;) if (sector_list[i] == sid) return;

	sector_list[sector_list_count++] = sid;

	first_wall = FIRST_WALL(sid), wid = first_wall;

	do RexObjectWallCollision(object, wid); while ((wid = NEXT_WALL(wid)) != first_wall);
}

// Handle object to world collisions.
void RexObjectCollision(OBJECT* object)
{
	sector_list_count = 0;
	RexObjectSectorCollision(object, object->sid);
}
