//
// Launch HTML Help browser
//
// This module checks for two environenment variables:
//  "XP_HELP_DIR"  = directory path where help files are found
//  "HTML_BROWSER" = name of HTML browser program to launch
// both are optional, if not specified help files will be
// assumed in "./help" directory, and netscape will be assumed
// as browser.
//

#include "gen.h"
#include <avs/f_utils.h>


#ifdef MSDOS
#include <process.h>
#endif

// set default path, must end in forward slash!
// #define HTML_HELP_DEFAULT_DIR "help/"
#define HTML_HELP_DEFAULT_DIR "./"


// set default browser for Unix platforms
#define HTML_HELP_DEFAULT_BROWSER "netscape"


int
HtmlHelp_HtmlHelpMods_HtmlHelpCore::update(OMevent_mask event_mask, int seq_num)
{
   char file_path[AVS_PATH_MAX];
   char cmd[AVS_PATH_MAX];
   char *file = (char *)help_filename;
   char *helpdir;

   // Get the help environment variable
   helpdir = getenv("XP_HELP_DIR");
   if (helpdir == NULL) {
      sprintf(file_path, "%s%s", HTML_HELP_DEFAULT_DIR, file);
   } else {
      sprintf(file_path, "%s%s", helpdir, file);
   }


   // Check that the file exists.
   FILE *fp = fopen(file_path, "r");
   if (fp == NULL) {
      ERRverror("",ERR_PRINT|ERR_NO_HEADER,"cannot find help file %s\n",file_path);
      return(0);
   } else {
      fclose(fp);
   }


#ifdef MSDOS
   // Run file, hence starting associated web browser
   HINSTANCE hInstance = ShellExecute(NULL, "open", file_path, NULL, NULL, SW_SHOWNORMAL);

   // Check that command was successful
   if (hInstance < (HINSTANCE)HINSTANCE_ERROR) {
      ERRverror("",ERR_PRINT|ERR_NO_HEADER, "Unable to open help file. Check that file extensions\n\
                                            \".html\" are associated with an html browser such as Internet Explorer\n\
                                            or Netscape Navigator");
      return(0);
   }

#else
   // get browser environment variable
   char *browser = getenv("HTML_BROWSER");
   if (browser == NULL)
      browser = HTML_HELP_DEFAULT_BROWSER;

   // built and execute command string
   sprintf(cmd, "%s %s &", browser, file_path);
   system(cmd);
#endif

   return(1);
}

