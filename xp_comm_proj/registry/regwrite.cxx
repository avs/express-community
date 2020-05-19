
#include "xp_comm_proj/registry/gen.h"

#define FREE_ARRAYS \
   if (hkeyStr!=NULL) free(hkeyStr);\
   if (keyStr!=NULL) free(keyStr);\
   if (valueStr!=NULL) free(valueStr);\
   if (value!=NULL) free(value);



int
Registry_RegistryMods_RegistryWrite::update(OMevent_mask event_mask, int seq_num)
{
   // Key (OMXstr read)
   // HKey (OMXstr read)
   // Value (OMXstr read notify)
   // done (OMXint write)
   
   LONG error;
   HKEY MHKEY;
   HKEY hkey;

   char* hkeyStr = NULL;
   char* keyStr = NULL;
   char* valueStr = NULL;
   char* value = NULL;

   DWORD valLen;
   DWORD dwDisposition;


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
      ERRverror("RegistryWrite", ERR_NO_HEADER | ERR_PRINT, "HKey must be specified.\n");
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
      ERRverror("RegistryWrite", ERR_NO_HEADER | ERR_PRINT, "HKey is not valid.\n");
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

   // Check if user has given a Value
   if (Value.valid_obj()) {
      value = OMret_str_val(Value, NULL, 0);
      fprintf(stdout, "value = %s\n", value);
   } else {
      ERRverror("RegistryWrite", ERR_NO_HEADER | ERR_PRINT, "Value must be given.\n");
      fprintf(stdout, "### No value to write\n");
      FREE_ARRAYS;
      return(0);
   }


   // Create or Open the Specified Registry Key
   error = RegCreateKeyExA(
      MHKEY, 
      keyStr,
      0, 
      NULL, //"REG_SZ", 
      REG_OPTION_NON_VOLATILE,
      KEY_ALL_ACCESS,
      NULL,
      &hkey,
      &dwDisposition
   );

   if (error) {
      ERRverror("RegistryWrite", ERR_NO_HEADER | ERR_PRINT, "Could not open or create the specified Registry Key.\n");
      fprintf(stdout, "### CreateKey Failed\n");
      FREE_ARRAYS;
      ErrorCode = error;
      return(0);
   }

   // Show if key was created or simply opened
   if (dwDisposition==REG_CREATED_NEW_KEY) {
      fprintf(stdout, "Created New Registry Key.\n");
   } else if (dwDisposition==REG_OPENED_EXISTING_KEY) {
      fprintf(stdout, "Opened Existing Registry Key.\n");
   }

   // Calculate the string value length.
   valLen = strlen(value) + 1;
   
   // Set the specified registry value
   error = RegSetValueExA(
      hkey,
      valueStr,
      0,
      REG_SZ,
      (const unsigned char *)value,
      valLen
   );
  
   if (error) {
      ERRverror("RegistryWrite", ERR_NO_HEADER | ERR_PRINT, "Could not set the specified Registry Value.\n");
      fprintf(stdout, "### SetValue Failed\n");
      ErrorCode = error;
      return(0);
   } else {
      fprintf(stdout, "Set Registry Value Successfully.\n");
   }

   FREE_ARRAYS;
   RegFlushKey(hkey);
   RegCloseKey(hkey);

   // return 1 for success
   return(1);
}

