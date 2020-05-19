Root.Applications
   {
   assist0.assist1
       {
       button_labels = 
             {"streamlines","glyphs","particle advector","isosurface"};
       butlabels = 
             {"streamlines","glyphs","particle advector","isosurface"};
       start={1,0,0,0};
       UIshell.title="Computational Fluid Dynamics";
       };

   ModuleStack
       {
$include $ASSIST_HOME/read_ucd.v   
       Read_UCD.read_ucd_ui.file_browser.filename = 
           "$ASSIST_HOME/cfd/data/cyclone.inp";
       MODS.Mappers.external_edges external_edges<NEx=231.,NEy=55.> 
          {
          in_field => <-.Read_UCD.field;
          };

$include $ASSIST_HOME/view3d.v   
$link Viewer3D.objs_in external_edges.out_obj
$link Viewer3D.objs_in streamlines.out_obj
      };
$include $ASSIST_HOME/norm.v
   };
