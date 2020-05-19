#include <stdio.h>
#include <zlib.h>
#include <avs/f_utils.h>

#include "xp_comm_proj/zip_io/gen.h"
#include "xp_comm_proj/zip_io/fn_util.h"

#define BUFLEN      16384
#define MAX_NAME_LEN 1024
#define GZ_SUFFIX ".gz"

/* #define USE_MMAP */

/* Uncomment the following line to enable the display of debugging information */
/* #define DEBUG */

#ifdef DEBUG
#define PRINT(A) { printf(A); }
#define PRINT_VAR(A,B) { printf(A, B); }
#else
#define PRINT(A) { }
#define PRINT_VAR(A,B) { }
#endif


/* Forward references */
int gz_compress (FILE * in, gzFile out);
#ifdef USE_MMAP
int gz_compress_mmap (FILE * in, gzFile out);
#endif

int
gz_compress (FILE * in, gzFile out)
{
   char buf[BUFLEN];
   int len;
   int err;
   int ret = 1;

#ifdef USE_MMAP
   /* Try first compressing with mmap. If mmap fails (minigzip used in a
    * pipe), use the normal fread loop.
    */
   if (gz_compress_mmap (in, out) == Z_OK)
      return 1;
#endif

   for (;;) {
      len = fread (buf, 1, sizeof (buf), in);
      if (ferror (in)) {
         fprintf (stderr, "Compress File Error: Could not read from input file\n");
         return 0;
      }
      if (len == 0)
         break;

      if (gzwrite (out, buf, (unsigned) len) != len) {
         fprintf (stderr, "Compress File Error: %s\n", (char *) gzerror (out, &err));
         ret = 0;
      }
   }

   if (fclose (in)) {
      fprintf (stderr, "Compress File Error: failed fclose\n");
      ret = 0;
   }
   if (gzclose (out) != Z_OK) {
      fprintf (stderr, "Compress File Error: failed gzclose\n");
      ret = 0;
   }

   return ret;
}



#ifdef USE_MMAP                 /* MMAP version, Miguel Albrecht <malbrech@eso.org> */

/* Try compressing the input file at once using mmap. Return Z_OK if
 * if success, Z_ERRNO otherwise.
 */
int
gz_compress_mmap (FILE * in, gzFile out)
{
   int len;
   int err;
   int ifd = fileno (in);
   caddr_t buf;                 /* mmap'ed buffer for the entire input file */
   off_t buf_len;               /* length of the input file */
   struct stat sb;

   /* Determine the size of the file, needed for mmap: */
   if (fstat (ifd, &sb) < 0)
      return Z_ERRNO;
   buf_len = sb.st_size;
   if (buf_len <= 0)
      return Z_ERRNO;

   /* Now do the actual mmap: */
   buf = mmap ((caddr_t) 0, buf_len, PROT_READ, MAP_SHARED, ifd, (off_t) 0);
   if (buf == (caddr_t) (-1))
      return Z_ERRNO;

   /* Compress the whole file at once: */
   len = gzwrite (out, (char *) buf, (unsigned) buf_len);

   if (len != (int) buf_len) {
      fprintf (stderr, "Compress File Error: %s\n", (char *)gzerror (out, &err));
   }

   munmap (buf, buf_len);
   if (fclose (in)) {
      fprintf (stderr, "Compress File Error: failed fclose\n");
   }
   if (gzclose (out) != Z_OK) {
      fprintf (stderr, "Compress File Error: failed gzclose\n");
   }

   return Z_OK;
}
#endif /* USE_MMAP */



/* ===========================================================================
 * Compress the given file, creates a corresponding .gz file.
 */
int
compress_file (char *in_file, char *out_file, int remove_orig)
{
   FILE *in;
   gzFile out;

   in = fopen (in_file, "rb");
   if (in == NULL) {
      fprintf (stderr, "Compress File Error: Can't fopen %s\n", in_file);
      return (0);
   }

   out = gzopen (out_file, "wb");
   if (out == NULL) {
      fprintf (stderr, "Compress File Error: Can't gzopen %s\n", out_file);
      fclose (in);
      return (0);
   }

   if (!gz_compress (in, out)) {
      return (0);
   }

   if (remove_orig) {
      if (remove(in_file)) {
         fprintf (stderr, "Compress File Error: Can't remove %s\n", in_file);
         return (0);
      };

      PRINT_VAR ("Removed File: %s\n", in_file);
   }

   return (1);
}



int
zipio_compress (OMobj_id zipio_comp_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   OMobj_id params_id;
   OMobj_id in_filename_id;
   OMobj_id filename_id;

   char *filename = NULL;
   char filename_buf[AVS_PATH_MAX];

   char *output_dir = NULL;
   char output_dir_buf[AVS_PATH_MAX];

   int remove_orig;

   char temp1[AVS_PATH_MAX];
   char temp2[AVS_PATH_MAX];
   char out_filename[AVS_PATH_MAX];


   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get parameters's id */
   params_id = OMfind_subobj(zipio_comp_id, OMstr_to_name ("params"), OM_OBJ_RD);

   /* Get in_filename's id */
   in_filename_id = OMfind_subobj(zipio_comp_id, OMstr_to_name ("in_filename"), OM_OBJ_RD);

   /* Get filename's id */
   filename_id = OMfind_subobj(params_id, OMstr_to_name ("filename"), OM_OBJ_RD);

   
   /* Check which filename input is valid and get the appropriate one */
   if (OMvalid_obj(in_filename_id, OMnull_obj, 0)) {
      if (OMget_str_val (in_filename_id, &filename, 0) != OM_STAT_SUCCESS) {
         fprintf (stderr, "ERROR: Could not get in_filename\n");
         return 0;
      }
   }
   else if (OMvalid_obj(filename_id, OMnull_obj, 0)) {
      if (OMget_str_val (filename_id, &filename, 0) != OM_STAT_SUCCESS) {
         fprintf (stderr, "ERROR: Could not get filename\n");
         return 0;
      }
   }
   else {
      PRINT ("in_filename and filename are both invalid\n");
      return 1;
   }


   /* Get output_dir's value */
   if (OMget_name_str_val (params_id, OMstr_to_name ("output_dir"), &output_dir, 0) != OM_STAT_SUCCESS) {
      free(filename);
      fprintf (stderr, "ERROR: Could not get output_dir\n");
      return 0;
   }

   /* Get remove_orig's value */
   if (OMget_name_int_val (params_id, OMstr_to_name ("remove_orig"), &remove_orig) != OM_STAT_SUCCESS) {
      free(filename);
      free(output_dir);
      fprintf (stderr, "ERROR: Could not get remove_orig\n");
      return 0;
   }


   /***********************/
   /* Function's Body     */
   /***********************/
   /* ERRverror ("", ERR_NO_HEADER | ERR_INFO, "I'm in function: zipio_compress generated from method: CompressFileCore.compress\n"); */

   /* Use AVS/Express function to map enviroment variables */
   FILEmap_variables(filename, filename_buf);
   FILEmap_variables(output_dir, output_dir_buf);

   PRINT_VAR ("Filename  : %s\n", filename_buf);
   PRINT_VAR ("Output_dir: %s\n", output_dir_buf);
   PRINT_VAR ("Remove_orig: %d\n", remove_orig);

   spiltFilename(filename_buf, temp1, NULL);

   PRINT_VAR ("Partial Filename: %s\n", temp1);

   if (addSuffix(temp1, GZ_SUFFIX, temp2)==0) {
      printf ("ERROR: Could not add .gz suffix.\n");
      free(filename);
      free(output_dir);
      return 0;
   }

   PRINT_VAR ("Filename & Suffix: %s\n", temp2);

   combinePathAndFilename(output_dir_buf, temp2, out_filename);

   PRINT_VAR ("Full Filename: %s\n", out_filename);

   if (compress_file (filename_buf, out_filename, remove_orig)) {
      PRINT ("  Compressing done.\n");

      /* Set out_filename's value */
      OMset_name_str_val (zipio_comp_id, OMstr_to_name ("out_filename"), out_filename);
   }
   else {
      fprintf (stderr, "ERROR: an error occured during compressing.\n");
      free(filename);
      free(output_dir);
      return 0;
   }


   /*************************/
   /*  Free input variables */
   /*************************/
   if (filename)
      free (filename);

   if (output_dir)
      free (output_dir);

   return 1;
}

