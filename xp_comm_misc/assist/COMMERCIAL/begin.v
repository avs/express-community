Root.Applications
{
assist0.assist1
   {
   button_labels=
{"city_plot","AXIS3D","Ribbon Plot"
,"
Pick Value

using GDPick_process

Picking with -Ctrl Left Button-

"};
      butlabels = {"city_plot","Axis3D","Ribbon Plot","Pick Value"};
      start={1,0,0,0};
   };

ModuleStack
   {
$include $ASSIST_HOME/read_fld.v   
$include $ASSIST_HOME/view3d.v   
$link Viewer3D.objs_in Read_Field.out_obj
   };
$include $ASSIST_HOME/norm.v

ModuleStack.Viewer3D.View3D.Light1.type=3;
};


