
// Sabreen's X-ray module
//


#include "xp_comm_proj/xray/gen.h"


   
// Copy data from a row in the input array into a temporary storage array
void collect_row(int *row, int direction, unsigned char *input_arr,
                 int I, int J, int L, int mx, int my)
{
   int i;
   int start;

   switch(direction) {

      // Get a x-axis aligned row
      case 0:
         start = J*mx*my + I*mx;
         for (i=0;i<L;i++)
            row[i] = input_arr[start + i];
         break;

      // Get an y-axis aligned row
      case 1:
         start = J*mx*my + I;
         for (i=0;i<L;i++)
            row[i] = input_arr[start + i*mx];
         break;

      // Get an z-axis aligned row
      case 2:
         start = mx*my;
         for (i=0;i<L;i++)
            row[i] = input_arr[i*start + J*mx + I];
         break;
   }
}


// Comparasion array using by QuickSort function
int comp(const void *a, const void *b)
{
   if ((int*)a > (int*)b)
      return 1;
   else if ((int*)a < (int*)b)
      return -1;
   else
      return(0);
}


// Perform analysis operation on temporary array containing row data
int operate_on_row(int *row, int operation, int L)
{

   int i,result;

   result = 0;

   switch(operation)
   {
      case 0:		//sum
         for (i=0;i<L;i++)
            result += row[i];
         break;
      case 1:		//mean
         for (i=0;i<L;i++)
            result += row[i];
         result /= L;
         break;
      case 2:		//median
         qsort(row,L,sizeof(int),comp);
         result = row[L/2];
         break;
      case 3:		//min
         result = row[0];
         for (i=1;i<L;i++)
            if (row[i] < result)
               result = row[i];
            break;
      case 4:		//max
         result = row[0];
         for (i=1;i<L;i++)
            if (row[i] > result)
               result = row[i];
            break;
   }

   return(result);
}



// Main update function called from AVS/Express
int
Xray_XrayMods_XrayCore::x_ray_update(OMevent_mask event_mask, int seq_num)
{

   // input (OMXbyte_array read req notify)
   int input_size;
   unsigned char *input_arr;

   // dims (OMXint_array read req notify)
   int dims_size;
   int *dims_arr;

   // axis (OMXint read req notify)
   // mode (OMXint read req notify)
   // nx (OMXint write)
   // ny (OMXint write)
   // output (OMXint_array write)
   int output_size;
   int *output_arr;

   int i, j;
   int count, status_step, nx_val, ny_val;
   
   int *row,row_length;

   int direction;
   int operation;

   /***********************/
   /* Function Body       */
   /***********************/


/***
   ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: XrayCore::x_ray_update\n");
 ***/

   input_arr = (unsigned char *)input.ret_array_ptr(OM_GET_ARRAY_RD,&input_size);
   if (!input_arr) {
      ERRverror("XrayCore",ERR_NO_HEADER | ERR_PRINT,"Couldn't get input array\n");
      return(0);
   }

   dims_arr = (int *)dims.ret_array_ptr(OM_GET_ARRAY_RD,&dims_size);
   if (!dims_arr) {
      ERRverror("XrayCore",ERR_NO_HEADER | ERR_PRINT,"Couldn't get dims array\n");
      ARRfree(input_arr);
      return(0);
   }

   if (dims_size != 3) {
      ERRverror("XrayCore",ERR_NO_HEADER | ERR_PRINT,"Input field must be a 3D volume\n");
      ARRfree(input_arr);
      ARRfree(dims_arr);
      return(0);
   }


   // figure out which axis defines the output storage size
   // get from the axis param, and dims
   direction = axis;
   operation = mode;

   if (direction == 0) {
      nx = dims_arr[1];
      ny = dims_arr[2];
   }
   else if (direction == 1) {
      nx = dims_arr[0];
      ny = dims_arr[2];
   }
   else if (direction == 2) {
      nx = dims_arr[0];
      ny = dims_arr[1];
   }
   else {
      ERRverror("XrayCore",ERR_NO_HEADER | ERR_PRINT,"axis direction out of range\n");
      ARRfree(input_arr);
      ARRfree(dims_arr);
      return(0);
   }

   if (operation < 0 || operation > 4) {
      ERRverror("XrayCore",ERR_NO_HEADER | ERR_PRINT,"operation code out of range\n");
      ARRfree(input_arr);
      ARRfree(dims_arr);
      return(0);
   }
 

   output_arr = (int *)output.ret_array_ptr(OM_GET_ARRAY_WR,&output_size);
   if (!output_arr) {
      ERRverror("XrayCore",ERR_NO_HEADER | ERR_PRINT,"Couldn't get output array\n");
      ARRfree(input_arr);
      ARRfree(dims_arr);
      return(0);
   }


   // Allocate temporary array to store row before processing
   row_length = dims_arr[direction];
   row = (int *)malloc(row_length * sizeof(int));

   // take local copy so OM is not referenced in inner loop
   nx_val = nx;
   ny_val = ny;

   // Setup status bar
   OMpush_status_range(0, 100);
   OMstatus_check(0, "XrayCore: Generating Visualization", NULL);

   // If we are calculating the median then display proper status info 
   if (operation==2) {
      count = 0;
      status_step = (nx_val*ny_val)/10;

      // Loop through each pixel in slice and
      //   - Copy data into temporary array
      //   - Analyse temporary array
      for (j=0; j<ny_val; j++) {
         for (i=0; i<nx_val; i++) {
            collect_row(row,direction,input_arr,i,j,row_length,dims_arr[0],dims_arr[1]);
            output_arr[j*nx_val + i] = operate_on_row(row,operation,row_length);

            count++;
         }

         if ((count % status_step) == 0)
            OMstatus_check(count*10 / status_step, "XrayCore: Generating Visualization", NULL);
      }
   }
   else
   {
      // Loop through each pixel in slice and
      //   - Copy data into temporary array
      //   - Analyse temporary array
      for (j=0; j<ny_val; j++) {
         for (i=0; i<nx_val; i++) {
            collect_row(row,direction,input_arr,i,j,row_length,dims_arr[0],dims_arr[1]);
            output_arr[j*nx_val + i] = operate_on_row(row,operation,row_length);
         }
      }
   }

   OMstatus_check(100, "XrayCore: Finished Visualization", NULL);
   OMpop_status_range();

   free(row);

   ARRfree(input_arr);
   ARRfree(dims_arr);
   ARRfree(output_arr);

   return(1);
}

