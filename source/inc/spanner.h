// ========================================================
//
// FILE:			/source/inc/spanner.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		2D span clipping function prototypes.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

void SpanArrayInvalidate(SPAN_ARRAY *spanline, int x1, int x2);
void SpanArrayValidate(SPAN_ARRAY *array, int x1, int x2);
void SpannerInvalidate(SPANNER *spanner);
void SpannerValidate(SPANNER *spanner, int x1, int y1, int x2, int y2);
void SpannerCreate(SPANNER *spanner, int width, int height);
void SpannerDelete(SPANNER *spanner);
