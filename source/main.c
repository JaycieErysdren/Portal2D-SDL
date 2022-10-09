// ========================================================
//
// FILE:			/source/main.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D-SDL
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		The program's main entry point.
//
// LAST EDITED:		October 9th, 2022
//
// ========================================================

#include "rex.h"

void main(int argc, char *argv[])
{
	char *start_map;
	if (argc > 1) start_map = argv[1];

	view.width = 320;
	view.height = 200;
	view.zoom = 1;
	view.zoom_div = WORLD_UNIT;
	view.grid = WORLD_UNIT * 8;

	RexEngineCreate();

	RexMouseShow(1);

	if (start_map)
	{
		RexLevelLoad(start_map);

		for (;;)
		{
			RexEngineExecute();

			if (view.key == 'q') break;
		}
	}
	else
	{
		RexLevelLoad("test.map");

		for (;;)
		{
			RexEngineExecute();

			if (view.key == 'q') break;
		}
	}

	RexEngineDestroy();

	exit(EXIT_SUCCESS);
}
