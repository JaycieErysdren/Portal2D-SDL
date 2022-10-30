// ========================================================
//
// FILE:			/source/inc/matrix.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Matrix functions header.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

void MatrixCopy(MATRIX dst, MATRIX src);
void MatrixIdentity(MATRIX dst);
void MatrixPosition(MATRIX dst, int x, int y, int z);
void MatrixScale(MATRIX dst, int x, int y, int z);
void MatrixRotateX(MATRIX dst, int angle);
void MatrixRotateY(MATRIX dst, int angle);
void MatrixRotateZ(MATRIX dst, int angle);
void MatrixTranspose(MATRIX dst, MATRIX src);
void MatrixMultiply(MATRIX dst, MATRIX ma1, MATRIX ma2);
