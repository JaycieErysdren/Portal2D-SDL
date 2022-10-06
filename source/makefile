# ========================================================
#
# FILE:				/source/makefile
#
# AUTHORS:			Jaycie Ewald
#
# PROJECT:			Portal2D-SDL
#
# LICENSE:			ACSL 1.4
#
# DESCRIPTION:		Portal2D makefile for OWv2
#
# LAST EDITED:		October 6th, 2022
#
# ========================================================

# Target executable
TARGET_EXEC=game2.exe
TARGET_PATH=../game
TARGET=$(TARGET_PATH)/$(TARGET_EXEC)

# Include directories
WATCOM_DIR=/usr/bin/watcom
GBM_DIR=../modules/gbm
DIV0_DIR=../modules/div0

# C compiler
CC=wcc386
CCFLAGS=-w4 -e25 -zq -otexan -d1 -fp3 -4r -mf -bt=dos&
	-Iinc&
	-I$(WATCOM_DIR)/h&
	-I$(GBM_DIR)&
	-I$(DIV0_DIR)&
	-DREX_DOS

# ASM compiler
AS=wasm
ASFLAGS=-mf -5r -w4 -e25 -zq

# Compiled objects to include
OBJS=&
	utils.o&
	sintable.o&
	math_x.o&
	math_i.o&
	matrix.o&
	polygon.o&
	wall.o&
	sector.o&
	spanner.o&
	picture.o&
	object.o&
	dev_io.o&
	file_io.o&
	console.o&
	render.o&
	engine.o&
	main.o&

LIBS=&
	$(GBM_DIR)/gbm.lib&
	$(DIV0_DIR)/div0.lib&

# C rules
.c.o:
	set _CCARGS=$(CCFLAGS) $*.c
	$(CC) @_CCARGS

# ASM rules
.asm.o:
	set _ASARGS=$(ASFLAGS) $*.asm
	$(AS) @_ASARGS

# Target operation
$(TARGET): $(OBJS) $(TARGET_EXEC).lnk
	wlink @$(TARGET_EXEC).lnk

# Link response file
$(TARGET_EXEC).lnk: makefile
	echo name $(TARGET) >$(TARGET_EXEC).lnk
	echo system dos4g >>$(TARGET_EXEC).lnk
	for %i in ($(OBJS)) do echo file %i >>$(TARGET_EXEC).lnk
	for %i in ($(LIBS)) do echo library %i >>$(TARGET_EXEC).lnk

clean: .SYMBOLIC
	rm *.obj
	rm *.o
	rm *.lnk
	rm *.rsp
	rm *.err
	rm *.exe
