/*
 * Image to Postscript Module for AVS/Express 3.1,
 * John Sheehan, Terry Martin, Advanced Visual Systems, Inc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xp_comm_proj/wr_ps/gen.h"

typedef struct
{
   float x, y;
} Point2D, Vector2D;


static int RLEEncode(void *inBuf, int nVals, int stride,
		     unsigned char *rleBuf);
static int ASCII85Encode(unsigned char *inBuf, int inBufSize,
			 char *ascii85Buf);
static int WriteProlog(FILE *fp, const char *user, const char *title,
		       int portrait);
static int WriteSetup(FILE *fp);
static int WritePageHeader(FILE *fp, int pageNum);
static int WriteTrailer(FILE *fp);

static int RLDecode(unsigned char *inBuf, int dataSize, void *outBuf);

static int StrideData(void *inBuf, int nVals, int stride,
		      unsigned char *rleBuf);


int
FLDtoPS(OMobj_id Image_to_PS_id, OMevent_mask event_mask, int seq_num)
{
   OMobj_id image_id;
   int image_veclen;
   int image_data_type;
   int image_ndata;
   int nRows = 0;
   int nCols = 0;
   int image_ndim;
   int *image_dims;
   unsigned char *image_data;
   char *file_name = NULL;

   image_id = OMfind_subobj(Image_to_PS_id, OMstr_to_name("image_in"),
			    OM_OBJ_RD);

   FLDget_node_data_veclen(image_id, 0, &image_veclen);

   if (FLDget_dims(image_id, &image_dims, &image_ndim) == OM_STAT_SUCCESS)
   {
      if (image_dims)
      {
	 nCols = image_dims[0];
	 nRows = image_dims[1];

	 ARRfree(image_dims);
      }
   }

   FLDget_node_data(image_id, 0, &image_data_type, (char **)&image_data,
                    &image_ndata, OM_GET_ARRAY_RD);

   if (OMget_name_str_val(Image_to_PS_id,
			  OMstr_to_name("file_name"), &file_name, 0) != 1)
      file_name = NULL;


   if (image_data)
   {
      if (file_name && (nRows > 0) && (nCols > 0))
      {
	 FILE *fp = fopen(file_name, "wt");

	 FILE *xz = fopen("strides", "wt");

	 if (fp)
	 {
	    unsigned char *rleBuf;
	    char *asc85Buf;
	    Vector2D offset;
	    Vector2D scale;

unsigned char dbugBuf[2048];

	    offset.x = 17.0;
	    offset.y = 17.0;

	    scale.x = 0.24;
	    scale.y = 0.24;

	    WriteProlog(fp, "", "FLD to PostScript", 1);
	    WriteSetup(fp);
	    WritePageHeader(fp, 1);

	    fprintf(fp, "/rstr %d string def\n", nCols);
	    fprintf(fp, "/gstr %d string def\n", nCols);
	    fprintf(fp, "/bstr %d string def\n", nCols);

	    rleBuf = (unsigned char *)malloc(nCols * 2);
	    asc85Buf = (char *)malloc(nCols * 2);

	    if (rleBuf && asc85Buf)
	    {
	       int r;
	       int outSize;
	       int rowWidth = nCols * image_veclen;
	       unsigned char *cp;

	       for (r = 0; r < nRows; r++)
	       {
		  fprintf(fp, "S\n241 %d tr 1925 4 sc\n", 279 + (r*4));
		  fprintf(fp, "%d 1 8 [%d 0 0 1 0 0]\n", nCols, nCols);
		  fprintf(fp, "{cf /ASCII85Decode fl ");
		  fprintf(fp, "rstr rds pop}\n");
		  fprintf(fp, "{cf /ASCII85Decode fl ");
		  fprintf(fp, "gstr rds pop}\n");
		  fprintf(fp, "{cf /ASCII85Decode fl ");
		  fprintf(fp, "bstr rds pop}\n");
		  fprintf(fp, "true 3 ci\n");

		  cp = image_data + (r * rowWidth);

if (xz)
{
   unsigned char *zx = cp;
   int q;

   fprintf(xz, "\n--- alpha channel, row %d ---------------------\n", r);
   for (q = 0; q < nCols; q++, zx += 4)
   {
      if ((q % 8) == 0)
	 fprintf(xz, "\n");

      fprintf(xz, "%4u ", (unsigned)(*zx));
   }

   fprintf(xz, "\n--- red channel, row %d -----------------------\n", r);
   for (zx = cp + 1, q = 0; q < nCols; q++, zx += 4)
   {
      if ((q % 8) == 0)
	 fprintf(xz, "\n");

      fprintf(xz, "%4u ", (unsigned)(*zx));
   }

   fprintf(xz, "\n--- green channel, row %d ---------------------\n", r);
   for (zx = cp + 2, q = 0; q < nCols; q++, zx += 4)
   {
      if ((q % 8) == 0)
	 fprintf(xz, "\n");

      fprintf(xz, "%4u ", (unsigned)(*zx));
   }

   fprintf(xz, "\n--- blue channel, row %d ----------------------\n", r);
   for (zx = cp + 3, q = 0; q < nCols; q++, zx += 4)
   {
      if ((q % 8) == 0)
	 fprintf(xz, "\n");

      fprintf(xz, "%4u ", (unsigned)(*zx));
   }
   fprintf(xz, "\n\n*****************************************\n\n");
}


/*		  outSize = RLEEncode(cp + 1, nCols, image_veclen, rleBuf);

		  if (RLDecode(rleBuf, outSize, dbugBuf))
		  {
		     unsigned char *x = cp + 1;
		     unsigned char *y = dbugBuf;
		     int q;

		     for (q = 0; q < nCols; q++, x += 4, y++)
			if (*x != *y)
			{
			   printf("RLDecode mismatch for row %d red!\n", r);
			   break;
			}
		  }
		  else
		     printf("RLDecode failed for row %d red!\n", r);
*/
		  outSize = StrideData(cp + 1, nCols, image_veclen, rleBuf);
		  ASCII85Encode(rleBuf, outSize, asc85Buf);

		  fprintf(fp, "%s\n", asc85Buf);

/*		  outSize = RLEEncode(cp + 2, nCols, image_veclen, rleBuf);

		  if (RLDecode(rleBuf, outSize, dbugBuf))
		  {
		     unsigned char *x = cp + 2;
		     unsigned char *y = dbugBuf;
		     int q;

		     for (q = 0; q < nCols; q++, x += 4, y++)
			if (*x != *y)
			{
			   printf("RLDecode mismatch for row %d green!\n", r);
			   break;
			}
		  }
		  else
		     printf("RLDecode failed for row %d green!\n", r);
*/
		  outSize = StrideData(cp + 2, nCols, image_veclen, rleBuf);
		  ASCII85Encode(rleBuf, outSize, asc85Buf);

		  fprintf(fp, "%s\n", asc85Buf);

/*		  outSize = RLEEncode(cp + 3, nCols, image_veclen, rleBuf);

		  if (RLDecode(rleBuf, outSize, dbugBuf))
		  {
		     unsigned char *x = cp + 3;
		     unsigned char *y = dbugBuf;
		     int q;

		     for (q = 0; q < nCols; q++, x += 4, y++)
			if (*x != *y)
			{
			   printf("RLDecode mismatch for row %d blue!\n", r);
			   break;
			}
		  }
		  else
		     printf("RLDecode failed for row %d blue!\n", r);
*/
		  outSize = StrideData(cp + 3, nCols, image_veclen, rleBuf);
		  ASCII85Encode(rleBuf, outSize, asc85Buf);

		  fprintf(fp, "%s\nR\n", asc85Buf);
	       }

	       OMset_name_int_val(Image_to_PS_id, OMstr_to_name("done"), 1);
	    }

	    WriteTrailer(fp);
	    fclose(fp);

	    if (xz) fclose(xz);
	 }
      }

      ARRfree(image_data);
   }

   if (file_name)
      free(file_name);

   return 1;
}


static int RLEEncode(void *inBuf, int nVals, int stride,
                     unsigned char *out)
{
   /* The run length encoding scheme used by PostScript simply prepends
    * runs of similar or dissimilar bytes by a count.  Specifically:
    *
    * A run of N dissimilar characters, where N is in [1, 128], is
    * preceded by a count of N-1.
    * A run of N similar characters, where N is in [2, 128], is preceded
    * by a count of 257-N.
    * The final byte in the data set is a 128 EOD marker.
    */
   int numOut = 0;
   int numRLCounts = 0;
   short runKind = 0;     /* 0 = initial, 1 = different, 2 = same */
   unsigned char previous;
   unsigned char count;
   unsigned char *in = (unsigned char *)inBuf;
   unsigned char *countPtr;

   if (nVals < 1)   /* Handle this trivial case separately */
      return 0;

   /* Set up some variables for the loop that performs the run length
    * encoding.  Because we're comparing two values (in order to detect
    * runs of similar or dissimilar entries), we run through every element
    * in the input buffer, retaining a pointer to the preceding entry.
    */
   previous = *in;
   in += stride;
   --nVals;

   countPtr = out++;    /* leave room for the first count */
   count = 0;
   ++numRLCounts;

   while (nVals)    /* perform run length encoding on all input data values */
   {
      if (previous == (*in))
      {
         /* A run of two or more entries of the same value has been
          * detected.  This may be a continuation of a longer run, or it
          * may represent the end of a run of dissimilar values.
          */
         if (runKind == 1)
	 {
            /* This new run of two similar values terminates a run of
             * dissimilar values.  We need to put the run length count
             * into the output buffer, reset the count for our new
             * run of similar values, and reserve space in the output
             * buffer for our count of similar values.
             * In case that this is a second run of similar values
             * immediately following another run, the count will be 0.
             * If so, we don't want to write out the count.
             */
            if (count > 0)
	    {
               *countPtr = count - 1;
               count = 0;

               countPtr = out++;
               ++numRLCounts;
	     }
	  }

         if (count < 127)
	   {
            if (count == 0)
	      {
               /* If this is the first entry in a string of similar values,
                * we need to write one instance of the value to the output
                * buffer.
                */
               *out++ = previous;
               ++numOut;
	     }

            /* The "count" for similar values is actually one less than the
             * number of similar values encountered.  This is convenient,
             * as it extents the maximum length of a run by 1 element, while
             * introducing no extra overhead.
             */
            ++count;
            runKind = 2;
	  }
         else   /* our run of similar values is too long */
	   {
            /* Because unsigned chars (bytes) are being used to hold the
             * run lengths, we are limited to runs of 128 bytes.
             * Runs longer than this have to be split apart.
             */
            *countPtr = (unsigned char)(256 - count);
            count = 0;

            countPtr = out++;
            ++numRLCounts;

            runKind = 0;
	  }
       }
      else   /* the two most recent entries are different */
	{
         /* A run of two or more entries with different values has been
          * detected.  This may be a continuation of a longer run, or it
          * may represent the end of a run of same values.
          */
         if (runKind == 2)
	   {
            /* This new run of two dissimilar values terminates a run of
             * same values.  We need to put the run length count into the
             * output buffer, reset the count for our new run of dissimilar
             * values, and reserve space in the output buffer for our count
             * of dissimilar values.
             */
            if (count > 0)
	      {
               *countPtr = (unsigned char)(256 - count);
               count = 0;

               countPtr = out++;
               ++numRLCounts;
	     }
	  }
         else   /* continuing an existing run */
	   {
            /* This is a continuation of an existing run of dissimilar
             * values.  Because unsigned chars (bytes) are being used to
             * hold the run lengths, we are limited to runs of 128 bytes.
             * Runs longer than this have to be split apart.
             */
            if (count >= 128)
	      {
               *countPtr = count - 1;
               count = 0;

               countPtr = out++;
               ++numRLCounts;

               runKind = -1;
	     }

            /* Unlike same-value runs, we need to explicitly store in the
             * output buffer each value in the run.
             */
            *out++ = previous;
            ++numOut;
            ++count;
	  }

         runKind = (runKind < 0) ? 0 : 1;
       }

      previous = *in;      /* prepare for the next loop iteration */
      in += stride;
      --nVals;
    }

   /* The preceding loop terminates without completing the final run, so
    * we have to clean up a little here.  If the run was of same values,
    * we just need to put the final count in to the output buffer.  If it
    * was of dissimilar values, we need to put the final count and the
    * final value into the output buffer.
    */
   if (runKind == 2)
      *countPtr = 256 - count;
   else
     {
      *countPtr = count;
      *out++ = previous;
      ++numOut;
    }

   /* As required by the PostScript RLE algorithm, we have to add an EOD
    * (End Of Data) marker to the tail of the buffer.  This is a value
    * of 128.
    */
   *out = 128;

   /* The final size of the output buffer is the number of run length
    * bytes (accumulated in "numRLCounts") plus the number of bytes
    * written to the output buffer plus 1 for the 128 EOD marker.
    */
   return numRLCounts + numOut + 1;
 }



#ifdef HFIENCSKLJDHF

static int RLEEncode(void *inBuf, int nVals, int stride,
		     unsigned char *out)
{
   /* The run length encoding scheme used by PostScript simply prepends
    * runs of similar or dissimilar bytes by a count.  Specifically:
    *
    * A run of N dissimilar characters, where N is in [1, 128], is
    * preceded by a count of N-1.
    * A run of N similar characters, where N is in [2, 128], is preceded
    * by a count of 257-N.
    * The final byte in the data set is a 128 EOD marker.
    */
   int numOut = 0;
   int numRLCounts = 0;
   short runKind = 0;     /* 0 = initial, 1 = different, 2 = same */
   unsigned char previous;
   unsigned char count;
   unsigned char *in = (unsigned char *)inBuf;
   unsigned char *countPtr;

   if (nVals < 1)   /* Handle this trivial case separately */
      return 0;

   /* Set up some variables for the loop that performs the run length
    * encoding.  Because we're comparing two values (in order to detect
    * runs of similar or dissimilar entries), we run through every element
    * in the input buffer, retaining a pointer to the preceding entry.
    */
   previous = *in;
   in += stride;
   --nVals;

   countPtr = out++;    /* leave room for the first count */
   count = 0;
   ++numRLCounts;

   while (nVals)    /* perform run length encoding on all input data values */
   {
      if (previous == (*in))
      {
	 /* A run of two or more entries of the same value has been
	  * detected.  This may be a continuation of a longer run, or it
	  * may represent the end of a run of dissimilar values.
	  */
	 if (count < 127)
	 {
	    if (runKind == 1)
	    {
	       /* This new run of two similar values terminates a run of
		* dissimilar values.  We need to put the run length count
		* into the output buffer, reset the count for our new
		* run of similar values, and reserve space in the output
		* buffer for our count of similar values.
		* In case that this is a second run of similar values
		* immediately following another run, the count will be 0.
		* If so, we don't want to write out the count.
		*/
	       if (count > 0)
	       {
		  *countPtr = count - 1;
		  count = 0;

		  countPtr = out++;
		  ++numRLCounts;
	       }
	    }

	    if (count == 0)
	    {
	       /* If this is the first entry in a string of similar values,
		* we need to write one instance of the value to the output
		* buffer.
		*/
	       *out++ = previous;
	       ++numOut;
	    }

	    /* The "count" for similar values is actually one less than the
	     * number of similar values encountered.  This is convenient,
	     * as it extents the maximum length of a run by 1 element, while
	     * introducing no extra overhead.
	     */
	    ++count;
	    runKind = 2;
	 }
	 else   /* our run of similar values is too long */
	 {
	    /* Because unsigned chars (bytes) are being used to hold the
	     * run lengths, we are limited to runs of 128 bytes.
	     * Runs longer than this have to be split apart.
	     */
	    *countPtr = (unsigned char)(256 - count);
	    count = 0;

	    countPtr = out++;
	    ++numRLCounts;

	    runKind = 0;
	 }
      }
      else   /* the two most recent entries are different */
      {
	 /* A run of two or more entries with different values has been
	  * detected.  This may be a continuation of a longer run, or it
	  * may represent the end of a run of same values.
	  */
	 if (runKind == 2)
	 {
	    /* This new run of two dissimilar values terminates a run of
	     * same values.  We need to put the run length count into the
	     * output buffer, reset the count for our new run of dissimilar
	     * values, and reserve space in the output buffer for our count
	     * of dissimilar values.
	     */
	    if (count > 0)
	    {
	       *countPtr = (unsigned char)(256 - count);
	       count = 0;

	       countPtr = out++;
	       ++numRLCounts;
	    }
	 }
	 else   /* continuing an existing run */
	 {
	    /* This is a continuation of an existing run of dissimilar
	     * values.  Because unsigned chars (bytes) are being used to
	     * hold the run lengths, we are limited to runs of 128 bytes.
	     * Runs longer than this have to be split apart.
	     */
	    if (count >= 128)
	    {
	       *countPtr = count - 1;
	       count = 0;

	       countPtr = out++;
	       ++numRLCounts;
	    }

	    /* Unlike same-value runs, we need to explicitly store in the
	     * output buffer each value in the run.
	     */
	    *out++ = previous;
	    ++numOut;
	    ++count;
	 }

	 runKind = 1;
      }

      previous = *in;      /* prepare for the next loop iteration */
      in += stride;
      --nVals;
   }

   /* The preceding loop terminates without completing the final run, so
    * we have to clean up a little here.  If the run was of same values,
    * we just need to put the final count in to the output buffer.  If it
    * was of dissimilar values, we need to put the final count and the
    * final value into the output buffer.
    */
   if (runKind == 2)
      *countPtr = 256 - count;
   else
   {
      *countPtr = count;
      *out++ = previous;
      ++numOut;
   }

   /* As required by the PostScript RLE algorithm, we have to add an EOD
    * (End Of Data) marker to the tail of the buffer.  This is a value
    * of 128.
    */
   *out = 128;

   /* The final size of the output buffer is the number of run length
    * bytes (accumulated in "numRLCounts") plus the number of bytes
    * written to the output buffer plus 1 for the 128 EOD marker.
    */
   return numRLCounts + numOut + 1;
}

#endif



static int ASCII85Encode(unsigned char *inBuf, int inBufSize,
			 char *ascii85Buf)
{
   /* The "ASCII85" filter used by PostScript encodes binary information
    * as ASCII using the following scheme:
    *
    *  Each group of 4 bytes is treated as a single base-256 number.
    *  This number is represented in base-85, using the characters
    *  '!' through 'u' (inclusive).
    *  If all 4 bytes are 0, then this is treated as a special case:
    *  the character 'z' is written instead of "!!!!!".
    *  If there are not 4 bytes remaining in the data set, it is
    *  padded with 0 bytes and then encoded normally, except that the
    *  special 'z' case described above is not applied.  The first
    *  n+1 bytes (where n is the number, less than 4, of bytes encoded--
    *  must be 1, 2, or 3) of this encoded number are written to the
    *  output.
    *  To terminate the output, the character sequence "~>" is used.
    */
   int i;
   int n;
   int grp = 0;
   int remaining = inBufSize;
   unsigned char *cp = inBuf;
   char word[5];
   union _val
   {
      char c[4];
      unsigned i;
   } val;

   while (remaining > 0)
   {
      for (i = 0; (i < 4) && (i < remaining); i++)
      {
#if defined(ALPHA) || defined(MSDOS)
	 val.c[3 - i] = *cp++;    /* Little-endian order */
#else
	 val.c[i] = *cp++;    /* Big-endian order */
#endif
      }

      for ( ; i < 4; i++)   /* pad in case remaining < 4 */
      {
#if defined(ALPHA) || defined(MSDOS)
	 val.c[3 - i] = 0;    /* Little-endian order */
#else
	 val.c[i] = 0;    /* Big-endian order */
#endif
      }

      /* Now convert this number to base-85, and offset it up to the
       * '!' character in ASCII.  Note that 0 is a special case, unless
       * we're down to the last < 4 bytes.
       */
      if ((val.i == 0) && (remaining >= 4))
      {
	 *ascii85Buf++ = 'z';    /* 0 is a special case */
      }
      else
      {
	 int nOut = remaining + 1;

	 for (i = 0; i < 5; i++)
	 {
	    word[i] = (char)(val.i % 85) + '!';
	    val.i /= 85;
	 }

	 for (i = 0; (i < 5) && (i < nOut); i++)
	    *ascii85Buf++ = word[4 - i];
      }

      remaining -= 4;
      if ((++grp % 15) == 0)
	 *ascii85Buf++ = '\n';
   }

   *ascii85Buf++ = '~';
   *ascii85Buf++ = '>';
   *ascii85Buf = '\0';

   return 1;
}


static int WriteProlog(FILE *fp, const char *user, const char *title,
		       int portrait)
{
   fprintf(fp, "%%!PS-Adobe-3.0\n");
   fprintf(fp, "%%%%BoundingBox: 17 17 594 774\n");
   fprintf(fp, "%%%%Creator: AVS/Express 3.1 Field_to_PS v1.0\n");  
   fprintf(fp, "%%%%For: %s\n", user);
   fprintf(fp, "%%%%Title: %s\n", title);
   fprintf(fp, "%%%%CreationDate: \n");
   fprintf(fp, "%%%%DocumentData: Clean7Bit\n");
   fprintf(fp, "%%%%LanguageLevel: 2\n");
   fprintf(fp, "%%%%DocumentNeededResources: (atend)\n");
   fprintf(fp, "%%%%Orientation: %s\n", portrait ? "Portrait" : "Landscape");
   fprintf(fp, "%%%%Pages: (atend)\n");
   fprintf(fp, "%%%%EndComments\n");
   fprintf(fp, "%%%%BeginProlog\n");
   fprintf(fp, "/bdef {bind def} bind def\n");
   fprintf(fp, "/ldef {load def} bdef\n");
   fprintf(fp, "/a /arc ldef\n");
   fprintf(fp, "/cf /currentfile ldef\n");
   fprintf(fp, "/ci /colorimage ldef\n");
   fprintf(fp, "/clw /currentlinewidth ldef\n");
   fprintf(fp, "/cp /closepath ldef\n");
   fprintf(fp, "/f /eofill ldef\n");
   fprintf(fp, "/fl /filter ldef\n");
   fprintf(fp, "/ff /findfont ldef\n");
   fprintf(fp, "/i /image ldef\n");
   fprintf(fp, "/j [] def\n");
   fprintf(fp, "/l /rlineto ldef\n");
   fprintf(fp, "/m /moveto ldef\n");
   fprintf(fp, "/np /newpath ldef\n");
   fprintf(fp, "/qp {pop pop pop pop} bdef\n");
   fprintf(fp, "/r /grestore ldef\n");
   fprintf(fp, "/R /restore ldef\n");
   fprintf(fp, "/rds /readstring ldef\n");
   fprintf(fp, "/re /read ldef\n");
   fprintf(fp, "/ro /rotate ldef\n");
   fprintf(fp, "/rf /rectfill ldef\n");
   fprintf(fp, "/rl /readline ldef\n");
   fprintf(fp, "/rs /rectstroke ldef\n");
   fprintf(fp, "/rx /readhexstring ldef\n");
   fprintf(fp, "/s /gsave ldef\n");
   fprintf(fp, "/S /save ldef\n");
   fprintf(fp, "/sc /scale ldef\n");
   fprintf(fp, "/sco /setrgbcolor ldef\n");
   fprintf(fp, "/sd /setdash ldef\n");
   fprintf(fp, "/sf /selectfont ldef\n");
   fprintf(fp, "/sgr /setgray ldef\n");
   fprintf(fp, "/sh /show ldef\n");
   fprintf(fp, "/slw /setlinewidth ldef\n");
   fprintf(fp, "/st /stroke ldef\n");
   fprintf(fp, "/tr /translate ldef\n");
   fprintf(fp, "%%\n");
   fprintf(fp, "/k {[] 0 sd} bdef\n");
   fprintf(fp, "/n {j {clw dup 1 le {pop 1}if mul}forall\n");
   fprintf(fp, "  j length array astore 0 sd} bdef\n");
   fprintf(fp, "/ol {s dup stringwidth pop neg 0 rmoveto\n");
   fprintf(fp, "  false charpath st r} bdef\n");
   fprintf(fp, "/reco {exch ff dup length dict begin\n");
   fprintf(fp, "  {1 index /FID ne{def}{pop pop}ifelse}forall\n");
   fprintf(fp, "  /Encoding ISOLatin1Encoding def\n");
   fprintf(fp, "  currentdict end definefont pop} bdef\n");
   fprintf(fp, "/RData {currentfile Rstr rx pop} bdef\n");
   fprintf(fp, "/GData {currentfile Gstr rx pop} bdef\n");
   fprintf(fp, "/BData {currentfile Bstr rx pop} bdef\n");
   fprintf(fp, "/DoIm {dup dup /Rstr exch string def\n");
   fprintf(fp, "  /Gstr exch string def /Bstr exch string def\n");
   fprintf(fp, "  RData GData BData true 3 ci} bdef\n");
   fprintf(fp, "%%%%EndProlog\n");
}


static int WriteSetup(FILE *fp)
{
   fprintf(fp, "%%%%BeginSetup\n");
   fprintf(fp, "true setstrokeadjust\n");
   fprintf(fp, "%%%%EndSetup\n");
}


static int WritePageHeader(FILE *fp, int pageNum)
{
   fprintf(fp, "%%%%Page: 1 %d\n", pageNum);
   fprintf(fp, "%%%%PageResources: (atend)\n");
   fprintf(fp, "%%%%BeginPageSetup\n");
   fprintf(fp, "17 17 tr 0.24 0.24 sc\n");
   fprintf(fp, "1 setlinecap\n");
   fprintf(fp, "1 setlinejoin\n");
   fprintf(fp, "%%%%EndPageSetup\n");

   fprintf(fp, "1.0 1.0 1.0 sco\n");      /* ??? */
   fprintf(fp, "[0 0 2405 2405] rf\n");   /* ??? */
}


static int WriteTrailer(FILE *fp)
{
   fprintf(fp, "showpage\n");
   fprintf(fp, "%%%%PageTrailer\n");
   fprintf(fp, "%%%%PageResources:\n");
   fprintf(fp, "%%%%Trailer\n");
   fprintf(fp, "%%%%Pages: \n");
   fprintf(fp, "%%%%DocumentNeededResources:\n");
   fprintf(fp, "%%%%EOF\n");
}





/*****************************************************************************
Function: RLD1Byte()
Purpose:  Decodes run length encoded streams whose elements are exactly one
          byte in size.  Refer to the documentation for the RunLengthDecode()
	  and RunLengthEncode() functions for more information.
Params:   inData  A pointer to an RLEBlock containing the encoded stream
	          to decode (restore).
          outBuf  A pointer to a buffer into which the original stream will
	          be decoded.  This stream must be nEls bytes in length.
State:    ---
Returns:  1  for success,
          0  for failure (unable to decode).
*****************************************************************************/
static int RLDecode(unsigned char *inBuf, int dataSize, void *outBuf)
{
   unsigned char *in = inBuf;
   unsigned char *out = (unsigned char *)outBuf;
   unsigned char *count;
   int n;
   unsigned num = 0;

   if ((dataSize == 0) || (!inBuf))
      return 0;

   n = 0;

   while (n < dataSize)
   {
      count = in++;
      num = (unsigned)(*count);
      ++n;

      if (num <= 127)
      {
	 int i;

	 for (i = 0; i <= num; i++)
	 {
	    *out++ = *in++;
	    ++n;
	 }
      }
      else if (num >= 129)
      {
	 int rep = 257 - num;
	 unsigned char ch = *in++;

	 while (rep--)
	    *out++ = ch;

         ++n;
      }
      else
	 break;
   }

   return (n == dataSize);
}


static int StrideData(void *inBuf, int nVals, int stride,
		      unsigned char *rleBuf)
{
   int i;
   unsigned char *in = (unsigned char *)inBuf;

   for (i = 0; i < nVals; i++, in += stride)
      *rleBuf++ = *in;

   return nVals;
}
