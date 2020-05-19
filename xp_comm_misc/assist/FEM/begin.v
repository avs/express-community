Root.Applications
   {
   assist0.assist1
      {
      button_labels={"Display Edges","Extract Component","Explode Material",
"Displacement


Example using OFFSET Module  ",

"
Numbering

Example using TextGlyph Module

"
};
       butlabels={"Display Edges","Extract Component","Explode Material","Displacement","Numbering"};
       start={0,0,0,0,0};
       UIshell.title => "Finite Element Data";
       };
   Applications.ModuleStack
       {
$include $ASSIST_HOME/read_ucd.v   
       Read_UCD.read_ucd_ui.file_browser.filename = 
            "$XP_PATH<0>/data/ucd/shock.inp";
$include $ASSIST_HOME/view3d.v   
$link Viewer3D.objs_in Read_UCD.out_obj
       };
$include $ASSIST_HOME/norm.v
   };
