cmake_minimum_required(VERSION 3.13)
project(Portal2D)

find_package(SDL2 REQUIRED)

link_libraries(${SDL2_LIBRARIES})
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(inc)

add_compile_definitions("REX_SDL")

set(PORTAL2D_SRC
	utils.c
	sintable.c
	math_x.c
	math_i.c
	matrix.c
	polygon.c
	wall.c
	sector.c
	spanner.c
	picture.c
	object.c
	dev_io.c
	file_io.c
	console.c
	render.c
	engine.c
	main.c
)

add_executable(portal2d ${PORTAL2D_SRC})
