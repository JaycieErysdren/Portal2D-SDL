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
// LAST EDITED:		October 6th, 2022
//
// ========================================================

#include "rex.h"

void main(int argc, char *argv[])
{
	char *start_map;
	if (argc > 1) start_map = argv[1];

	view.width = 640;
	view.height = 480;
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
			div0_init(DM_SATURATE);
			RexEngineExecute();
			div0_close();

			if (view.key == 'q') break;
		}
	}

	RexEngineDestroy();

	exit(EXIT_SUCCESS);
}
