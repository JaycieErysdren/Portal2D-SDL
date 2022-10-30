// ========================================================
//
// FILE:			/source/engine.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Main engine functions.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

// Include global header
#include "portal2d.h"

// Global variables
WALL				walls[MAX_WALL];
SECTOR				sectors[MAX_SECTOR];
OBJECT				objects[MAX_OBJECT];
PICTURE				textures[MAX_TEXTURE];
WORD				sector_list[MAX_SECTOR];

int					sector_list_count;
int					tick;

CLUT				blender;
LIGHTMAP			lightmaps[32];

VIEW				view;

OBJECT				*camera = &objects[1];
OBJECT				*thing = &objects[2];

PICTURE				pic_stencil;
PICTURE				pic_fbuffer;
PICTURE				pic_bbuffer;
PICTURE				pic_zbuffer;

PICTURE				pic_sky;
PICTURE				pic_console;
PICTURE				pic_font;

SURFACE				clipboard;

PALETTE				palette;

// General purpose string buffer. Results should be copied after use.
char				sbuf[1024];

void LightmapsCreate(LIGHTMAP lightmap, PALETTE palette, int brightness)
{
	int fr = 0;
	int fg = 0;
	int fb = 0;
	int i, j;

	for (i = 0; i < 256; i++)
	{
		for (j = 0; j < 32; j++)
		{
			lightmap[j][i] = PaletteSearch(palette,
				imin(imuldiv(fr + imuldiv(palette[i][0] - fr, j, 31), brightness * 4, 31), 255),
				imin(imuldiv(fg + imuldiv(palette[i][1] - fg, j, 31), brightness * 4, 31), 255),
				imin(imuldiv(fb + imuldiv(palette[i][2] - fb, j, 31), brightness * 4, 31), 255));
		}
	}
}

void TablesCreate(void)
{
	int i, x, y, r;

	say("Calculating Lightmaps.");
	for (i = 32; i--;) LightmapsCreate(lightmaps[i], palette, i);

	say("Calculating Blender LUT.");

	for (x = 255; x--;)
	{
		for (y = 255; y--;)
		{
			r = (palette[x][0] + palette[y][0]) >> 1;

			blender[y][x] = PaletteSearch(palette,
				(palette[x][0] + palette[y][0]) >> 1,
				(palette[x][1] + palette[y][1]) >> 1,
				(palette[x][2] + palette[y][2]) >> 1);
		}
	}
}

void EngineDestroy(void)
{
	div0_close();

	TimerRemove();
	KeyboardRemove();

	PictureDestroy(&pic_fbuffer);
	PictureDestroy(&pic_bbuffer);
	PictureDestroy(&pic_zbuffer);
	PictureDestroy(&pic_stencil);
	PictureDestroy(&pic_console);

	MouseShow(0);

	GraphicsRemove();

	exit(EXIT_SUCCESS);
}

void EngineExecute(void)
{
	int half_x = 160, half_y = 100;

	#ifdef EDITABLE_SURFACES
	int x, y;
	#endif

	int i;
	int floor_z, ceil_z, under;
	int frame_count = 0;

	div0_init(DM_SATURATE);

	MouseShow(0);

	GraphicsInstall("Portal2D", 320, 200);

	PictureCreate(&pic_fbuffer, view.width, view.height, 8, 0, (void *)0xA0000);

	PictureCreate(&pic_bbuffer, view.width, view.height, 8, 0, 0);

	PictureCreate(&pic_zbuffer, view.width, view.height, 16, 0, 0);
	PictureCreate(&pic_stencil, view.width, view.height, 16, 0, 0);

	PictureCreate(&pic_console, 40, 10, 8, 0, 0);

	PaletteInstall(palette);

	KeyboardInstall();
	TimerInstall(120);

	thing->sid = 0;
	thing->x = -900;
	thing->y = 2000;
	thing->z = 200;
	thing->front.texture = 124;
	//thing->front.flags |= SURFACE_OPAQUE;
	thing->back.texture = 10;
	//thing->back.flags |= SURFACE_OPAQUE;

	camera->sid = 0;
	camera->z = WORLD_UNIT * 4;

	for (tick = timer;;)
	{
		PictureLiquidEffect8(&textures[41], &textures[42], tick);
		PictureLiquidEffect8(&textures[43], &textures[44], tick);

		PictureBlend8(&textures[113], &textures[41], &textures[15], blender);
		PictureBlend8(&textures[114], &textures[41], &textures[64], blender);

		RenderView(camera);
		DevicesRead();

		// reset position after reading it
		MouseSet(&half_x, &half_y);

		sprintf(sbuf, "Key:%3d X:%4d Y:%4d Z:%4d SID:%d",
			input_key_last,
			camera->x,
			camera->y,
			camera->z,
			camera->sid);

		ConsoleOutText(0, 0, sbuf);

		SectorZ(camera->sid, camera->x, camera->y, &floor_z, &ceil_z, 0);

		sprintf(sbuf, "%d %d FPS: %d", floor_z, ceil_z, imuldiv(frame_count++, 120, timer + 1));
		ConsoleOutText(0, 1, sbuf);

		for (; tick < timer; tick++)
		{
			camera->zz -= fl2f(0.4); // Gravity

			ObjectUpdate(thing);
			thing->rot.y += 2;

			ObjectUpdate(camera);
			ObjectCollision(camera);

			SectorZ(camera->sid, camera->x, camera->y, &floor_z, &ceil_z, 0);

			under = ((camera->z - 300) << 6) - floor_z;

			// movement controls
			if (under < 0)
			{
				camera->xx -= camera->xx / 18;
				camera->yy -= camera->yy / 18;
				camera->zz -= camera->zz / 18;

				camera->zz -= under * 8;

				camera->zz &= 0xFFFFF000;

				if (KEY_DOWN(KB_W))
				{
					camera->xx += imuldiv(fixsin(camera->rot.y), 6, 8);
					camera->yy += imuldiv(fixcos(camera->rot.y), 6, 8);
				}
				if (KEY_DOWN(KB_S))
				{
					camera->xx -= imuldiv(fixsin(camera->rot.y), 6, 8);
					camera->yy -= imuldiv(fixcos(camera->rot.y), 6, 8);
				}
				if (KEY_DOWN(KB_A))
				{
					camera->xx -= imuldiv(fixcos(camera->rot.y), 6, 8);
					camera->yy += imuldiv(fixsin(camera->rot.y), 6, 8);
				}
				if (KEY_DOWN(KB_D))
				{
					camera->xx += imuldiv(fixcos(camera->rot.y), 6, 8);
					camera->yy -= imuldiv(fixsin(camera->rot.y), 6, 8);
				}
				if (KEY_PRESSED(KB_SPACE)) // Jump
				{
					camera->zz += fl2f(15);
				}
				if (KEY_DOWN(KB_CTRL)) // Crouch
				{
					camera->zz -= fl2f(0.7);
				}
			}

			//if (KEY_PRESSED(88)) PictureSave(&pic_bbuffer, "screen.pcx", view.palette);

			// view controls
			camera->rot.y += (input_mouse_x - half_x);
			camera->rot.x -= (input_mouse_y - half_y);

			if (camera->rot.x > 320) camera->rot.x = 320;
			else if (camera->rot.x < -320) camera->rot.x = -320;

			//if (KEY_DOWN(30) && camera->rot.x < 320) camera->rot.x += 7;
			//if (KEY_DOWN(44) && camera->rot.x > -320) camera->rot.x -= 7;
			//if (KEY_DOWN(KB_RTARROW)) camera->rot.y += 7;
			//if (KEY_DOWN(KB_LTARROW)) camera->rot.y -= 7;

			if (KEY_DOWN(KB_ESC)) EngineDestroy();

			#ifdef EDITABLE_SURFACES
				#define SURFACE_EDIT(VAR,SUR,A) (clipboard.VAR = (SUR.VAR = KEY_DOWN(KB_CTRL) ? clipboard.VAR : SUR.VAR + (KEY_DOWN(KB_LTSHIFT) ? -(A) : (A))))

				if (KEY_PRESSED(24)) // Opacity
				{
					WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

					if ((code >> 12) == 3) sectors[id].mid.flags ^= SURFACE_OPAQUE;
					if ((code >> 12) == 8) objects[id].front.flags ^= SURFACE_OPAQUE;
					if ((code >> 12) == 9) objects[id].back.flags ^= SURFACE_OPAQUE;
				}
				if (KEY_PRESSED(20)) // Texture
				{
					WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

					if ((code >> 12) == 0) SURFACE_EDIT(texture, walls[id].surface, 1);
					if ((code >> 12) == 1) SURFACE_EDIT(texture, sectors[id].bot, 1);
					if ((code >> 12) == 2) SURFACE_EDIT(texture, sectors[id].top, 1);
					if ((code >> 12) == 3) SURFACE_EDIT(texture, sectors[id].mid, 1);

					if ((code >> 12) == 8) SURFACE_EDIT(texture, objects[id].front, 1);
					if ((code >> 12) == 9) SURFACE_EDIT(texture, objects[id].back, 1);
				}
				if (KEY_PRESSED(45)) // X Repeat
				{
					WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

					if ((code >> 12) == 0) SURFACE_EDIT(repeatx, walls[id].surface, 1);
					if ((code >> 12) == 1) SURFACE_EDIT(repeatx, sectors[id].bot, 1);
					if ((code >> 12) == 2) SURFACE_EDIT(repeatx, sectors[id].top, 1);
				}
				if (KEY_PRESSED(21)) // Y Repeat
				{
					WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

					if ((code >> 12) == 0) SURFACE_EDIT(repeaty, walls[id].surface, 1);
					if ((code >> 12) == 1) SURFACE_EDIT(repeaty, sectors[id].bot, 1);
					if ((code >> 12) == 2) SURFACE_EDIT(repeaty, sectors[id].top, 1);
				}

				if (KEY_PRESSED(35)) // X Panning
				{
					WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

					if ((code >> 12) == 0) SURFACE_EDIT(panningx, walls[id].surface, i2f(16));
					if ((code >> 12) == 1) SURFACE_EDIT(panningx, sectors[id].bot, i2f(16));
					if ((code >> 12) == 2) SURFACE_EDIT(panningx, sectors[id].top, i2f(16));
				}
				if (KEY_PRESSED(47)) // Y Panning
				{
					WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

					if ((code >> 12) == 0) SURFACE_EDIT(panningy, walls[id].surface, i2f(16));
					if ((code >> 12) == 1) SURFACE_EDIT(panningy, sectors[id].bot, i2f(16));
					if ((code >> 12) == 2) SURFACE_EDIT(panningy, sectors[id].top, i2f(16));
				}
				if (KEY_PRESSED(38)) // Light (Brightness)
				{
					WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

					if ((code >> 12) == 0) SURFACE_EDIT(light, walls[id].surface, 1);
					if ((code >> 12) == 1) SURFACE_EDIT(light, sectors[id].bot, 1);
					if ((code >> 12) == 2) SURFACE_EDIT(light, sectors[id].top, 1);
				}

				if (KEY_PRESSED(KB_INSERT))
				{
					WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

					if ((code >> 12) == 1) sectors[id].bot.slopex += 4;
					if ((code >> 12) == 2) sectors[id].top.slopex += 4;
					if ((code >> 12) == 3) sectors[id].mid.slopex += 4;
				}
				if (KEY_PRESSED(KB_DELETE))
				{
					WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

					if ((code >> 12) == 1) sectors[id].bot.slopex -= 4;
					if ((code >> 12) == 2) sectors[id].top.slopex -= 4;
					if ((code >> 12) == 3) sectors[id].mid.slopex -= 4;
				}
				if (KEY_PRESSED(KB_HOME))
				{
					WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

					if ((code >> 12) == 1) sectors[id].bot.slopey += 4;
					if ((code >> 12) == 2) sectors[id].top.slopey += 4;
					if ((code >> 12) == 3) sectors[id].mid.slopey += 4;
				}
				if (KEY_PRESSED(KB_END))
				{
					WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

					if ((code >> 12) == 1) sectors[id].bot.slopey -= 4;
					if ((code >> 12) == 2) sectors[id].top.slopey -= 4;
					if ((code >> 12) == 3) sectors[id].mid.slopey -= 4;
				}
				if (KEY_PRESSED(KB_PAGEUP))
				{
					WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

					if ((code >> 12) == 1) SURFACE_EDIT(slopez, sectors[id].bot, -WORLD_UNIT);
					if ((code >> 12) == 2) SURFACE_EDIT(slopez, sectors[id].top, -WORLD_UNIT);
					if ((code >> 12) == 3) SURFACE_EDIT(slopez, sectors[id].mid, -WORLD_UNIT);
				}
				if (KEY_PRESSED(KB_PAGEDN))
				{
					WORD code = pic_stencil.scanlines.w[y][x], id = code & 0xFFF;

					if ((code >> 12) == 1) SURFACE_EDIT(slopez, sectors[id].bot, WORLD_UNIT);
					if ((code >> 12) == 2) SURFACE_EDIT(slopez, sectors[id].top, WORLD_UNIT);
					if ((code >> 12) == 3) SURFACE_EDIT(slopez, sectors[id].mid, WORLD_UNIT);
				}
				if (KEY_PRESSED(50))
				{
					sectors[camera->sid].flags ^= SECTOR_RENDER_MIDDLE;
				}
			#endif

			memcpy(input_keys_prev, input_keys, sizeof(input_keys_prev));
		}
	}

	div0_close();
}

void EngineCreate(void)
{
	int i;

	say("AlienKinetics 3D Engine [DOS] - Copyright (C) 2001-2002 AlienKinetics");
	say("---------------------------------------------------------------------");
	say("Modified 2022-2023 by Erysdren Media");
	say("---------------------------------------------------------------------");
	say("");

	say("Installing mouse driver.");
	MouseInstall();

	say("Allocating offscreen buffers.");

	PictureLoad(&pic_font, "images/font8x8.bmp", palette);
	PictureLoad(&pic_sky, "images/sky.bmp", palette);
	//PictureLoad(&pic_arrow, "images/arrow.pcx", palette);
	//PictureLoad(&pic_lens, "images/lens.pcx", palette);

	say("Installing palette.");
	PaletteLoad("gfx/palette.dat");

	TablesCreate();

	say("Loading textures.");

	for (i = 0; i < MAX_TEXTURE; i++)
	{
		sprintf(sbuf, "textures/%d.bmp", i);
		PictureLoad(&textures[i], sbuf, palette);
	}
}
