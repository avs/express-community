Applications.ModuleStack
    {
    MODS.Readers.Read_Geom Read_Geom<NEx=308.,NEy=44.>
      {
      read_geom_ui.file_browser.visible => 
             assist0.assist1.frames[0].toggle_set.output;
      ACread_geom.filename="$ASSIST_HOME/environment/data/map.geo";
      };
    };
$push
$link ModuleStack.stdlink ModuleStack.Read_Geom.geom
$pop
!assist0.assist1.frames[0]{old=1;};
