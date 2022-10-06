// ========================================================
//
// FILE:			/source/matrix.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Matrix functions.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

#include "rex.h"

// Copies the contents of one matrix to another.
void RexMatrixCopy(MATRIX dst, MATRIX src)
{
	memcpy(dst, src, sizeof(MATRIX));
}

// Returns a identity matrix.
void RexMatrixIdentity(MATRIX dst)
{
	MATRIX src =
	{
		{i2f(1), i2f(0), i2f(0), i2f(0)},
		{i2f(0), i2f(1), i2f(0), i2f(0)},
		{i2f(0), i2f(0), i2f(1), i2f(0)},
		{i2f(0), i2f(0), i2f(0), i2f(1)}
	};

	RexMatrixCopy(dst, src);
}

void RexMatrixPosition(MATRIX dst, int x, int y, int z)
{
	RexMatrixIdentity(dst);
	dst[0][3] = x;
	dst[1][3] = y;
	dst[2][3] = z;
}

void RexMatrixScale(MATRIX dst, int x, int y, int z)
{
	RexMatrixIdentity(dst);
	dst[0][0] = x;
	dst[1][1] = y;
	dst[2][2] = z;
}

void RexMatrixRotateX(MATRIX dst, int angle)
{
	RexMatrixIdentity(dst);
	dst[1][1] =   (dst[2][2] = fixcos(angle));
	dst[1][2] = - (dst[2][1] = fixsin(angle));
}

void RexMatrixRotateY(MATRIX dst, int angle)
{
	RexMatrixIdentity(dst);
	dst[0][0] =   (dst[2][2] = fixcos(angle));
	dst[0][2] = - (dst[2][0] = fixsin(angle));
}

void RexMatrixRotateZ(MATRIX dst, int angle)
{
	RexMatrixIdentity(dst);
	dst[0][0] =   (dst[1][1] = fixcos(angle));
	dst[0][1] = - (dst[1][0] = fixsin(angle));
}

void RexMatrixMultiply(MATRIX dst, MATRIX ma1, MATRIX ma2)
{
	int i, j;
	MATRIX tmp;

	for (i = 4; i--;)
	{
		for (j = 4; j--;)
		{
			tmp[i][j] =
				fixmul(ma2[i][0], ma1[0][j]) +
				fixmul(ma2[i][1], ma1[1][j]) +
				fixmul(ma2[i][2], ma1[2][j]) +
				fixmul(ma2[i][3], ma1[3][j]);
		}
	}

	RexMatrixCopy(dst, tmp);
}

void RexMatrixTranspose(MATRIX dst, MATRIX src)
{
	int x, y;

	for (y = 4; y--;) for (x = 4; x--;) dst[x][y] = src[y][x];
}
