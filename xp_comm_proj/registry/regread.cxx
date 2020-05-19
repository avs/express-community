
#include "xp_comm_proj/registry/gen.h"

#define FREE_ARRAYS \
   if (hkeyStr!=NULL) free(hkeyStr);\
   if (keyStr!=NULL) free(keyStr);\
   if (valueStr!=NULL) free(valueStr);\
   if (value!=NULL) free(value);


int
Registry_RegistryMods_RegistryRead::update(OMevent_mask event_mask, int seq_num)
{
   // Key (OMXstr read notify)
   // HKey (OMXstr read)
   // Value (OMXstr write)
   // ValueF (OMXfloat write)

   LONG error;
   HKEY MHKEY;
   HKEY hkey;

   char* hkeyStr = NULL;
   char* keyStr = NULL;
   char* valueStr = NULL;

   DWORD valType;
   DWORD valLen;
   char* value = NULL;



   /***********************/
   /* Function's Body     */
   /***********************/

   // Start log on a clear line
   fprintf( stdout, "\n");

   // Clear ErrorCode
   ErrorCode = 0;

   // Check if root HKey has been given
   if (HKey.valid_obj()) {
      hkeyStr = OMret_str_val(HKey, NULL, 0);
      fprintf(stdout, "HKey = %s\n", hkeyStr);
   } else {
      ERRverror("RegistryRead", ERR_NO_HEADER | ERR_PRINT, "HKey must be specified.\n");
      fprintf(stdout, "### HKey is not available\n");
      return(0);
   }

   // Check if root HKey is valid
   if (strcmp(hkeyStr,"HKEY_CLASSES_ROOT")==0)
      MHKEY=HKEY_CLASSES_ROOT;
   else if (strcmp(hkeyStr,"HKEY_CURRENT_USER")==0)
      MHKEY=HKEY_CURRENT_USER;
   else if (strcmp(hkeyStr,"HKEY_LOCAL_MACHINE")==0)
      MHKEY=HKEY_LOCAL_MACHINE;
   else if (strcmp(hkeyStr,"HKEY_USERS")==0)
      MHKEY=HKEY_USERS;
   else if (strcmp(hkeyStr,"HKEY_CURRENT_CONFIG")==0)
      MHKEY=HKEY_CURRENT_CONFIG; 
   else if (strcmp(hkeyStr,"HKEY_DYN_DATA")==0)
      MHKEY=HKEY_DYN_DATA;
   else if (strcmp(hkeyStr,"HKEY_PERFORMANCE_DATA")==0)
      MHKEY=HKEY_PERFORMANCE_DATA;
   else
   {
      ERRverror("RegistryRead", ERR_NO_HEADER | ERR_PRINT, "HKey is not valid.\n");
      fprintf(stdout, "### HKey is invalid\n");
      FREE_ARRAYS;
      return(0);
   }

   fprintf(stdout, "HKey is valid\n");
   free(hkeyStr);
   hkeyStr = NULL;


   // Check if user has given a KeyName
   if (KeyName.valid_obj()) {
      keyStr = OMret_str_val(KeyName, NULL, 0);
      fprintf(stdout, "keyStr = %s\n", keyStr);
   } else {
      keyStr = NULL;
      fprintf(stdout, "Reading root key\n");
   }

   // Check if user has given a ValueName
   if (ValueName.valid_obj()) {
      valueStr = OMret_str_val(ValueName, NULL, 0);
      fprintf(stdout, "valueStr = %s\n", valueStr);
   } else {
      valueStr = NULL;
      fprintf(stdout, "Reading default value\n");
   }


   // Open specified Registry Key
   error = RegOpenKeyEx(
      MHKEY,
      keyStr,
      0,
      KEY_READ,
      &hkey
   );

   if (error) {
      ERRverror("RegistryRead", ERR_NO_HEADER | ERR_PRINT, "Could not open specified Registry Key.\n");
      fprintf(stdout, "### OpenKey Failed\n");
      FREE_ARRAYS;
      ErrorCode = error;
      return(0);
   }



   // Get the required buffer size for the specified value.
   error = RegQueryValueExA(
      hkey,
      valueStr,
      NULL,
      &valType,
      NULL,
      &valLen
   );
  
   if (error) {
      ERRverror("RegistryRead", ERR_NO_HEADER | ERR_PRINT, "Could not find specified Registry Value.\n");
      fprintf(stdout, "### GetSize QueryKey Failed\n");
      FREE_ARRAYS;
      ErrorCode = error;
      RegCloseKey(hkey);
      return(0);
   }


   fprintf(stdout, "Value Length = %d\n", valLen);

   if (valType!=REG_SZ) {
      ERRverror("RegistryRead", ERR_NO_HEADER | ERR_PRINT, "Registry Value is not a string.\n");
      fprintf(stdout, "### Value is not String\n");
      FREE_ARRAYS;
      ErrorCode = 0;
      RegCloseKey(hkey);
      return(0);
   }

   
   value = (char*)malloc(valLen * sizeof(char) );

   error = RegQueryValueExA(
      hkey,
      valueStr,
      NULL,
      &valType,
      (unsigned char*)value,
      &valLen
   );


   if (error) {
      ERRverror("RegistryRead", ERR_NO_HEADER | ERR_PRINT, "Could not get Registry Value.\n");
      fprintf(stdout, "### GetValue QueryKey Failed\n");
      FREE_ARRAYS;
      ErrorCode = error;
      RegCloseKey(hkey);
      return(0);
   }


   fprintf(stdout, "Value = (%s)\n", value);

   // Set string version of value
   Value.set_str_val(value);

   // Set float version of value
   float V;
   char *tVal;
   V = strtod(value,&tVal);
   ValueF = V;


   FREE_ARRAYS;
   RegCloseKey(hkey);

   // return 1 for success
   return(1);
}

