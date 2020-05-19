
flibrary Registry {

#ifdef MSDOS
   "../xp_comm_proj/registry/reg_mods.v"   RegistryMods;
   "../xp_comm_proj/registry/reg_macs.v"   RegistryMacs;
#else
   group Registry_Only_Available_On_Windows;
#endif

};

