$push
Root.Applications
{
assist0.assist1
   {
   button_labels={"geometry","isosurface","glyphs","multislice"};
   butlabels={"geometry","isosurface","glyphs","multislice"};
   start={1,1,0,0};
       UIshell.title => "Environment Pollution Data";
   };
ModuleStack
   {
$include $ASSIST_HOME/read_fld.v   
   MODS.Mappers.bounds bounds<NEx=132.,NEy=110.> 
      {
      in_field => <-.Read_Field.field;
      };
   mlink stdlink<NEportLevels=1,NEx=352.,NEy=209.> =>  
        {.bounds.out_obj};
   GD.DefaultObject DefaultObject<NEx=407.,NEy=121.>
      {
      objects => <-.stdlink;
      };
$include $ASSIST_HOME/view3d.v   
$link Viewer3D.objs_in DefaultObject
   };
$include $ASSIST_HOME/norm.v
};
$pop