#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <avs/f_utils.h>

#ifdef MSDOS
#include <process.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#endif

#include "xp_comm_proj/st_proc/gen.hxx"


int
StartProcess_StartProcessMods_StartProcessFunc::update(OMevent_mask event_mask, int seq_num)
{
#ifdef MSDOS
   DWORD result_val = -1;
#else
   int result_val = -1;
#endif

   int stat;
   char command_buf[AVS_PATH_MAX];

   if ( (!(trigger.valid_obj())) ||
        (!(command.valid_obj())) )
   {
      // return 0 for failure
      return(0);
   }

   // Use AVS/Express function to map enviroment variables
   FILEmap_variables((char *)command, command_buf);

   if (wait_on_exit) result = -1;


#ifdef MSDOS
   STARTUPINFO si = {0};
   PROCESS_INFORMATION pi = {0};
   si.cb = sizeof(si);
   si.lpDesktop = NULL;
   si.dwFlags = STARTF_USESHOWWINDOW;
   si.wShowWindow = SW_NORMAL;

   stat = CreateProcess (NULL, command_buf,
                         NULL, NULL,
                         TRUE, 0,
                         NULL, NULL,
                         &si, &pi);
   if (!stat) {
      ERRverror("", ERR_NO_HEADER | ERR_PRINT, "Error while attempting to Create Process.\n");
      // return 0 for failure
      return(0);
   }


   if (wait_on_exit) {
      result_val = STILL_ACTIVE;

      while (result_val == STILL_ACTIVE) {
         Sleep(100);

         stat = GetExitCodeProcess(pi.hProcess, &result_val);
         if (!stat) {
            ERRverror("", ERR_NO_HEADER | ERR_PRINT, "Error while getting Exit Code.\n");
            // return 0 for failure
            return(0);
         }
      }
   }
#else
   if (!wait_on_exit) {
      strcat(command_buf, " &");
   }

   result_val = system(command_buf);

   if (result_val==-1) {
      ERRverror("", ERR_NO_HEADER | ERR_PRINT, "Error while attempting to Create Process.\n");
      // return 0 for failure
      return(0);
   }

   stat = WIFEXITED(result_val);

   if (stat) {
      result_val = WEXITSTATUS(result_val);
   } else {
      ERRverror("", ERR_NO_HEADER | ERR_PRINT, "Process did not exit normally.\n");
      // return 0 for failure
      return(0);
   }
#endif

   if (wait_on_exit) result = result_val;


   // return 1 for success
   return(1);
}

