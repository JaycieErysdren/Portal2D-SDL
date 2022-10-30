// ========================================================
//
// FILE:			/source/inc/sector.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Sector handling function prototypes.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================


int SectorPointInside(int sid, int x, int y);
int SectorFromPoint(int x, int y);
void SectorZ(int sid, int x, int y, int* bot, int* top, int* mid);
void SectorTransform(int sid, MATRIX matrix);
int SectorExtrudeCeiling(int sid, POLYGON poly);
int SectorExtrudeFloor(int sid, POLYGON poly);
int SectorExtrudeMiddle(int sid, POLYGON poly);
POINT SectorCenter(int sid);
RECT SectorBounds(int sid);
int SectorsLinked(int s1, int s2);
void SectorCalcVis(OBJECT* camera);
