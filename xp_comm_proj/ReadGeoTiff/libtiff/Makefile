#	"@(#)$RCSfile: Makefile,v $ $Revision: 1.1 $ $AVS$ $Date: 1997/12/24 23:58:54 $"
#			Copyright (c) 1994 by
#			Advanced Visual Systems Inc.
#			All Rights Reserved
#	
#	This software comprises unpublished confidential information of
#	Advanced Visual Systems Inc. and may not be used, copied or made
#	available to anyone, except in accordance with the license
#	under which it is furnished.
#	
#	This file is under sccs control at AVS in:
# $Source: /cvs/repository/TidalWave/Utility/Modules/GeoTIFF/libtiff/Makefile,v $
#	
# include standard variable defs
include ../../../include/xp_defs.mk

SRCDIR=$(XP_ROOT)/xp_comm_proj/ReadGeoTiff/libtiff

LLIB=$(XP_LIB_DIR)/libtiff34.a
DLLIB=$(XP_LIB_DIR)/libtiff34

TIFFINCDIR= .
# PROJ_CFLAGS = $(CFLAGS) -I$(TIFFINCDIR)

# Please add files in alphabetical order
LIBOFILES= \
    $(LLIB)(tif_aux.o) \
    $(LLIB)(tif_compress.o) \
    $(LLIB)(tif_dir.o) \
    $(LLIB)(tif_dirinfo.o) \
    $(LLIB)(tif_dirread.o) \
    $(LLIB)(tif_fax3.o) \
    $(LLIB)(tif_fax3sm.o) \
    $(LLIB)(tif_getimage.o) \
    $(LLIB)(tif_jpeg.o) \
    $(LLIB)(tif_lzw.o) \
    $(LLIB)(tif_open.o) \
    $(LLIB)(tif_pixarlog.o) \
    $(LLIB)(tif_predict.o) \
    $(LLIB)(tif_read.o) \
    $(LLIB)(tif_unix.o) \
    $(EMPTY)

default: $(LLIB) $(DLLIBNAME)

include $(XP_MACHRULE_PATH)

clean::
	rm -f $(LLIB) $(DLLIBNAME)
