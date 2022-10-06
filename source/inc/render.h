// ========================================================
//
// FILE:			/source/inc/render.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Rendering functions.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

void RexRenderPolygon(POLYGON src, int n, const SURFACE *surface, const WORD id, const int width, const int height);
void RexRenderPolygonGL(POLYGON polygon, int n, SURFACE *surface, int id, MATRIX matrix);
void RexRenderWallTexture(int wid, MATRIX matrix);
void RexRenderSector(int sid, MATRIX matrix);
void RexRenderSectorMiddle(int sid, MATRIX matrix);
void RexRenderObject(int oid, MATRIX matrix);
void RexRenderConsole(void);
void RexRenderView(OBJECT* camera);
