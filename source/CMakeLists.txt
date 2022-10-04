cmake_minimum_required(VERSION 3.13)
project(Portal2D)

find_package(SDL2 REQUIRED)

link_libraries(${SDL2_LIBRARIES})
include_directories(${SDL2_INCLUDE_DIRS})

set(PORTAL2D_SRC
	div0.c
	dos.c
	editor.c
	engine.c
	fix.c
	level.c
	math.c
	matrix.c
	object.c
	picture.c
	polygon.c
	sector.c
	sintable.c
	spanner.c
	wall.c
	)

add_executable(portal2d ${PORTAL2D_SRC})
