/* Standard C include files */
#include <stdio.h>
#include <string.h>

/* AVS/Express include files.
   Not needed in this case as they are included through gen.h. */
/*
#include <avs/util.h>
#include <avs/err.h>
#include <avs/om.h>
#include <avs/fld.h>
*/

/* AVS/Express filename manipulation functions. */
#include <avs/f_utils.h>

/* AVS/Express defintions of node data id parameter. */
/*    Definitions not currently used.  For some reason caused problems on Solaris!? */
/*#include <avs/gd_def.h>*/


#include "gen.h"     /* auto-generated header file. */
#include "image.h"   /* image library defines and prototypes. */




/* protos for funcs private to this file */
static int FUNCcheck_image_formats(const char *, int, int, int);
static int FUNCget_image_format (const char *);

static int FUNCread_volume (char *, int, int, int, int, OMobj_id);
static int FUNCget_image_funcs(void *);
static int FUNCset_volume_field (int, int, int, int, OMobj_id);
static int FUNCread_image_dims (char *, int *, int *);
static int FUNCread_image (char *, int, int, int, void *);
static int FUNCread_wrapup (const char *, void *);

static int FUNCget_image_format_name (int, char *);



/*## IAC HACK: DLLIB_SUFFIX appears to get defined when the full version of AVS/Express
 *             is built.  We define it explicitly here to get around the problem.
 *             Symbol is not used on Win32 so problem does not occur.
 */
#ifndef DLLIB_SUFFIX
#define DLLIB_SUFFIX "so"
#endif


/*## IAC: Defining this symbol does not currently work.  Persumably the image libraries
 *        are not compiled into Express by default.  The code to use static functions
 *        has been written but will need to be tested properly.
 */
/*#define NO_DL_LOAD*/


#ifndef NO_DL_LOAD

/* for dynamic, loaded at runtime, libs make and use an array of info strings
 * containing the library and function names: these get looked up
 */

/* indices into reader info tables below */
#define LIBNAME         0
#define OPEN_FUNC       1
#define CLOSE_FUNC      2
#define GETWIDTH_FUNC   3
#define GETHEIGHT_FUNC  4
#define GETARGB_FUNC    5

static const char *avs_info[] =
{
#ifdef MSDOS
  "libavsx",
#else
  "libavsx." DLLIB_SUFFIX,
#endif
  "avsOpen", "avsClose", "avsGetWidth", "avsGetHeight", "avsGetARGBImage"
};

static const char *bmp_info[] =
{
#ifdef MSDOS
  "libbmp",
#else
  "libbmp." DLLIB_SUFFIX,
#endif
  "bmpOpen", "bmpClose", "bmpGetWidth", "bmpGetHeight", "bmpGetARGBImage"
};

static const char *gif_info[] =
{
#ifdef MSDOS
  "libgif",
#else
  "libgif." DLLIB_SUFFIX,
#endif
  "GIFOpen", "GIFClose", "GIFGetWidth", "GIFGetHeight", "GIFGetARGBImage"
};

static const char *jpeg_info[] =
{
#ifdef MSDOS
  "libjpeg",
#else
  "libjpeg." DLLIB_SUFFIX,
#endif
  "jpegOpen", "jpegClose", "jpegGetWidth", "jpegGetHeight", "jpegGetARGBImage"
};

static const char *pbm_info[] =
{
#ifdef MSDOS
  "libpbm",
#else
  "libpbm." DLLIB_SUFFIX,
#endif
  "pbmOpen", "pbmClose", "pbmGetWidth", "pbmGetHeight", "pbmGetARGBImage"
};

static const char *sgiRGB_info[] =
{
#ifdef MSDOS
  "libsgiim",
#else
  "libsgiim." DLLIB_SUFFIX,
#endif
  "sgiRGBOpen", "sgiRGBClose", "sgiRGBGetWidth", "sgiRGBGetHeight", "sgiRGBGetARGBImage"
};

static const char *sunras_info[] =
{
#ifdef MSDOS
  "libsunrs",
#else
  "libsunrs." DLLIB_SUFFIX,
#endif
  "SunRasOpen", "SunRasClose", "SunRasGetWidth", "SunRasGetHeight", "SunRasGetARGBImage"
};

static const char *tiff_info[] =
{
#ifdef MSDOS
  "libtif",
#else
  "libtif." DLLIB_SUFFIX,
#endif
  "TIFFBegin", "TIFFEnd", "TIFFGetWidth", "TIFFGetHeight", "TIFFGetARGBImage"
};

#else

/* for bound-at-link-time ("NO_DL_LOAD") libraries, make and use a function
 * table as the interface to FUNCread_image()
 */

typedef struct _funcs_t
{
  void *(*Open)();
  void (*Close)();
  int  (*GetWidth)();
  int  (*GetHeight)();
  int  (*GetARGBImage)();
} funcs_t;

void *NULLfunc_voidp() { return((void *) NULL); }
void NULLfunc_void() { return; }
int NULLfunc_int() { return(0); }

void *avsOpen(), avsClose();
int avsGetWidth(), avsGetHeight(), avsGetARGBImage();
static funcs_t avs_funcs = { avsOpen, avsClose,
			     avsGetWidth, avsGetHeight, avsGetARGBImage };

void *bmpOpen(), bmpClose();
int bmpGetWidth(), bmpGetHeight(), bmpGetARGBImage();
static funcs_t bmp_funcs = { bmpOpen, bmpClose,
			     bmpGetWidth, bmpGetHeight, bmpGetARGBImage };

void *GIFOpen(), GIFClose();
int GIFGetWidth(), GIFGetHeight(), GIFGetARGBImage();
static funcs_t gif_funcs = { GIFOpen, GIFClose,
			     GIFGetWidth, GIFGetHeight, GIFGetARGBImage };

void *jpegOpen(), jpegClose();
int jpegGetWidth(), jpegGetHeight(), jpegGetARGBImage();
static funcs_t jpeg_funcs = { jpegOpen, jpegClose,
			      jpegGetWidth, jpegGetHeight, jpegGetARGBImage };

void *pbmOpen(), pbmClose();
int pbmGetWidth(), pbmGetHeight(), pbmGetARGBImage();
static funcs_t pbm_funcs = { pbmOpen, pbmClose,
			     pbmGetWidth, pbmGetHeight, pbmGetARGBImage };

void *sgiRGBOpen(), sgiRGBClose();
int sgiRGBGetWidth(), sgiRGBGetHeight(), sgiRGBGetARGBImage();
static funcs_t sgi_funcs = { sgiRGBOpen, sgiRGBClose,
			     sgiRGBGetWidth, sgiRGBGetHeight,
			     sgiRGBGetARGBImage };

void *SunRasOpen(), SunRasClose();
int SunRasGetWidth(), SunRasGetHeight(), SunRasGetARGBImage();
static funcs_t sun_funcs = { SunRasOpen, SunRasClose,
			     SunRasGetWidth, SunRasGetHeight,
			     SunRasGetARGBImage };

void *TIFFBegin(), TIFFEnd();
int TIFFGetWidth(), TIFFGetHeight(), TIFFGetARGBImage();
static funcs_t tiff_funcs = { TIFFBegin, TIFFEnd,
			      TIFFGetWidth, TIFFGetHeight, TIFFGetARGBImage };

#endif


typedef struct _read_map {
  int     dv_val;
#ifndef NO_DL_LOAD
  const char **info;
#else
  funcs_t *info;
#endif
} read_map;


static read_map FUNCread_map[] = {
  { DV_IMAGE_FORMAT_AVSX,		/* 0 */
#ifndef NO_DL_LOAD
    avs_info
#else
    &avs_funcs
#endif
  },
  { DV_IMAGE_FORMAT_BMP,		/* 1 */
#ifndef NO_DL_LOAD
    bmp_info
#else
    &bmp_funcs
#endif
  },
  { DV_IMAGE_FORMAT_GIF,		/* 2 */
#ifndef NO_DL_LOAD
    gif_info
#else
    &gif_funcs
#endif
  },
  { DV_IMAGE_FORMAT_JPEG,		/* 3 */
#ifndef NO_DL_LOAD
    jpeg_info
#else
    &jpeg_funcs
#endif
  },
  { DV_IMAGE_FORMAT_PBM,		/* 4 */
#ifndef NO_DL_LOAD
    pbm_info
#else
    &pbm_funcs
#endif
  },
  { DV_IMAGE_FORMAT_SGIRGB,		/* 5 */
#ifndef NO_DL_LOAD
    sgiRGB_info
#else
    &sgi_funcs
#endif
  },
  { DV_IMAGE_FORMAT_SUNRAS,		/* 6 */
#ifndef NO_DL_LOAD
    sunras_info
#else
    &sun_funcs
#endif
  },
  { DV_IMAGE_FORMAT_TIFF,		/* 7 */
#ifndef NO_DL_LOAD
    tiff_info
#else
    &tiff_funcs
#endif
  }
};




/*
 * Definition of macro that displays an error, frees the arrays and returns.
 */
#define ERR_RETURN(A) {\
  ERRerror("Build3D_2_Core", 0, ERR_ORIG, A);\
  if (filename!=NULL) free(filename);\
  return(0);\
}


/*
 * Definitions for types of output formats.
 */
#define NUM_OUT_FORMATS 7

#define  ARGB_OUT_FORMAT 0
#define ALPHA_OUT_FORMAT 1
#define   RED_OUT_FORMAT 2
#define GREEN_OUT_FORMAT 3
#define  BLUE_OUT_FORMAT 4
#define  LUMI_OUT_FORMAT 5
#define   RGB_OUT_FORMAT 6



/*----------------------------------------------------------------------
 * the public entry point (update method in bd3dmods.v)
 */
int Build3D_2_Core_update(OMobj_id elem_id, OMevent_mask event_mask, int seq_num)
{
  OMobj_id file_id;
  char *filename = NULL;
  int rb_val, /*from_format,*/ format, result;
  int start, end, out_format;


   /* Get filename specification string */
  file_id = OMfind_subobj(elem_id, OMstr_to_name("filename"), OM_OBJ_RD);
  if (OMis_null_obj(file_id) || OMget_str_val(file_id, &filename, 0) != OM_STAT_SUCCESS)
    return(0);

  if (strchr(filename, '%')==NULL)
    ERR_RETURN("Filename does not contain a format specifier");
    

   /* Get input file format parameter */

  if (OMget_name_int_val(elem_id, OMstr_to_name("format"), &rb_val) != OM_STAT_SUCCESS)
    rb_val = 0;

  /* map format UIradiobox val to internal def */
  if (rb_val < 0 || rb_val > sizeof(FUNCread_map) / sizeof(read_map))
    format = DV_IMAGE_FORMAT_UNKNOWN;
  else if (rb_val == 0)
    format = DV_IMAGE_FORMAT_AUTO;
  else
    format = FUNCread_map[rb_val-1].dv_val;


   /* Get start and end parameters */

  if (OMget_name_int_val(elem_id, OMstr_to_name("start"), &start) != OM_STAT_SUCCESS)
    ERR_RETURN("Image start index is not valid.");

  if (OMget_name_int_val(elem_id, OMstr_to_name("end"), &end) != OM_STAT_SUCCESS)
    ERR_RETURN("Image end index is not valid.");

  if (start>=end)
    ERR_RETURN("Starting image index must be greater than ending image index.");


   /* Get output data format parameter */

  if (OMget_name_int_val(elem_id, OMstr_to_name("out_format"), &rb_val) != OM_STAT_SUCCESS)
    rb_val = LUMI_OUT_FORMAT;

  /* map format UIradiobox val to internal def */
  if (rb_val < 0 || rb_val >= NUM_OUT_FORMATS)
    out_format = ARGB_OUT_FORMAT;
  else
    out_format = rb_val;


  /* Check all files to ensure that they exist and that they are all of the same
   * format.  If we haven't explicitly stated a format then store the returned format.
   */
  format = FUNCcheck_image_formats(filename, start, end, format);


  /* now go read the file of particular format */
  result = 0;
  switch (format)
  {
    case DV_IMAGE_FORMAT_AVSX:
    case DV_IMAGE_FORMAT_BMP:
    case DV_IMAGE_FORMAT_GIF:
    case DV_IMAGE_FORMAT_JPEG:
    case DV_IMAGE_FORMAT_PBM:
    case DV_IMAGE_FORMAT_SGIRGB:
    case DV_IMAGE_FORMAT_SUNRAS:
    case DV_IMAGE_FORMAT_TIFF:
    {
      result = FUNCread_volume(filename, format, start, end, out_format, elem_id);
      break;
    }

    case DV_IMAGE_FORMAT_NOFILE: break;

    case DV_IMAGE_FORMAT_UNKNOWN: break;

    case DV_IMAGE_FORMAT_MISMATCH: break;

    default:
      ERRerror("Build3D_2_Core", 0, ERR_ORIG, "File format is not supported");
      break;
  }

  if (filename)
    free(filename);

  return(result);
}


/*------------------------------------------------------------------------------
 * checks the image files to ensure that they exist and are all of the same type
 */
static int FUNCcheck_image_formats(const char *filename, int start, int end, int format)
{
  char filename_buf[AVS_PATH_MAX];
  int i, from_format;

  OMpush_status_range(0, 10);
  OMstatus_check(0, "Checking Image files", NULL);

   /* Loop through all specified files */
  for (i=start; i<=end; i++) {
    sprintf(filename_buf, filename, i);

     /* Get format from file */
    from_format = FUNCget_image_format(filename_buf);

     /* Exit early if file doesn't exist or can't be identified */
    if (from_format==DV_IMAGE_FORMAT_NOFILE) {
      ERRerror("Build3D_2_Core", 1, ERR_ORIG, "Cannot open file: %s", filename_buf);
      format = DV_IMAGE_FORMAT_NOFILE;
      break;
    }
    else if (from_format==DV_IMAGE_FORMAT_UNKNOWN) {
      ERRerror("Build3D_2_Core", 1, ERR_ORIG, "File is of unknown format: %s", filename_buf);
      format = DV_IMAGE_FORMAT_UNKNOWN;
      break;
    }

     /* If we are automatically selecting a format assume all files are of the type read from the first file. */
    if (format == DV_IMAGE_FORMAT_AUTO) {
      format = from_format;
    }
      /* else exit early if this file doesn't match the format of the previous files. */
    else if (format != from_format) {
      ERRerror("Build3D_2_Core", 1, ERR_ORIG, "File is of differing type: %s", filename_buf);
      format = DV_IMAGE_FORMAT_MISMATCH;
      break;
    }
  }

  OMstatus_check(100, "Checking Image files", NULL);
  OMpop_status_range();

  return(format);
}





/*----------------------------------------------------------------------
 * attempts to determine the format from the file itself
 */
static int FUNCget_image_format(const char *filename)
{
  FILE *fp;
  char *extension;
  unsigned char file_id[8];

  /* first open the file for existence check */
  if ((fp = (FILE *) FILEfopen(filename, SIO_R_BIN)) == NULL)
    return(DV_IMAGE_FORMAT_NOFILE);

  /* all others we try and id by their header info... */
  if ((fread(file_id, 8, 1, fp)) != 1)
  {
    /* not much we can do with < 8 bytes */
    fclose(fp);
    return(DV_IMAGE_FORMAT_UNKNOWN);
  }

  fclose(fp);

  /* try to id from the file first, then if the filename has a .x/X extension
   * guess that it's an AVS .x file, else call it unknown
   */
  if (!strncmp((char *) file_id, "GIF87a", 6) || !strncmp((char *) file_id, "GIF89a", 6))
    return(DV_IMAGE_FORMAT_GIF);
  else if (file_id[0] == 0xff && file_id[1] == 0xd8 && file_id[2] == 0xff)
    return(DV_IMAGE_FORMAT_JPEG);
  else if (file_id[0] == 'P' && file_id[1] >= '1' && file_id[1] <= '6')
    return(DV_IMAGE_FORMAT_PBM);
  else if ((file_id[0] == 0x01 && file_id[1] == 0xda) || (file_id[0] == 0xda && file_id[1] == 0x01))
    return(DV_IMAGE_FORMAT_SGIRGB);
  else if (file_id[0] == 0x59 && (file_id[1] & 0x7f) == 0x26 && file_id[2] == 0x6a && (file_id[3] & 0x7f) == 0x15)
    return(DV_IMAGE_FORMAT_SUNRAS);
  else if ((file_id[0] == 'M' && file_id[1] == 'M') || (file_id[0] == 'I' && file_id[1] == 'I'))
    return(DV_IMAGE_FORMAT_TIFF);

  else if (file_id[0] == '%' && file_id[1] == '!')
    return(DV_IMAGE_FORMAT_PS);
  else if (file_id[0] == 'B' && file_id[1] == 'M')
    return(DV_IMAGE_FORMAT_BMP);
  else if (file_id[0] == 0x52 && file_id[1] == 0xcc)
    return(DV_IMAGE_FORMAT_UTAH);
  else if (file_id[0] == 0x1f && file_id[1] == 0x9d)
    return(DV_IMAGE_FORMAT_Z);
  else if (file_id[0] == 0x1f && file_id[1] == 0x8b)
    return(DV_IMAGE_FORMAT_GZIP);

  else if ((extension = strrchr(filename, '.')) && (!strcmp(&extension[1], "x") || !strcmp(&extension[1], "X")))
    return(DV_IMAGE_FORMAT_AVSX);

  else
    return(DV_IMAGE_FORMAT_UNKNOWN);
}





/*------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/


/*
 * Data pointer used to cope with freeing the data more easily on error
 */
static void *node_data;

/*
 * Function pointers used to decouple accessing functions from whether
 * they are in a shared or static library.
 */
static void *(*funcOpen)() = NULL;
static void *(*funcClose)() = NULL;
static void *(*funcGetWidth)() = NULL;
static void *(*funcGetHeight)() = NULL;
static void *(*funcGetARGBImage)() = NULL;


/*
 * Handle for shared library if we need to use it.
 */
static void *handle = NULL;



/*----------------------------------------------------------------------
 * builds the specified images into a volume by reading each one
 * and placing it into a single slice of a volume field.
 */
static int FUNCread_volume(char* filename, int format, int start, int end, int out_format, OMobj_id elem_id)
{
  char filename_buf[AVS_PATH_MAX];
  char info[128];
  int i, /*curr,*/ width, height, num_slices, nnodes, image_size;
  OMobj_id out_id, info_id;

  unsigned char *node_data_b;
  float *node_data_f;

  num_slices = end - start + 1;  /* Calculate number of slices in volume */
  node_data = NULL;              /* Ensure old node data variable is cleared */


  /* find the functions that correspond to the format number */
  for (i = 0; i < sizeof(FUNCread_map) / sizeof(read_map); i++)
  {
    if (format == FUNCread_map[i].dv_val)
    {
      if (!FUNCget_image_funcs((void *)FUNCread_map[i].info))
        return(0);
      else
        break;
    }
  }


  /* get dims of first image */
  sprintf(filename_buf, filename, start);

  if (!FUNCread_image_dims(filename_buf, &width, &height))
    return(0);

  nnodes = width*height*num_slices;
  image_size = width*height;

  /* Allocate memory to store volume data. Amount needed depends upon selected output format. */
  if (out_format==ARGB_OUT_FORMAT) {
    node_data_b = (unsigned char *)ARRalloc(NULL, DTYPE_BYTE, 4*nnodes, NULL);
    node_data = node_data_b;
    image_size = image_size * 4;
  }
  else if (out_format==RGB_OUT_FORMAT) {
    node_data_f = (float *)ARRalloc(NULL, DTYPE_FLOAT, 3*nnodes, NULL);
    node_data = node_data_f;
    image_size = image_size * 3;
  }
  else if (out_format==LUMI_OUT_FORMAT) {
    node_data_f = (float *)ARRalloc(NULL, DTYPE_FLOAT, nnodes, NULL);
    node_data = node_data_f;
  }
  else {
    node_data_b = (unsigned char *)ARRalloc(NULL, DTYPE_BYTE, nnodes, NULL);
    node_data = node_data_b;
  }

  OMpush_status_range(10, 100);

  /* read images into volume field */
  for (i=start; i<=end; i++) {
    OMstatus_check((i-start)*100/num_slices, "Reading Image files", NULL);
    sprintf(filename_buf, filename, i);

    if ((out_format==LUMI_OUT_FORMAT) || (out_format==RGB_OUT_FORMAT)) {
      if (!FUNCread_image(filename_buf, width, height, out_format, node_data_f)) {
        OMpop_status_range();
        return(0);
      }
      node_data_f += image_size;
    }
    else {
      if (!FUNCread_image(filename_buf, width, height, out_format, node_data_b)) {
        OMpop_status_range();
        return(0);
      }
      node_data_b += image_size;
    }

  }

  OMstatus_check(100, "Reading Image files", NULL);
  OMpop_status_range();

  /* get volume data output reference */
  out_id = OMfind_subobj(elem_id, OMstr_to_name("out"), OM_OBJ_RW);

  /* set the output volume field */
  if (!FUNCset_volume_field(width, height, num_slices, out_format, out_id))
    return(0);


#ifndef NO_DL_LOAD
  /* close the library */
  if (handle)
    DL_CLOSE(handle);
#endif


  /* give some info about what was read */
  info_id = OMfind_subobj(elem_id, OMstr_to_name("info"), OM_OBJ_RW);

  FUNCget_image_format_name(format, info);
  sprintf(info, "%s %dx%dx%d", info, width, height, num_slices);

  OMset_str_val(info_id, info);

  return(1);
}




/*----------------------------------------------------------------------
 * given an access variable loads the appropriate shared library if
 * necessary and then sets the function pointers.
 * "NO_DL_LOAD" means we don't do the dynamic library/dll open and
 * lookup but just set the function pointers from an assumed-to-be
 * filled-in table
 */
static int FUNCget_image_funcs(void *access)
{
#ifndef NO_DL_LOAD
  char **info = (char **) access;
#else
  funcs_t *funcs = (funcs_t *) access;
#endif

  /* open the lib */
#ifndef NO_DL_LOAD

#ifdef sol1
  /* SunOS4 wants an absolute path and doesn't pay attention to LD_LIBRARY_PATH
   * it seems... get a path to the lib directory and glue it to front of
   * library name
   */
  {
    char pathbuf[AVS_PATH_MAX], *pathdir, *libname;

    pathdir = FILEmap_variables("$XP_PATH<0>/lib/$MACHINE", pathbuf);
    if ((strlen(pathdir) + strlen(info[LIBNAME]) + 2) >= AVS_PATH_MAX)
      libname = malloc(strlen(pathdir) + strlen(info[LIBNAME]) + 2);
    else
      libname = pathbuf;

    sprintf(libname, "%s/%s", pathdir, info[LIBNAME]);

    DL_OPEN(libname, DL_BIND, handle);

    if (libname != pathbuf)
      free(libname);
  }
#else
  if (info[LIBNAME] != NULL)
    DL_OPEN(info[LIBNAME], DL_BIND, handle);
  else
    return(FUNCread_wrapup("No library name to load?", NULL));
#endif

  if (handle == NULL)
  {
    char err[512];
#if defined(hpux) || defined(__hpux)
    const char *libpath = "SHLIB_PATH";
#else
#ifdef MSDOS
    const char *libpath = "PATH";
    DWORD last_err = GetLastError();
#else
    const char *libpath = "LD_LIBRARY_PATH";
#endif
#endif

    sprintf(err, "Cannot open library '%s'.  Most likely cause is an unset or incorrectly set '%s' environment variable: please see the System Prerequisites Document.  The system error message is:\n'%s'\n", info[LIBNAME], libpath, DL_ERROR());

#ifdef MSDOS
    sprintf(err, "%s and GetLastError number is %d", err, last_err);
#endif

    return(FUNCread_wrapup(err, NULL));
  }
#endif


  /* lookup the Image access functions */
#ifndef NO_DL_LOAD
  DL_LOOKUP(handle, info[OPEN_FUNC], void *(*)(), funcOpen);
  if (funcOpen == NULL)
    return(FUNCread_wrapup("Cannot find Open function in library", NULL));

  DL_LOOKUP(handle, info[CLOSE_FUNC], void *(*)(), funcClose);
  if (funcClose == NULL)
    return(FUNCread_wrapup("Cannot find Close function in library", NULL));

  DL_LOOKUP(handle, info[GETWIDTH_FUNC], void *(*)(), funcGetWidth);
  if (funcGetWidth == NULL)
    return(FUNCread_wrapup("Cannot find GetWidth function in library", NULL));

  DL_LOOKUP(handle, info[GETHEIGHT_FUNC], void *(*)(), funcGetHeight);
  if (funcGetHeight == NULL)
    return(FUNCread_wrapup("Cannot find GetHeight function in library", NULL));

  DL_LOOKUP(handle, info[GETARGB_FUNC], void *(*)(), funcGetARGBImage);
  if (funcGetARGBImage == NULL)
    return(FUNCread_wrapup("Cannot find GetARGBImage function in library", NULL));
#else
  funcOpen = (void *(*)()) funcs->Open;
  funcClose = (void *(*)()) funcs->Close;
  funcGetWidth = (void *(*)()) funcs->GetWidth;
  funcGetHeight = (void *(*)()) funcs->GetHeight;
  funcGetARGBImage = (void *(*)()) funcs->GetARGBImage;
#endif

  return(1);
}




/*------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------
 * composes a volume field
 */
static int FUNCset_volume_field(int w, int h, int s, int out_format, OMobj_id out)
{
  int dims[3], /*type,*/ size, nnodes;
  float *points;

  /* First set geometry parameters */

  dims[0] = w;  dims[1] = h;  dims[2] = s;

  nnodes = w*h*s;

  if (FLDset_nnodes (out, nnodes) != 1)
    return(FUNCread_wrapup("Error setting nnodes", NULL));
  if (FLDset_dims(out, dims) != 1)
    return(FUNCread_wrapup("Error setting dims", NULL));
  if ( (FLDget_points(out, &points, &size, OM_GET_ARRAY_WR) != 1) || (size!=6) )
    return(FUNCread_wrapup("Error getting points", NULL));

  points[0] = points[1] = points[2] = 0.0;
  points[3] = (float) (w - 1);
  points[4] = (float) (h - 1);
  points[5] = (float) (s - 1);

  ARRfree(points);


  /* reset node data by setting ncomp to 0. */
  if (FLDset_node_data_ncomp(out, 0) != 1)
    return(FUNCread_wrapup("Error resetting node data", NULL));

  /* Next set node data parameters. */
  if (FLDset_node_data_ncomp(out, 1) != 1)
    return(FUNCread_wrapup("Error setting node ncomps", NULL));

  if (out_format==ARGB_OUT_FORMAT)
  {
    /*if (FLDset_node_data_id(out, 0, GD_RGB_DATA_ID) != 1)
      return(FUNCread_wrapup("Error setting comp id", NULL));*/

    if (FLDset_node_data_veclen(out, 0, 4) != 1)
      return(FUNCread_wrapup("Error setting comp veclen", NULL));

    if (FLDset_node_data(out, 0, node_data, DTYPE_BYTE, nnodes*4, OM_SET_ARRAY_FREE) != 1)
      return(FUNCread_wrapup("Error setting node data array", NULL));
  }
  else if (out_format==RGB_OUT_FORMAT)
  {
    /*if (FLDset_node_data_id(out, 0, GD_COLOR_DATA_ID) != 1)
      return(FUNCread_wrapup("Error setting comp id", NULL));*/

    if (FLDset_node_data_veclen(out, 0, 3) != 1)
      return(FUNCread_wrapup("Error setting comp veclen", NULL));

    if (FLDset_node_data(out, 0, node_data, DTYPE_FLOAT, nnodes*3, OM_SET_ARRAY_FREE) != 1)
      return(FUNCread_wrapup("Error setting node data array", NULL));
  }
  else if (out_format==LUMI_OUT_FORMAT)
  {
    /*if (FLDset_node_data_id(out, 0, GD_UNDEFINED_DATA_ID) != 1)
      return(FUNCread_wrapup("Error setting comp id", NULL));*/

    if (FLDset_node_data_veclen(out, 0, 1) != 1)
      return(FUNCread_wrapup("Error setting comp veclen", NULL));

    if (FLDset_node_data(out, 0, node_data, DTYPE_FLOAT, nnodes, OM_SET_ARRAY_FREE) != 1)
      return(FUNCread_wrapup("Error setting node data array", NULL));
  }
  else
  {
    /*if (FLDset_node_data_id(out, 0, GD_UNDEFINED_DATA_ID) != 1)
      return(FUNCread_wrapup("Error setting comp id", NULL));*/

    if (FLDset_node_data_veclen(out, 0, 1) != 1)
      return(FUNCread_wrapup("Error setting comp veclen", NULL));

    if (FLDset_node_data(out, 0, node_data, DTYPE_BYTE, nnodes, OM_SET_ARRAY_FREE) != 1)
      return(FUNCread_wrapup("Error setting node data array", NULL));
  }

  return(1);
}



/*----------------------------------------------------------------------
 * gets width and height from specified image
 */
static int FUNCread_image_dims(char *filename, int *width, int *height)
{
  int w, h;
  void *priv = NULL;

  *width = *height = 0;
  if (!filename || strlen(filename) == 0)
    return(0);

  /* open the file */
  if (!(priv = (*funcOpen)(filename, DV_IMAGE_FILE_ACCESS_READ)))
    return(FUNCread_wrapup("Cannot open file", priv));

  /* get the width */
  (*funcGetWidth)(priv, &w);

  /* get the height */
  (*funcGetHeight)(priv, &h);

  /* store the dimensions */
  *width = w;  *height = h;

  /* close the file */
  (*funcClose)(priv);

  return(1);
}



/*----------------------------------------------------------------------
 * generic single image reader
 */
static int FUNCread_image(char *filename, int width, int height, int out_format, void* image_data)
{
  unsigned char *data = NULL;
  int w, h;
  void *priv = NULL;
  char err[512];

  if (!filename || strlen(filename) == 0)
    return(0);

  /* open the file */
  if (!(priv = (*funcOpen)(filename, DV_IMAGE_FILE_ACCESS_READ))) {
    sprintf(err, "Cannot open file: %s", filename);
    return(FUNCread_wrapup(err, priv));
  }

  /* get the width */
  (*funcGetWidth)(priv, &w);

  /* get the height */
  (*funcGetHeight)(priv, &h);

  if ((w!=width) || (h!=height)) {
    sprintf(err, "File is of differing size: %s", filename);
    return(FUNCread_wrapup(err, priv));
  }

  /* get the image data */
  if (!((*funcGetARGBImage)(priv, &data)))
    return(FUNCread_wrapup("Failure accessing ARGB data", priv));


  /* copy data to field */
  if (out_format==ARGB_OUT_FORMAT)
  {
    unsigned char *image_data_b = (unsigned char*)image_data;
    memcpy(image_data_b, data, w*h*4);
  }
  else if (out_format==RGB_OUT_FORMAT)
  {
    int i,j;
    float *image_data_f = (float*)image_data;

    for (i=0, j=0; i<w*h*3; i+=3, j+=4) {
      image_data_f[i+0] = (float)data[j+AVS_RED_BYTE] / 255.f;
      image_data_f[i+1] = (float)data[j+AVS_GREEN_BYTE] / 255.f;
      image_data_f[i+2] = (float)data[j+AVS_BLUE_BYTE] / 255.f;
    }

  }
  else if (out_format==LUMI_OUT_FORMAT)
  {
    int i,j;
    float *image_data_f = (float*)image_data;

    for (i=0, j=0; i<w*h; i++, j+=4) {
      image_data_f[i] = ( ((float)data[j+AVS_RED_BYTE]*0.299f) +
	                       ((float)data[j+AVS_GREEN_BYTE]*0.587f) +
	                       ((float)data[j+AVS_BLUE_BYTE]*0.114f) ) / 255.f;
    }

  }
  else
  {
    int i,j;
    unsigned char *image_data_b = (unsigned char*)image_data;

    switch(out_format) {
      case ALPHA_OUT_FORMAT: j = AVS_ALPHA_BYTE; break;
      case   RED_OUT_FORMAT: j = AVS_RED_BYTE;   break;
      case GREEN_OUT_FORMAT: j = AVS_GREEN_BYTE; break;
      case  BLUE_OUT_FORMAT: j = AVS_BLUE_BYTE;  break;
    }

    for (i=0; i<w*h; i++, j+=4) {
      image_data_b[i] = data[j];
    }
  }


  /* close the file */
  (*funcClose)(priv);

  return(1);
}



/*----------------------------------------------------------------------
 * what to do at the end of reading files or when we get a critical error
 */
static int FUNCread_wrapup(const char *msg, void *priv)
{
  /* close the file */
  if (funcClose && priv)
   (*funcClose)(priv);

  /* free the field data */
  if (node_data!=NULL)
    ARRfree(node_data);

#ifndef NO_DL_LOAD
  /* close the library */
  if (handle)
    DL_CLOSE(handle);
#endif

  /* output message and return 0 if error */
  if (msg) {
    ERRerror("Build3D_2_Core", 0, ERR_ORIG, msg);
    return(0);
  }
  else
    return(1);
}



/*----------------------------------------------------------------------
 * returns the ASCII equiv of an integer format
 */
static int FUNCget_image_format_name(int format, char *name)
{
  int result = 1;
  switch (format)
  {
    case DV_IMAGE_FORMAT_AVSX:
      sprintf(name, "%s", "AVSX");
      break;
    case DV_IMAGE_FORMAT_GIF:
      sprintf(name, "%s", "GIF");
      break;
    case DV_IMAGE_FORMAT_JPEG:
      sprintf(name, "%s", "JPEG");
      break;
    case DV_IMAGE_FORMAT_PBM:
      sprintf(name, "%s", "PBM");
      break;
    case DV_IMAGE_FORMAT_SUNRAS:
      sprintf(name, "%s", "Sun Raster");
      break;
    case DV_IMAGE_FORMAT_SGIRGB:
      sprintf(name, "%s", "SGI Image");
      break;
    case DV_IMAGE_FORMAT_TIFF:
      sprintf(name, "%s", "TIFF");
      break;
    case DV_IMAGE_FORMAT_PS:
      sprintf(name, "%s", "PostScript");
      break;
    case DV_IMAGE_FORMAT_BMP:
      sprintf(name, "%s", "BMP");
      break;
    case DV_IMAGE_FORMAT_UTAH:
      sprintf(name, "%s", "Utah RLE");
      break;
    case DV_IMAGE_FORMAT_Z:
      sprintf(name, "%s", "compressed");
      break;
    case DV_IMAGE_FORMAT_GZIP:
      sprintf(name, "%s", "gzip compressed");
      break;
    default:
      sprintf(name, "%s", "<unknown>");
      result = 0;
      break;
  }
  return(result);
}

