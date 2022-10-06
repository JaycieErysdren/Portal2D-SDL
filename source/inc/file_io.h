// ========================================================
//
// FILE:			/source/inc/file_io.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		File I/O header.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

// Function prototypes
int RexChunkWrite(FILE* fp, int type, int id, int flag, void* buff, int size);
int RexChunkRead(FILE* fp, int* type, int* id, int* flag, void* buff, int* size);
void RexWallWrite(int wid, FILE* fp);
void RexSectorWrite(int sid, FILE* fp);
void RexLevelWrite(FILE* fp);
void RexLevelClear(void);
void RexLevelRead(FILE* fp);
void RexLevelSave(PATH fn);
void RexLevelLoad(PATH fn);
