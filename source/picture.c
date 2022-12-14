// ========================================================
//
// FILE:			/source/picture.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Picture handling functions.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

// Include global header
#include "portal2d.h"

// creates a picture. allocates required pixel buffer and pre-calculates scanline pointers.
void PictureCreate(PICTURE *picture, int width, int height, int bpp, int bytes_per_row, void *buffer)
{
	memset(picture, 0, sizeof(PICTURE));

	if (bpp <= 0) bpp = 8; // default to 8 bpp.

	if (bytes_per_row <= 0) bytes_per_row = (bpp * width + 31) >> 5 << 2;

	picture->width				= width;
	picture->height				= height;
	picture->bpp				= bpp;
	picture->bytes_per_row		= bytes_per_row;
	picture->buffer				= buffer ? buffer : calloc(height, bytes_per_row);
	picture->shared				= buffer != 0;
	picture->scanlines.b		= malloc(height * sizeof(void *));

	while (height--) picture->scanlines.b[height] = (BYTE *)((DWORD)picture->buffer + bytes_per_row * height);
}

void PictureCreateMip(PICTURE *dst, PICTURE *src, CLUT blender)
{
	int x, y;

	PictureCreate(dst, src->width, src->height, src->bpp, 0, 0);

	for (y = 0; y < dst->height; y += 2)
	{
		for (x = 0; x < dst->width; x += 2)
		{
			int a = blender[src->scanlines.b[y + 0][x + 0]][src->scanlines.b[y + 0][x + 1]];
			int b = blender[src->scanlines.b[y + 1][x + 0]][src->scanlines.b[y + 1][x + 1]];
			int c = blender[a][b];

			dst->scanlines.b[y][x] = c;
			dst->scanlines.b[y][x + 1] = c;
			dst->scanlines.b[y + 1][x] = c;
			dst->scanlines.b[y + 1][x + 1] = c;
		}
	}
}

// destroys the resourced allocated to a picture.
void PictureDestroy(PICTURE *picture)
{
	assert(picture);
	if (!picture->shared) free(picture->buffer);
	free(picture->scanlines.b);
}

// resizes a pictures buffer. does not maintain picture contents.
void PictureResize(PICTURE *picture, int width, int height)
{
	if (picture->width != width || picture->height != height)
	{
		int bpp = picture->bpp;
		PictureDestroy(picture);
		PictureCreate(picture, width, height, bpp, 0, 0);
	}
}

// A fast picture content clear.
void PictureClear(PICTURE *picture)
{
	memset(picture->buffer, 0, picture->bytes_per_row * picture->height);
}

// A fast picture content copy. Pictures must be compatible.
void PictureCopy(PICTURE *dst, PICTURE *src)
{
	memcpy(dst->buffer, src->buffer, dst->bytes_per_row * dst->height);
}

void PictureLoad(PICTURE *picture, PATH filename, PALETTE palette)
{
	FILE *fp;
	WORD width, height;
	WORD num_palette_colors;

	fp = fopen(filename, "rb");

	if (fp == NULL)
		return;

	if (fgetc(fp) != 'B' || fgetc(fp) != 'M')
	{
		fclose(fp);
		fail("%s is not a bitmap file", filename);
	}

	fskip(fp, 16);
	fread(&width, sizeof(WORD), 1, fp);
	fskip(fp, 2);
	fread(&height, sizeof(WORD), 1, fp);
	fskip(fp, 22);
	fread(&num_palette_colors, sizeof(WORD), 1, fp);

	PictureCreate(picture, width, height, 0, 0, 0);

	fskip(fp, 6);
	fskip(fp, num_palette_colors * 4);

	fread(picture->buffer, height, picture->bytes_per_row, fp);

	fclose(fp);
}

void PictureSave(PICTURE *picture, PATH filename, PALETTE palette)
{
	fail("PictureSave is not implemented at this time.");
}

void PictureAssertSame(PICTURE *dst, PICTURE *src)
{
	assert(dst && src);
	assert(dst->width == src->width);
	assert(dst->height == src->height);
	assert(dst->bpp == src->bpp);
}

void PictureFlip8(PICTURE *dst, PICTURE *src)
{
	int x, y1, y2;

	PictureAssertSame(dst, src);

	for (y1 = 0, y2 = dst->height - 1; y1 < y2; y1++, y2--)
	{
		BYTE *s1 = src->scanlines.b[y1];
		BYTE *s2 = src->scanlines.b[y2];
		BYTE *d1 = dst->scanlines.b[y1];
		BYTE *d2 = dst->scanlines.b[y2];

		for (x = dst->width; x--;)
		{
			BYTE tmp = *s1++;
			*d1++ = *s2++;
			*d2++ = tmp;
		}
	}
}

void PictureDraw8(PICTURE *dst, PICTURE *src, int x, int y, int flags)
{
	PictureBlit8(dst, x, y, x + src->width, y + src->height, src, 0, 0, src->width, src->height, flags);
}

void PictureBlit8(PICTURE *dst, int x1, int y1, int x2, int y2, PICTURE *src, int u1, int v1, int u2, int v2, int mode)
{
	int w, h, u, uu, vv;

	if ((w = x2 - x1) <= 0 || (h = y2 - y1) <= 0) return;

	u1 = i2f(u1);
	v1 = i2f(v1);
	u2 = i2f(u2);
	v2 = i2f(v2);

	uu = (u2 - u1) / w;
	vv = (v2 - v1) / h;

	if (x1 < 0) {u1 -= x1 * uu; x1 = 0;}
	if (y1 < 0) {v1 -= y1 * vv; y1 = 0;}
	if (x2 > dst->width ) {x2  = dst->width;}
	if (y2 > dst->height) {y2  = dst->height;}

	if ((w = x2 - x1) <= 0 || (h = y2 - y1) <= 0) return;

	#define INNER_LOOP \
	for (;y1 < y2; y1++, v1 += vv) \
	{ \
		BYTE *I = src->scanlines.b[f2i(v1)]; \
		BYTE *O = &dst->scanlines.b[y1][x1]; \
		for(w = x2 - x1, u = u1; w--; u += uu, O++) {AFFINE} \
	}

	 switch (mode)
	 {
		case PICTURE_MODE_COPY:
		{
			if (uu == i2f(1))
			{
				// High speed picture blitter.
				for (;y1 < y2; y1++, v1 += vv)
				{
					memcpy(&dst->scanlines.b[y1][x1], &src->scanlines.b[f2i(v1)][f2i(u1)], x2 - x1);
				}
			}
			else
			{
				#define AFFINE *O = I[f2i(u)];
				INNER_LOOP
				#undef AFFINE
			}
		}
		case PICTURE_MODE_COLORKEY:
		{
			BYTE pen;
			#define AFFINE if (pen = I[f2i(u)]) *O = pen;
			INNER_LOOP
			#undef AFFINE
		}
	}
}

void PictureLiquidEffect8(PICTURE *dst, PICTURE *src, int tick)
{
	int y;
	for (y = dst->height; y--;)
	{
		int x = fixsin(y * 32 + tick * 5) >> 13;

		PictureBlit8(dst, x, y, x + dst->width, y + 1, src, 0, y, src->width, y + 1, PICTURE_MODE_COPY);
		PictureBlit8(dst, x - dst->width, y, x, y + 1, src, 0, y, src->width, y + 1, PICTURE_MODE_COPY);
		PictureBlit8(dst, x + dst->width, y, x + dst->width + dst->width, y + 1, src, 0, y, src->width, y + 1, PICTURE_MODE_COPY);
	}
}

void PictureBlend8(PICTURE *dst, PICTURE *src1, PICTURE *src2, CLUT blender)
{
	int x, y;

	for (y = dst->height; y--;)
	{
		BYTE *a = dst ->scanlines.b[y];
		BYTE *b = src1->scanlines.b[y];
		BYTE *c = src2->scanlines.b[y];

		for (x = dst->width; x--;)
		{
			*a++ = blender[*b++][*c++];
		}
	}
}

int PaletteSearch(PALETTE palette, int r, int g, int b)
{
	int i, pen, dist = INT_MAX;

	for (i = 256; i--;)
	{
		int rank = isqr(palette[i][0] - r) + isqr(palette[i][1] - g) + isqr(palette[i][2] - b);

		if (rank < dist)
		{
			pen  = i;
			dist = rank;
		}
	}

	return pen;
}
