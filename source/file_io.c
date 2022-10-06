// ========================================================
//
// FILE:			/source/file_io.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		File I/O functions.
//
// LAST EDITED:		October 5th, 2022
//
// ========================================================

#include "rex.h"

int RexChunkWrite(FILE* fp, int type, int id, int flag, void* buff, int size)
{
	return
		(fwrite(&type, sizeof(int), 1, fp) == 1) &&
		(fwrite(&id, sizeof(int), 1, fp) == 1) &&
		(fwrite(&flag, sizeof(int), 1, fp) == 1) &&
		(fwrite(&size, sizeof(int), 1, fp) == 1) &&
		(fwrite(buff, size, 1, fp) == 1);
}

int RexChunkRead(FILE* fp, int* type, int* id, int* flag, void* buff, int* size)
{
	return
		(fread(type, sizeof(int), 1, fp) == 1) &&
		(fread(id, sizeof(int), 1, fp) == 1) &&
		(fread(flag, sizeof(int), 1, fp) == 1) &&
		(fread(size, sizeof(int), 1, fp) == 1) &&
		(fread(buff, *size, 1, fp) == 1);
}

void RexWallWrite(int wid, FILE* fp)
{
	RexChunkWrite(fp, 'WALL', wid, 0, &walls[wid], offsetof(WALL, reserved));
}

void RexSectorWrite(int sid, FILE* fp)
{
	RexChunkWrite(fp, 'SECT', sid, 0, &sectors[sid], offsetof(SECTOR, reserved));
}

void RexLevelWrite(FILE* fp)
{
	int i;
	for (i = 0; i < MAX_WALL ; i++) if (walls[i].sid) RexWallWrite(i, fp);
	for (i = 0; i < MAX_SECTOR; i++) if (sectors[i].lid) RexSectorWrite(i, fp);
}

void RexLevelClear(void)
{
	memset(walls, 0, sizeof(walls));
	memset(sectors, 0, sizeof(sectors));
}

void RexLevelRead(FILE* fp)
{
	char buffer[1024];
	int type, id, flag, size;

	RexLevelClear();

	while (RexChunkRead(fp, &type, &id, &flag, buffer, &size))
	{
		if (type == 'WALL')
		{
			assert(id < MAX_WALL);
			memcpy(&walls[id], buffer, size);
		}
		if (type == 'SECT')
		{
			assert(id < MAX_SECTOR);
			memcpy(&sectors[id], buffer, size);
		}
	}
}

void RexLevelSave(PATH fn)
{
	FILE* fp = fopen(fn, "wb");

	if (fp)
	{
		RexLevelWrite(fp);
		fclose(fp);
	}
}

void RexLevelLoad(PATH fn)
{
	FILE* fp = fopen(fn, "rb");

	if (fp)
	{
		RexLevelRead(fp);
		fclose(fp);
	}
}
