## ========================================================
##
## FILE:			/source/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Portal2D
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		Portal2D CMake project definition.
##
## LAST EDITED:		October 30th, 2022
##
## ========================================================

##
## CMake gates
##

## CMake version gate
cmake_minimum_required(VERSION 3.13)

## Open Watcom v2 gate
if(NOT CMAKE_GENERATOR STREQUAL "Watcom WMake")
	message(FATAL_ERROR "Portal2D can only be compiled with Open Watcom.")
endif()

## Processor architechture gate
if(CMAKE_SYSTEM_PROCESSOR STREQUAL I86)
	message(FATAL_ERROR "Portal2D can only be compiled in 32-bit protected mode.")
endif()

## DOS target gate
if(NOT CMAKE_SYSTEM_NAME STREQUAL DOS)
	message(FATAL_ERROR "Portal2D can only compile under DOS.")
endif()

## Add custom CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

## WAM compiler gate
## https://cmake.cmake.narkive.com/yG2NA3xR/asm-support
find_program(WASM wasm REQUIRED)
set(CMAKE_ASM_COMPILER ${WASM})
set(CMAKE_ASM_COMPILER_INIT ${WASM})
set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS asm)
set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -fo=<OBJECT> <SOURCE>")

## WLIB linker gate
## https://cmake.cmake.narkive.com/yG2NA3xR/asm-support
find_program(WLIB wlib REQUIRED)
set(CMAKE_AR ${WLIB})
SET(CMAKE_ASM_CREATE_STATIC_LIBRARY "<CMAKE_AR> <TARGET> <LINK_FLAGS> <OBJECTS>")

##
## Project
##

## Define project
project(Portal2D LANGUAGES C ASM)

## Build type
set(CMAKE_BUILD_TYPE Debug)

## Setup project directories
set(DIV0_DIR ${CMAKE_SOURCE_DIR}/external/div0)
set(PORTAL2D_DIR ${CMAKE_SOURCE_DIR})

## Global compile options & definitions
add_compile_options("-w4")
add_compile_options("-e25")
add_compile_options("-mf")
add_compile_definitions("REX_DOS")

## C compile options & definitions
add_compile_options("$<$<COMPILE_LANGUAGE:C>:-bcl=dos4g>")
add_compile_options("$<$<COMPILE_LANGUAGE:C>:-bt=dos4g>")
add_compile_options("$<$<COMPILE_LANGUAGE:C>:-otexan>")
add_compile_options("$<$<COMPILE_LANGUAGE:C>:-d1>")
add_compile_options("$<$<COMPILE_LANGUAGE:C>:-fp3>")
add_compile_options("$<$<COMPILE_LANGUAGE:C>:-4r>")

## ASM compile options & definitions
add_compile_options("$<$<COMPILE_LANGUAGE:ASM>:-5r>")
add_compile_options("$<$<COMPILE_LANGUAGE:ASM>:-zq>")

## Set DIV0 source files
set(DIV0_SRC
	${DIV0_DIR}/div0.asm
)

## Add DIV0 library
add_library(div0 ${DIV0_SRC})

## Link DIV0 library
include_directories(${DIV0_DIR})
link_libraries(div0)

## Include Portal2D headers
include_directories(${PORTAL2D_DIR}/inc)

## Add Portal2D source files
set(PORTAL2D_SRC
	${PORTAL2D_DIR}/utils.c
	${PORTAL2D_DIR}/sintable.c
	${PORTAL2D_DIR}/math_x.c
	${PORTAL2D_DIR}/math_i.c
	${PORTAL2D_DIR}/pragmas.c
	${PORTAL2D_DIR}/matrix.c
	${PORTAL2D_DIR}/polygon.c
	${PORTAL2D_DIR}/wall.c
	${PORTAL2D_DIR}/sector.c
	${PORTAL2D_DIR}/spanner.c
	${PORTAL2D_DIR}/picture.c
	${PORTAL2D_DIR}/object.c
	${PORTAL2D_DIR}/dev_io.c
	${PORTAL2D_DIR}/file_io.c
	${PORTAL2D_DIR}/console.c
	${PORTAL2D_DIR}/render.c
	${PORTAL2D_DIR}/engine.c
	${PORTAL2D_DIR}/main.c
)

## Add Portal2D executable
add_executable(portal2d ${PORTAL2D_SRC})

## Set output directory
set_target_properties(portal2d
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../game"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../game"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../game"
)
