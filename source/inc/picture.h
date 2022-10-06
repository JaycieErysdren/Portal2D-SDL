// ========================================================
//
// FILE:			/source/inc/picture.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Picture handling function prototypes.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

void RexPictureCreate(PICTURE* picture, int width, int height, int bpp, int bytes_per_row, void* buffer);
void RexPictureCreateMip(PICTURE* dst, PICTURE* src, CLUT blender);
void RexPictureDestroy(PICTURE* picture);
void RexPictureResize(PICTURE* picture, int width, int height);
void RexPictureClear(PICTURE* picture);
void RexPictureCopy(PICTURE* dst, PICTURE* src);
void RexPictureLoad(PICTURE* picture, PATH filename, PALETTE palette);
void RexPictureSave(PICTURE* picture, PATH filename, PALETTE palette);
void RexPictureAssertSame(PICTURE* dst, PICTURE* src);
void RexPictureFlip8(PICTURE* dst, PICTURE* src);
void RexPictureDraw8(PICTURE* dst, PICTURE* src, int x, int y, int flags);
void RexPictureBlit8(PICTURE* dst, int x1, int y1, int x2, int y2, PICTURE* src, int u1, int v1, int u2, int v2, int mode);
void RexPictureLiquidEffect8(PICTURE* dst, PICTURE* src, int tick);
void RexPictureBlend8(PICTURE* dst, PICTURE* src1, PICTURE* src2, CLUT blender);
int RexPaletteSearch(PALETTE palette, int r, int g, int b);
