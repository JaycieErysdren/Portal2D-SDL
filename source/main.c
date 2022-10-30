// ========================================================
//
// FILE:			/source/main.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		The program's main entry point.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

// Include global header
#include "portal2d.h"

void main(int argc, char *argv[])
{
	char *start_map;
	if (argc > 1) start_map = argv[1];

	view.width = 320;
	view.height = 200;
	view.zoom = 1;
	view.zoom_div = WORLD_UNIT;
	view.grid = WORLD_UNIT * 8;

	EngineCreate();

	MouseShow(1);

	if (start_map)
	{
		LevelLoad(start_map);

		for (;;)
		{
			EngineExecute();

			if (view.key == 'q') break;
		}
	}
	else
	{
		LevelLoad("test.map");

		for (;;)
		{
			EngineExecute();

			if (view.key == 'q') break;
		}
	}

	EngineDestroy();

	exit(EXIT_SUCCESS);
}
