
#include <string.h>
#include <malloc.h>

#include "flt_gen.h"

#include "dbf.h"


int ReadDbase_ReadDbaseMods_ReadDBFFloat::ReadDBFFloat(OMevent_mask event_mask, int seq_num)
{
   // DBFFileName (OMXstr read req notify)
   // ColumnName (OMXstr read notify)
   // ColumnNumber (OMXint read notify)
   // ColumnValues (OMXfloat_array write)
   int ColumnValues_size;
   float *ColumnValues_arr;


   /***********************/
   /* Function's Body     */
   /***********************/
#ifdef DEBUG
   ERRverror("",ERR_NO_HEADER | ERR_PRINT,
             "I'm in method: ReadDBFFloat::ReadDBFFloat\n");
#endif


   //
   //  Create the dbf file object and give it the file name.
   //

   XP_GIS_DBF_c DBFFile;
   if (DBFFile.FileName(DBFFileName) != XP_GIS_OK)
   {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Error opening dbf file\n");
     return 0;
   }

   //
   //  Print the dbf file header.
   //
#ifdef DEBUG
   DBFFile.Header().PrintHeader(cout);
#endif

   //
   //  Get the number of rows
   //
    
   NumberOfValues = DBFFile.NumberOfRowsPresent();

   //
   //  The column name and number are optional, but one must be
   //  provided.  Make sure at least one is provided and determine
   //  the column number.
   //

   if (!ColumnName.valid_obj() && !ColumnNumber.valid_obj())
   {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "Neither the column name nor the column number was provided\n");
     return 0;
   }

   //
   //  If the column number was not provided, get it using the column name.
   //

   if (!ColumnNumber.valid_obj()) // no column number provided, so check name
   {
     ColumnNumber = DBFFile.ColumnNameToNumber((const char*)ColumnName);
     if (ColumnNumber == 0)
     {
       ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                 "Error getting column number from column name\n");
       return 0;
     }
   }

   //
   //  Validate the column number
   //

   unsigned char LocalColumnNumber = ColumnNumber;
   if ((LocalColumnNumber < 1) ||
       (LocalColumnNumber > DBFFile.Header().NumberOfColumns()))
   {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
               "The column number is invalid\n");
     return 0;
   }
     
     
   //
   //  Print the file descriptor for the specified column
   //

#ifdef DEBUG
   DBFFile.FieldDescriptor(ColumnNumber).PrintDescriptor(cout);
#endif

   //
   //  Check to see if this is a float field.  Note that we allow an
   //  integer to be read into a float, so we don't care if the decimal
   //  count is zero.
   //

   if (DBFFile.FieldDescriptor(ColumnNumber).FieldType() != 
       XP_GIS_DBF_FieldDescriptor_c::NUMERIC)
   {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                "The column type is not numeric\n");
     return 0;
   }

   //
   //  Get the pointer to the array of column values.
   //

   ColumnValues_size = NumberOfValues;
   ColumnValues_arr = (float *)ColumnValues.ret_array_ptr(OM_GET_ARRAY_WR,
                                                         &ColumnValues_size);

   //
   //  Now, for each row in the file that has not been marked as deleted,
   //  read the value from column number ColumnNumber.
   //

   for (unsigned long RowNumber = 1;
                      RowNumber <= NumberOfValues;
                      RowNumber++)
   {
     {
       float Float;
       if (DBFFile.ReadFloat(RowNumber,
                             ColumnNumber,
                            &Float) != XP_GIS_OK)
       {
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                    "Error reading float value\n");
         return 0;
       }
       ColumnValues_arr[RowNumber-1] = Float;
#ifdef DEBUG
       char FormatString[81];
       sprintf(FormatString,"%%%d.%df",
               DBFFile.FieldDescriptor(ColumnNumber).FieldLength(),
               DBFFile.FieldDescriptor(ColumnNumber).FieldDecimalCount());
       printf("Row # %d: ",RowNumber);
       printf(FormatString,ColumnValues_arr[RowNumber-1]);
       printf("\n");
       fflush(stdout);
#endif
     }
   }

   if (ColumnValues_arr)
   ARRfree((char *)ColumnValues_arr);

   // return 1 for success
   return(1);
}



