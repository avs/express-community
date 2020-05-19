
#include "xp_comm_proj/dispfile/gen.h"
#include "string.h"


// Setup defines so that the appropriate separator is searched for first

#ifdef MSDOS

#define DSF_FileSeparator1 92
#define DSF_FileSeparator2 47

#else

#define DSF_FileSeparator1 47
#define DSF_FileSeparator2 92

#endif


int
DisplaySplitFilename_DisplaySplitFilenameMods_DSFCore::update(OMevent_mask event_mask, int seq_num)
{
   // FullFileName (OMXstr read notify)
   // FileName (OMXstr write)
   // Path (OMXstr write)
   // ValidName (OMXint write)

   /***********************/
   /* Function's Body     */
   /***********************/
   //ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: DSFCore::update\n");

   char *pdest;   // pointer to position of last slash character
   int path_len;  // Length of path string

   char orig_str[255];  // Buffer used to store original filename
   char temp_str[255];  // Buffer used to temporarily store returned strings

   strcpy(orig_str, "");
   OMret_str_val(FullFileName, orig_str, 255);
   pdest = strrchr(orig_str, DSF_FileSeparator1);

   if (pdest==NULL)
      pdest = strrchr(orig_str, DSF_FileSeparator2);

   if (pdest!=NULL)
   {
      path_len = pdest - orig_str + 1;
      strcpy(temp_str, orig_str);
      temp_str[path_len-1] = '\0';
      Path.set_str_val(temp_str);
      
   //	strncpy( tstring , string , result-1);
   //	strcat(tstring,"\n");
      
      strcpy(temp_str, "");
      strncpy(temp_str, (orig_str+path_len), (255-path_len) );
      FileName.set_str_val(temp_str);

      ValidName = 1;
   }
   else
   {
      Path.set_str_val("");
      FileName.set_str_val("");
      ValidName = 0;
   }


   // return 1 for success
   return(1);
}
// end of file

