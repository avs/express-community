/**********************************************************************
 *
 *  geo_tiffp.h - Private interface for TIFF tag parsing.
 *
 *   Written by: Niles D. Ritter
 *
 *   This interface file encapsulates the interface to external TIFF
 *   file-io routines and definitions. The current configuration
 *   assumes that the "libtiff" module is used, but if you have your
 *   own TIFF reader, you may replace the definitions with your own
 *   here, and replace the implementations in geo_tiffp.c. No other
 *   modules have any explicit dependence on external TIFF modules.
 *
 *  Revision History;
 *
 *    20 June, 1995      Niles D. Ritter         New
 *    6 July,  1995      Niles D. Ritter         Fix prototypes
 *
 **********************************************************************/

#ifndef __geo_tiffp_h_
#define __geo_tiffp_h_

/**********************************************************************
 *
 *                        Private includes
 *
 *   If you are not using libtiff and XTIFF, replace this include file
 *    with the appropriate one for your own TIFF parsing routines.
 *
 *   Revision History
 * 
 *      19 September 1995   ndr    Demoted Intergraph trans matrix.
 *
 **********************************************************************/
#include <stdlib.h> /* for size_t */
#include "xtiffiop.h"

/*
 * dblparam_t is the type that a double precision
 * floating point value will have on the parameter
 * stack (when coerced by the compiler). You shouldn't
 * have to change this.
 */
#ifndef DBLPARAM_T
#ifdef applec
typedef extended dblparam_t;
#else
typedef double dblparam_t;
#endif
#endif

/**********************************************************************
 *
 *                        Private defines
 *
 *   If you are not using "libtiff"/LIBXTIFF, replace these definitions
 *   with the appropriate definitions to access the geo-tags
 *
 **********************************************************************/
 
typedef unsigned short pinfo_t;    /* SHORT ProjectionInfo tag type */
typedef TIFF    tiff_t;            /* TIFF file descriptor          */
typedef tdata_t  gdata_t;          /* pointer to data */
typedef tsize_t  gsize_t;          /* data allocation size */
 
#define GTIFF_GEOKEYDIRECTORY   TIFFTAG_GEOKEYDIRECTORY /* from xtiffio.h */
#define GTIFF_DOUBLEPARAMS      TIFFTAG_GEODOUBLEPARAMS
#define GTIFF_ASCIIPARAMS       TIFFTAG_GEOASCIIPARAMS
#define GTIFF_PIXELSCALE        TIFFTAG_GEOPIXELSCALE
#define GTIFF_TRANSMATRIX       TIFFTAG_GEOTRANSMATRIX
#define GTIFF_INTERGRAPH_MATRIX TIFFTAG_INTERGRAPH_MATRIX
#define GTIFF_TIEPOINTS         TIFFTAG_GEOTIEPOINTS
#define GTIFF_LOCAL          0

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * Method function pointer types
 */
typedef int        (*GTGetFunction) (tiff_t *tif, pinfo_t tag, int *count, void *value );
typedef int        (*GTSetFunction) (tiff_t *tif, pinfo_t tag, int  count, void *value );
typedef tagtype_t  (*GTTypeFunction)  (tiff_t *tif, pinfo_t tag);
typedef struct     _TIFFMethod {
	GTGetFunction get;
	GTSetFunction set;
	GTTypeFunction type;
} TIFFMethod;

/**********************************************************************
 *
 *               Protected Function Declarations  
 *
 *   These routines are exposed implementations, and should not
 *   be used by external GEOTIFF client programs.
 *
 **********************************************************************/

extern gsize_t _gtiff_size[]; /* TIFF data sizes */
extern void _GTIFSetDefaultTIFF(TIFFMethod *method);
extern gdata_t _GTIFcalloc(gsize_t);
extern void _GTIFFree(gdata_t data);
extern void _GTIFmemcpy(gdata_t out,gdata_t in,gsize_t size);

#if defined(__cplusplus)
} 
#endif


/******************************************************************/
/******************From geo_keyp.h*********************************/

/*
 * This structure contains the internal program
 * representation of the key entry.
 */
struct GeoKey {
	int       gk_key;    /* GeoKey ID        */
	size_t    gk_size;   /* data byte size   */
	tagtype_t gk_type;   /* TIFF data type   */
	long      gk_count;  /* number of values */
	char*     gk_data;   /* pointer to data, or value */
};
typedef struct GeoKey GeoKey;

/*
 *  This structure represents the file-organization of
 *  the key entry. Note that it assumes that short entries
 *  are aligned along 2-byte boundaries.
 */
struct KeyEntry {
	pinfo_t ent_key;        /* GeoKey ID            */
	pinfo_t ent_location;   /* TIFF Tag ID or 0     */
	pinfo_t ent_count;      /* GeoKey value count   */
	pinfo_t ent_val_offset; /* value or tag offset  */
};
typedef struct KeyEntry KeyEntry;

/*
 * This is the header of the CoordSystemInfoTag. The 'Version'
 *  will only change if the CoorSystemInfoTag structure changes;
 *  The Major Revision will be incremented whenever a new set of
 *  Keys is added or changed, while the Minor revision will be
 *  incremented when only the set of Key-values is increased.
 */
struct KeyHeader{
	pinfo_t hdr_version;      /* GeoTIFF Version          */
	pinfo_t hdr_rev_major;    /* GeoKey Major Revision #  */
	pinfo_t hdr_rev_minor;    /* GeoKey Minor Revision #  */
	pinfo_t hdr_num_keys;     /* Number of GeoKeys        */
};
typedef struct KeyHeader KeyHeader;


struct gtiff {
   tiff_t*    gt_tif;      /* TIFF file descriptor  */
   TIFFMethod gt_methods;  /* TIFF i/o methods      */
   int        gt_flags;    /* file flags            */
   
   pinfo_t    gt_version;  /* GeoTIFF Version       */
   pinfo_t    gt_rev_major;/* GeoKey Key Revision   */
   pinfo_t    gt_rev_minor;/* GeoKey Code Revision  */
   
   int        gt_num_keys; /* number of keys        */
   GeoKey*    gt_keys;     /* array of keys         */
   int*       gt_keyindex; /* index of a key, if set*/
   int        gt_keymin;   /* smallest key set      */
   int        gt_keymax;   /* largest key set       */
   
   pinfo_t*   gt_short;    /* array of SHORT vals   */
   double*    gt_double;   /* array of DOUBLE vals  */
   char*      gt_ascii;    /* array of ASCII string */
   int        gt_nshorts;  /* number of SHORT vals  */
   int        gt_ndoubles; /* number of DOUBLE vals */
   int        gt_nascii;   /* number of ASCII chars */
   
};  

typedef enum {
	FLAG_FILE_OPEN=1,
	FLAG_FILE_MODIFIED=2
} gtiff_flags;

#define MAX_KEYINDEX 65535   /* largest possible key    */
#define MAX_KEYS 100         /* maximum keys in a file  */
#define MAX_VALUES 1000      /* maximum values in a tag */


#endif /* __geo_tiffp_h_ */
