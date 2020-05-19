"$XP_PATH<0>\\v\\templ.v" Templates {

   // Express Configuration setup current project
   CONFIG
   {
      gd_vps_disabled = 1;
      ac_kit_disabled = 1;
      gis_disabled = 1;
      gd_prt_disabled = 1;
      anim_kit_disabled = 0;
      w3c_disabled = 1;
      gd_xgl_disabled = 1;
      gd_pex_disabled = 1;
      gd_xil_disabled = 1;
      ag_kit_disabled = 1;
      db_kit_disabled = 0;
      db_oracle_disabled = 1;
      db_sybase_disabled = 1;
      db_informix_disabled = 1;
   };

  // Load MERL library
  //
  $echo Loading MERL Library
  "merlren.v" MERL;

  $echo

};
