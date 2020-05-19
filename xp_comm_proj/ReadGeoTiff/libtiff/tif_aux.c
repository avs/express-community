/* $Header: /cvs/repository/TidalWave/Utility/Modules/GeoTIFF/libtiff/tif_aux.c,v 1.1 1997/12/25 00:01:04 jmethot Exp $ */

/*
 * Copyright (c) 1991-1996 Sam Leffler
 * Copyright (c) 1991-1996 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Sam Leffler and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Sam Leffler and Silicon Graphics.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 * IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

/*
 * TIFF Library.
 *
 * Auxiliary Support Routines.
 */
#include "tiffiop.h"
#include <ctype.h>

#ifdef MSDOS
#define O_RDONLY _O_RDONLY
#endif

#ifdef COLORIMETRY_SUPPORT
#include <math.h>

static void
TIFFDefaultTransferFunction(TIFFDirectory* td)
{
	uint16 **tf = td->td_transferfunction;
	long i, n = 1<<td->td_bitspersample;

	tf[0] = (uint16 *)_TIFFmalloc(n * sizeof (uint16));
	tf[0][0] = 0;
	for (i = 1; i < n; i++) {
		double t = (double)i/((double) n-1.);
		tf[0][i] = (uint16)floor(65535.*pow(t, 2.2) + .5);
	}
	if (td->td_samplesperpixel - td->td_extrasamples > 1) {
		tf[1] = (uint16 *)_TIFFmalloc(n * sizeof (uint16));
		_TIFFmemcpy(tf[1], tf[0], n * sizeof (uint16));
		tf[2] = (uint16 *)_TIFFmalloc(n * sizeof (uint16));
		_TIFFmemcpy(tf[2], tf[0], n * sizeof (uint16));
	}
}

static void
TIFFDefaultRefBlackWhite(TIFFDirectory* td)
{
	int i;

	td->td_refblackwhite = (float *)_TIFFmalloc(6*sizeof (float));
	for (i = 0; i < 3; i++) {
	    td->td_refblackwhite[2*i+0] = 0;
	    td->td_refblackwhite[2*i+1] = (float)((1L<<td->td_bitspersample)-1L);
	}
}
#endif

/*
 * Like TIFFGetField, but return any default
 * value if the tag is not present in the directory.
 *
 * NB:	We use the value in the directory, rather than
 *	explcit values so that defaults exist only one
 *	place in the library -- in TIFFDefaultDirectory.
 */
int
TIFFVGetFieldDefaulted(TIFF* tif, ttag_t tag, va_list ap)
{
	TIFFDirectory *td = &tif->tif_dir;

	if (TIFFVGetField(tif, tag, ap))
		return (1);
	switch (tag) {
	case TIFFTAG_SUBFILETYPE:
		*va_arg(ap, uint32 *) = td->td_subfiletype;
		return (1);
	case TIFFTAG_BITSPERSAMPLE:
		*va_arg(ap, uint16 *) = td->td_bitspersample;
		return (1);
	case TIFFTAG_THRESHHOLDING:
		*va_arg(ap, uint16 *) = td->td_threshholding;
		return (1);
	case TIFFTAG_FILLORDER:
		*va_arg(ap, uint16 *) = td->td_fillorder;
		return (1);
	case TIFFTAG_ORIENTATION:
		*va_arg(ap, uint16 *) = td->td_orientation;
		return (1);
	case TIFFTAG_SAMPLESPERPIXEL:
		*va_arg(ap, uint16 *) = td->td_samplesperpixel;
		return (1);
	case TIFFTAG_ROWSPERSTRIP:
		*va_arg(ap, uint32 *) = td->td_rowsperstrip;
		return (1);
	case TIFFTAG_MINSAMPLEVALUE:
		*va_arg(ap, uint16 *) = td->td_minsamplevalue;
		return (1);
	case TIFFTAG_MAXSAMPLEVALUE:
		*va_arg(ap, uint16 *) = td->td_maxsamplevalue;
		return (1);
	case TIFFTAG_PLANARCONFIG:
		*va_arg(ap, uint16 *) = td->td_planarconfig;
		return (1);
	case TIFFTAG_RESOLUTIONUNIT:
		*va_arg(ap, uint16 *) = td->td_resolutionunit;
		return (1);
#ifdef CMYK_SUPPORT
	case TIFFTAG_DOTRANGE:
		*va_arg(ap, uint16 *) = 0;
		*va_arg(ap, uint16 *) = (1<<td->td_bitspersample)-1;
		return (1);
	case TIFFTAG_INKSET:
		*va_arg(ap, uint16 *) = td->td_inkset;
		return (1);
#endif
	case TIFFTAG_EXTRASAMPLES:
		*va_arg(ap, uint16 *) = td->td_extrasamples;
		*va_arg(ap, uint16 **) = td->td_sampleinfo;
		return (1);
	case TIFFTAG_MATTEING:
		*va_arg(ap, uint16 *) =
		    (td->td_extrasamples == 1 &&
		     td->td_sampleinfo[0] == EXTRASAMPLE_ASSOCALPHA);
		return (1);
	case TIFFTAG_TILEDEPTH:
		*va_arg(ap, uint32 *) = td->td_tiledepth;
		return (1);
	case TIFFTAG_DATATYPE:
		*va_arg(ap, uint16 *) = td->td_sampleformat-1;
		return (1);
	case TIFFTAG_IMAGEDEPTH:
		*va_arg(ap, uint32 *) = td->td_imagedepth;
		return (1);
#ifdef YCBCR_SUPPORT
	case TIFFTAG_YCBCRCOEFFICIENTS:
		if (!td->td_ycbcrcoeffs) {
			td->td_ycbcrcoeffs = (float *)
			    _TIFFmalloc(3*sizeof (float));
			/* defaults are from CCIR Recommendation 601-1 */
			td->td_ycbcrcoeffs[0] = 0.299f;
			td->td_ycbcrcoeffs[1] = 0.587f;
			td->td_ycbcrcoeffs[2] = 0.114f;
		}
		*va_arg(ap, float **) = td->td_ycbcrcoeffs;
		return (1);
	case TIFFTAG_YCBCRSUBSAMPLING:
		*va_arg(ap, uint16 *) = td->td_ycbcrsubsampling[0];
		*va_arg(ap, uint16 *) = td->td_ycbcrsubsampling[1];
		return (1);
	case TIFFTAG_YCBCRPOSITIONING:
		*va_arg(ap, uint16 *) = td->td_ycbcrpositioning;
		return (1);
#endif
#ifdef COLORIMETRY_SUPPORT
	case TIFFTAG_TRANSFERFUNCTION:
		if (!td->td_transferfunction[0])
			TIFFDefaultTransferFunction(td);
		*va_arg(ap, uint16 **) = td->td_transferfunction[0];
		if (td->td_samplesperpixel - td->td_extrasamples > 1) {
			*va_arg(ap, uint16 **) = td->td_transferfunction[1];
			*va_arg(ap, uint16 **) = td->td_transferfunction[2];
		}
		return (1);
	case TIFFTAG_REFERENCEBLACKWHITE:
		if (!td->td_refblackwhite)
			TIFFDefaultRefBlackWhite(td);
		*va_arg(ap, float **) = td->td_refblackwhite;
		return (1);
#endif
	}
	return (0);
}

/*
 * Like TIFFGetField, but return any default
 * value if the tag is not present in the directory.
 */
int
TIFFGetFieldDefaulted(TIFF* tif, ttag_t tag, ...)
{
	int ok;
	va_list ap;

	va_start(ap, tag);
	ok =  TIFFVGetFieldDefaulted(tif, tag, ap);
	va_end(ap);
	return (ok);
}

void
_TIFFprintAscii(FILE* fd, const char* cp)
{
	for (; *cp != '\0'; cp++) {
		const char* tp;

		if (isprint(*cp)) {
			fputc(*cp, fd);
			continue;
		}
		for (tp = "\tt\bb\rr\nn\vv"; *tp; tp++)
			if (*tp++ == *cp)
				break;
		if (*tp)
			fprintf(fd, "\\%c", *tp);
		else
			fprintf(fd, "\\%03o", *cp & 0xff);
	}
}

void
_TIFFprintAsciiTag(FILE* fd, const char* name, const char* value)
{
	fprintf(fd, "  %s: \"", name);
	_TIFFprintAscii(fd, value);
	fprintf(fd, "\"\n");
}

TIFFErrorHandler
TIFFSetErrorHandler(TIFFErrorHandler handler)
{
	TIFFErrorHandler prev = _TIFFerrorHandler;
	_TIFFerrorHandler = handler;
	return (prev);
}

void
TIFFError(const char* module, const char* fmt, ...)
{
	if (_TIFFerrorHandler) {
		va_list ap;
		va_start(ap, fmt);
		(*_TIFFerrorHandler)(module, fmt, ap);
		va_end(ap);
	}
}

TIFFErrorHandler
TIFFSetWarningHandler(TIFFErrorHandler handler)
{
	TIFFErrorHandler prev = _TIFFwarningHandler;
	_TIFFwarningHandler = handler;
	return (prev);
}

void
TIFFWarning(const char* module, const char* fmt, ...)
{
	if (_TIFFwarningHandler) {
		va_list ap;
		va_start(ap, fmt);
		(*_TIFFwarningHandler)(module, fmt, ap);
		va_end(ap);
	}
}

int
TIFFFlush(TIFF* tif)
{

	if (tif->tif_mode != O_RDONLY) {
		if (!TIFFFlushData(tif))
			return (0);
		/*if ((tif->tif_flags & TIFF_DIRTYDIRECT) &&
		    !TIFFWriteDirectory(tif))
			return (0);*/			
	}
	return (1);
}


/*****************************Flush**************************/
/*
 * Flush buffered data to the file.
 */
int
TIFFFlushData(TIFF* tif)
{
	if ((tif->tif_flags & TIFF_BEENWRITING) == 0)
		return (0);
	if (tif->tif_flags & TIFF_POSTENCODE) {
		tif->tif_flags &= ~TIFF_POSTENCODE;
		if (!(*tif->tif_postencode)(tif))
			return (0);
	}
	return (TIFFFlushData1(tif));
}


/*
 * Append the data to the specified strip.
 *
 * NB: We don't check that there's space in the
 *     file (i.e. that strips do not overlap).
 */
static int
TIFFAppendToStrip(TIFF* tif, tstrip_t strip, tidata_t data, tsize_t cc)
{
	TIFFDirectory *td = &tif->tif_dir;
	static const char module[] = "TIFFAppendToStrip";

	if (td->td_stripoffset[strip] == 0 || tif->tif_curoff == 0) {
		/*
		 * No current offset, set the current strip.
		 */
		if (td->td_stripoffset[strip] != 0) {
			if (!SeekOK(tif, td->td_stripoffset[strip])) {
				TIFFError(module,
				    "%s: Seek error at scanline %lu",
				    tif->tif_name, (unsigned long) tif->tif_row);
				return (0);
			}
		} else
			td->td_stripoffset[strip] =
			    TIFFSeekFile(tif, (toff_t) 0, SEEK_END);
		tif->tif_curoff = td->td_stripoffset[strip];
	}
	if (!WriteOK(tif, data, cc)) {
		TIFFError(module, "%s: Write error at scanline %lu",
		    tif->tif_name, (unsigned long) tif->tif_row);
		return (0);
	}
	tif->tif_curoff += cc;
	td->td_stripbytecount[strip] += cc;
	return (1);
}

/*
 * Internal version of TIFFFlushData that can be
 * called by ``encodestrip routines'' w/o concern
 * for infinite recursion.
 */
int
TIFFFlushData1(TIFF* tif)
{
	if (tif->tif_rawcc > 0) {
		if (!isFillOrder(tif, tif->tif_dir.td_fillorder) &&
		    (tif->tif_flags & TIFF_NOBITREV) == 0)
			TIFFReverseBits((unsigned char *)tif->tif_rawdata,
			    tif->tif_rawcc);
		if (!TIFFAppendToStrip(tif,
		    isTiled(tif) ? tif->tif_curtile : tif->tif_curstrip,
		    tif->tif_rawdata, tif->tif_rawcc))
			return (0);
		tif->tif_rawcc = 0;
		tif->tif_rawcp = tif->tif_rawdata;
	}
	return (1);
}

void
TIFFClose(TIFF* tif)
{
	if (tif->tif_mode != O_RDONLY)
		/*
		 * Flush buffered data and directory (if dirty).
		 */
		TIFFFlush(tif);
	if (tif->tif_cleanup)
		(*tif->tif_cleanup)(tif);
	TIFFFreeDirectory(tif);
	if (tif->tif_rawdata && (tif->tif_flags&TIFF_MYBUFFER))
		_TIFFfree(tif->tif_rawdata);
	if (isMapped(tif))
		TIFFUnmapFileContents(tif, tif->tif_base, tif->tif_size);
	(void) TIFFCloseFile(tif);
	if (tif->tif_fieldinfo)
		_TIFFfree(tif->tif_fieldinfo);
	_TIFFfree(tif);
}


/*********Tiled Image Support Routines.****************/

/*
 * Compute which tile an (x,y,z,s) value is in.
 */
ttile_t
TIFFComputeTile(TIFF* tif, uint32 x, uint32 y, uint32 z, tsample_t s)
{
	TIFFDirectory *td = &tif->tif_dir;
	uint32 dx = td->td_tilewidth;
	uint32 dy = td->td_tilelength;
	uint32 dz = td->td_tiledepth;
	ttile_t tile = 1;

	if (td->td_imagedepth == 1)
		z = 0;
	if (dx == (uint32) -1)
		dx = td->td_imagewidth;
	if (dy == (uint32) -1)
		dy = td->td_imagelength;
	if (dz == (uint32) -1)
		dz = td->td_imagedepth;
	if (dx != 0 && dy != 0 && dz != 0) {
		uint32 xpt = TIFFhowmany(td->td_imagewidth, dx); 
		uint32 ypt = TIFFhowmany(td->td_imagelength, dy); 
		uint32 zpt = TIFFhowmany(td->td_imagedepth, dz); 

		if (td->td_planarconfig == PLANARCONFIG_SEPARATE) 
			tile = (xpt*ypt*zpt)*s +
			     (xpt*ypt)*(z/dz) +
			     xpt*(y/dy) +
			     x/dx;
		else
			tile = (xpt*ypt)*(z/dz) + xpt*(y/dy) + x/dx + s;
	}
	return (tile);
}

/*
 * Check an (x,y,z,s) coordinate
 * against the image bounds.
 */
int
TIFFCheckTile(TIFF* tif, uint32 x, uint32 y, uint32 z, tsample_t s)
{
	TIFFDirectory *td = &tif->tif_dir;

	if (x >= td->td_imagewidth) {
		TIFFError(tif->tif_name, "Col %ld out of range, max %lu",
		    (long) x, (unsigned long) td->td_imagewidth);
		return (0);
	}
	if (y >= td->td_imagelength) {
		TIFFError(tif->tif_name, "Row %ld out of range, max %lu",
		    (long) y, (unsigned long) td->td_imagelength);
		return (0);
	}
	if (z >= td->td_imagedepth) {
		TIFFError(tif->tif_name, "Depth %ld out of range, max %lu",
		    (long) z, (unsigned long) td->td_imagedepth);
		return (0);
	}
	if (td->td_planarconfig == PLANARCONFIG_SEPARATE &&
	    s >= td->td_samplesperpixel) {
		TIFFError(tif->tif_name, "Sample %d out of range, max %u",
		    (int) s, td->td_samplesperpixel);
		return (0);
	}
	return (1);
}

/*
 * Compute how many tiles are in an image.
 */
ttile_t
TIFFNumberOfTiles(TIFF* tif)
{
	TIFFDirectory *td = &tif->tif_dir;
	uint32 dx = td->td_tilewidth;
	uint32 dy = td->td_tilelength;
	uint32 dz = td->td_tiledepth;
	ttile_t ntiles;

	if (dx == (uint32) -1)
		dx = td->td_imagewidth;
	if (dy == (uint32) -1)
		dy = td->td_imagelength;
	if (dz == (uint32) -1)
		dz = td->td_imagedepth;
	ntiles = (dx == 0 || dy == 0 || dz == 0) ? 0 :
	    (TIFFhowmany(td->td_imagewidth, dx) *
	     TIFFhowmany(td->td_imagelength, dy) *
	     TIFFhowmany(td->td_imagedepth, dz));
	if (td->td_planarconfig == PLANARCONFIG_SEPARATE)
		ntiles *= td->td_samplesperpixel;
	return (ntiles);
}

/*
 * Compute the # bytes in each row of a tile.
 */
tsize_t
TIFFTileRowSize(TIFF* tif)
{
	TIFFDirectory *td = &tif->tif_dir;
	tsize_t rowsize;
	
	if (td->td_tilelength == 0 || td->td_tilewidth == 0)
		return ((tsize_t) 0);
	rowsize = td->td_bitspersample * td->td_tilewidth;
	if (td->td_planarconfig == PLANARCONFIG_CONTIG)
		rowsize *= td->td_samplesperpixel;
	return ((tsize_t) TIFFhowmany(rowsize, 8));
}

/*
 * Compute the # bytes in a variable length, row-aligned tile.
 */
tsize_t
TIFFVTileSize(TIFF* tif, uint32 nrows)
{
	TIFFDirectory *td = &tif->tif_dir;
	tsize_t tilesize;

	if (td->td_tilelength == 0 || td->td_tilewidth == 0 ||
	    td->td_tiledepth == 0)
		return ((tsize_t) 0);
#ifdef YCBCR_SUPPORT
	if (td->td_planarconfig == PLANARCONFIG_CONTIG &&
	    td->td_photometric == PHOTOMETRIC_YCBCR &&
	    !isUpSampled(tif)) {
		/*
		 * Packed YCbCr data contain one Cb+Cr for every
		 * HorizontalSampling*VerticalSampling Y values.
		 * Must also roundup width and height when calculating
		 * since images that are not a multiple of the
		 * horizontal/vertical subsampling area include
		 * YCbCr data for the extended image.
		 */
		tsize_t w =
		    TIFFroundup(td->td_tilewidth, td->td_ycbcrsubsampling[0]);
		tsize_t rowsize = TIFFhowmany(w*td->td_bitspersample, 8);
		tsize_t samplingarea =
		    td->td_ycbcrsubsampling[0]*td->td_ycbcrsubsampling[1];
		nrows = TIFFroundup(nrows, td->td_ycbcrsubsampling[1]);
		/* NB: don't need TIFFhowmany here 'cuz everything is rounded */
		tilesize = nrows*rowsize + 2*(nrows*rowsize / samplingarea);
	} else
#endif
		tilesize = nrows * TIFFTileRowSize(tif);
	return ((tsize_t)(tilesize * td->td_tiledepth));
}

/*
 * Compute the # bytes in a row-aligned tile.
 */
tsize_t
TIFFTileSize(TIFF* tif)
{
	return (TIFFVTileSize(tif, tif->tif_dir.td_tilelength));
}

/*
 * Compute a default tile size based on the image
 * characteristics and a requested value.  If a
 * request is <1 then we choose a size according
 * to certain heuristics.
 */
void
TIFFDefaultTileSize(TIFF* tif, uint32* tw, uint32* th)
{
	(*tif->tif_deftilesize)(tif, tw, th);
}

void
_TIFFDefaultTileSize(TIFF* tif, uint32* tw, uint32* th)
{
	(void) tif;
	if (*(int32*) tw < 1)
		*tw = 256;
	if (*(int32*) th < 1)
		*th = 256;
	/* roundup to a multiple of 16 per the spec */
	if (*tw & 0xf)
		*tw = TIFFroundup(*tw, 16);
	if (*th & 0xf)
		*th = TIFFroundup(*th, 16);
}


/******"Null" Compression Algorithm Support.****************/

/*
 * Encode a hunk of pixels.
 */
static int
DumpModeEncode(TIFF* tif, tidata_t pp, tsize_t cc, tsample_t s)
{
	(void) s;
	while (cc > 0) {
		tsize_t n;

		n = cc;
		if (tif->tif_rawcc + n > tif->tif_rawdatasize)
			n = tif->tif_rawdatasize - tif->tif_rawcc;
		/*
		 * Avoid copy if client has setup raw
		 * data buffer to avoid extra copy.
		 */
		if (tif->tif_rawcp != pp)
			_TIFFmemcpy(tif->tif_rawcp, pp, n);
		tif->tif_rawcp += n;
		tif->tif_rawcc += n;
		pp += n;
		cc -= n;
		if (tif->tif_rawcc >= tif->tif_rawdatasize &&
		    !TIFFFlushData1(tif))
			return (-1);
	}
	return (1);
}

/*
 * Decode a hunk of pixels.
 */
static int
DumpModeDecode(TIFF* tif, tidata_t buf, tsize_t cc, tsample_t s)
{
	(void) s;
	if (tif->tif_rawcc < cc) {
		TIFFError(tif->tif_name,
		    "DumpModeDecode: Not enough data for scanline %d",
		    tif->tif_row);
		return (0);
	}
	/*
	 * Avoid copy if client has setup raw
	 * data buffer to avoid extra copy.
	 */
	if (tif->tif_rawcp != buf)
		_TIFFmemcpy(buf, tif->tif_rawcp, cc);
	tif->tif_rawcp += cc;
	tif->tif_rawcc -= cc;
	return (1);
}

/*
 * Seek forwards nrows in the current strip.
 */
static int
DumpModeSeek(TIFF* tif, uint32 nrows)
{
	tif->tif_rawcp += nrows * tif->tif_scanlinesize;
	tif->tif_rawcc -= nrows * tif->tif_scanlinesize;
	return (1);
}

/*
 * Initialize dump mode.
 */
int
TIFFInitDumpMode(TIFF* tif, int scheme)
{
	(void) scheme;
	tif->tif_decoderow = DumpModeDecode;
	tif->tif_decodestrip = DumpModeDecode;
	tif->tif_decodetile = DumpModeDecode;
	tif->tif_encoderow = DumpModeEncode;
	tif->tif_encodestrip = DumpModeEncode;
	tif->tif_encodetile = DumpModeEncode;
	tif->tif_seek = DumpModeSeek;
	return (1);
}

/********Compute which strip a (row,sample) value is in.********/
tstrip_t
TIFFComputeStrip(TIFF* tif, uint32 row, tsample_t sample)
{
	TIFFDirectory *td = &tif->tif_dir;
	tstrip_t strip;

	strip = row / td->td_rowsperstrip;
	if (td->td_planarconfig == PLANARCONFIG_SEPARATE) {
		if (sample >= td->td_samplesperpixel) {
			TIFFError(tif->tif_name,
			    "%u: Sample out of range, max %u",
			    sample, td->td_samplesperpixel);
			return ((tstrip_t) 0);
		}
		strip += sample*td->td_stripsperimage;
	}
	return (strip);
}

/*
 * Compute how many strips are in an image.
 */
tstrip_t
TIFFNumberOfStrips(TIFF* tif)
{
	TIFFDirectory *td = &tif->tif_dir;
	tstrip_t nstrips;

	nstrips = (td->td_rowsperstrip == (uint32) -1 ?
	     (td->td_imagelength != 0 ? 1 : 0) :
	     TIFFhowmany(td->td_imagelength, td->td_rowsperstrip));
	if (td->td_planarconfig == PLANARCONFIG_SEPARATE)
		nstrips *= td->td_samplesperpixel;
	return (nstrips);
}

/*
 * Compute the # bytes in a variable height, row-aligned strip.
 */
tsize_t
TIFFVStripSize(TIFF* tif, uint32 nrows)
{
	TIFFDirectory *td = &tif->tif_dir;

	if (nrows == (uint32) -1)
		nrows = td->td_imagelength;
#ifdef YCBCR_SUPPORT
	if (td->td_planarconfig == PLANARCONFIG_CONTIG &&
	    td->td_photometric == PHOTOMETRIC_YCBCR &&
	    !isUpSampled(tif)) {
		/*
		 * Packed YCbCr data contain one Cb+Cr for every
		 * HorizontalSampling*VerticalSampling Y values.
		 * Must also roundup width and height when calculating
		 * since images that are not a multiple of the
		 * horizontal/vertical subsampling area include
		 * YCbCr data for the extended image.
		 */
		tsize_t w =
		    TIFFroundup(td->td_imagewidth, td->td_ycbcrsubsampling[0]);
		tsize_t scanline = TIFFhowmany(w*td->td_bitspersample, 8);
		tsize_t samplingarea =
		    td->td_ycbcrsubsampling[0]*td->td_ycbcrsubsampling[1];
		nrows = TIFFroundup(nrows, td->td_ycbcrsubsampling[1]);
		/* NB: don't need TIFFhowmany here 'cuz everything is rounded */
		return ((tsize_t)
		    (nrows*scanline + 2*(nrows*scanline / samplingarea)));
	} else
#endif
		return ((tsize_t)(nrows * TIFFScanlineSize(tif)));
}

/*
 * Compute the # bytes in a (row-aligned) strip.
 *
 * Note that if RowsPerStrip is larger than the
 * recorded ImageLength, then the strip size is
 * truncated to reflect the actual space required
 * to hold the strip.
 */
tsize_t
TIFFStripSize(TIFF* tif)
{
	TIFFDirectory* td = &tif->tif_dir;
	uint32 rps = td->td_rowsperstrip;
	if (rps > td->td_imagelength)
		rps = td->td_imagelength;
	return (TIFFVStripSize(tif, rps));
}

/*
 * Compute a default strip size based on the image
 * characteristics and a requested value.  If the
 * request is <1 then we choose a strip size according
 * to certain heuristics.
 */
uint32
TIFFDefaultStripSize(TIFF* tif, uint32 request)
{
	return (*tif->tif_defstripsize)(tif, request);
}

uint32
_TIFFDefaultStripSize(TIFF* tif, uint32 s)
{
	if ((int32) s < 1) {
		/*
		 * If RowsPerStrip is unspecified, try to break the
		 * image up into strips that are approximately 8Kbytes.
		 */
		tsize_t scanline = TIFFScanlineSize(tif);
		s = (uint32)(8*1024) / (scanline == 0 ? 1 : scanline);
		if (s == 0)		/* very wide images */
			s = 1;
	}
	return (s);
}

/*
 * Return the number of bytes to read/write in a call to
 * one of the scanline-oriented i/o routines.  Note that
 * this number may be 1/samples-per-pixel if data is
 * stored as separate planes.
 */
tsize_t
TIFFScanlineSize(TIFF* tif)
{
	TIFFDirectory *td = &tif->tif_dir;
	tsize_t scanline;
	
	scanline = td->td_bitspersample * td->td_imagewidth;
	if (td->td_planarconfig == PLANARCONFIG_CONTIG)
		scanline *= td->td_samplesperpixel;
	return ((tsize_t) TIFFhowmany(scanline, 8));
}

/*
 * Return the number of bytes required to store a complete
 * decoded and packed raster scanline (as opposed to the
 * I/O size returned by TIFFScanlineSize which may be less
 * if data is store as separate planes).
 */
tsize_t
TIFFRasterScanlineSize(TIFF* tif)
{
	TIFFDirectory *td = &tif->tif_dir;
	tsize_t scanline;
	
	scanline = td->td_bitspersample * td->td_imagewidth;
	if (td->td_planarconfig == PLANARCONFIG_CONTIG) {
		scanline *= td->td_samplesperpixel;
		return ((tsize_t) TIFFhowmany(scanline, 8));
	} else
		return ((tsize_t)
		    TIFFhowmany(scanline, 8)*td->td_samplesperpixel);
}


/********TIFF Library Bit & Byte Swapping Support.******/

#ifndef TIFFSwabShort
void
TIFFSwabShort(uint16* wp)
{
	register unsigned char* cp = (unsigned char*) wp;
	int t;

	t = cp[1]; cp[1] = cp[0]; cp[0] = t;
}
#endif

#ifndef TIFFSwabLong
void
TIFFSwabLong(uint32* lp)
{
	register unsigned char* cp = (unsigned char*) lp;
	int t;

	t = cp[3]; cp[3] = cp[0]; cp[0] = t;
	t = cp[2]; cp[2] = cp[1]; cp[1] = t;
}
#endif

#ifndef TIFFSwabArrayOfShort
void
TIFFSwabArrayOfShort(uint16* wp, register unsigned long n)
{
	register unsigned char* cp;
	register int t;

	/* XXX unroll loop some */
	while (n-- > 0) {
		cp = (unsigned char*) wp;
		t = cp[1]; cp[1] = cp[0]; cp[0] = t;
		wp++;
	}
}
#endif

#ifndef TIFFSwabArrayOfLong
void
TIFFSwabArrayOfLong(register uint32* lp, register unsigned long n)
{
	register unsigned char *cp;
	register int t;

	/* XXX unroll loop some */
	while (n-- > 0) {
		cp = (unsigned char *)lp;
		t = cp[3]; cp[3] = cp[0]; cp[0] = t;
		t = cp[2]; cp[2] = cp[1]; cp[1] = t;
		lp++;
	}
}
#endif

#ifndef TIFFSwabDouble
void
TIFFSwabDouble(double *dp)
{
        register uint32* lp = (uint32*) dp;
        uint32 t;

	TIFFSwabArrayOfLong(lp, 2);
	t = lp[0]; lp[0] = lp[1]; lp[1] = t;
}
#endif

#ifndef TIFFSwabArrayOfDouble
void
TIFFSwabArrayOfDouble(double* dp, register unsigned long n)
{
	register uint32* lp = (uint32*) dp;
        register uint32 t;

	TIFFSwabArrayOfLong(lp, n + n);
        while (n-- > 0) {
		t = lp[0]; lp[0] = lp[1]; lp[1] = t;
                lp += 2;
        }
}
#endif

/*
 * Bit reversal tables.  TIFFBitRevTable[<byte>] gives
 * the bit reversed value of <byte>.  Used in various
 * places in the library when the FillOrder requires
 * bit reversal of byte values (e.g. CCITT Fax 3
 * encoding/decoding).  TIFFNoBitRevTable is provided
 * for algorithms that want an equivalent table that
 * do not reverse bit values.
 */
static const unsigned char TIFFBitRevTable[256] = {
    0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
    0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
    0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
    0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
    0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
    0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
    0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
    0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
    0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
    0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
    0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
    0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
    0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
    0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
    0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
    0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
    0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
    0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
    0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
    0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
    0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
    0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
    0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
    0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
    0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
    0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
    0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
    0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
    0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
    0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
    0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
    0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff
};
static const unsigned char TIFFNoBitRevTable[256] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 
    0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 
    0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 
    0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 
    0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 
    0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 
    0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 
    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 
    0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 
    0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 
    0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 
    0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 
    0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 
    0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 
    0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 
    0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 
    0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 
    0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 
    0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff, 
};

const unsigned char*
TIFFGetBitRevTable(int reversed)
{
	return (reversed ? TIFFBitRevTable : TIFFNoBitRevTable);
}

void
TIFFReverseBits(register unsigned char* cp, register unsigned long n)
{
	for (; n > 8; n -= 8) {
		cp[0] = TIFFBitRevTable[cp[0]];
		cp[1] = TIFFBitRevTable[cp[1]];
		cp[2] = TIFFBitRevTable[cp[2]];
		cp[3] = TIFFBitRevTable[cp[3]];
		cp[4] = TIFFBitRevTable[cp[4]];
		cp[5] = TIFFBitRevTable[cp[5]];
		cp[6] = TIFFBitRevTable[cp[6]];
		cp[7] = TIFFBitRevTable[cp[7]];
		cp += 8;
	}
	while (n-- > 0)
		*cp = TIFFBitRevTable[*cp], cp++;
}

/****************************/
