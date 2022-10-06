// ========================================================
//
// FILE:			/source/inc/polygon.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Polygon handling function prototypes.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

int RexPolygonClip(POLYGON dst, POLYGON src, int n, int dist[]);
int RexPolygonClipToFrustrum(POLYGON dst, POLYGON src, int n);
void RexPolygonProject(POLYGON dst, POLYGON src, int n, int width, int height, SURFACE* surface);
