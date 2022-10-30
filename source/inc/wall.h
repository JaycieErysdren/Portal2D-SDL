// ========================================================
//
// FILE:			/source/inc/wall.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Wall handling function prototypes.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

int WallCreate(int sid);
void WallExtrude(int wid, POLYGON polygon);
void WallTransform(int wid, MATRIX matrix);
int WallsLinked(int w1, int w2);
int WallsTouch(int w1, int w2);
void WallNormal(int wid, int *x, int *y);
int WallSegment(VECTOR poly[], int w1, int w2);
void WallLine(int wid, int *x1, int *y1, int *x2, int *y2);
int WallIsVisible(int wid, int x, int y);
POINT WallClosestPoint(int wid, int x, int y);
