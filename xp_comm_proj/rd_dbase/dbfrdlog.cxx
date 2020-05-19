
#include <string.h>
#include <malloc.h>

#include "log_gen.h"

#include "dbf.h"

int
ReadDbase_ReadDbaseMods_ReadDBFLogical::ReadDBFLogical(OMevent_mask event_mask, int seq_num)
{
   // DBFFileName (OMXstr read req notify)
   // ColumnName (OMXstr read notify)
   // ColumnNumber (OMXint read notify)
   // ColumnValues (OMXchar_array write)
   int ColumnValues_size;
   char *ColumnValues_arr;


   /***********************/
   /* Function's Body     */
   /***********************/
#ifdef DEBUG
   ERRverror("",ERR_NO_HEADER | ERR_PRINT,
             "I'm in method: ReadDBFLogical::ReadDBFLogical\n");
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
   //  Check to see if this is an logical field.  
   //

   if (DBFFile.FieldDescriptor(ColumnNumber).FieldType() != 
       XP_GIS_DBF_FieldDescriptor_c::LOGICAL)
   {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                "The column type is not logical\n");
     return 0;
   }

   //
   //  Get the pointer to the array of column values.
   //


   ColumnValues_arr = (char *)ColumnValues.ret_array_ptr(OM_GET_ARRAY_WR,
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
       char Logical;
       if (DBFFile.ReadLogical(RowNumber,
                               ColumnNumber,
                              &Logical) != XP_GIS_OK)
       {
         ERRverror("",ERR_NO_HEADER | ERR_PRINT,
                    "Error reading logical value\n");
         return 0;
       }
       ColumnValues_arr[RowNumber-1] = Logical;
#ifdef DEBUG
       printf("Row # %d: ",RowNumber);
       printf("%c",ColumnValues_arr[RowNumber-1]);
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



