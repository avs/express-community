Applications.ModuleStack
   {

MODS.Mappers.external_edges external_edges<NEx=22.,NEy=154.> {
   in_field => <-.Read_UCD.field;
   ExtEdgeUI {
      ang_slider {
         parent => assist0.assist1.frames[0].br;
         x = 30;
         y = 50;
      };
   };

};
$link Viewer3D.objs_in external_edges.out_obj
$push
$del_elem_ref Viewer3D.objs_in Read_UCD.out_obj
$pop
};
!assist0.assist1.frames[0]{old=1;};
