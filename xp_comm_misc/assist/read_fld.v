//
// Standard field reader reads example.fld from the data directory
//
   MODS.Readers.Read_Field Read_Field<NEx=44.,NEy=44.> 
      {
      DVread_field.filename = assist0.assist1.my_local_dir+"/data/example.fld";
      read_field_ui.file_browser.visible => assist0.assist1.Read_data.do;
      };
