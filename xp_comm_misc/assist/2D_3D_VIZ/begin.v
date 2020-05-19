Applications
{
assist0.assist1.button_labels={};
assist0.assist1.butlabels={};
ModuleStack
   {
   int xxx;
   macro field_dim_change<NEx=66.,NEy=77.> 
      {
      link in_field<NEportLevels={2,1},NEx=11.,NEy=22.> => <-.Read_Field.field;
      int dim<NEportLevels=1,NEx=11.,NEy=55.> => in_field.ndim;
      GMOD.copy_on_change copy_on_change<NEx=11.,NEy=99.,NEportLevels={0,1}> 
         {
         input => <-.dim;
         output<NEportLevels={0,3}>;
         };
      int olddim<NEportLevels=1,NEx=121.,NEy=165.> => .copy_on_change.output;
      GMOD.instancer instancer<NEx=121.,NEy=132.> 
         {
         Value => abs((dim - olddim));
         Group => <-.copy_on_change;
         };
      string cmacro<NEx=121.,NEy=200.> = "assist0.assist1.dummy=0;-ModuleStack.xxx;";
      string ndir<NEx=121.,NEy=230.> => "/"+dim+"d";
$push
      GMOD.parse_v clear<NEx=55.,NEy=260.> 
         {
         v_commands =>"dummy=0;";
         trigger<NEportLevels={2,2}> => <-.copy_on_change.output;
         on_inst=0;
         relative=Applications.assist0.assist1;
         };
      GMOD.parse_v del_old<NEx=55.,NEy=260.> 
         {
         v_commands => <-.cmacro+
           "ModuleStack.field_dim_change.cmacro=str_format(\"-ModuleStack.mapper_%dD;\",dim);";
         trigger<NEportLevels={2,2}> => <-.copy_on_change.output;
         on_inst=0;
         relative=Applications;
         };
$pop
      GMOD.load_v_script l_new<NEx=55.,NEy=300.> 
         {
         filename => ((assist0.assist1.my_dir + <-.ndir) + "/begin.v");
         trigger => <-.del_old.trigger;
         on_inst = 0;
         relative=Applications;
         };
       
      };
   MODS.Readers.Read_Field Read_Field<NEx=198.,NEy=121.>
      {
      read_field_ui.file_browser.visible => assist0.assist1.Read_data.do;
      DVread_field.filename = "$ASSIST_HOME/2D_3D_VIZ/2d/data/sample2d.fld";
      };
   MODS.Mappers.bounds bounds<NEx=198.,NEy=187.>;
$link bounds.in_field Read_Field.field
$include $ASSIST_HOME/view3d.v   
$link Viewer3D.objs_in bounds.out_obj
    };

$include $ASSIST_HOME/norm.v
};
