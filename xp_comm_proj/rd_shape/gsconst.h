//
//  Include file for common constants used by the geospatial software.
//

#ifndef XP_GIS_CONSTANTS_H
#define XP_GIS_CONSTANTS_H

//
//  MIN and MAX are not defined on all systems.  If they are not
//  defined, define them here.
//

#ifndef MIN
#define MIN(v1,v2) (((v1) < (v2)) ? (v1) : (v2))
#endif
#ifndef MAX
#define MAX(v1,v2) (((v1) > (v2)) ? (v1) : (v2))
#endif

//
//  We need true and false.  Set up some constants that give us the standard
//  C/C++ values.
//

#define XP_GIS_FALSE 0
#define XP_GIS_TRUE  1

//
//  Some machines do not have a max path defined.  If they don't define it.
//

#ifndef _MAX_PATH
#define _MAX_PATH 132
#endif

#endif   // XP_GIS_CONSTANTS_H
