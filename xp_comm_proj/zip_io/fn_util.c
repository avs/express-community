#include <string.h>

/* Setup defines so that the appropriate separator is searched for first */

#ifdef MSDOS

#define FNU_FileSeparator1 92
#define FNU_FileSeparator2 47

#else

#define FNU_FileSeparator1 47
#define FNU_FileSeparator2 92

#endif


void spiltFilename(char* fullFilename, char* filename, char* pathname) {
   char *pdest;   /* pointer to position of last slash character */
   int path_len;  /* Length of path string */

   pdest = strrchr(fullFilename, FNU_FileSeparator1);

   if (pdest==NULL)
      pdest = strrchr(fullFilename, FNU_FileSeparator2);

   if (pdest!=NULL)
   {
      path_len = pdest - fullFilename + 1;

      if (pathname!=NULL) {
        strcpy(pathname, fullFilename);
        pathname[path_len-1] = '\0';
      }
      
      if (filename!=NULL) {
         strcpy(filename, (fullFilename+path_len) );
      }
   }
   else {
      if (pathname!=NULL) {
        strcpy(pathname, "");
      }
      if (filename!=NULL) {
         strcpy(filename, fullFilename);
      }
   }
}


int removeSuffix(char* filename, char* suffix, char* newFilename) {
   int filename_len = strlen(filename);
   int suffix_len   = strlen(suffix);
   int result;


   if ( (filename_len > suffix_len) && (strcmp( (filename + filename_len - suffix_len), suffix) == 0) ) {
      if (newFilename!=NULL) {
         strcpy(newFilename, filename);
         newFilename[filename_len - suffix_len] = '\0';
      }

      result = 1;
   }
   else {
      /* Filename doesn't have the right suffix */
      result = 0;
   }

   return result;
}


int addSuffix(char* filename, char* suffix, char* newFilename) {
   int filename_len = strlen(filename);
   int suffix_len   = strlen(suffix);
   int result;


   if ( (filename_len > suffix_len) && (strcmp( (filename + filename_len - suffix_len), suffix) == 0) ) {
      /* Filename already has the suffix specified */
      result = 0;
   }
   else {
      if (newFilename!=NULL) {
         strcpy(newFilename, filename);
         strcat(newFilename, suffix);
      }

      result = 1;
   }

   return result;
}



void combinePathAndFilename(char* pathname, char* filename, char* fullFilename) {
   int pathname_len = strlen(pathname);
   int filename_len = strlen(filename);

   strcpy(fullFilename, pathname);

   if ((fullFilename[pathname_len-1]!=FNU_FileSeparator1) && (fullFilename[pathname_len-1]!=FNU_FileSeparator2)) {
      fullFilename[pathname_len] = FNU_FileSeparator1;
      fullFilename[pathname_len+1] = '\0';
      pathname_len++;
   }

   strcat(fullFilename, filename);
}

