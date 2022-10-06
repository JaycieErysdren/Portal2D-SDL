// ========================================================
//
// FILE:			/source/inc/spanner.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		2D span clipping function prototypes.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

void RexSpanArrayInvalidate(SPAN_ARRAY* spanline, int x1, int x2);
void RexSpanArrayValidate(SPAN_ARRAY* array, int x1, int x2);
void RexSpannerInvalidate(SPANNER* spanner);
void RexSpannerValidate(SPANNER* spanner, int x1, int y1, int x2, int y2);
void RexSpannerCreate(SPANNER* spanner, int width, int height);
void RexSpannerDelete(SPANNER* spanner);
