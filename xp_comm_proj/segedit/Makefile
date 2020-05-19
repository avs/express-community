#Makefile for version handling (Segmentation Editor)
#Tobias Schiebeck, 19/03/2004

include ../../include/xp_defs.mk

default: version.v

INCLUDE_DIR = -Iinclude $(XP_INC_OPTS)

include $(XP_MACHRULE_PATH)

version.v: xpversion
	./xpversion; rm xpversion
	
xpversion: version.c
	$(CC) $(CXX_OPTS) $(MACH_CFLAGS) version.c $(INCLUDE_DIR)  -o$@
