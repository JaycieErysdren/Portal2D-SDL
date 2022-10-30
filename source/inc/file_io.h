// ========================================================
//
// FILE:			/source/inc/file_io.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		File I/O header.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

// Function prototypes
int ChunkWrite(FILE *fp, int type, int id, int flag, void *buff, int size);
int ChunkRead(FILE *fp, int *type, int *id, int *flag, void *buff, int *size);
void WallWrite(int wid, FILE *fp);
void SectorWrite(int sid, FILE *fp);
void LevelWrite(FILE *fp);
void LevelClear(void);
void LevelRead(FILE *fp);
void LevelSave(PATH fn);
void LevelLoad(PATH fn);
