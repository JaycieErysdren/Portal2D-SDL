// ========================================================
//
// FILE:			/source/render.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Portal2D
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Rendering functions.
//
// LAST EDITED:		October 30th, 2022
//
// ========================================================

// Include global header
#include "portal2d.h"

// render polygon to the back buffer. standard perspective correct dda style rendering. also handles sky rendering & mip mapping.
void RenderPolygon(POLYGON src, int n, const SURFACE *surface, const WORD id, const int width, const int height)
{
	int i;
	POLYGON tmp1, tmp2;
	int ys[MAX_POLYGON];

	for (i = n; i--; ys[i] = src [i].x + src [i].z); n = PolygonClip(tmp2,  src, n, ys); // Left
	for (i = n; i--; ys[i] = tmp2[i].z - tmp2[i].x); n = PolygonClip(tmp1, tmp2, n, ys); // Right
	for (i = n; i--; ys[i] = tmp1[i].y + tmp1[i].z); n = PolygonClip(tmp2, tmp1, n, ys); // Top
	for (i = n; i--; ys[i] = tmp2[i].z - tmp2[i].y); n = PolygonClip(tmp1, tmp2, n, ys); // Bottom

	if (n >= 3)
	{
		int i, j;
		int lt_i, lt_x, lt_xx, lt_z, lt_zz, lt_u, lt_uu, lt_v, lt_vv, lt_w, lt_ww;
		int rt_i, rt_x, rt_xx, rt_z, rt_zz, rt_u, rt_uu, rt_v, rt_vv, rt_w, rt_ww;
		int x, xx;

		TEXTURE66 *tb = textures[surface->texture].buffer;
		LIGHTMAP *lb = lightmaps[(surface->light + 16) & 31];

		int lt_length = 0;
		int rt_length = 0;
		int y1 = INT_MAX;
		int y2 = INT_MIN;

		int cx = i2f(width) >> 1;
		int cy = i2f(height) >> 1;

		for (i = n; i--;)
		{
			VERTEX p = tmp1[i];

			p.z += 6; // 6 == 1 in view space.

			tmp2[i].x = imuldiv(p.x, cx - i2f(0), p.z) + cx - (FRACCEIL >> 1);
			tmp2[i].y = - imuldiv(p.y, cy - i2f(0), p.z) + cy - (FRACCEIL >> 1);
			tmp2[i].z = j = fixinv(p.z);
			tmp2[i].u = mulscale(p.u + surface->panningx, j, surface->repeatx + 18);
			tmp2[i].v = mulscale(p.v + surface->panningy, j, surface->repeaty + 18);
			tmp2[i].w = p.w;

			ys[i] = fixceil(tmp2[i].y);

			if (tmp2[i].y < y1) {y1 = tmp2[i].y; lt_i = rt_i = i;}
			if (tmp2[i].y > y2) {y2 = tmp2[i].y;}
		}

		for (y1 = fixceil(y1), y2 = fixceil(y2); y1 < y2; y1++)
		{
			if (--lt_length <= 0)
			{
				do
				{
					i = lt_i;
					if (--lt_i < 0) lt_i = n - 1;
				}
				while ((lt_length = ys[lt_i] - ys[i]) <= 0);

				#ifdef RENDER_RECIPROCAL
					lt_xx = fixmul(tmp2[lt_i].x - tmp2[i].x, j = fixinv(tmp2[lt_i].y - tmp2[i].y));
					lt_zz = fixmul(tmp2[lt_i].z - tmp2[i].z, j);
					lt_uu = fixmul(tmp2[lt_i].u - tmp2[i].u, j);
					lt_vv = fixmul(tmp2[lt_i].v - tmp2[i].v, j);
					lt_ww = fixmul(tmp2[lt_i].w - tmp2[i].w, j);
				#else
					lt_xx = fixdiv(tmp2[lt_i].x - tmp2[i].x, j = tmp2[lt_i].y - tmp2[i].y);
					lt_zz = fixdiv(tmp2[lt_i].z - tmp2[i].z, j);
					lt_uu = fixdiv(tmp2[lt_i].u - tmp2[i].u, j);
					lt_vv = fixdiv(tmp2[lt_i].v - tmp2[i].v, j);
					lt_ww = fixdiv(tmp2[lt_i].w - tmp2[i].w, j);
				#endif

				lt_x = tmp2[i].x + fixmul(lt_xx, j = i2f(ys[i]) - tmp2[i].y);
				lt_z = tmp2[i].z + fixmul(lt_zz, j);
				lt_u = tmp2[i].u + fixmul(lt_uu, j);
				lt_v = tmp2[i].v + fixmul(lt_vv, j);
				lt_w = tmp2[i].w + fixmul(lt_ww, j);

				assert(y1 == ys[i]);
			}

			if (--rt_length <= 0)
			{
				do
				{
					i = rt_i;
					if (++rt_i >= n) rt_i = 0;
				}
				while ((rt_length = ys[rt_i] - ys[i]) <= 0);

				#ifdef RENDER_RECIPROCAL
					rt_xx = fixmul(tmp2[rt_i].x - tmp2[i].x, j = fixinv(tmp2[rt_i].y - tmp2[i].y));
					rt_zz = fixmul(tmp2[rt_i].z - tmp2[i].z, j);
					rt_uu = fixmul(tmp2[rt_i].u - tmp2[i].u, j);
					rt_vv = fixmul(tmp2[rt_i].v - tmp2[i].v, j);
					rt_ww = fixmul(tmp2[rt_i].w - tmp2[i].w, j);
				#else
					rt_xx = fixdiv(tmp2[rt_i].x - tmp2[i].x, j = tmp2[rt_i].y - tmp2[i].y);
					rt_zz = fixdiv(tmp2[rt_i].z - tmp2[i].z, j);
					rt_uu = fixdiv(tmp2[rt_i].u - tmp2[i].u, j);
					rt_vv = fixdiv(tmp2[rt_i].v - tmp2[i].v, j);
					rt_ww = fixdiv(tmp2[rt_i].w - tmp2[i].w, j);
				#endif

				rt_x = tmp2[i].x + fixmul(rt_xx, j = i2f(ys[i]) - tmp2[i].y);
				rt_z = tmp2[i].z + fixmul(rt_zz, j);
				rt_u = tmp2[i].u + fixmul(rt_uu, j);
				rt_v = tmp2[i].v + fixmul(rt_vv, j);
				rt_w = tmp2[i].w + fixmul(rt_ww, j);

				assert(y1 == ys[i]);
			}

			xx = fixceil(rt_x) - (x = fixceil(lt_x));

			if (xx > 0)
			{
				BYTE *fb = &pic_bbuffer.scanlines.b[y1][x];
				WORD *sb = &pic_stencil.scanlines.w[y1][x];
				WORD *zb = &pic_zbuffer.scanlines.w[y1][x];

				#ifdef RENDER_RECIPROCAL
					int zz = fixmul(rt_z - lt_z, j = fixinv(rt_x - lt_x));
					int uu = fixmul(rt_u - lt_u, j);
					int vv = fixmul(rt_v - lt_v, j);
					int ww = fixmul(rt_w - lt_w, j);
				#else
					int zz = fixdiv(rt_z - lt_z, j = rt_x - lt_x);
					int uu = fixdiv(rt_u - lt_u, j);
					int vv = fixdiv(rt_v - lt_v, j);
					int ww = fixdiv(rt_w - lt_w, j);
				#endif

				int z = lt_z + fixmul(zz, j = i2f(x) - lt_x);
				int u = lt_u + fixmul(uu, j);
				int v = lt_v + fixmul(vv, j);
				int w = lt_w + fixmul(ww, j);

				int zz16 = zz << 4;
				int uu16 = uu << 4;
				int vv16 = vv << 4;
				int ww16 = ww << 4;

				#define INNER_LOOP \
					int a = fixmul(u, j = fixinv(z)); \
					int b = fixmul(v, j); \
					int aa = (fixmul(u += uu16, j = fixinv(z + zz16)) - a) >> 4; \
					int bb = (fixmul(v += vv16, j ) - b) >> 4; \
					int c = w, cc = ((w += ww16) - c) >> 4; \
					for (; xx >= 16; xx -= 16) { \
						AFFINE(0) AFFINE(1) AFFINE(2) AFFINE(3) AFFINE(4) AFFINE(5) AFFINE(6) AFFINE(7) \
						AFFINE(8) AFFINE(9) AFFINE(10) AFFINE(11) AFFINE(12) AFFINE(13) AFFINE(14) AFFINE(15) \
						fb += 16; zb += 16; sb += 16; \
						aa = (fixmul(u += uu16, j = fixinv(z + zz16)) - a) >> 4; \
						bb = (fixmul(v += vv16, j ) - b) >> 4; \
						cc = ((w += ww16) - c) >> 4; \
					} for (j = 0; j < xx; j++) {AFFINE(j)}

				if (tb)
				{
					if (surface->flags & SURFACE_OPAQUE)
					{
						#undef AFFINE
						#define AFFINE(I) if ((WORD)(z>>8) > zb[I]) {zb[I] = (z>>8); sb[I] = id; fb[I] = (*lb)[f2i(c)][blender[TEXEL66(tb,a,b)][fb[I]]];} z += zz; a += aa; b += bb; c += cc;

						INNER_LOOP
					}
					else
					{
						#undef AFFINE
						#define AFFINE(I) if ((WORD)(z>>8) > zb[I]) {zb[I] = (z>>8); sb[I] = id; fb[I] = (*lb)[f2i(c)][TEXEL66(tb,a,b)];} z += zz; a += aa; b += bb; c += cc;

						INNER_LOOP;
					}
				}
				else
				{
					#undef AFFINE
					#define AFFINE if ((z>>8) > *zb) {*zb = (z>>8); *sb = id; *fb = *sky;} sky++; zb++; fb++; sb++; z += zz;

					int sky_pos = (x + (camera->rot.y >> 1) + (tick >> 1)) & 511;
					int sky_lft = imin(512 - sky_pos, xx);
					int sky_rht = xx - sky_lft;

					BYTE *sky = &pic_sky.scanlines.b[y1][sky_pos];

					while (sky_lft--) {AFFINE}

					sky = &pic_sky.scanlines.b[y1][0];

					while (sky_rht--) {AFFINE}

				}
			}

			lt_x += lt_xx; lt_z += lt_zz; lt_u += lt_uu; lt_v += lt_vv; lt_w += lt_ww;
			rt_x += rt_xx; rt_z += rt_zz; rt_u += rt_uu; rt_v += rt_vv; rt_w += rt_ww;
		}
	}
}

void RenderPolygonGL(POLYGON polygon, int n, SURFACE *surface, int id, MATRIX matrix)
{
	RenderPolygon(polygon, n, surface, id, pic_bbuffer.width, pic_bbuffer.height);
}

// render the polygons of a wall.
void RenderWallTexture(int wid, MATRIX matrix)
{
	POLYGON polygon;

	assert(wid);

	if (walls[wid].visible)
	{
		if (walls[wid].port)
		{
			polygon[0] = walls[wid].poly[3];
			polygon[1] = walls[wid].poly[2];
			polygon[2] = walls[wid].poly[2];
			polygon[3] = walls[wid].poly[3];

			if (walls[walls[wid].port].poly[2].y > polygon[0].y)
			{
				polygon[0] = walls[walls[wid].port].poly[2];
				polygon[0].u = walls[walls[wid].port].poly[3].u;
			}
			if (walls[walls[wid].port].poly[3].y > polygon[1].y)
			{
				polygon[1] = walls[walls[wid].port].poly[3];
				polygon[1].u = walls[walls[wid].port].poly[2].u;
			}

			RenderPolygonGL(polygon, 4, &walls[wid].surface, 0x0000 | wid, matrix);

			polygon[0] = walls[wid].poly[0];
			polygon[1] = walls[wid].poly[1];
			polygon[2] = walls[walls[wid].port].poly[0];
			polygon[3] = walls[walls[wid].port].poly[1];

			polygon[2].u = walls[walls[wid].port].poly[1].u;
			polygon[3].u = walls[walls[wid].port].poly[0].u;

			RenderPolygonGL(polygon, 4, &walls[wid].surface, 0x0000 | wid, matrix);
		}
		else
		{
			RenderPolygonGL(walls[wid].poly, 4, &walls[wid].surface, 0x0000 | wid, matrix);
		}
	}
}

// render all the components of a sector.
void RenderSector(int sid, MATRIX matrix)
{
	POLYGON polygon;

	int first_wall = FIRST_WALL(sid), wid = first_wall;

	do RenderWallTexture(wid, matrix); while ((wid = NEXT_WALL(wid)) != first_wall);

	if (sectors[sid].flags & SECTOR_RENDER_MIDDLE)
	{
		//RenderPolygonGL(polygon, SectorExtrudeMiddle(sid, polygon), &sectors[sid].mid, 0x3000 | sid, matrix);
	}

	RenderPolygonGL(polygon, SectorExtrudeFloor(sid, polygon), &sectors[sid].bot, 0x1000 | sid, matrix);
	RenderPolygonGL(polygon, SectorExtrudeCeiling(sid, polygon), &sectors[sid].top, 0x2000 | sid, matrix);
}

void RenderSectorMiddle(int sid, MATRIX matrix)
{
	if (sectors[sid].flags & SECTOR_RENDER_MIDDLE)
	{
		POLYGON polygon;
		RenderPolygonGL(polygon, SectorExtrudeMiddle(sid, polygon), &sectors[sid].mid, 0x3000 | sid, matrix);
	}
}

void RenderObject(int oid, MATRIX matrix)
{
	MATRIX m;
	POLYGON polygon;

	int w = 500 << 6, h = 500 << 6;

	ObjectLocalSpace(&objects[oid], m);

	MatrixMultiply(m, m, matrix);

	polygon[0].x = fixdot3(-w, m[0][0], h, m[0][1], 0, m[0][2]) + m[0][3];
	polygon[0].y = fixdot3(-w, m[1][0], h, m[1][1], 0, m[1][2]) + m[1][3];
	polygon[0].z = fixdot3(-w, m[2][0], h, m[2][1], 0, m[2][2]) + m[2][3];
	polygon[0].u = 0;
	polygon[0].v = 0;

	polygon[1].x = fixdot3( w, m[0][0], h, m[0][1], 0, m[0][2]) + m[0][3];
	polygon[1].y = fixdot3( w, m[1][0], h, m[1][1], 0, m[1][2]) + m[1][3];
	polygon[1].z = fixdot3( w, m[2][0], h, m[2][1], 0, m[2][2]) + m[2][3];
	polygon[1].u = i2f(1024);
	polygon[1].v = 0;

	polygon[2].x = fixdot3( w, m[0][0], -h, m[0][1], 0, m[0][2]) + m[0][3];
	polygon[2].y = fixdot3( w, m[1][0], -h, m[1][1], 0, m[1][2]) + m[1][3];
	polygon[2].z = fixdot3( w, m[2][0], -h, m[2][1], 0, m[2][2]) + m[2][3];
	polygon[2].u = i2f(1024);
	polygon[2].v = i2f(1024);

	polygon[3].x = fixdot3(-w, m[0][0], -h, m[0][1], 0, m[0][2]) + m[0][3];
	polygon[3].y = fixdot3(-w, m[1][0], -h, m[1][1], 0, m[1][2]) + m[1][3];
	polygon[3].z = fixdot3(-w, m[2][0], -h, m[2][1], 0, m[2][2]) + m[2][3];
	polygon[3].u = 0;
	polygon[3].v = i2f(1024);

	RenderPolygonGL(polygon, 4, &objects[oid].front, 0x8000 | oid, matrix);

	polygon[4] = polygon[3];
	polygon[5] = polygon[2];
	polygon[6] = polygon[1];
	polygon[7] = polygon[0];

	RenderPolygonGL(&polygon[4], 4, &objects[oid].back, 0x9000 | oid, matrix);
}

void RenderConsole(void)
{
	int x, y;

	for (y = pic_console.height; y--;)
	{
		for (x = pic_console.width; x--;)
		{
			int xx = x << 3, yy = y << 3;

			int c = pic_console.scanlines.b[y][x] << 3;

			PictureBlit8(&pic_bbuffer, xx, yy, xx + 8, yy + 8, &pic_font, c, 0, c + 8, 8, PICTURE_MODE_COLORKEY);
		}
	}
}

void RenderView(OBJECT *camera)
{
	int x, y;

	// Clear the zbuffer.
	PictureClear(&pic_zbuffer);

	if (camera->sid)
	{
		MATRIX matrix;
		int i;

		SectorCalcVis(camera);

		ObjectMatrix(camera, matrix, pic_bbuffer.width, pic_bbuffer.height);

		for (i = 0; i < sector_list_count; i++) SectorTransform(sector_list[i], matrix);
		for (i = 0; i < sector_list_count; i++) RenderSector(sector_list[i], matrix);
		for (i = 0; i < sector_list_count; i++) RenderSectorMiddle(sector_list[i], matrix);

		RenderObject(2, matrix);
	}

	// Render console output
	RenderConsole();

	// Copy view buffer to video memory.
	DoubleBuffer();
}
