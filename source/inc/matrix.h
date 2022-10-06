// ========================================================
//
// FILE:			/source/inc/matrix.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Matrix functions header.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

void RexMatrixCopy(MATRIX dst, MATRIX src);
void RexMatrixIdentity(MATRIX dst);
void RexMatrixPosition(MATRIX dst, int x, int y, int z);
void RexMatrixScale(MATRIX dst, int x, int y, int z);
void RexMatrixRotateX(MATRIX dst, int angle);
void RexMatrixRotateY(MATRIX dst, int angle);
void RexMatrixRotateZ(MATRIX dst, int angle);
void RexMatrixTranspose(MATRIX dst, MATRIX src);
void RexMatrixMultiply(MATRIX dst, MATRIX ma1, MATRIX ma2);
