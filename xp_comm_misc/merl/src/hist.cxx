
#include "express.h"
#include "hist.hxx"


int
Volume_Histogram::ComputeHistogram(OMevent_mask , int )
{
   // volume (Mesh_Unif+Node_Data read notify)
   // data_component (OMXint read notify)
   // threshold (OMXint read notify)
   // histogram (OMXint_array write)

   int comp = (int)data_component;
   int thresh = (int)threshold;
   int dataSize;
   int dataType;
   void *data = volume.node_data[comp].values.ret_array_ptr(OM_GET_ARRAY_RD,
                                                            &dataSize,
                                                            &dataType);
   int histSize;
   int *hist = (int *)histogram.ret_array_ptr(OM_GET_ARRAY_WR, &histSize);

   if (data && hist)
   {
      int i;
      int value;

      for (i = 0; i < histSize; i++)
         hist[i] = 0;

      if ((dataType == OM_TYPE_CHAR) || (dataType == OM_TYPE_BYTE))
      {
         unsigned char *bytes = (unsigned char *)data;

         for (i = 0; i < dataSize; i++, bytes++)
         {
            value = (int)(*bytes);

            if (value >= thresh)
               ++hist[value];
         }
      }
      else if (dataType == OM_TYPE_SHORT)   // actually 12 bits
      {
         unsigned short *shorts = (unsigned short *)data;
         unsigned short clamped;

         for (i = 0; i < dataSize; i++, shorts++)
         {
            clamped = *shorts;

            if (clamped > 0x0FFF)
               clamped = 0x0FFF;

            value = (int)clamped;

            if (value >= thresh)
               ++hist[value >> 4];
         }
      }
      else
         ERRverror("Histogram", ERR_ERROR,
                   "Histogram only supports char, byte, or short data\n");
   }
   else
      ERRverror("Histogram", ERR_ERROR,
                "Unable to get volume data array\n");

   if (data)
      ARRfree(data);

   if (hist)
      ARRfree(hist);

   return 1;
}
