// ========================================================
//
// FILE:			/source/inc/object.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Object handling function prototypes.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

void ObjectLocalSpace(OBJECT* object, MATRIX matrix);
void ObjectMatrix(OBJECT* object, MATRIX matrix, int width, int height);
void ObjectUpdate(OBJECT* object);
void ObjectWallCollision(OBJECT* object, int wid);
void ObjectSectorCollision(OBJECT* object, int sid);
void ObjectCollision(OBJECT* object);
