// ========================================================
//
// FILE:			/source/inc/picture.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Picture handling function prototypes.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

void PictureCreate(PICTURE* picture, int width, int height, int bpp, int bytes_per_row, void* buffer);
void PictureCreateMip(PICTURE* dst, PICTURE* src, CLUT blender);
void PictureDestroy(PICTURE* picture);
void PictureResize(PICTURE* picture, int width, int height);
void PictureClear(PICTURE* picture);
void PictureCopy(PICTURE* dst, PICTURE* src);
void PictureLoad(PICTURE* picture, PATH filename, PALETTE palette);
void PictureSave(PICTURE* picture, PATH filename, PALETTE palette);
void PictureAssertSame(PICTURE* dst, PICTURE* src);
void PictureFlip8(PICTURE* dst, PICTURE* src);
void PictureDraw8(PICTURE* dst, PICTURE* src, int x, int y, int flags);
void PictureBlit8(PICTURE* dst, int x1, int y1, int x2, int y2, PICTURE* src, int u1, int v1, int u2, int v2, int mode);
void PictureLiquidEffect8(PICTURE* dst, PICTURE* src, int tick);
void PictureBlend8(PICTURE* dst, PICTURE* src1, PICTURE* src2, CLUT blender);
int PaletteSearch(PALETTE palette, int r, int g, int b);
