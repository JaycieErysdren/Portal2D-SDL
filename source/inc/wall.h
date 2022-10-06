// ========================================================
//
// FILE:			/source/inc/wall.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Wall handling function prototypes.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

int RexWallCreate(int sid);
void RexWallExtrude(int wid, POLYGON polygon);
void RexWallTransform(int wid, MATRIX matrix);
int RexWallsLinked(int w1, int w2);
int RexWallsTouch(int w1, int w2);
void RexWallNormal(int wid, int* x, int* y);
int RexWallSegment(VECTOR poly[], int w1, int w2);
void RexWallLine(int wid, int* x1, int* y1, int* x2, int* y2);
int RexWallIsVisible(int wid, int x, int y);
POINT RexWallClosestPoint(int wid, int x, int y);
