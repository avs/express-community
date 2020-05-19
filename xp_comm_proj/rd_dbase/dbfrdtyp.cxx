
#include <string.h>
#include <malloc.h>

#include "typ_gen.h"

#include "dbf.h"

int
ReadDbase_ReadDbaseMods_ReadDBFType::ReadDBFType(OMevent_mask event_mask, int seq_num)
{
   // DBFFileName (OMXstr read req notify)
   // ColumnName (OMXstr read notify)
   // ColumnNumber (OMXint read notify)
   // ColumnType (OMXint write)

   /***********************/
   /* Function's Body     */
   /***********************/
#ifdef DEBUG
   ERRverror("",ERR_NO_HEADER | ERR_PRINT,
             "I'm in method: ReadDBFType::ReadDBFType\n");
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
   //  Get the column type.
   //

   switch (DBFFile.FieldDescriptor(ColumnNumber).FieldType())
   {
     case XP_GIS_DBF_FieldDescriptor_c::CHARACTER:
       ColumnType = 1;          // string
       break;
     case XP_GIS_DBF_FieldDescriptor_c::DATE:
       ColumnType = 2;          // date
       break;
     case XP_GIS_DBF_FieldDescriptor_c::LOGICAL:
       ColumnType = 3;          // logical
       break;
     case XP_GIS_DBF_FieldDescriptor_c::NUMERIC:
       if (DBFFile.FieldDescriptor(ColumnNumber).FieldDecimalCount() == 0)
       {
         ColumnType = 4;        // integer
       }
       else
       {
         ColumnType = 5;        // float
       }
       break;
     default:
       ColumnType = 0;          // invalid
   }

   // return 1 for success
   return(1);
}

