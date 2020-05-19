Root.Applications
   {
   assist0.assist1
      {
      button_labels=
          {"isosurface","slice","cropped iso","volumetry","tile"};
      butlabels=
          {"isosurface","slice","cropped iso","volumetry","tile"};
      start={1,0,0,0,0};
      UIshell.title => "Computer Tomography Scan";
      };
   ModuleStack
      {
$include $ASSIST_HOME/read_fld.v   
      MODS.Mappers.bounds bounds<NEx=242.,NEy=198.>;
$link bounds.in_field Read_Field.field
$include $ASSIST_HOME/view3d.v   
$link Viewer3D.objs_in bounds.out_obj
      };
$include $ASSIST_HOME/norm.v
   };
