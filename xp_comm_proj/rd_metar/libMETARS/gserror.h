//
//  Include file for the GIS error information.
//

#ifndef XP_GIS_ERROR_H
#define XP_GIS_ERROR_H

enum XP_GIS_ErrorCode_e {XP_GIS_OK               =  1,
                         XP_GIS_ERROR            =  2,
                         XP_GIS_OPEN_ERROR       =  3,
                         XP_GIS_READ_ERROR       =  4,
                         XP_GIS_WRITE_ERROR      =  5,
                         XP_GIS_IO_ERROR         =  6,
                         XP_GIS_BAD_MAGIC_NUMBER =  7,
                         XP_GIS_BAD_VERSION      =  8,
                         XP_GIS_BAD_SHAPE_TYPE   =  9,
                         XP_GIS_BAD_BOUNDING_BOX = 10,
                         XP_GIS_SEEK_ERROR       = 11,
                         XP_GIS_EOF              = 12,
                         XP_GIS_NOT_IMPLEMENTED  = 13,
                         XP_GIS_NOT_OPEN         = 14,
                         XP_GIS_ALLOC_ERROR      = 15,
                         XP_GIS_BAD_VALUE        = 16,
                         XP_GIS_BAD_CHECKSUM     = 17,
                         XP_GIS_NO_REAL_ROOTS    = 18};


#endif   // XP_GIS_ERROR_H
