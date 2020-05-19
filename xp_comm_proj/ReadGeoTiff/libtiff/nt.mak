#		"@(#)$RCSfile: nt.mak,v $ $Revision: 1.2 $ $AVS$ $Date: 1997/12/25 01:29:32 $"
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
#	$Source: /cvs/repository/TidalWave/Utility/Modules/GeoTIFF/libtiff/nt.mak,v $
#	

!include ../../../include/xp_defs.mk

SRCDIR = $(XP_ROOT)\xp_comm_proj\ReadGeoTiff\libtiff
LLIB=$(XP_LIB_DIR)\libtiff34.a
ODIR=$(XP_LIB_DIR)\Utility\Modules\GeoTIFF\libtiff

LIBOFILES = \
            $(ODIR)\tif_aux.obj \
            $(ODIR)\tif_compress.obj \
            $(ODIR)\tif_dir.obj \
            $(ODIR)\tif_dirinfo.obj \
            $(ODIR)\tif_dirread.obj \
            $(ODIR)\tif_fax3.obj \
            $(ODIR)\tif_fax3sm.obj \
            $(ODIR)\tif_getimage.obj \
            $(ODIR)\tif_jpeg.obj \
            $(ODIR)\tif_lzw.obj \
            $(ODIR)\tif_open.obj \
            $(ODIR)\tif_pixarlog.obj \
            $(ODIR)\tif_predict.obj \
            $(ODIR)\tif_read.obj \
            $(ODIR)\tif_win32.obj \
            $(EMPTY)

default: $(LLIB)

clean:
	del $(LLIB) $(ODIR)\*.obj

!include $(XP_MACHRULE_PATH)

INCLUDES = \
           $(SRCDIR)\port.h \
           $(SRCDIR)\t4.h \
           $(SRCDIR)\tif_dir.h \
           $(SRCDIR)\tif_fax3.h \
           $(SRCDIR)\tif_predict.h \
           $(SRCDIR)\tiff.h \
           $(SRCDIR)\tiffcomp.h \
           $(SRCDIR)\tiffconf.h \
           $(SRCDIR)\tiffio.h \
           $(SRCDIR)\tiffiop.h \
           $(EMPTY)

$(ODIR)\tif_compress.obj: $(SRCDIR)\tif_compress.c $(INCLUDES)
$(ODIR)\tif_dir.obj: $(SRCDIR)\tif_dir.c $(INCLUDES)
$(ODIR)\tif_dirinfo.obj: $(SRCDIR)\tif_dirinfo.c $(INCLUDES)
$(ODIR)\tif_dirread.obj: $(SRCDIR)\tif_dirread.c $(INCLUDES)
$(ODIR)\tif_fax3.obj: $(SRCDIR)\tif_fax3.c $(INCLUDES)
$(ODIR)\tif_fax3sm.obj: $(SRCDIR)\tif_fax3sm.c $(INCLUDES)
$(ODIR)\tif_getimage.obj: $(SRCDIR)\tif_getimage.c $(INCLUDES)
$(ODIR)\tif_jpeg.obj: $(SRCDIR)\tif_jpeg.c $(INCLUDES)
$(ODIR)\tif_lzw.obj: $(SRCDIR)\tif_lzw.c $(INCLUDES)
$(ODIR)\tif_open.obj: $(SRCDIR)\tif_open.c $(INCLUDES)
$(ODIR)\tif_pixarlog.obj: $(SRCDIR)\tif_pixarlog.c $(INCLUDES)
$(ODIR)\tif_predict.obj: $(SRCDIR)\tif_predict.c $(INCLUDES)
$(ODIR)\tif_read.obj: $(SRCDIR)\tif_read.c $(INCLUDES)
$(ODIR)\tif_win32.obj: $(SRCDIR)\tif_win32.c $(INCLUDES)
