# Target executable
TARGET_EXEC=dr_pcx.lib
TARGET_PATH=.
TARGET=$(TARGET_PATH)/$(TARGET_EXEC)

# Include directories
WATCOM_DIR=/usr/bin/watcom

# C compiler
CC=wcc386
CCFLAGS=-w4 -za99 -e25 -zq -otexan -d1 -fp3 -4r -mf -bt=dos&

# Compiled objects to include
OBJS=&
	dr_pcx.o&

# C rules
.c.o:
	set _CCARGS=$(CCFLAGS) $*.c
	$(CC) @_CCARGS

# Target operation
$(TARGET): $(OBJS)
	wlib $(TARGET) $(OBJS)

clean: .SYMBOLIC
	rm *.obj
	rm *.o
	rm *.lnk
	rm *.rsp
	rm *.err
	rm *.exe
