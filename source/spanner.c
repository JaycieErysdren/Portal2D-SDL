// ========================================================
//
// FILE:			/source/spanner.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		2D span clipping functions.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

#include "rex.h"

void RexSpanArrayInvalidate(SPAN_ARRAY* spanline, int x1, int x2)
{
	spanline->nspans = 1;
	spanline->spans[0].x1 = x1;
	spanline->spans[0].x2 = x2;
}

void RexSpanArrayValidate(SPAN_ARRAY* array, int x1, int x2)
{
	int n;
	SPAN* span = array->spans;
	SPAN* newspan = &array->spans[array->nspans];

	for (n = array->nspans; n--; span++)
	{
		if (x1 < span->x2 && x2 > span->x1)
		{
			if (x1 > span->x1)
			{
				if (x2 < span->x2)
				{
					newspan->x1 = x2;
					newspan->x2 = span->x2;
					newspan++;
					array->nspans++;
				}
				span->x2 = x1;
			}
			else
			{
				if (x2 < span->x2)
				{
					span->x1 = x2;
				}
				else
				{
					SPAN* s;
					for (s = span; s != newspan; s[0] = s[1], s++);

					newspan--;
					span--;
					array->nspans--;
				}
			}
		}
	}
	assert(array->nspans >= 0 && array->nspans < 60);
}

void RexSpannerInvalidate(SPANNER* spanner)
{
	int i;

	spanner->ngaps = spanner->height;

	for (i = 0; i < spanner->height; i++)
	{
		RexSpanArrayInvalidate(&spanner->lines[i], 0, spanner->width);
	}
}

void RexSpannerValidate(SPANNER* spanner, int x1, int y1, int x2, int y2)
{
	int i;

	for (i = y1; i < y2; i++)
	{
		RexSpanArrayValidate(&spanner->lines[i], x1, x2);

		if (spanner->lines[i].nspans == 0) spanner->ngaps--;
	}
}

void RexSpannerCreate(SPANNER* spanner, int width, int height)
{
	memset(spanner, 0, sizeof(SPANNER));

	spanner->width = width;
	spanner->height = height;
	spanner->lines = (SPAN_ARRAY*) calloc(height, sizeof(SPAN_ARRAY));
}

void RexSpannerDelete(SPANNER* spanner)
{
	free(spanner->lines);
}
