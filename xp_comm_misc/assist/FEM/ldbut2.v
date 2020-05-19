Applications.ModuleStack
{

MODS.Filters.extract_component extract_component<NEx=187.,NEy=154.> {
   in_field => <-.Read_UCD.field;
   ExtrCompUI {
   panel.option.set=1;
      DVnode_data_labels {
         labels[];
      };
      UIradioBoxLabel {
         UIradioBox {
            y = 30;
            parent => assist0.assist1.frames[1].br;
            x = 10;
         };
         UIlabel {
            y = 0;
            parent => assist0.assist1.frames[1].br;
            x = 10;
         };
      };
   };
};
$link Viewer3D.objs_in extract_component.out_obj
$push
$del_elem_ref Viewer3D.objs_in Read_UCD.out_obj
$pop
#define BUT2
};
!assist0.assist1.frames[1]{old=1;};
