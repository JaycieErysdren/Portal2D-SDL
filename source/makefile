# ========================================================
#
# FILE:				makefile
#
# AUTHORS:			Jaycie Ewald
#
# PROJECT:			Portal2D-SDL
#
# LICENSE:			ACSL 1.4
#
# DESCRIPTION:		Portal2D makefile for OWv2
#
# LAST EDITED:		October 4th, 2022
#
# ========================================================

# Target executable
TARGET_EXEC=game.exe
TARGET_PATH=../game
TARGET=$(TARGET_PATH)/$(TARGET_EXEC)

# Include directories
WATCOM_DIR=/usr/bin/watcom
GBM_DIR=../modules/gbm

# C compiler
CC=wcc386
CCFLAGS=-w4 -e25 -zq -otexan -d1 -fp3 -4r -mf -bt=dos&
	-I$(WATCOM_DIR)/h&
	-DREX_DOS

# ASM compiler
AS=wasm
ASFLAGS=-mf -5r -w4 -e25 -zq

# Compiled objects to include
OBJS=&
	div0.o&
	dev_io.o&
	editor.o&
	engine.o&
	fix.o&
	level.o&
	math.o&
	matrix.o&
	object.o&
	picture.o&
	polygon.o&
	sector.o&
	sintable.o&
	spanner.o&
	wall.o&

LIBS=&
	$(GBM_DIR)/gbm.lib&

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
