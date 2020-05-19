#ifndef IMAGE_H
#define IMAGE_H 1

/* file formats */
#define DV_IMAGE_FORMAT_NOFILE  -1
#define DV_IMAGE_FORMAT_UNKNOWN -2
#define DV_IMAGE_FORMAT_MISMATCH -3  /* ## IAC: Added to cope with mismatched sets of image files */

  /* *must* match the UI radiobox value order... */
#define DV_IMAGE_FORMAT_AUTO    0

#define DV_IMAGE_FORMAT_AVSX    1
#define DV_IMAGE_FORMAT_BMP     2  /* ## IAC: Modified so that values are in the same order as UI radiobox */
#define DV_IMAGE_FORMAT_GIF     3
#define DV_IMAGE_FORMAT_JPEG    4
#define DV_IMAGE_FORMAT_PBM     5
#define DV_IMAGE_FORMAT_SGIRGB  6
#define DV_IMAGE_FORMAT_SUNRAS  7
#define DV_IMAGE_FORMAT_TIFF    8

#define DV_IMAGE_FORMAT_PS      9
#define DV_IMAGE_FORMAT_UTAH   10
#define DV_IMAGE_FORMAT_Z      11
#define DV_IMAGE_FORMAT_GZIP   12

/* file access types (bitfield) */
#define DV_IMAGE_FILE_ACCESS_UNSET      0
#define DV_IMAGE_FILE_ACCESS_READ       1
#define DV_IMAGE_FILE_ACCESS_WRITE      2
#define DV_IMAGE_FILE_ACCESS_READ_WRITE 3

/* file types (for output)
 * these *must* match the UI radiobox values
 */
#define DV_IMAGE_FILE_TYPE_BINARY 0
#define DV_IMAGE_FILE_TYPE_ASCII  1
#define DV_IMAGE_FILE_TYPE_MAX    1

/* depth (bits per pixel) for output
 * *must* match the UI radiobox value order...
 */
#define DV_IMAGE_DEPTH_NA  0	/* not applicable */
#define DV_IMAGE_DEPTH_1   1
#define DV_IMAGE_DEPTH_4   2
#define DV_IMAGE_DEPTH_8   3
#define DV_IMAGE_DEPTH_16  4
#define DV_IMAGE_DEPTH_24  5
#define DV_IMAGE_DEPTH_32  6
#define DV_IMAGE_DEPTH_MAX 6

/* color type (related to depth but not 1:1) (for output)
 * *must* match the UI radiobox value order...
 */
#define DV_IMAGE_COLORTYPE_RGB  0
#define DV_IMAGE_COLORTYPE_GREY 1
#define DV_IMAGE_COLORTYPE_BW   2
#define DV_IMAGE_COLORTYPE_MAX  2

/* compression method (for output)
 * *must* match the UI radiobox value order...
 */
#define DV_IMAGE_COMPRESS_NONE   0
#define DV_IMAGE_COMPRESS_RLE    1
#define DV_IMAGE_COMPRESS_LZW    2
#define DV_IMAGE_COMPRESS_JPEG   3
#define DV_IMAGE_COMPRESS_CCITT3 4
#define DV_IMAGE_COMPRESS_CCITT4 5
#define DV_IMAGE_COMPRESS_MAX    5

/* RGB to 8 bit color reduction (a.k.a. quantization) method (for output)
 * *must* match the UI radiobox value order
 */
#define DV_IMAGE_QUANTIZE_NA  0	/* not applicable */
#define DV_IMAGE_QUANTIZE_ES  1	/* exhaustive search */
#define DV_IMAGE_QUANTIZE_PH  2	/* Heckbert Median Cut */
#define DV_IMAGE_QUANTIZE_POP 3	/* histogram popularity */
#define DV_IMAGE_QUANTIZE_FS  4	/* Floyd-Steinberg dither */
#define DV_IMAGE_QUANTIZE_OCT 5 /* octree */
#define DV_IMAGE_QUANTIZE_VAR 6 /* variance-based */
#define DV_IMAGE_QUANTIZE_LK  7 /* local k-means */
#define DV_IMAGE_QUANTIZE_NEU 8	/* neural net */
#define DV_IMAGE_QUANTIZE_MAX 8

/* protos for interface funcs */
void *avsOpen PTARGS((char *, int));
void avsClose PTARGS((void *));
int avsGetWidth PTARGS((void *, int *));
int avsGetHeight PTARGS((void *, int *));
int avsGetARGBImage PTARGS((void *, unsigned char **));
int avsSetWidth PTARGS((void *, int));
int avsSetHeight PTARGS((void *, int));
int avsSetFileType PTARGS((void *, int));
int avsSetDepth PTARGS((void *, int));
int avsSetColorType PTARGS((void *, int));
int avsSetCompressionType PTARGS((void *, int, int));
int avsSetReductionType PTARGS((void *, int));
int avsSetARGBImage PTARGS((void *, unsigned char *, int));

void *GIFOpen PTARGS((char *, int));
void GIFClose PTARGS((void *));
int GIFGetWidth PTARGS((void *, int *));
int GIFGetHeight PTARGS((void *, int *));
int GIFGetARGBImage PTARGS((void *, unsigned char **));
int GIFSetWidth PTARGS((void *, int));
int GIFSetHeight PTARGS((void *, int));
int GIFSetFileType PTARGS((void *, int));
int GIFSetDepth PTARGS((void *, int));
int GIFSetColorType PTARGS((void *, int));
int GIFSetCompressionType PTARGS((void *, int, int));
int GIFSetReductionType PTARGS((void *, int));
int GIFSetARGBImage PTARGS((void *, unsigned char *, int));

void *bmpOpen PTARGS((char *, int));
void bmpClose PTARGS((void *));
int bmpGetWidth PTARGS((void *, int *));
int bmpGetHeight PTARGS((void *, int *));
int bmpGetARGBImage PTARGS((void *, unsigned char **));
int bmpSetWidth PTARGS((void *, int));
int bmpSetHeight PTARGS((void *, int));
int bmpSetFileType PTARGS((void *, int));
int bmpSetDepth PTARGS((void *, int));
int bmpSetColorType PTARGS((void *, int));
int bmpSetCompressionType PTARGS((void *, int, int));
int bmpSetReductionType PTARGS((void *, int));
int bmpSetARGBImage PTARGS((void *, unsigned char *, int));

void *jpegOpen PTARGS((char *, int));
void jpegClose PTARGS((void *));
int jpegGetWidth PTARGS((void *, int *));
int jpegGetHeight PTARGS((void *, int *));
int jpegGetARGBImage PTARGS((void *, unsigned char **));
int jpegSetWidth PTARGS((void *, int));
int jpegSetHeight PTARGS((void *, int));
int jpegSetFileType PTARGS((void *, int));
int jpegSetDepth PTARGS((void *, int));
int jpegSetColorType PTARGS((void *, int));
int jpegSetCompressionType PTARGS((void *, int, int));
int jpegSetReductionType PTARGS((void *, int));
int jpegSetARGBImage PTARGS((void *, unsigned char *, int));

void *pbmOpen PTARGS((char *, int));
void pbmClose PTARGS((void *));
int pbmGetWidth PTARGS((void *, int *));
int pbmGetHeight PTARGS((void *, int *));
int pbmGetARGBImage PTARGS((void *, unsigned char **));
int pbmSetWidth PTARGS((void *, int));
int pbmSetHeight PTARGS((void *, int));
int pbmSetFileType PTARGS((void *, int));
int pbmSetDepth PTARGS((void *, int));
int pbmSetColorType PTARGS((void *, int));
int pbmSetCompressionType PTARGS((void *, int, int));
int pbmSetReductionType PTARGS((void *, int));
int pbmSetARGBImage PTARGS((void *, unsigned char *, int));

void *sgiRGBOpen PTARGS((char *, int));
void sgiRGBClose PTARGS((void *));
int sgiRGBGetWidth PTARGS((void *, int *));
int sgiRGBGetHeight PTARGS((void *, int *));
int sgiRGBGetARGBImage PTARGS((void *, unsigned char **));
int sgiRGBSetWidth PTARGS((void *, int));
int sgiRGBSetHeight PTARGS((void *, int));
int sgiRGBSetFileType PTARGS((void *, int));
int sgiRGBSetDepth PTARGS((void *, int));
int sgiRGBSetColorType PTARGS((void *, int));
int sgiRGBSetCompressionType PTARGS((void *, int, int));
int sgiRGBSetReductionType PTARGS((void *, int));
int sgiRGBSetARGBImage PTARGS((void *, unsigned char *, int));

void *SunRasOpen PTARGS((char *, int));
void SunRasClose PTARGS((void *));
int SunRasGetWidth PTARGS((void *, int *));
int SunRasGetHeight PTARGS((void *, int *));
int SunRasGetARGBImage PTARGS((void *, unsigned char **));
int SunRasSetWidth PTARGS((void *, int));
int SunRasSetHeight PTARGS((void *, int));
int SunRasSetFileType PTARGS((void *, int));
int SunRasSetDepth PTARGS((void *, int));
int SunRasSetColorType PTARGS((void *, int));
int SunRasSetCompressionType PTARGS((void *, int, int));
int SunRasSetReductionType PTARGS((void *, int));
int SunRasSetARGBImage PTARGS((void *, unsigned char *, int));

void *TIFFBegin PTARGS((char *, int));
void TIFFEnd PTARGS((void *));
int TIFFGetWidth PTARGS((void *, int *));
int TIFFGetHeight PTARGS((void *, int *));
int TIFFGetARGBImage PTARGS((void *, unsigned char **));
int TIFFSetWidth PTARGS((void *, int));
int TIFFSetHeight PTARGS((void *, int));
int TIFFSetFileType PTARGS((void *, int));
int TIFFSetDepth PTARGS((void *, int));
int TIFFSetColorType PTARGS((void *, int));
int TIFFSetCompressionType PTARGS((void *, int, int));
int TIFFSetReductionType PTARGS((void *, int));
int TIFFSetARGBImage PTARGS((void *, unsigned char *, int));

/* public utilities */
int UTILreduce_32to8 PTARGS((unsigned char *, int, unsigned char *,
			     int, int, int, int *,
			     unsigned char *,
			     unsigned char *,
			     unsigned char *));

#endif
