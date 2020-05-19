Applications.ModuleStack
{
MODS.Filters.combine_vect combine_vect<NEx=506.,NEy=110.> {
   in_field => <-.Read_UCD.field;
   CombineVectParam {
      components = {1,2,3};
   };
   CombVectUI {
      DVnode_data_labels {
         labels[];
      };
   };
};
MODS.Mappers.offset offset<NEx=506.,NEy=187.> {
   in_field => combine_vect.out_fld;
   OffsetUI {
      DVnode_data_labels<NEx=0.,NEy=242.> {
         labels[];
      };
      UIpanel<NEx=22.,NEy=308.>;
      UIradioBoxLabel<NEx=66.,NEy=374.>;
      scale_slider<NEx=22.,NEy=176.> {
         parent => assist0.assist1.frames[3].br;
         y = 20;
         x = 10;
         min = 0.;
         max<NEportLevels={2,0}> => <-.UIfield.value;
         value=700000;
      };
      Controls.UIfield UIfield<NEx=407.,NEy=154.> {
         parent => assist0.assist1.frames[3].br;
         y = 70;
         x = 160;
         value=1000000;
      };

      UIlabel max_value<NEx=495.,NEy=77.> {
         parent => assist0.assist1.frames[3].br;
         y = 100;
         x = 160;
         label="slider max value";
      };
   };
};
MODS.Mappers.external_faces displ_face<NEx=473.,NEy=253.> {
   in_field => <-.offset.out_fld;
};

$link Viewer3D.objs_in displ_face.out_obj
#define BUT4
};
!assist0.assist1.frames[3]{old=1;};
