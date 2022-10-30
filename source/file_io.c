// ========================================================
//
// FILE:			/source/file_io.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		File I/O functions.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

// Include global header
#include "portal2d.h"

int ChunkWrite(FILE *fp, int type, int id, int flag, void *buff, int size)
{
	return
		(fwrite(&type, sizeof(int), 1, fp) == 1) &&
		(fwrite(&id, sizeof(int), 1, fp) == 1) &&
		(fwrite(&flag, sizeof(int), 1, fp) == 1) &&
		(fwrite(&size, sizeof(int), 1, fp) == 1) &&
		(fwrite(buff, size, 1, fp) == 1);
}

int ChunkRead(FILE *fp, int *type, int *id, int *flag, void *buff, int *size)
{
	return
		(fread(type, sizeof(int), 1, fp) == 1) &&
		(fread(id, sizeof(int), 1, fp) == 1) &&
		(fread(flag, sizeof(int), 1, fp) == 1) &&
		(fread(size, sizeof(int), 1, fp) == 1) &&
		(fread(buff, *size, 1, fp) == 1);
}

void WallWrite(int wid, FILE *fp)
{
	ChunkWrite(fp, 'WALL', wid, 0, &walls[wid], offsetof(WALL, reserved));
}

void SectorWrite(int sid, FILE *fp)
{
	ChunkWrite(fp, 'SECT', sid, 0, &sectors[sid], offsetof(SECTOR, reserved));
}

void LevelWrite(FILE *fp)
{
	int i;
	for (i = 0; i < MAX_WALL ; i++) if (walls[i].sid) WallWrite(i, fp);
	for (i = 0; i < MAX_SECTOR; i++) if (sectors[i].lid) SectorWrite(i, fp);
}

void LevelClear(void)
{
	memset(walls, 0, sizeof(walls));
	memset(sectors, 0, sizeof(sectors));
}

void LevelRead(FILE *fp)
{
	char buffer[1024];
	int type, id, flag, size;

	LevelClear();

	while (ChunkRead(fp, &type, &id, &flag, buffer, &size))
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

void LevelSave(PATH fn)
{
	FILE *fp = fopen(fn, "wb");

	if (fp)
	{
		LevelWrite(fp);
		fclose(fp);
	}
}

void LevelLoad(PATH fn)
{
	FILE *fp = fopen(fn, "rb");

	if (fp)
	{
		LevelRead(fp);
		fclose(fp);
	}
	else
	{
		fail("Failed to load level %s", fn);
	}
}
