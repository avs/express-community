

/*

  Copyright / Disclaimer :

  This software/documentation was produced as part of the INDEX project
  (Intelligent Data Extraction) which is funded under contract ESPRIT EP22745 of 
  the European Community. For further details see
  http://www.man.ac.uk/MVC/research/INDEX/Public/. 

  Copyright (c) June 1998, Manchester Visualisation Centre, UK. All Rights Reserved.

  Permission to use, copy, modify and distribute this software and its documentation
  is hereby granted without fee, provided that the above copyright notice and this
  permission notice appear in all copies of this software/documentation.

  This software/documentation is provided with no warranty, express or implied, 
  including, without limitation, warrant of merchantability or fitness for a 
  particular purpose.

*/


/*
  deflate.c

  Functions to use zlib deflate/inflate.
  Somewhat verbose implementation with lots of fns to make it easy to understand.
  See deflate.h for user callable function prototypes.
*/


#include <stdlib.h>
#include <stdio.h>
#include "deflate.h"
#include "zlib.h"


#define ONE_MBYTE   (1024*1024)
#define BUFFER_SIZE (1024*1024)


void open_file
  (char *filename_p, char *mode_p, FILE **file_pp);

void allocate_array
  (int n_bytes, Bytef **array_pp);

void reallocate_array
  (int n_bytes, Bytef **array_pp);

void print_stream
  (z_stream *stream_p);

void initialise_stream
  (z_stream *stream_p, Bytef *in_buffer_p, int in_contains, Bytef *out_buffer_p,
   int out_size);

void initialise_deflate
  (z_stream *stream_p, Bytef *in_buffer_p, int in_contains, Bytef *out_buffer_p, 
   int out_size, int level);

void initialise_inflate
  (z_stream *stream_p, Bytef *in_buffer_p, int in_contains, Bytef *out_buffer_p,
   int out_size);

void fill_input_buffer
  (z_stream *stream_p, Bytef *in_buffer_p, int in_size, FILE *in_file_p);

void empty_output_buffer
  (z_stream *stream_p, Bytef *out_buffer_p, int out_size, FILE *out_file_p);

void lengthen_output_buffer
  (z_stream *stream_p, Bytef **out_buffer_pp, int *out_size_p);

int input_buffer_not_empty
  (z_stream *stream_p);

int output_buffer_full
  (z_stream *stream_p);

int no_more_input
  (FILE *in_file_p);

void deflate_it
  (z_stream *stream_p, int mode);

void inflate_it
  (z_stream *stream_p, int mode);


/* -------------------------------------------------------------------------- */

void open_file
  (char *filename_p, char *mode_p, FILE **file_pp)
{
  *file_pp = fopen (filename_p, mode_p);
  if (*file_pp == NULL) {
    fprintf (stderr,"  error in `open_file' : couldn't open %s.\n",filename_p);
    exit (0);
  }
}

/* -------------------------------------------------------------------------- */

void allocate_array
  (int n_bytes, Bytef **array_pp)
{
  *array_pp = (Bytef *) malloc (n_bytes);
  if (*array_pp == NULL) {
    fprintf (stderr, "  error in `allocate_array' : out of memory.\n");
    exit (0);
  }
}

/* -------------------------------------------------------------------------- */

void reallocate_array
  (int n_bytes, Bytef **array_pp)
{
  *array_pp = (Bytef *) realloc ((void *) *array_pp, n_bytes);
  if (*array_pp == NULL) {
    fprintf (stderr, "  error in `reallocate_array' : realloc failed.\n");
    exit (0);
  }
}

/* -------------------------------------------------------------------------- */

void print_stream
  (z_stream *stream_p)
{
  printf ("  next   in %p.\n", stream_p->next_in);
  printf ("  avail  in %d.\n", stream_p->avail_in);
  printf ("  total  in %d.\n", stream_p->total_in);
  printf ("  next  out %p.\n", stream_p->next_out);
  printf ("  avail out %d.\n", stream_p->avail_out);
  printf ("  total out %d.\n", stream_p->total_out);
  printf ("\n");
}

/* -------------------------------------------------------------------------- */

void initialise_stream
  (z_stream *stream_p, Bytef *in_buffer_p, int in_contains, Bytef *out_buffer_p,
   int out_size)
{
  stream_p->next_in   = in_buffer_p;
  stream_p->avail_in  = in_contains;
  stream_p->next_out  = out_buffer_p;
  stream_p->avail_out = out_size;
  stream_p->zalloc    = Z_NULL;
  stream_p->zfree     = Z_NULL;
  stream_p->opaque    = Z_NULL;
}

/* -------------------------------------------------------------------------- */

void initialise_deflate
  (z_stream *stream_p, Bytef *in_buffer_p, int in_contains, Bytef *out_buffer_p,
   int out_size, int level)
{
  initialise_stream (stream_p, in_buffer_p, in_contains, out_buffer_p, out_size);
  if ((level<Z_NO_COMPRESSION)||(level>Z_BEST_COMPRESSION))
    if (level!=Z_DEFAULT_COMPRESSION) {
      fprintf (stderr, "  error in `initialise_deflate' :");
      fprintf (stderr, " invalid level, using default.\n");
      level = Z_DEFAULT_COMPRESSION;
    }
  if (deflateInit (stream_p, level) != Z_OK) {
    fprintf (stderr, "  error in `initialise_deflate' : deflateInit.\n");
    exit (0);
  }
}

/* -------------------------------------------------------------------------- */

void initialise_inflate
  (z_stream *stream_p, Bytef *in_buffer_p, int in_contains, Bytef *out_buffer_p,
   int out_size)
{
  initialise_stream (stream_p, in_buffer_p, in_contains, out_buffer_p, out_size);
  if (inflateInit (stream_p) != Z_OK) {
    fprintf (stderr, "  error in `initialise_inflate' : inflateInit.\n");
    exit (0);
  }
}

/* -------------------------------------------------------------------------- */

void fill_input_buffer
  (z_stream *stream_p, Bytef *in_buffer_p, int in_size, FILE *in_file_p)
{
  int n_bytes;

  n_bytes = fread (in_buffer_p, 1, in_size, in_file_p);
  stream_p->next_in  = in_buffer_p;
  stream_p->avail_in = n_bytes;
}  

/* -------------------------------------------------------------------------- */

void empty_output_buffer
  (z_stream *stream_p, Bytef *out_buffer_p, int out_size, FILE *out_file_p)
{
  int n_bytes;

  n_bytes = out_size - stream_p->avail_out;
  fwrite (out_buffer_p, 1, n_bytes, out_file_p);
  stream_p->next_out = out_buffer_p;
  stream_p->avail_out = out_size;
}

/* -------------------------------------------------------------------------- */

void lengthen_output_buffer
  (z_stream *stream_p, Bytef **out_buffer_pp, int *out_size_p)
{
  int old_size, new_size, out_contains;

  out_contains = stream_p->next_out - (*out_buffer_pp);
  old_size =  out_contains + stream_p->avail_out;
  if (old_size > 50*ONE_MBYTE)
    new_size = old_size + 50*ONE_MBYTE;
  else
    new_size = 2*old_size;
  reallocate_array (new_size, out_buffer_pp);
  stream_p->next_out = *out_buffer_pp + out_contains;
  stream_p->avail_out = new_size - out_contains;
  *out_size_p = new_size;
}

/* -------------------------------------------------------------------------- */

int input_buffer_not_empty
  (z_stream *stream_p)
{
  if (stream_p->avail_in != 0)
    return (1);
  else
    return (0);
}

/* -------------------------------------------------------------------------- */

int output_buffer_full
  (z_stream *stream_p)
{
  if (stream_p->avail_out == 0)
    return (1);
  else
    return (0);
}

/* -------------------------------------------------------------------------- */

int input_exists
  (FILE *in_file_p)
{
  return (!feof(in_file_p) && !ferror(in_file_p));
}

/* -------------------------------------------------------------------------- */

void deflate_it
  (z_stream *stream_p, int mode)
{
  int status;

  status = deflate (stream_p, mode);
  if ((status != Z_OK) && (status!= Z_STREAM_END)) {
    fprintf (stderr, "  error in `deflate_it' : status %d, %s.\n", status,
     stream_p->msg);
    exit (0);
  }
}

/* -------------------------------------------------------------------------- */

void inflate_it
  (z_stream *stream_p, int mode)
{
  int status;

  status = inflate (stream_p, mode);
  if ((status!= Z_OK) && (status != Z_STREAM_END)) {
    fprintf (stderr, "  error in `inflate_it' : status %d, %s.\n", status,
     stream_p->msg);
    exit (0);
  }
}

/* -------------------------------------------------------------------------- */

void deflate_array
  (Bytef *in_array_p, int in_size, Bytef **out_array_pp, int *out_size_p,
   int level)
{
  z_stream stream;

  *out_size_p = 1.001*in_size + 13;
  allocate_array (*out_size_p, out_array_pp);
  initialise_deflate (&stream, in_array_p, in_size, *out_array_pp, 
   *out_size_p, level);
  deflate_it (&stream, Z_FINISH);
  reallocate_array (stream.total_out, out_array_pp);
  *out_size_p = stream.total_out;
  deflateEnd (&stream);
}

/* -------------------------------------------------------------------------- */

void inflate_array
  (Bytef *in_array_p, int in_size, Bytef **out_array_pp, int *out_size_p)
{
  z_stream stream;

  *out_size_p = 1.001*in_size + 13;
  allocate_array (*out_size_p, out_array_pp);
  initialise_inflate (&stream, in_array_p, in_size, *out_array_pp, 
   *out_size_p);
  do {
    inflate_it (&stream, Z_PARTIAL_FLUSH);
    if (output_buffer_full (&stream))
      lengthen_output_buffer (&stream, out_array_pp, out_size_p);
  } while (input_buffer_not_empty (&stream));
  reallocate_array (stream.total_out, out_array_pp);
  *out_size_p = stream.total_out;
  inflateEnd (&stream);
}

/* -------------------------------------------------------------------------- */

void deflate_file
  (FILE *in_file_p, FILE *out_file_p, int level)
{
  int      in_size, out_size;
  Bytef    *in_buffer_p, *out_buffer_p;
  z_stream stream;

  in_size = BUFFER_SIZE;
  out_size = 1.001*in_size + 13;
  allocate_array (in_size, &in_buffer_p);
  allocate_array (out_size, &out_buffer_p);
  initialise_deflate (&stream, in_buffer_p, 0, out_buffer_p, out_size, level);
  do {
    fill_input_buffer (&stream, in_buffer_p, in_size, in_file_p);
    do {
      deflate_it (&stream, Z_NO_FLUSH);
      if (output_buffer_full (&stream))
        empty_output_buffer (&stream, out_buffer_p, out_size, out_file_p);
    } while (input_buffer_not_empty (&stream));
  } while (input_exists (in_file_p));
  do {
    empty_output_buffer (&stream, out_buffer_p, out_size, out_file_p);
    deflate_it (&stream, Z_FINISH);
  } while (output_buffer_full (&stream));
  empty_output_buffer (&stream, out_buffer_p, out_size, out_file_p);
  deflateEnd (&stream);
  free (in_buffer_p);
  free (out_buffer_p);
}

/* -------------------------------------------------------------------------- */

void inflate_file
  (FILE *in_file_p, FILE *out_file_p)
{
  int      in_size, out_size;
  Bytef    *in_buffer_p, *out_buffer_p;
  z_stream stream;

  in_size = BUFFER_SIZE;
  out_size = 1.001*in_size+13;
  allocate_array (in_size, &in_buffer_p);
  allocate_array (out_size, &out_buffer_p);
  initialise_inflate (&stream, in_buffer_p, 0, out_buffer_p, out_size);
  do {
    fill_input_buffer (&stream, in_buffer_p, in_size, in_file_p);
    do {
      inflate_it (&stream, Z_PARTIAL_FLUSH);
      if (output_buffer_full (&stream))
        empty_output_buffer (&stream, out_buffer_p, out_size, out_file_p);
    } while (input_buffer_not_empty (&stream));
  } while (input_exists (in_file_p));
  empty_output_buffer (&stream, out_buffer_p, out_size, out_file_p);
  inflateEnd (&stream);
  free (in_buffer_p);
  free (out_buffer_p);
}

/* -------------------------------------------------------------------------- */

int deflated_array
  (Bytef *array_p)
/*
  The first two bytes of a Zlib compressed array are :
  
  first byte  : bits 0 to 7 is cmf      (compression method and flags)
                bits 0 to 3 is cm       (compression method)
                bits 4 to 7 is cinfo    (compression info)

  second byte : bits 0 to 8 is flg      (flags)
                bits 0 to 4 is fcheck   (check bits for cmf and flg)
                bits 5 to 5 is fdict    (preset dictionary)
                bits 6 to 7 is flevel   (compression level)

  where the bits in a byte are numbered 76543210 (i.e. 0 is least significant).

  For deflate, cinfo <= 7, cm == 8, fcheck set such that (cmf*256 + fld) is a 
  multiple of 31.
*/
{
  unsigned char cmf, flg, cinfo, cm;

  cmf = array_p[0];
  flg = array_p[1];
  cinfo = cmf >> 4;
  cm = cmf & 15;
  if (cinfo > 7)
    return (0);
  if (cm != 8)
    return (0);
  if (((cmf*256 + flg)%31)!=0)
    return (0);
  return (1);
}

/* -------------------------------------------------------------------------- */
