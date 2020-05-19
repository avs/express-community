/**********************************************************************
 *
 *  geo_tiffp.c  Private TIFF interface module for GEOTIFF
 *
 *    This module implements the interface between the GEOTIFF
 *    tag parser and the TIFF i/o module. The current setup
 *    relies on the "libtiff" code, but if you use your own
 *    TIFF reader software, you may replace the module implementations
 *    here with your own calls. No "libtiff" dependencies occur
 *    anywhere else in this code.
 *
 **********************************************************************/
 
#include <string.h>
#include <stdio.h>     /* for sprintf             */
#include "geotiff.h"    /* public GTIFF interface */


#include "geo_tiffp.h"  /* Private TIFF interface */

/* tiff size array global */
gsize_t _gtiff_size[] = { 0, 1, 2, 4, 8, 1, 4, 8, 1, 2, 4, 1 };

static int        _GTIFGetField (tiff_t *tif, pinfo_t tag, int *count, void *value );
static int        _GTIFSetField (tiff_t *tif, pinfo_t tag, int  count, void *value );
static tagtype_t  _GTIFTagType  (tiff_t *tif, pinfo_t tag);

/*
 * Set up default TIFF handlers. 
 */
void _GTIFSetDefaultTIFF(TIFFMethod *method)
{
	if (!method) return;
	
	method->get = _GTIFGetField;
	method->set = _GTIFSetField;
	method->type = _GTIFTagType;
}

gdata_t _GTIFcalloc(gsize_t size)
{
    gdata_t data=(gdata_t)_TIFFmalloc((tsize_t)size);
	if (data) _TIFFmemset((tdata_t)data,0,(tsize_t)size);
	return data;
}

void _GTIFmemcpy(gdata_t out,gdata_t in,gsize_t size)
{
	_TIFFmemcpy((tdata_t)out,(tdata_t)in,(tsize_t)size);
}

void _GTIFFree(gdata_t data)
{
	if (data) _TIFFfree((tdata_t)data);
}



/* returns the value of TIFF tag <tag>, or if
 * the value is an array, returns an allocated buffer
 * containing the values. Allocate a copy of the actual
 * buffer, sized up for updating.
 */
static int _GTIFGetField (tiff_t *tif, pinfo_t tag, int *count, void *val )
{
	int status;
	unsigned short scount=0;
	char *tmp;
	char *value;
	gsize_t size = _gtiff_size[_GTIFTagType (tif,tag)];
	
	if (_GTIFTagType(tif,  tag) == TYPE_ASCII)
	{
		status = TIFFGetField((TIFF *)tif,tag,&tmp);
		if (!status) return status;
		scount = strlen(tmp)+1;
	}
	else status = TIFFGetField((TIFF *)tif,tag,&scount,&tmp);
	if (!status) return status;
	
	*count = scount;

	value = (char *)_GTIFcalloc( (scount+MAX_VALUES)*size);
	if (!value) return 0;
	
	_TIFFmemcpy( value, tmp,  size * scount);
	
	*(char **)val = value;
	return status;
}

/* 
 * Set a GeoTIFF TIFF field.
 */
static int _GTIFSetField (tiff_t *tif, pinfo_t tag, int count, void *value )
{
	int status;
	unsigned short scount = count;

	/* libtiff ASCII uses null-delimiter */
	if (_GTIFTagType(tif,  tag) == TYPE_ASCII)
		status = TIFFSetField((TIFF *)tif,tag,value);
	else 
		status = TIFFSetField((TIFF *)tif,tag,scount,value);
	return status;
}


/*
 *  This routine is supposed to return the TagType of the <tag>
 *  TIFF tag. Unfortunately, "libtiff" does not provide this
 *  service by default, so we just have to "know" what type of tags
 *  we've got, and how many. We only define the ones Geotiff
 *  uses here, and others return UNKNOWN. The "tif" parameter
 *  is provided for those TIFF implementations that provide
 *  for tag-type queries.
 */
static tagtype_t  _GTIFTagType  (tiff_t *tif, pinfo_t tag)
{
	tagtype_t ttype;

	(void) tif; /* dummy reference */
	
	switch (tag)
	{
		case GTIFF_ASCIIPARAMS:    ttype=TYPE_ASCII; break;
		case GTIFF_PIXELSCALE:
		case GTIFF_TRANSMATRIX:
		case GTIFF_TIEPOINTS:
		case GTIFF_DOUBLEPARAMS:   ttype=TYPE_DOUBLE; break;
		case GTIFF_GEOKEYDIRECTORY: ttype=TYPE_SHORT; break;
		default: ttype = TYPE_UNKNOWN;
	}
	
	return ttype;
}


/* return the Header info of this geotiff file */

void GTIFDirectoryInfo(GTIF *gtif, int version[3], int *keycount)
{
        if (version)
        {
                version[0]  = gtif->gt_version;
                version[1]  = gtif->gt_rev_major;
                version[2]  = gtif->gt_rev_minor;
        }
        if (keycount) *keycount = gtif->gt_num_keys;
}


int GTIFKeyInfo(GTIF *gtif, geokey_t key, int *size, tagtype_t* type)
{
        int index = gtif->gt_keyindex[ key ];
        GeoKey *keyptr;

        if (!index) return 0;

        keyptr = gtif->gt_keys + index;
        if (size) *size = (int) keyptr->gk_size;
        if (type) *type = keyptr->gk_type;

        return keyptr->gk_count;
}


/*
 * Get <count> values of Key <key>, starting with the <index>'th value.
 */
int GTIFKeyGet(GTIF *gtif, geokey_t thekey, void *val, int index, int count)
{
        int kindex = gtif->gt_keyindex[ thekey ];
        GeoKey *key;
        gsize_t size;
        char *data;
        tagtype_t type;

        if (!kindex) return 0;

        key = gtif->gt_keys+kindex;
        if (!count) count = key->gk_count - index;
        if (count <=0) return 0;
        if (count > key->gk_count) count = key->gk_count;
        size = key->gk_size;
        type = key->gk_type;

        if (count==1 && type==TYPE_SHORT) data = (char *)&key->gk_data;
        else data = key->gk_data;

        _GTIFmemcpy( val, data + index*size, count*size );

        if (type==TYPE_ASCII)
           ((char *)val)[count-1] = '\0'; /* replace last char with NULL */

        return count;
}

int GTIFKeySet(GTIF *gtif, geokey_t keyID, tagtype_t type, int count,...)
{
	va_list ap;
	int index = gtif->gt_keyindex[ keyID ];
	int newvalues = 0;
	GeoKey *key;
	char *data;
	char *val;
	pinfo_t sval;
	double dval;

	va_start(ap, count);
	/* pass singleton keys by value */
	if (count>1 && type!=TYPE_ASCII) val = va_arg(ap, char*);
	else switch (type)
	{
	    case TYPE_SHORT:  sval=va_arg(ap, int); val=(char *)&sval;     break;
	    case TYPE_DOUBLE: dval=va_arg(ap, dblparam_t); val=(char *)&dval;  break;
	    case TYPE_ASCII: 
			val=va_arg(ap, char*);
			count = strlen(val) + 1; /* force = string length */
			break;
	}
	va_end(ap);
	
	/* We assume here that there are no multi-valued SHORTS ! */
	if (index)
	{
		/* Key already exists */
		key = gtif->gt_keys+index;
		if (type!=key->gk_type || count > key->gk_count)
		{
			/* need to reset data pointer */
			key->gk_type = type;
			key->gk_count = count;
			key->gk_size = _gtiff_size[ type ];
			newvalues = 1;
		}
	}
	else
	{
		/* We need to create the key */
		if (gtif->gt_num_keys == MAX_KEYS) return 0;
		key = gtif->gt_keys + ++gtif->gt_num_keys;
		index = gtif->gt_num_keys;
		gtif->gt_keyindex[ keyID ] = index;
		key->gk_key = keyID;
		key->gk_type = type;
		key->gk_count = count;
		key->gk_size = _gtiff_size[ type ];
		if (gtif->gt_keymin > keyID)  gtif->gt_keymin=keyID;
		if (gtif->gt_keymax < keyID)  gtif->gt_keymax=keyID;
		newvalues = 1;
	}

	if (newvalues)
	{
	   switch (type)
	   {
	    case TYPE_SHORT:  
			if (count > 1) return 0;
			data = (char *)&key->gk_data; /* store value *in* data */
			break;
	    case TYPE_DOUBLE:
			key->gk_data = (char *)(gtif->gt_double + gtif->gt_ndoubles);
			data = key->gk_data;
			gtif->gt_ndoubles += count;
			break;
	    case TYPE_ASCII:
			key->gk_data = (char *)(gtif->gt_ascii + gtif->gt_nascii);
			data = key->gk_data;
			gtif->gt_nascii += count;
			data[--count] = '|'; /* replace NULL with '|' */
			break;
	    default:
			va_end(ap);
	    	return 0;
	    	break;
	  }
	   gtif->gt_nshorts += sizeof(KeyEntry)/sizeof(pinfo_t);
	}

	
	_GTIFmemcpy(data, val, count*key->gk_size);
	
	gtif->gt_flags |= FLAG_FILE_MODIFIED;
	return 1;
}

/**********************************************************************
 *
 *                        Private Routines
 *
 **********************************************************************/

/*
 * Given KeyEntry, read in the GeoKey value location and set up
 *  the Key structure, returning 0 if failure.
 */

static int ReadKey(GTIF* gt, KeyEntry* entptr,GeoKey* keyptr)
{
	int offset,count;
	
	keyptr->gk_key = entptr->ent_key;
	keyptr->gk_count = entptr->ent_count;
	count = entptr->ent_count;
	offset = entptr->ent_val_offset;
	if (gt->gt_keymin > keyptr->gk_key)  gt->gt_keymin=keyptr->gk_key;
	if (gt->gt_keymax < keyptr->gk_key)  gt->gt_keymax=keyptr->gk_key;
	
	if (entptr->ent_location)
	  keyptr->gk_type = (gt->gt_methods.type)(gt->gt_tif,entptr->ent_location);
	else
	  keyptr->gk_type = (gt->gt_methods.type)(gt->gt_tif,GTIFF_GEOKEYDIRECTORY);
	  
	switch (entptr->ent_location)
	{
		case GTIFF_LOCAL:
			/* store value into data value */
			*(pinfo_t *)(&keyptr->gk_data) = entptr->ent_val_offset;
			break;
		case GTIFF_GEOKEYDIRECTORY:
			keyptr->gk_data = (char *)(gt->gt_short+offset);
			if (gt->gt_nshorts < offset+count)
				gt->gt_nshorts = offset+count;
			break;
		case GTIFF_DOUBLEPARAMS:
			keyptr->gk_data = (char *)(gt->gt_double+offset);
			if (gt->gt_ndoubles < offset+count)
				gt->gt_ndoubles = offset+count;
			break;
		case GTIFF_ASCIIPARAMS:
			keyptr->gk_data = (char *)(gt->gt_ascii+offset);
			if (gt->gt_nascii < offset+count)
				gt->gt_nascii = offset+count;
			break;
		default:
			return 0; /* failure */
			break;
	}
	keyptr->gk_size = _gtiff_size[keyptr->gk_type];
	
	return 1; /* success */
}

/**********************************************************************
 *
 *                        Public Routines
 *
 **********************************************************************/


/*
 * Given an open TIFF file, look for GTIF keys and 
 *  values and return GTIF structure. The TIFF file is
 *  declared "void" to allow TIFF handlers other than libtiff to operate.
 */
 
GTIF* GTIFNew(void *tif)
{
	GTIF* gt=(GTIF*)0;
	int status=1;
	int count,bufcount,index;
	GeoKey *keyptr;
	pinfo_t *data;
	KeyEntry *entptr;
	KeyHeader *header;
	
	gt = (GTIF*)_GTIFcalloc( sizeof(GTIF));
	if (!gt) goto failure;	
	
	/* install TIFF file and I/O methods */
	gt->gt_tif = (tiff_t *)tif;
	_GTIFSetDefaultTIFF(&gt->gt_methods);
	
	/* since this is an array, GTIF will allocate the memory */
	if (!(gt->gt_methods.get)((tiff_t *)tif, GTIFF_GEOKEYDIRECTORY, &gt->gt_nshorts, &data ))
	{
		/* No ProjectionInfo, create a blank one */
		data=(pinfo_t*)_GTIFcalloc((4+MAX_VALUES)*sizeof(pinfo_t));
		if (!data) goto failure;	
		header = (KeyHeader *)data;
		header->hdr_version = GvCurrentVersion;
		header->hdr_rev_major = GvCurrentRevision;
		header->hdr_rev_minor = GvCurrentMinorRev;
		gt->gt_nshorts=sizeof(KeyHeader)/sizeof(pinfo_t);
	}
	gt->gt_short = data;
	header = (KeyHeader *)data;
	
	if (header->hdr_version > GvCurrentVersion) goto failure;
	if (header->hdr_rev_major > GvCurrentRevision)
	{
		/* issue warning */
	}
	
	/* If we got here, then the geokey can be parsed */
	count = header->hdr_num_keys;
	gt->gt_num_keys = count;
	gt->gt_version  = header->hdr_version;
	gt->gt_rev_major  = header->hdr_rev_major;
	gt->gt_rev_minor  = header->hdr_rev_minor;

	bufcount = count+MAX_KEYS; /* allow for expansion */

	/* Get the PARAMS Tags, if any */
	if (!(gt->gt_methods.get)((tiff_t *)tif, GTIFF_DOUBLEPARAMS, &gt->gt_ndoubles, &gt->gt_double ))
	{
		gt->gt_double=(double*)_GTIFcalloc(MAX_VALUES*sizeof(double));
		if (!gt->gt_double) goto failure;	
	}
	if (!(gt->gt_methods.get)((tiff_t *)tif, GTIFF_ASCIIPARAMS, &gt->gt_nascii, &gt->gt_ascii ))
	{
		gt->gt_ascii = (char*)_GTIFcalloc(MAX_VALUES*sizeof(char));
		if (!gt->gt_ascii) goto failure;
	}
	else  gt->gt_nascii--; /* last NULL doesn't count; "|" used for delimiter */

	/* allocate space for GeoKey array and its index */
	gt->gt_keys = (GeoKey *)_GTIFcalloc( sizeof(GeoKey)*bufcount);
	if (!gt->gt_keys) goto failure;
	gt->gt_keyindex = (int *)_GTIFcalloc( sizeof(int)*(MAX_KEYINDEX+1));
	if (!gt->gt_keyindex) goto failure;
	
	/*  Loop to get all GeoKeys */
	entptr = ((KeyEntry *)data) + 1;
	keyptr = gt->gt_keys;
	gt->gt_keymin = MAX_KEYINDEX;
	gt->gt_keymax = 0;
	for (index=1; index<=count; index++,entptr++)
	{
		if (!ReadKey(gt, entptr, ++keyptr))
			goto failure;
			
		/* Set up the index (start at 1, since 0=unset) */
		gt->gt_keyindex[entptr->ent_key] = index;		
	}
	
	return gt;
	
failure:
	/* Notify of error */
	if (gt) free(gt);
	return (GTIF *)0;
}

void GTIFFree(GTIF* gtif)
{
	if (!gtif) return;
	
	/* Free parameter arrays */
	if (gtif->gt_ascii) _GTIFFree (gtif->gt_ascii);
	if (gtif->gt_double) _GTIFFree (gtif->gt_double);
	if (gtif->gt_short) _GTIFFree (gtif->gt_short);
	
	/* Free GeoKey arrays */
	if (gtif->gt_keys) _GTIFFree (gtif->gt_keys);
	if (gtif->gt_keyindex) _GTIFFree (gtif->gt_keyindex);
	
	_GTIFFree (gtif);
}

