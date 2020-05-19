
#include "xp_comm_proj/collapse/gen.hxx"

#define ERROR_RETURN(MESS) {\
   ERRverror("CollapseFieldCore", ERR_ERROR, MESS);\
   return 0;\
}

#define FREE_ARRAYS {\
   if (dims_arr!=NULL) ARRfree(dims_arr);\
   if (values_arr!=NULL) ARRfree(values_arr);\
   if (veclen_arr!=NULL) ARRfree(veclen_arr);\
   if (planes_arr!=NULL) ARRfree(planes_arr);\
   if (out_dims_arr!=NULL) ARRfree(out_dims_arr);\
   if (out_values_arr!=NULL) ARRfree(out_values_arr);\
}


//
//	This module takes a 1, 2 or 3D array (ie the values array of an uniform field)
//	and collapse it along one axis. The data can have any veclen desidered.
//	There are various kind of collapsing operation (the code is in 'operation'):
//		0: sums the values along the collapsing axis
//		1: orthoslice along the collapsing axis at specified plane
//		2: sums the values along the collapsing axis except specified plane
//		3: sum along the collapsing dimension only the specified planes
//
int
Collapse_CollapseMods_CollapseFieldCore::update(OMevent_mask event_mask, int seq_num)
{
   // operation (OMXint read req notify)
   // axis (OMXint read req notify)
   // dims (OMXint_array read req notify)
   int dims_size;
   int *dims_arr = NULL;
   
   // values (OMXdouble_array read req notify)
   int values_size;
   double *values_arr = NULL;
   
   // veclen (OMXint_array read req notify)
   int veclen_size;
   int *veclen_arr = NULL;
   
   // planes (OMXint_array read req notify)
   int planes_size;
   int *planes_arr = NULL;

   // out_dims (OMXint_array write)
   int out_dims_size;
   int *out_dims_arr = NULL;
   
   // out_values (OMXdouble_array write)
   int out_values_size;
   double *out_values_arr = NULL;

   int i, j, k, t, m;
   int vlen;
   int limiti, limitj, limitk, stepi, stepj, stepk;
      
   //
   //	Retrieve the input dimensions and set the output sizes
   //
   dims_arr = (int *)dims.ret_array_ptr(OM_GET_ARRAY_RD, &dims_size);
   if (dims_arr==NULL) {
      FREE_ARRAYS;
      ERROR_RETURN("Could not get dimensions array");
   }
   
   if ((dims_size<0) || (dims_size>3)) {
      FREE_ARRAYS;
      ERROR_RETURN("Invalid data dimensions. Module supports 3D, 2D or 1D data only");
   }

   veclen_arr = (int *)veclen.ret_array_ptr(OM_GET_ARRAY_RD, &veclen_size);
   if (veclen_arr==NULL) {
      FREE_ARRAYS;
      ERROR_RETURN("Could not get vector length array");
   }

   vlen = veclen_arr[0];
   ARRfree(veclen_arr);    veclen_arr = NULL;
   
   out_dims_arr = (int *)out_dims.ret_array_ptr(OM_GET_ARRAY_WR, &out_dims_size);
   if (out_dims_arr==NULL) {
      FREE_ARRAYS;
      ERROR_RETURN("Could not get output dimensions array");
   }

   if ( ((dims_size==3) && (out_dims_size!=2)) ||
        ( ((dims_size==2)||(dims_size==1)) && (out_dims_size!=1)) ) {
      FREE_ARRAYS;
      ERROR_RETURN("Output dimensions array is incorrect size");
   }


   //
   //	Compute the various limits and strides
   //
   switch(axis+10*dims_size) {

      // 3D input -> 2D output
   case 30:
      out_dims_arr[0] = dims_arr[1];
      out_dims_arr[1] = dims_arr[2];
      limiti = dims_arr[1];
      limitj = dims_arr[2];
      limitk = dims_arr[0];
      stepi = dims_arr[0];
      stepj = dims_arr[0]*dims_arr[1];
      stepk = 1;
      break;
   case 31:
      out_dims_arr[0] = dims_arr[0];
      out_dims_arr[1] = dims_arr[2];
      limiti = dims_arr[0];
      limitj = dims_arr[2];
      limitk = dims_arr[1];
      stepi = 1;
      stepj = dims_arr[0]*dims_arr[1];
      stepk = dims_arr[0];
      break;
   case 32:
      out_dims_arr[0] = dims_arr[0];
      out_dims_arr[1] = dims_arr[1];
      limiti = dims_arr[0];
      limitj = dims_arr[1];
      limitk = dims_arr[2];
      stepi = 1;
      stepj = dims_arr[0];
      stepk = dims_arr[0]*dims_arr[1];
      break;

      // 2D input -> 1D output
   case 20:
      out_dims_arr[0] = dims_arr[1];
      limiti = dims_arr[1];
      limitj = 1;
      limitk = dims_arr[0];
      stepi = dims_arr[0];
      stepj = 0;
      stepk = 1;
      break;
   case 21:
      out_dims_arr[0] = dims_arr[0];
      limiti = dims_arr[0];
      limitj = 1;
      limitk = dims_arr[1];
      stepi = 1;
      stepj = 0;
      stepk = dims_arr[0];
      break;
      
      // 1D input -> one point output
   case 10:
      out_dims_arr[0] = 1;
      limiti = 1;
      limitj = 1;
      limitk = dims_arr[0];
      stepi = 0;
      stepj = 0;
      stepk = 1;
      break;

   default:
      ERRverror("CollapseFieldCore", ERR_ERROR, "Invalid axis %d for input space dimension %d\n", (int)axis, dims_size);
      FREE_ARRAYS;
      return 0;
   }

   ARRfree(dims_arr);      dims_arr=NULL;
   ARRfree(out_dims_arr);  out_dims_arr=NULL;

   //
   //	Setup input and output arrays
   //
   values_arr = (double *)values.ret_array_ptr(OM_GET_ARRAY_RD, &values_size);
   if (values_arr==NULL) {
      FREE_ARRAYS;
      ERROR_RETURN("Could not get values array");
   }

   out_values_arr = (double *)out_values.ret_array_ptr(OM_GET_ARRAY_WR, &out_values_size);
   if (out_values_arr==NULL) {
      FREE_ARRAYS;
      ERROR_RETURN("Could not get output values array");
   }
   
   planes_arr = (int *)planes.ret_array_ptr(OM_GET_ARRAY_RD, &planes_size);
   if (planes_arr==NULL)
      planes_size = 0;

   //
   //	Perform the requested operation on every value
   //
   switch(operation)
   {
   case 0:
      //
      //	Sum along the collapsing dimension
      //
      for(i=0; i < limiti; i++)
      {
         for(j=0; j < limitj; j++)
         {
            for(t=0; t < vlen; t++)
            {
               double tmp = 0.0;
               for(k=0; k < limitk; k++)
               {
                  tmp += values_arr[(i*stepi+j*stepj+k*stepk)*vlen+t];
               }
               out_values_arr[(i+j*limiti)*vlen+t] = tmp;
            }
         }
      }
      break;

   case 1:
      //
      //	orthoslice along the collapsing axis at the first specified plane
      //
      if (planes_size < 1) {
         FREE_ARRAYS;
         ERROR_RETURN("Plane specification required for operation\n");
      }
      if (planes_arr[0] < 0 || planes_arr[0] >= limitk) {
         ERRverror("CollapseFieldCore", ERR_ERROR, "Invalid plane value %d; must be (0 - %d)\n", (int)planes_arr[0], limitk);
         FREE_ARRAYS;
         return 0;
      }
      for(i=0; i < limiti; i++)
      {
         for(j=0; j < limitj; j++)
         {
            for(t=0; t < vlen; t++)
            {
               out_values_arr[(i+j*limiti)*vlen+t] = values_arr[(i*stepi+j*stepj+planes_arr[0]*stepk)*vlen+t];
            }
         }
      }
      break;

   case 2:
      //
      //	sum along the collapsing dimension excluding specified planes
      //
      if (planes_size < 1) {
         FREE_ARRAYS;
         ERROR_RETURN("Plane specification required for operation\n");
      }
      for(i=0; i < planes_size; i++)
      {
         if (planes_arr[i] < 0 || planes_arr[i] >= limitk) {
            ERRverror("CollapseFieldCore", ERR_ERROR, "Invalid planes[%d] value %d; must be (0 - %d)\n", i, (int)planes_arr[i], limitk);
            FREE_ARRAYS;
            return 0;
         }
      }
      for(i=0; i < limiti; i++)
      {
         for(j=0; j < limitj; j++)
         {
            for(t=0; t < vlen; t++)
            {
               double tmp = 0.0;
               for(k=0; k < limitk; k++)
               {
                  for (m=0; m < planes_size; m++) {
                     if (k == planes_arr[m])
                        break;
                  }
                  if (m != planes_size)
                     continue;

                  tmp += values_arr[(i*stepi+j*stepj+k*stepk)*vlen+t];
               }
               out_values_arr[(i+j*limiti)*vlen+t] = tmp;
            }
         }
      }

      break;

   case 3:
      //
      //	sum along the collapsing dimension only the specified planes
      //
      if (planes_size < 1) {
         FREE_ARRAYS;
         ERROR_RETURN("Plane specification required for operation\n");
      }
      for(i=0; i < planes_size; i++)
      {
         if (planes_arr[i] < 0 || planes_arr[i] >= limitk) {
            ERRverror("CollapseFieldCore", ERR_ERROR, "Invalid planes[%d] value %d; must be (0 - %d)\n", i, (int)planes_arr[i], limitk);
            FREE_ARRAYS;
            return 0;
         }
      }
      for(i=0; i < limiti; i++)
      {
         for(j=0; j < limitj; j++)
         {
            for(t=0; t < vlen; t++)
            {
               double tmp = 0.0;
               for(k=0; k < limitk; k++)
               {
                  for (m=0; m < planes_size; m++) {
                     if (k == planes_arr[m])
                        break;
                  }
                  if (m == planes_size)
                     continue;

                  tmp += values_arr[(i*stepi+j*stepj+k*stepk)*vlen+t];
               }
               out_values_arr[(i+j*limiti)*vlen+t] = tmp;
            }
         }
      }

      break;

   default:
      ERRverror("CollapseFieldCore", ERR_ERROR, "Invalid operation code: %d\n", (int)operation);
      FREE_ARRAYS;
      return 0;
   }

   //
   //	Release arrays and return 1 for success
   //
   FREE_ARRAYS;

   return 1;
}

