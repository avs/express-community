

flibrary RegistryMacs <compile_subs=0> {

   APPS.DefaultApplication RegistryEg {
      XP_COMM_PROJ.Registry.RegistryMods.RegistryWrite RegistryWrite<NEdisplayMode="open"> {
         HKey = "HKEY_CURRENT_USER";
         KeyName = "Software\\IAC Test Key";
         ValueName = "Test Value";
         Value = "Hello World";
      };

      XP_COMM_PROJ.Registry.RegistryMods.RegistryRead RegistryRead<NEdisplayMode="open"> {
         HKey = "HKEY_CURRENT_USER";
         KeyName = "Software\\IAC Test Key";
         ValueName = "Test Value";
      };
   };

};

