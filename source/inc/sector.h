// ========================================================
//
// FILE:			/source/inc/sector.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Sector handling function prototypes.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================


int RexSectorPointInside(int sid, int x, int y);
int RexSectorFromPoint(int x, int y);
void RexSectorZ(int sid, int x, int y, int* bot, int* top, int* mid);
void RexSectorTransform(int sid, MATRIX matrix);
int RexSectorExtrudeCeiling(int sid, POLYGON poly);
int RexSectorExtrudeFloor(int sid, POLYGON poly);
int RexSectorExtrudeMiddle(int sid, POLYGON poly);
POINT RexSectorCenter(int sid);
RECT RexSectorBounds(int sid);
int RexSectorsLinked(int s1, int s2);
void RexSectorCalcVis(OBJECT* camera);
