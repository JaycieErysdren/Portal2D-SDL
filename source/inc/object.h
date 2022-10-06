// ========================================================
//
// FILE:			/source/inc/object.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Object handling function prototypes.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

void RexObjectLocalSpace(OBJECT* object, MATRIX matrix);
void RexObjectMatrix(OBJECT* object, MATRIX matrix, int width, int height);
void RexObjectUpdate(OBJECT* object);
void RexObjectWallCollision(OBJECT* object, int wid);
void RexObjectSectorCollision(OBJECT* object, int sid);
void RexObjectCollision(OBJECT* object);
