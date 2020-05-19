#Makefile for version handling (SegmentationEditor) for Windows
#Tobias Schiebeck, 19/03/2004

!include ../../include/xp_defs.mk

default: version.v
		
INCLUDE_DIR = -I$(XP_INC_OPTS) -Iinclude

!include $(XP_MACHRULE_PATH)

version.v: xpversion.exe
	xpversion.exe
	del xpversion.exe
	 
xpversion.exe: version.c
	$(CC) $(CXX_OPTS) version.c $(INCLUDE_DIR) -Fe$*.exe
