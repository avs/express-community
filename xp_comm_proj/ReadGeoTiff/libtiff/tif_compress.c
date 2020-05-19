/* $Header: /cvs/repository/TidalWave/Utility/Modules/GeoTIFF/libtiff/tif_compress.c,v 1.1 1997/12/25 00:01:31 jmethot Exp $ */

/*
 * Copyright (c) 1988-1996 Sam Leffler
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
 * TIFF Library
 *
 * Compression Scheme Configuration Support.
 */
#include "tiffiop.h"

static	int NotConfigured(TIFF*, int);

#ifndef	LZW_SUPPORT
#define	TIFFInitLZW		NotConfigured
#endif
#ifndef	PACKBITS_SUPPORT
#define	TIFFInitPackbits	NotConfigured
#endif
#ifndef	THUNDER_SUPPORT
#define	TIFFInitThunderScan	NotConfigured
#endif
#ifndef	NEXT_SUPPORT
#define	TIFFInitNeXT		NotConfigured
#endif
#ifndef	JPEG_SUPPORT
#define	TIFFInitJPEG		NotConfigured
#endif
#ifndef	OJPEG_SUPPORT
#define	TIFFInitOJPEG		NotConfigured
#endif
#ifndef	CCITT_SUPPORT
#define	TIFFInitCCITTRLE	NotConfigured
#define	TIFFInitCCITTRLEW	NotConfigured
#define	TIFFInitCCITTFax3	NotConfigured
#define	TIFFInitCCITTFax4	NotConfigured
#endif
#ifndef JBIG_SUPPORT
#define	TIFFInitJBIG		NotConfigured
#endif
#ifndef	ZIP_SUPPORT
#define	TIFFInitZIP		NotConfigured
#endif
#ifndef	PIXARLOG_SUPPORT
#define	TIFFInitPixarLog	NotConfigured
#endif

/*
 * Compression schemes statically built into the library.
 */
#ifdef VMS
const TIFFCodec _TIFFBuiltinCODECS[] = {
#else
TIFFCodec _TIFFBuiltinCODECS[] = {
#endif
    { "None",		COMPRESSION_NONE,	TIFFInitDumpMode },
    { "LZW",		COMPRESSION_LZW,	TIFFInitLZW },
    { "PackBits",	COMPRESSION_PACKBITS,	TIFFInitPackBits },
    { "ThunderScan",	COMPRESSION_THUNDERSCAN,TIFFInitThunderScan },
    { "NeXT",		COMPRESSION_NEXT,	TIFFInitNeXT },
    { "JPEG",		COMPRESSION_JPEG,	TIFFInitJPEG },
    { "Old-style JPEG",	COMPRESSION_OJPEG,	TIFFInitOJPEG },
    { "CCITT RLE",	COMPRESSION_CCITTRLE,	TIFFInitCCITTRLE },
    { "CCITT RLE/W",	COMPRESSION_CCITTRLEW,	TIFFInitCCITTRLEW },
    { "CCITT Group 3",	COMPRESSION_CCITTFAX3,	TIFFInitCCITTFax3 },
    { "CCITT Group 4",	COMPRESSION_CCITTFAX4,	TIFFInitCCITTFax4 },
    { "ISO JBIG",	COMPRESSION_JBIG,	TIFFInitJBIG },
    { "Deflate",	COMPRESSION_DEFLATE,	TIFFInitZIP },
    { "PixarLog",	COMPRESSION_PIXARLOG,	TIFFInitPixarLog },
    { NULL },
};

static int
_notConfigured(TIFF* tif)
{
	const TIFFCodec* c = TIFFFindCODEC(tif->tif_dir.td_compression);

	TIFFError(tif->tif_name,
	    "%s compression support is not configured", c->name);
	return (0);
}

static int
NotConfigured(TIFF* tif, int scheme)
{
	tif->tif_setupdecode = _notConfigured;
	tif->tif_setupencode = _notConfigured;
	return (1);
}


static int
TIFFNoEncode(TIFF* tif, char* method)
{
	const TIFFCodec* c = TIFFFindCODEC(tif->tif_dir.td_compression);

	if (c)
		TIFFError(tif->tif_name, "%s %s encoding is not implemented",
		    c->name, method);
	else
		TIFFError(tif->tif_name,
		    "Compression scheme %u %s encoding is not implemented",
		    tif->tif_dir.td_compression, method);
	return (-1);
}

int
_TIFFNoRowEncode(TIFF* tif, tidata_t pp, tsize_t cc, tsample_t s)
{
	(void) pp; (void) cc; (void) s;
	return (TIFFNoEncode(tif, "scanline"));
}

int
_TIFFNoStripEncode(TIFF* tif, tidata_t pp, tsize_t cc, tsample_t s)
{
	(void) pp; (void) cc; (void) s;
	return (TIFFNoEncode(tif, "strip"));
}

int
_TIFFNoTileEncode(TIFF* tif, tidata_t pp, tsize_t cc, tsample_t s)
{
	(void) pp; (void) cc; (void) s;
	return (TIFFNoEncode(tif, "tile"));
}

static int
TIFFNoDecode(TIFF* tif, char* method)
{
	const TIFFCodec* c = TIFFFindCODEC(tif->tif_dir.td_compression);

	if (c)
		TIFFError(tif->tif_name, "%s %s decoding is not implemented",
		    c->name, method);
	else
		TIFFError(tif->tif_name,
		    "Compression scheme %u %s decoding is not implemented",
		    tif->tif_dir.td_compression, method);
	return (-1);
}

int
_TIFFNoRowDecode(TIFF* tif, tidata_t pp, tsize_t cc, tsample_t s)
{
	(void) pp; (void) cc; (void) s;
	return (TIFFNoDecode(tif, "scanline"));
}

int
_TIFFNoStripDecode(TIFF* tif, tidata_t pp, tsize_t cc, tsample_t s)
{
	(void) pp; (void) cc; (void) s;
	return (TIFFNoDecode(tif, "strip"));
}

int
_TIFFNoTileDecode(TIFF* tif, tidata_t pp, tsize_t cc, tsample_t s)
{
	(void) pp; (void) cc; (void) s;
	return (TIFFNoDecode(tif, "tile"));
}

int
_TIFFNoSeek(TIFF* tif, uint32 off)
{
	(void) off;
	TIFFError(tif->tif_name,
	    "Compression algorithm does not support random access");
	return (0);
}

int
_TIFFNoPreCode(TIFF* tif, tsample_t s)
{
	(void) tif; (void) s;
	return (1);
}

static int _TIFFtrue(TIFF* tif) { (void) tif; return (1); }
static void _TIFFvoid(TIFF* tif) { (void) tif; }

int
TIFFSetCompressionScheme(TIFF* tif, int scheme)
{
	const TIFFCodec *c = TIFFFindCODEC(scheme);

	tif->tif_setupdecode = _TIFFtrue;
	tif->tif_predecode = _TIFFNoPreCode;
	tif->tif_decoderow = _TIFFNoRowDecode;
	tif->tif_decodestrip = _TIFFNoStripDecode;
	tif->tif_decodetile = _TIFFNoTileDecode;
	tif->tif_setupencode = _TIFFtrue;
	tif->tif_preencode = _TIFFNoPreCode;
	tif->tif_postencode = _TIFFtrue;
	tif->tif_encoderow = _TIFFNoRowEncode;
	tif->tif_encodestrip = _TIFFNoStripEncode;
	tif->tif_encodetile = _TIFFNoTileEncode;
	tif->tif_close = _TIFFvoid;
	tif->tif_seek = _TIFFNoSeek;
	tif->tif_cleanup = _TIFFvoid;
	tif->tif_defstripsize = _TIFFDefaultStripSize;
	tif->tif_deftilesize = _TIFFDefaultTileSize;
	tif->tif_flags &= ~TIFF_NOBITREV;
	/*
	 * Don't treat an unknown compression scheme as an error.
	 * This permits applications to open files with data that
	 * the library does not have builtin support for, but which
	 * may still be meaningful.
	 */
	return (c ? (*c->init)(tif, scheme) : 1);
}

/*
 * Other compression schemes may be registered.  Registered
 * schemes can also override the builtin versions provided
 * by this library.
 */
typedef struct _codec {
	struct _codec*	next;
	TIFFCodec*	info;
} codec_t;
static	codec_t* registeredCODECS = NULL;

const TIFFCodec*
TIFFFindCODEC(uint16 scheme)
{
	const TIFFCodec* c;
	codec_t* cd;

	for (cd = registeredCODECS; cd; cd = cd->next)
		if (cd->info->scheme == scheme)
			return ((const TIFFCodec*) cd->info);
	for (c = _TIFFBuiltinCODECS; c->name; c++)
		if (c->scheme == scheme)
			return (c);
	return ((const TIFFCodec*) 0);
}

TIFFCodec*
TIFFRegisterCODEC(uint16 scheme, const char* name, TIFFInitMethod init)
{
	codec_t* cd = (codec_t*)
	    _TIFFmalloc(sizeof (codec_t) + sizeof (TIFFCodec) + strlen(name)+1);

	if (cd != NULL) {
		cd->info = (TIFFCodec*) ((tidata_t) cd + sizeof (codec_t));
		cd->info->name = (char*)
		    ((tidata_t) cd->info + sizeof (TIFFCodec));
		strcpy(cd->info->name, name);
		cd->info->scheme = scheme;
		cd->info->init = init;
		cd->next = registeredCODECS;
		registeredCODECS = cd;
	} else
		TIFFError("TIFFRegisterCODEC",
		    "No space to register compression scheme %s", name);
	return (cd->info);
}

void
TIFFUnRegisterCODEC(TIFFCodec* c)
{
	codec_t* cd;
	codec_t** pcd;

	for (pcd = &registeredCODECS; (cd = *pcd); pcd = &cd->next)
		if (cd->info == c) {
			*pcd = cd->next;
			_TIFFfree(cd);
			return;
		}
	TIFFError("TIFFUnRegisterCODEC",
	    "Cannot remove compression scheme %s; not registered", c->name);
}

/*********ThunderScan 4-bit Compression Algorithm Support********/
#ifdef THUNDER_SUPPORT
/*
 * TIFF Library.
 *
 * ThunderScan 4-bit Compression Algorithm Support
 */

/*
 * ThunderScan uses an encoding scheme designed for
 * 4-bit pixel values.  Data is encoded in bytes, with
 * each byte split into a 2-bit code word and a 6-bit
 * data value.  The encoding gives raw data, runs of
 * pixels, or pixel values encoded as a delta from the
 * previous pixel value.  For the latter, either 2-bit
 * or 3-bit delta values are used, with the deltas packed
 * into a single byte.
 */
#define	THUNDER_DATA		0x3f	/* mask for 6-bit data */
#define	THUNDER_CODE		0xc0	/* mask for 2-bit code word */
/* code values */
#define	THUNDER_RUN		0x00	/* run of pixels w/ encoded count */
#define	THUNDER_2BITDELTAS	0x40	/* 3 pixels w/ encoded 2-bit deltas */
#define	    DELTA2_SKIP		2	/* skip code for 2-bit deltas */
#define	THUNDER_3BITDELTAS	0x80	/* 2 pixels w/ encoded 3-bit deltas */
#define	    DELTA3_SKIP		4	/* skip code for 3-bit deltas */
#define	THUNDER_RAW		0xc0	/* raw data encoded */

static const int twobitdeltas[4] = { 0, 1, 0, -1 };
static const int threebitdeltas[8] = { 0, 1, 2, 3, 0, -3, -2, -1 };

#define	SETPIXEL(op, v) { \
	lastpixel = (v) & 0xf; \
	if (npixels++ & 1) \
	    *op++ |= lastpixel; \
	else \
	    op[0] = lastpixel << 4; \
}

static int
ThunderDecode(TIFF* tif, tidata_t op, tsize_t maxpixels)
{
	register unsigned char *bp;
	register tsize_t cc;
	unsigned int lastpixel;
	tsize_t npixels;

	bp = (unsigned char *)tif->tif_rawcp;
	cc = tif->tif_rawcc;
	lastpixel = 0;
	npixels = 0;
	while (cc > 0 && npixels < maxpixels) {
		int n, delta;

		n = *bp++, cc--;
		switch (n & THUNDER_CODE) {
		case THUNDER_RUN:		/* pixel run */
			/*
			 * Replicate the last pixel n times,
			 * where n is the lower-order 6 bits.
			 */
			if (npixels & 1) {
				op[0] |= lastpixel;
				lastpixel = *op++; npixels++; n--;
			} else
				lastpixel |= lastpixel << 4;
			npixels += n;
			for (; n > 0; n -= 2)
				*op++ = lastpixel;
			if (n == -1)
				*--op &= 0xf0;
			lastpixel &= 0xf;
			break;
		case THUNDER_2BITDELTAS:	/* 2-bit deltas */
			if ((delta = ((n >> 4) & 3)) != DELTA2_SKIP)
				SETPIXEL(op, lastpixel + twobitdeltas[delta]);
			if ((delta = ((n >> 2) & 3)) != DELTA2_SKIP)
				SETPIXEL(op, lastpixel + twobitdeltas[delta]);
			if ((delta = (n & 3)) != DELTA2_SKIP)
				SETPIXEL(op, lastpixel + twobitdeltas[delta]);
			break;
		case THUNDER_3BITDELTAS:	/* 3-bit deltas */
			if ((delta = ((n >> 3) & 7)) != DELTA3_SKIP)
				SETPIXEL(op, lastpixel + threebitdeltas[delta]);
			if ((delta = (n & 7)) != DELTA3_SKIP)
				SETPIXEL(op, lastpixel + threebitdeltas[delta]);
			break;
		case THUNDER_RAW:		/* raw data */
			SETPIXEL(op, n);
			break;
		}
	}
	tif->tif_rawcp = (tidata_t) bp;
	tif->tif_rawcc = cc;
	if (npixels != maxpixels) {
		TIFFError(tif->tif_name,
		    "ThunderDecode: %s data at scanline %ld (%lu != %lu)",
		    npixels < maxpixels ? "Not enough" : "Too much",
		    (long) tif->tif_row, (long) npixels, (long) maxpixels);
		return (0);
	}
	return (1);
}

static int
ThunderDecodeRow(TIFF* tif, tidata_t buf, tsize_t occ, tsample_t s)
{
	tidata_t row = buf;
	
	(void) s;
	while ((long)occ > 0) {
		if (!ThunderDecode(tif, row, tif->tif_dir.td_imagewidth))
			return (0);
		occ -= tif->tif_scanlinesize;
		row += tif->tif_scanlinesize;
	}
	return (1);
}

int
TIFFInitThunderScan(TIFF* tif, int scheme)
{
	(void) scheme;
	tif->tif_decoderow = ThunderDecodeRow;
	tif->tif_decodestrip = ThunderDecodeRow;
	return (1);
}
#endif /* THUNDER_SUPPORT */

/***********ZIP (aka Deflate) Compression Support*************/
#ifdef ZIP_SUPPORT
/*
 * TIFF Library.
 *
 * ZIP (aka Deflate) Compression Support
 *
 * This file is simply an interface to the zlib library written by
 * Jean-loup Gailly and Mark Adler.  You must use version 1.0 or later
 * of the library: this code assumes the 1.0 API and also depends on
 * the ability to write the zlib header multiple times (one per strip)
 * which was not possible with versions prior to 0.95.  Note also that
 * older versions of this codec avoided this bug by supressing the header
 * entirely.  This means that files written with the old library cannot
 * be read; they should be converted to a different compression scheme
 * and then reconverted.
 *
 * The data format used by the zlib library is described in the files
 * zlib-3.1.doc, deflate-1.1.doc and gzip-4.1.doc, available in the
 * directory ftp://ftp.uu.net/pub/archiving/zip/doc.  The library was
 * last found at ftp://ftp.uu.net/pub/archiving/zip/zlib/zlib-0.99.tar.gz.
 */
#include "tif_predict.h"
#include "zlib.h"

#include <stdio.h>
#include <assert.h>

/*
 * Sigh, ZLIB_VERSION is defined as a string so there's no
 * way to do a proper check here.  Instead we guess based
 * on the presence of #defines that were added between the
 * 0.95 and 1.0 distributions.
 */
#if !defined(Z_NO_COMPRESSION) || !defined(Z_DEFLATED)
#error "Antiquated ZLIB software; you must use version 1.0 or later"
#endif

/*
 * State block for each open TIFF
 * file using ZIP compression/decompression.
 */
typedef	struct {
	TIFFPredictorState predict;
	z_stream	stream;
	int		zipquality;		/* compression level */
	int		state;			/* state flags */
#define	ZSTATE_INIT	0x1		/* zlib setup successfully */

	TIFFVGetMethod	vgetparent;		/* super-class method */
	TIFFVSetMethod	vsetparent;		/* super-class method */
} ZIPState;

#define	ZState(tif)		((ZIPState*) (tif)->tif_data)
#define	DecoderState(tif)	ZState(tif)
#define	EncoderState(tif)	ZState(tif)

static	int ZIPEncode(TIFF*, tidata_t, tsize_t, tsample_t);
static	int ZIPDecode(TIFF*, tidata_t, tsize_t, tsample_t);

static int
ZIPSetupDecode(TIFF* tif)
{
	ZIPState* sp = DecoderState(tif);
	static char module[] = "ZIPSetupDecode";

	assert(sp != NULL);
	if (inflateInit(&sp->stream) != Z_OK) {
		TIFFError(module, "%s: %s", tif->tif_name, sp->stream.msg);
		return (0);
	} else {
		sp->state |= ZSTATE_INIT;
		return (1);
	}
}

/*
 * Setup state for decoding a strip.
 */
static int
ZIPPreDecode(TIFF* tif, tsample_t s)
{
	ZIPState* sp = DecoderState(tif);

	(void) s;
	assert(sp != NULL);
	sp->stream.next_in = tif->tif_rawdata;
	sp->stream.avail_in = tif->tif_rawcc;
	return (inflateReset(&sp->stream) == Z_OK);
}

static int
ZIPDecode(TIFF* tif, tidata_t op, tsize_t occ, tsample_t s)
{
	ZIPState* sp = DecoderState(tif);
	static char module[] = "ZIPDecode";

	(void) s;
	assert(sp != NULL);
	sp->stream.next_out = op;
	sp->stream.avail_out = occ;
	do {
		int state = inflate(&sp->stream, Z_PARTIAL_FLUSH);
		if (state == Z_STREAM_END)
			break;
		if (state == Z_DATA_ERROR) {
			TIFFError(module,
			    "%s: Decoding error at scanline %d, %s",
			    tif->tif_name, tif->tif_row, sp->stream.msg);
			if (inflateSync(&sp->stream) != Z_OK)
				return (0);
			continue;
		}
		if (state != Z_OK) {
			TIFFError(module, "%s: zlib error: %s",
			    tif->tif_name, sp->stream.msg);
			return (0);
		}
	} while (sp->stream.avail_out > 0);
	if (sp->stream.avail_out != 0) {
		TIFFError(module,
		    "%s: Not enough data at scanline %d (short %d bytes)",
		    tif->tif_name, tif->tif_row, sp->stream.avail_out);
		return (0);
	}
	return (1);
}

static int
ZIPSetupEncode(TIFF* tif)
{
	ZIPState* sp = EncoderState(tif);
	static char module[] = "ZIPSetupEncode";

	assert(sp != NULL);
	if (deflateInit(&sp->stream, sp->zipquality) != Z_OK) {
		TIFFError(module, "%s: %s", tif->tif_name, sp->stream.msg);
		return (0);
	} else {
		sp->state |= ZSTATE_INIT;
		return (1);
	}
}

/*
 * Reset encoding state at the start of a strip.
 */
static int
ZIPPreEncode(TIFF* tif, tsample_t s)
{
	ZIPState *sp = EncoderState(tif);

	(void) s;
	assert(sp != NULL);
	sp->stream.next_out = tif->tif_rawdata;
	sp->stream.avail_out = tif->tif_rawdatasize;
	return (deflateReset(&sp->stream) == Z_OK);
}

/*
 * Encode a chunk of pixels.
 */
static int
ZIPEncode(TIFF* tif, tidata_t bp, tsize_t cc, tsample_t s)
{
	ZIPState *sp = EncoderState(tif);
	static char module[] = "ZIPEncode";

	(void) s;
	sp->stream.next_in = bp;
	sp->stream.avail_in = cc;
	do {
		if (deflate(&sp->stream, Z_NO_FLUSH) != Z_OK) {
			TIFFError(module, "%s: Encoder error: %s",
			    tif->tif_name, sp->stream.msg);
			return (0);
		}
		if (sp->stream.avail_out == 0) {
			tif->tif_rawcc = tif->tif_rawdatasize;
			TIFFFlushData1(tif);
			sp->stream.next_out = tif->tif_rawdata;
			sp->stream.avail_out = tif->tif_rawdatasize;
		}
	} while (sp->stream.avail_in > 0);
	return (1);
}

/*
 * Finish off an encoded strip by flushing the last
 * string and tacking on an End Of Information code.
 */
static int
ZIPPostEncode(TIFF* tif)
{
	ZIPState *sp = EncoderState(tif);
	static char module[] = "ZIPPostEncode";
	int state;

	sp->stream.avail_in = 0;
	do {
		state = deflate(&sp->stream, Z_FINISH);
		switch (state) {
		case Z_STREAM_END:
		case Z_OK:
		    if (sp->stream.avail_out != tif->tif_rawdatasize) {
			    tif->tif_rawcc =
				tif->tif_rawdatasize - sp->stream.avail_out;
			    TIFFFlushData1(tif);
			    sp->stream.next_out = tif->tif_rawdata;
			    sp->stream.avail_out = tif->tif_rawdatasize;
		    }
		    break;
		default:
		    TIFFError(module, "%s: zlib error: %s",
			tif->tif_name, sp->stream.msg);
		    return (0);
		}
	} while (state != Z_STREAM_END);
	return (1);
}

static void
ZIPCleanup(TIFF* tif)
{
	ZIPState* sp = ZState(tif);
	if (sp) {
		if (tif->tif_mode == O_RDONLY)
			inflateEnd(&sp->stream);
		else
			deflateEnd(&sp->stream);
		_TIFFfree(sp);
		tif->tif_data = NULL;
	}
}

static int
ZIPVSetField(TIFF* tif, ttag_t tag, va_list ap)
{
	ZIPState* sp = ZState(tif);
	static char module[] = "ZIPVSetField";

	switch (tag) {
	case TIFFTAG_ZIPQUALITY:
		sp->zipquality = va_arg(ap, int);
		if (tif->tif_mode != O_RDONLY && (sp->state&ZSTATE_INIT)) {
			if (deflateParams(&sp->stream,
			    sp->zipquality, Z_DEFAULT_STRATEGY) != Z_OK) {
				TIFFError(module, "%s: zlib error: %s",
				    tif->tif_name, sp->stream.msg);
				return (0);
			}
		}
		return (1);
	default:
		return (*sp->vsetparent)(tif, tag, ap);
	}
	/*NOTREACHED*/
}

static int
ZIPVGetField(TIFF* tif, ttag_t tag, va_list ap)
{
	ZIPState* sp = ZState(tif);

	switch (tag) {
	case TIFFTAG_ZIPQUALITY:
		*va_arg(ap, int*) = sp->zipquality;
		break;
	default:
		return (*sp->vgetparent)(tif, tag, ap);
	}
	return (1);
}

static const TIFFFieldInfo zipFieldInfo[] = {
    { TIFFTAG_ZIPQUALITY,	 0, 0,	TIFF_ANY,	FIELD_PSEUDO,
      TRUE,	FALSE,	"" },
};
#define	N(a)	(sizeof (a) / sizeof (a[0]))

int
TIFFInitZIP(TIFF* tif, int scheme)
{
	ZIPState* sp;

	assert(scheme == COMPRESSION_DEFLATE);

	/*
	 * Allocate state block so tag methods have storage to record values.
	 */
	tif->tif_data = (tidata_t) _TIFFmalloc(sizeof (ZIPState));
	if (tif->tif_data == NULL)
		goto bad;
	sp = ZState(tif);
	sp->stream.zalloc = NULL;
	sp->stream.zfree = NULL;
	sp->stream.opaque = NULL;
	sp->stream.data_type = Z_BINARY;

	/*
	 * Merge codec-specific tag information and
	 * override parent get/set field methods.
	 */
	_TIFFMergeFieldInfo(tif, zipFieldInfo, N(zipFieldInfo));
	sp->vgetparent = tif->tif_vgetfield;
	tif->tif_vgetfield = ZIPVGetField;	/* hook for codec tags */
	sp->vsetparent = tif->tif_vsetfield;
	tif->tif_vsetfield = ZIPVSetField;	/* hook for codec tags */

	/* Default values for codec-specific fields */
	sp->zipquality = Z_DEFAULT_COMPRESSION;	/* default comp. level */
	sp->state = 0;

	/*
	 * Install codec methods.
	 */
	tif->tif_setupdecode = ZIPSetupDecode;
	tif->tif_predecode = ZIPPreDecode;
	tif->tif_decoderow = ZIPDecode;
	tif->tif_decodestrip = ZIPDecode;
	tif->tif_decodetile = ZIPDecode;
	tif->tif_setupencode = ZIPSetupEncode;
	tif->tif_preencode = ZIPPreEncode;
	tif->tif_postencode = ZIPPostEncode;
	tif->tif_encoderow = ZIPEncode;
	tif->tif_encodestrip = ZIPEncode;
	tif->tif_encodetile = ZIPEncode;
	tif->tif_cleanup = ZIPCleanup;
	/*
	 * Setup predictor setup.
	 */
	(void) TIFFPredictorInit(tif);
	return (1);
bad:
	TIFFError("TIFFInitZIP", "No space for ZIP state block");
	return (0);
}
#endif /* ZIP_SUPORT */

/*****NeXT 2-bit Grey Scale Compression Algorithm Support****/
#ifdef NEXT_SUPPORT
/*
 * TIFF Library.
 *
 * NeXT 2-bit Grey Scale Compression Algorithm Support
 */

#define SETPIXEL(op, v) {			\
	switch (npixels++ & 3) {		\
	case 0:	op[0]  = (v) << 6; break;	\
	case 1:	op[0] |= (v) << 4; break;	\
	case 2:	op[0] |= (v) << 2; break;	\
	case 3:	*op++ |= (v);	   break;	\
	}					\
}

#define LITERALROW	0x00
#define LITERALSPAN	0x40
#define WHITE   	((1<<2)-1)

static int
NeXTDecode(TIFF* tif, tidata_t buf, tsize_t occ, tsample_t s)
{
	register unsigned char *bp, *op;
	register tsize_t cc;
	register int n;
	tidata_t row;
	tsize_t scanline;

	(void) s;
	/*
	 * Each scanline is assumed to start off as all
	 * white (we assume a PhotometricInterpretation
	 * of ``min-is-black'').
	 */
	for (op = buf, cc = occ; cc-- > 0;)
		*op++ = 0xff;

	bp = (unsigned char *)tif->tif_rawcp;
	cc = tif->tif_rawcc;
	scanline = tif->tif_scanlinesize;
	for (row = buf; (long)occ > 0; occ -= scanline, row += scanline) {
		n = *bp++, cc--;
		switch (n) {
		case LITERALROW:
			/*
			 * The entire scanline is given as literal values.
			 */
			if (cc < scanline)
				goto bad;
			_TIFFmemcpy(row, bp, scanline);
			bp += scanline;
			cc -= scanline;
			break;
		case LITERALSPAN: {
			int off;
			/*
			 * The scanline has a literal span
			 * that begins at some offset.
			 */
			off = (bp[0] * 256) + bp[1];
			n = (bp[2] * 256) + bp[3];
			if (cc < 4+n)
				goto bad;
			_TIFFmemcpy(row+off, bp+4, n);
			bp += 4+n;
			cc -= 4+n;
			break;
		}
		default: {
			register int npixels = 0, grey;
			unsigned long imagewidth = tif->tif_dir.td_imagewidth;

			/*
			 * The scanline is composed of a sequence
			 * of constant color ``runs''.  We shift
			 * into ``run mode'' and interpret bytes
			 * as codes of the form <color><npixels>
			 * until we've filled the scanline.
			 */
			op = row;
			for (;;) {
				grey = (n>>6) & 0x3;
				n &= 0x3f;
				while (n-- > 0)
					SETPIXEL(op, grey);
				if (npixels >= imagewidth)
					break;
				if (cc == 0)
					goto bad;
				n = *bp++, cc--;
			}
			break;
		}
		}
	}
	tif->tif_rawcp = (tidata_t) bp;
	tif->tif_rawcc = cc;
	return (1);
bad:
	TIFFError(tif->tif_name, "NeXTDecode: Not enough data for scanline %ld",
	    (long) tif->tif_row);
	return (0);
}

int
TIFFInitNeXT(TIFF* tif, int scheme)
{
	(void) scheme;
	tif->tif_decoderow = NeXTDecode;
	tif->tif_decodestrip = NeXTDecode;
	tif->tif_decodetile = NeXTDecode;
	return (1);
}
#endif /* NEXT_SUPPORT */

/*****PackBits Compression Algorithm Support*****/
#ifdef PACKBITS_SUPPORT
/*
 * TIFF Library.
 *
 * PackBits Compression Algorithm Support
 */
#include <assert.h>
#include <stdio.h>

static int
PackBitsPreEncode(TIFF* tif, tsample_t s)
{
	(void) s;
	/*
	 * Calculate the scanline/tile-width size in bytes.
	 */
	if (isTiled(tif))
		tif->tif_data = (tidata_t) TIFFTileRowSize(tif);
	else
		tif->tif_data = (tidata_t) TIFFScanlineSize(tif);
	return (1);
}

/*
 * NB: tidata is the type representing *(tidata_t);
 *     if tidata_t is made signed then this type must
 *     be adjusted accordingly.
 */
typedef unsigned char tidata;

/*
 * Encode a run of pixels.
 */
static int
PackBitsEncode(TIFF* tif, tidata_t buf, tsize_t cc, tsample_t s)
{
	unsigned char* bp = (unsigned char*) buf;
	tidata_t op, ep, lastliteral;
	long n, slop;
	int b;
	enum { BASE, LITERAL, RUN, LITERAL_RUN } state;

	(void) s;
	op = tif->tif_rawcp;
	ep = tif->tif_rawdata + tif->tif_rawdatasize;
	state = BASE;
	lastliteral = 0;
	while (cc > 0) {
		/*
		 * Find the longest string of identical bytes.
		 */
		b = *bp++, cc--, n = 1;
		for (; cc > 0 && b == *bp; cc--, bp++)
			n++;
	again:
		if (op + 2 >= ep) {		/* insure space for new data */
			/*
			 * Be careful about writing the last
			 * literal.  Must write up to that point
			 * and then copy the remainder to the
			 * front of the buffer.
			 */
			if (state == LITERAL || state == LITERAL_RUN) {
				slop = op - lastliteral;
				tif->tif_rawcc += lastliteral - tif->tif_rawcp;
				if (!TIFFFlushData1(tif))
					return (-1);
				op = tif->tif_rawcp;
				while (slop-- > 0)
					*op++ = *lastliteral++;
				lastliteral = tif->tif_rawcp;
			} else {
				tif->tif_rawcc += op - tif->tif_rawcp;
				if (!TIFFFlushData1(tif))
					return (-1);
				op = tif->tif_rawcp;
			}
		}
		switch (state) {
		case BASE:		/* initial state, set run/literal */
			if (n > 1) {
				state = RUN;
				if (n > 128) {
					*op++ = (tidata) -127;
					*op++ = b;
					n -= 128;
					goto again;
				}
				*op++ = (tidataval_t)(-(n-1));
				*op++ = b;
			} else {
				lastliteral = op;
				*op++ = 0;
				*op++ = b;
				state = LITERAL;
			}
			break;
		case LITERAL:		/* last object was literal string */
			if (n > 1) {
				state = LITERAL_RUN;
				if (n > 128) {
					*op++ = (tidata) -127;
					*op++ = b;
					n -= 128;
					goto again;
				}
				*op++ = (tidataval_t)(-(n-1));	/* encode run */
				*op++ = b;
			} else {			/* extend literal */
				if (++(*lastliteral) == 127)
					state = BASE;
				*op++ = b;
			}
			break;
		case RUN:		/* last object was run */
			if (n > 1) {
				if (n > 128) {
					*op++ = (tidata) -127;
					*op++ = b;
					n -= 128;
					goto again;
				}
				*op++ = (tidataval_t)(-(n-1));
				*op++ = b;
			} else {
				lastliteral = op;
				*op++ = 0;
				*op++ = b;
				state = LITERAL;
			}
			break;
		case LITERAL_RUN:	/* literal followed by a run */
			/*
			 * Check to see if previous run should
			 * be converted to a literal, in which
			 * case we convert literal-run-literal
			 * to a single literal.
			 */
			if (n == 1 && op[-2] == (tidata) -1 &&
			    *lastliteral < 126) {
				state = (((*lastliteral) += 2) == 127 ?
				    BASE : LITERAL);
				op[-2] = op[-1];	/* replicate */
			} else
				state = RUN;
			goto again;
		}
	}
	tif->tif_rawcc += op - tif->tif_rawcp;
	tif->tif_rawcp = op;
	return (1);
}

/*
 * Encode a rectangular chunk of pixels.  We break it up
 * into row-sized pieces to insure that encoded runs do
 * not span rows.  Otherwise, there can be problems with
 * the decoder if data is read, for example, by scanlines
 * when it was encoded by strips.
 */
static int
PackBitsEncodeChunk(TIFF* tif, tidata_t bp, tsize_t cc, tsample_t s)
{
	tsize_t rowsize = (tsize_t) tif->tif_data;

	assert(rowsize > 0);
	while ((long)cc > 0) {
		if (PackBitsEncode(tif, bp, rowsize, s) < 0)
			return (-1);
		bp += rowsize;
		cc -= rowsize;
	}
	return (1);
}

static int
PackBitsDecode(TIFF* tif, tidata_t op, tsize_t occ, tsample_t s)
{
	char *bp;
	tsize_t cc;
	long n;
	int b;

	(void) s;
	bp = (char*) tif->tif_rawcp;
	cc = tif->tif_rawcc;
	while (cc > 0 && (long)occ > 0) {
		n = (long) *bp++, cc--;
		/*
		 * Watch out for compilers that
		 * don't sign extend chars...
		 */
		if (n >= 128)
			n -= 256;
		if (n < 0) {		/* replicate next byte -n+1 times */
			if (n == -128)	/* nop */
				continue;
			n = -n + 1;
			occ -= n;
			b = *bp++, cc--;
			while (n-- > 0)
				*op++ = b;
		} else {		/* copy next n+1 bytes literally */
			_TIFFmemcpy(op, bp, ++n);
			op += n; occ -= n;
			bp += n; cc -= n;
		}
	}
	tif->tif_rawcp = (tidata_t) bp;
	tif->tif_rawcc = cc;
	if (occ > 0) {
		TIFFError(tif->tif_name,
		    "PackBitsDecode: Not enough data for scanline %ld",
		    (long) tif->tif_row);
		return (0);
	}
	/* check for buffer overruns? */
	return (1);
}

int
TIFFInitPackBits(TIFF* tif, int scheme)
{
	(void) scheme;
	tif->tif_decoderow = PackBitsDecode;
	tif->tif_decodestrip = PackBitsDecode;
	tif->tif_decodetile = PackBitsDecode;
	tif->tif_preencode = PackBitsPreEncode;
	tif->tif_encoderow = PackBitsEncode;
	tif->tif_encodestrip = PackBitsEncodeChunk;
	tif->tif_encodetile = PackBitsEncodeChunk;
	return (1);
}
#endif /* PACKBITS_SUPPORT */

