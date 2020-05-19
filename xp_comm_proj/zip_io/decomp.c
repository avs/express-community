#include <stdio.h>
#include <zlib.h>
#include <avs/f_utils.h>

#include "xp_comm_proj/zip_io/gen.h"
#include "xp_comm_proj/zip_io/fn_util.h"

#define BUFLEN      16384
#define MAX_NAME_LEN 1024
#define GZ_SUFFIX ".gz"


/* Uncomment the following line to enable the display of debugging information */
/* #define DEBUG */

#ifdef DEBUG
#define PRINT(A) { printf(A); }
#define PRINT_VAR(A,B) { printf(A, B); }
#else
#define PRINT(A) { }
#define PRINT_VAR(A,B) { }
#endif


int
gz_decompress (gzFile in, FILE * out)
{
   char buf[BUFLEN];
   int len;
   int err;
   int ret = 1;

   for (;;) {
      len = gzread (in, buf, sizeof (buf));
      if (len < 0) {
         fprintf (stderr, "Decompress File Error: %s\n", (char *) gzerror (in, &err));
         return 0;
      }
      if (len == 0)
         break;

      if ((int) fwrite (buf, 1, (unsigned) len, out) != len) {
         fprintf (stderr, "Decompress File Error: failed fwrite\n");
         ret = 0;
      }
   }

   if (fclose (out)) {
      fprintf (stderr, "Decompress File Error: failed fclose\n");
      ret = 0;
   }
   if (gzclose (in) != Z_OK) {
      fprintf (stderr, "Decompress File Error: failed gzclose\n");
      ret = 0;
   }

   return ret;
}



int
decompress_file (char *in_file, char *out_file, int remove_orig)
{
   FILE *out;
   gzFile in;

   in = gzopen (in_file, "rb");
   if (in == NULL) {
      fprintf (stderr, "Decompress File Error: Can't gzopen %s\n", in_file);
      return 0;
   }

   out = fopen (out_file, "wb");
   if (out == NULL) {
      fprintf (stderr, "Decompress File Error: Can't fopen %s\n", out_file);
      gzclose (in);
      return 0;
   }

   if (!gz_decompress (in, out)) {
      return (0);
   }

   if (remove_orig) {
      if (remove(in_file)) {
         fprintf (stderr, "Decompress File Error: Can't remove %s\n", in_file);
         return (0);
      };

      PRINT_VAR ("Removed File: %s\n", in_file);
   }

   return (1);
}




int
zipio_decompress (OMobj_id zipio_decomp_id, OMevent_mask event_mask, int seq_num)
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
   params_id = OMfind_subobj(zipio_decomp_id, OMstr_to_name ("params"), OM_OBJ_RD);

   /* Get in_filename's id */
   in_filename_id = OMfind_subobj(zipio_decomp_id, OMstr_to_name ("in_filename"), OM_OBJ_RD);

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
   if (OMget_name_str_val (params_id, OMstr_to_name ("output_dir"), &output_dir, 0) != 1) {
      free(filename);
      fprintf (stderr, "ERROR: Could not get output_dir\n");
      return 0;
   }

   /* Get remove_orig's value */
   if (OMget_name_int_val (params_id, OMstr_to_name ("remove_orig"), &remove_orig) != 1) {
      free(filename);
      free(output_dir);
      fprintf (stderr, "ERROR: Could not get remove_orig\n");
      return 0;
   }

   /***********************/
   /* Function's Body     */
   /***********************/
   /* ERRverror ("", ERR_NO_HEADER | ERR_INFO, "I'm in function: zipio_decompress generated from method: DecompressFileCore.decompress\n"); */

   /* Use AVS/Express function to map enviroment variables */
   FILEmap_variables(filename, filename_buf);
   FILEmap_variables(output_dir, output_dir_buf);

   PRINT_VAR ("Filename  : %s\n", filename_buf);
   PRINT_VAR ("Output_dir: %s\n", output_dir_buf);

   spiltFilename(filename_buf, temp1, NULL);

   PRINT_VAR ("Partial Filename: %s\n", temp1);

   if (removeSuffix(temp1, GZ_SUFFIX, temp2)==0) {
      fprintf (stderr, "ERROR: Could not remove .gz suffix.\n");
      free(filename);
      free(output_dir);
      return 0;
   }

   PRINT_VAR ("Filename & Suffix: %s\n", temp2);

   combinePathAndFilename(output_dir_buf, temp2, out_filename);

   PRINT_VAR ("Full Filename: %s\n", out_filename);

   if (decompress_file (filename_buf, out_filename, remove_orig)) {
      PRINT ("  Decompressing done.\n");

      /* Set out_filename's value */
      OMset_name_str_val (zipio_decomp_id, OMstr_to_name ("out_filename"), out_filename);
   }
   else {
      fprintf (stderr, "ERROR: an error occured during decompressing.\n");
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

