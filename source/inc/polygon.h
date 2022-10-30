// ========================================================
//
// FILE:			/source/inc/polygon.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Polygon handling function prototypes.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

int PolygonClip(POLYGON dst, POLYGON src, int n, int dist[]);
int PolygonClipToFrustrum(POLYGON dst, POLYGON src, int n);
void PolygonProject(POLYGON dst, POLYGON src, int n, int width, int height, SURFACE *surface);
