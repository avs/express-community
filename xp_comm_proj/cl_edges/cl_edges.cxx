
#include "xp_comm_proj/cl_edges/gen.h"


int
CleanExtEdges_CleanExtEdgesMods_CleanEdgesCore::update(OMevent_mask event_mask, int seq_num)
{
   // Current line connectivity array
   int curr_lines_size;
   int *curr_lines_arr;

   // Combined line connectivity array
   int cat_lines_size;
   int *cat_lines_arr;

   // Flag array used to indicate which lines should be removed
   char *found;

   // Cleaned line connectivity output array
   int lines_out_size;
   int *lines_out_arr;

   int i,j;             // Loop counters
   int i1, i2, j1, j2;  // Loop unrolling variables

   int total_cells;           // Total number of lines in input cells
   int cnum;                  // Number of current line
   int lines_out_internal;    // Total number of cleaned lines


   // Check input field has cell sets
   if (in.ncell_sets<1) {
      ERRverror("CleanEdgesCore",ERR_NO_HEADER | ERR_PRINT, "CleanEdgesCore: input field must have 1 or more cell sets.\n");
      return 0;
   }
      

   // Count number of lines in input field
   total_cells = 0;
   for (i=0; i < in.ncell_sets; i++) {
      // Check cell_set is line cell_set
      if ((in.cell_set[i].cell_ndim==1) && (in.cell_set[i].cell_order==1)) {
         total_cells += in.cell_set[i].ncells;
      }
   }

   // Check that lines have been found
   if (total_cells<1) {
      ERRverror("CleanEdgesCore",ERR_NO_HEADER | ERR_PRINT, "CleanEdgesCore: no line cell data found.\n");
      return 0;
   }

   // Allocate combined lines connectivity array 
   cat_lines_size = 2*total_cells;
   cat_lines_arr = (int *)malloc( cat_lines_size * sizeof(int) );


   //
   // Read line data from connectivity arrays and place it into combined line array
   //

   cnum = 0;

   // Setup status bar
   OMpush_status_range(0, 100);
   OMstatus_check(0, "CleanEdgesCore: Building line data", NULL);

   for (i=0; i<in.ncell_sets; i++) {
      // Check cell_set is line cell_set
      if ((in.cell_set[i].cell_ndim==1) && (in.cell_set[i].cell_order==1)) {

         // Get and check current line array
         curr_lines_arr = (int *)in.cell_set[i].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD, &curr_lines_size);
         if (!(curr_lines_arr)) {
            ERRverror("CleanEdgesCore",ERR_NO_HEADER | ERR_PRINT, "CleanEdgesCore: error getting connectivity array\n");
            OMstatus_check(100, "CleanEdgesCore: Finished", NULL);
            OMpop_status_range();
            free(cat_lines_arr);
            return 0;
         }

         // Loop through current lines and add them to combined line array
         for (j=0; j<curr_lines_size; j++, cnum++) {
            cat_lines_arr[cnum] = curr_lines_arr[j];
         }

         // Free current line array
         ARRfree((int *)curr_lines_arr);
      }
   }

   OMstatus_check(33, "CleanEdgesCore: Searching line data", NULL);

   // Allocate flag array
   found = (char *)malloc(total_cells * sizeof(char) );

   // Zero all values in flag array
   for (i=0;i<total_cells;i++)
      found[i] = 0;



   //
   // Loop through all lines in combined array and search for duplicated lines.
   //
   // - If in outer_edge mode both duplicated lines are removed
   // - If not in outer_edge mode only the later line is removed.
   //

   if (outer_edges)
   {
      for (i=0;i<total_cells;i++)
      {
         if (found[i] == 0) {
            i1 = cat_lines_arr[2*i];
            i2 = cat_lines_arr[2*i+1];

            for (j=i+1;j<total_cells;j++)
            {
               j1 = cat_lines_arr[2*j];
               j2 = cat_lines_arr[2*j+1];

               // If lines are duplicates then mark both lines to be removed
               if ( ((i1==j1) && (i2==j2)) || ((i1==j2) && (i2==j1)) )
               {
                  found[i] = 1;
                  found[j] = 1;
               }
            }
         }
      }
   }
   else
   {
      for (i=0;i<total_cells;i++)
      {
         if (found[i] == 0) {
            i1 = cat_lines_arr[2*i];
            i2 = cat_lines_arr[2*i+1];

            for (j=i+1;j<total_cells;j++)
            {
               j1 = cat_lines_arr[2*j];
               j2 = cat_lines_arr[2*j+1];

               // If lines are duplicates then mark second line to be removed
               if ( ((i1==j1) && (i2==j2)) || ((i1==j2) && (i2==j1)) )
               {
                  found[j] = 1;
               }
            }
         }
      }
   }


   OMstatus_check(66, "CleanEdgesCore: Copying line data", NULL);


   // Count number of lines that are left to be output
   lines_out_internal = 0;
   for (i=0;i<total_cells;i++)
      if (found[i]==0) lines_out_internal++;

   nlines_out = lines_out_internal;

   // If no lines are present we don't have anything else to do
   if (lines_out_internal == 0) {
      OMstatus_check(100, "CleanEdgesCore: Finished", NULL);
      OMpop_status_range();
      free(cat_lines_arr);
      free(found);

      return 1;
   }
      

   // Get & Check output line connectivity array
   lines_out_arr = (int *)lines_out.ret_array_ptr(OM_GET_ARRAY_WR,&lines_out_size);
   if (!(lines_out_arr)) {
      ERRverror("CleanEdgesCore",ERR_NO_HEADER | ERR_PRINT,"CleanEdgesCore: error getting lines_out array\n");
      OMstatus_check(100, "CleanEdgesCore: Finished", NULL);
      OMpop_status_range();
      free(cat_lines_arr);
      free(found);

      return 0;
   }

   // Ensure output array is of correct size
   if (lines_out_size != (2*lines_out_internal) ) {
      ERRverror("CleanEdgesCore",ERR_NO_HEADER | ERR_PRINT,"CleanEdgesCore: lines_out array is incorrect size\n");
      OMstatus_check(100, "CleanEdgesCore: Finished", NULL);
      OMpop_status_range();
      free(cat_lines_arr);
      free(found);
      ARRfree((char *)lines_out_arr);

      return 0;
   }


   //
   // Build output line array by copying data from combined line array
   //
   cnum = 0;
   for (i=0;i<total_cells;i++)
   {
      if (found[i] == 0) {
         lines_out_arr[2*cnum] = cat_lines_arr[2*i];
         lines_out_arr[2*cnum+1] = cat_lines_arr[2*i+1];
         cnum++;
      }
   }


   // Finish with status bar
   OMstatus_check(100, "CleanEdgesCore: Finished", NULL);
   OMpop_status_range();

   // Free used arrays
   free(cat_lines_arr);
   free(found);
   ARRfree((int *)lines_out_arr);

   // return 1 for success
   return(1);
}

