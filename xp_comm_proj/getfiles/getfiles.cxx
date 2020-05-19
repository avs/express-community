/*
   This module finds all the files with matching the file extension
   in the directory. This implementation does not use wildcards.

   Inputs:
      dir : the directory to look in
      ext: the file type to look for
   Outputs:
      nFiles : the number of files found in the directory
      files  : the filenames

   Revisions:
      98-11-12: written by J. Maccelari, visualn@iafrica.com
      99-03-17: added VISfind_file_close()'s, JM
      99-04-16: added UNIX VISfind_file_xxx stuff, JM
*/

#include "./xp_comm_proj/getfiles/gen.hxx"
#include <avs/f_utils.h>

#define MAX_DRIVE_LEN 3
#define MAX_DIR_LEN 1024
#define MAX_FILENAME_LEN 1024


#ifdef MSDOS

#include <io.h>
#include <wtypes.h>
long int getFilesHandle;
_finddata_t getFilesData;

#else

// POSIX compliant calls used - hope this works
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
DIR *getFilesDir;
struct dirent *getFilesEntry;

#endif


char* VISfind_file_first(char* dir,char* ext,char *buf);
char* VISfind_file_next(char *dir, char *ext, char *buf);
void VISfind_file_close();
bool VIScheck_ext(char *fname,char *ext);


// Uncomment the following line to get debugging data
//#define IAC_DEBUG


int
GetFiles_GetFilesMods_GetFilesCore::update(OMevent_mask event_mask, int seq_num)
{
   // dir (OMXstr read write notify)
   // ext (OMXstr read write notify)
   // nFiles (OMXint write)
   // files (OMXstring_array write)

   /***********************/
   /* Function's Body     */
   /***********************/
   int f, nf;
   char fname[MAX_DRIVE_LEN+MAX_DIR_LEN+MAX_FILENAME_LEN], msg[128];
   char ext_buf[AVS_PATH_MAX];
   char dir_buf[AVS_PATH_MAX];

#ifdef IAC_DEBUG
ERRverror("",ERR_NO_HEADER|ERR_PRINT,"I'm in GetFiles::update\n");
#endif


   // reset output
   nf = (int)nFiles;
   for (f = 0; f < nf; f++)
      OMset_str_array_val((OMobj_id)files,f,NULL);
   nFiles = (int)(nf = (int)0);

   // Use AVS/Express function to map enviroment variables
   FILEmap_variables((char *)dir, dir_buf);
   FILEmap_variables((char *)ext, ext_buf);

   // No directory supplied. No files can be found so quit.
   if (strlen(dir_buf)==0)
      return(1);


#ifdef IAC_DEBUG
ERRverror("",ERR_NO_HEADER|ERR_PRINT,"dir = %s, ext = %s\n", dir_buf, ext_buf);
#endif


   // count files
   if (VISfind_file_first(dir_buf, ext_buf, fname) != NULL)
   {
      nf++;
#ifdef IAC_DEBUG
ERRverror("",ERR_NO_HEADER|ERR_PRINT,"Found first %d\n", nf);
#endif
      while (VISfind_file_next(dir_buf, ext_buf, fname) != NULL)
      {
         nf++;
#ifdef IAC_DEBUG
ERRverror("",ERR_NO_HEADER|ERR_PRINT,"Found next %d\n",nf);
#endif
      }
   }
   VISfind_file_close();

   // get files
   nFiles = (int)nf;

   // if there are no files to list we can finish early
   if (nf==0)
      return(1);

   f = 0;
   if ( (f<nf) && (VISfind_file_first(dir_buf, ext_buf, fname) != NULL) )
   {
#ifdef IAC_DEBUG
ERRverror("",ERR_NO_HEADER|ERR_PRINT,"Found first file = %s\n", (char*)fname);
#endif
      OMset_str_array_val((OMobj_id)files,f,fname);
      f++;
      while (VISfind_file_next(dir_buf, ext_buf, fname) != NULL)
      {
#ifdef IAC_DEBUG
ERRverror("",ERR_NO_HEADER|ERR_PRINT,"Found next file = %s\n", (char*)fname);
#endif
         OMset_str_array_val((OMobj_id)files,f,fname);
         f++;
      }
   }
   VISfind_file_close();


   // return 1 for success
   return(1);
}



/*
   This function finds the first file matching ext in the dir. It returns
   the filename.
*/
char* VISfind_file_first(char* dir, char* ext, char *buf)
{
#ifdef MSDOS
   int res, dir_len, ext_len;
   bool found = false;

   dir_len = strlen(dir);
   if (dir[dir_len-1]=='\\') {
      dir[dir_len-1] = 0;
   }

   ext_len = strlen(ext);
   if (ext_len==0) {
      sprintf(buf, "%s\\*.", dir);
   } else if ((ext_len==1) && (ext[0]=='*')) {
      sprintf(buf, "%s\\*", dir);
   } else {
      sprintf(buf, "%s\\*.%s", dir, ext);
   }

#ifdef IAC_DEBUG
ERRverror("",ERR_NO_HEADER|ERR_PRINT,"Searching for : %s\n", buf);
#endif

   if ((getFilesHandle = _findfirst(buf,&getFilesData)) == -1L)
   {
      strcpy(buf,"");
      return(NULL);
   }
   else
   {
      // check for dir
      if (getFilesData.attrib == 16)
      {
         do 
         {
            res = _findnext(getFilesHandle,&getFilesData);
            found = (res != -1);
            // check for directory entry
            if (found)
            {
               if (getFilesData.attrib == 16)
                  found = false;
            }
         } while ((res != -1) && (found == false));
      }
      else
      {
         found = true;
      }

      if (found)
      {
         strcpy(buf,getFilesData.name);
         return(buf);
      }
      else
      {
         strcpy(buf,"");
         return(NULL);
      }
   }
#else
   char fname[MAX_DRIVE_LEN+MAX_DIR_LEN+MAX_FILENAME_LEN];
   bool found;
   DIR *testDir;

   if ((getFilesDir = opendir(dir)) == NULL)
      return(NULL);
   else
   {
      found = false;
      // find first matching entry
      do 
      {
         if ((getFilesEntry = readdir(getFilesDir)) == NULL)
            return(NULL);
         if (VIScheck_ext(getFilesEntry->d_name,ext))
            found = true;

         // check the match is not a directory
         sprintf(fname,"%s/%s", dir, getFilesEntry->d_name);
         if ((testDir = opendir(fname)) != NULL)
         {
            // if it is, continue
            closedir(testDir);
            found = false;
         }
      } while ((!found) && (getFilesEntry != NULL));

      if (getFilesEntry == NULL)
      {
         strcpy(buf,"");
         return(NULL);
      }
      else
      {
         strcpy(buf,getFilesEntry->d_name);
         return(buf);
      }
   }
#endif
}

/*
   This function finds the next file matching ext in the dir. It returns
   the filename. VISfind_file_first() must be called first.
*/
char* VISfind_file_next(char *dir,char *ext,char *buf)
{
#ifdef MSDOS
   bool found = false;
   int res;

   do 
   {
      res = _findnext(getFilesHandle,&getFilesData);
      found = (res != -1);
      // check for directory entry
      if (found)
      {
         if (getFilesData.attrib == 16)
            found = false;
      }
         
   } while ((res != -1) && (found == false));
   if (found)
   {
      strcpy(buf,getFilesData.name);
      return(buf);
   }
   else
   {
      strcpy(buf,"");
      return(NULL);
   }
#else
   char fname[MAX_DRIVE_LEN+MAX_DIR_LEN+MAX_FILENAME_LEN];
   bool found = false;
   DIR *testDir;

   do 
   {
      if ((getFilesEntry = readdir(getFilesDir)) == NULL)
         return(NULL);
      if (VIScheck_ext(getFilesEntry->d_name,ext))
         found = true;

      // check the match is not a directory
      sprintf(fname,"%s/%s", dir, getFilesEntry->d_name);
      if ((testDir = opendir(fname)) != NULL)
      {
         // if it is, continue
         closedir(testDir);
         found = false;
      }
   } while ((!found) && (getFilesEntry != NULL));

   if (getFilesEntry == NULL)
   {
      strcpy(buf,"");
      return(NULL);
   }
   else
   {
      strcpy(buf,getFilesEntry->d_name);
      return(buf);
   }
#endif
}


/*
   This funtion checks a file extension
*/
bool VIScheck_ext(char *fname,char *ext) {
   int ext_len, f_ext_len, min_len, i;
   char *f_ext;

   ext_len = strlen(ext);
   if ((ext_len>0) && (ext[0]=='*'))
      return true;
   
   f_ext = strrchr(fname, '.');
   if (f_ext==NULL) {
      if (ext_len==0)
         return true;
      else
         return false;
   }

   f_ext++;
   f_ext_len = strlen(f_ext);
      
   if ((ext_len==0) && (f_ext_len==0))
      return true;

   if (ext_len<f_ext_len)
      min_len = ext_len;
   else
      min_len = f_ext_len;
      
   if (min_len==0)
      return false;

   for (i=0; i<min_len; i++) {
      if (ext[i]=='*') {
         return true;
      }
      else if (f_ext[i]!=ext[i]) {
         return false;
      }
   }

   if ((i<ext_len) && (ext[i]=='*'))
      return true;

   if (ext_len==f_ext_len)
      return true;

   // Failed all tests, extension must not match
   return false;
}



/*
   This funtion closes the file find.
*/
void VISfind_file_close()
{
#ifdef MSDOS
   _findclose(getFilesHandle);
#else
   if (getFilesDir!=NULL)
      closedir(getFilesDir);
#endif
}

