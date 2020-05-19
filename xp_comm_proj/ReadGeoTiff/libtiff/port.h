/*
 * Warning, this file was automatically created by the TIFF configure script
 * VERSION:	 v3.4033
 * DATE:	 Thu Dec 11 14:08:30 1997
 * TARGET:	 hppa1.1-hp-hpux10.20
 * CCOMPILER:	 /usr/bin/cc
 */
#ifndef _PORT_
#define _PORT_ 1
#ifdef __cplusplus
extern "C" {
#endif
#include <sys/types.h>
#define HOST_FILLORDER FILLORDER_MSB2LSB
#define HOST_BIGENDIAN	1
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#ifndef DBLPARAM_T
#define DBLPARAM_T
#ifdef applec
typedef extended dblparam_t;
#else
typedef double dblparam_t;
#endif
#endif
#define	INLINE
#define GLOBALDATA(TYPE,NAME)	extern TYPE NAME
#ifdef __cplusplus
}
#endif
#endif
