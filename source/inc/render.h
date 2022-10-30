// ========================================================
//
// FILE:			/source/inc/render.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Rendering functions.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

void RenderPolygon(POLYGON src, int n, const SURFACE *surface, const WORD id, const int width, const int height);
void RenderPolygonGL(POLYGON polygon, int n, SURFACE *surface, int id, MATRIX matrix);
void RenderWallTexture(int wid, MATRIX matrix);
void RenderSector(int sid, MATRIX matrix);
void RenderSectorMiddle(int sid, MATRIX matrix);
void RenderObject(int oid, MATRIX matrix);
void RenderConsole(void);
void RenderView(OBJECT *camera);
