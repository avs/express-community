Applications.ModuleStack{

MODS.Mappers.isosurface isosurface<NEx=22.,NEy=253.> {
   in_field => <-.combine_comp.out_fld;
   IsoUI {
      UIpanel<NEx=187.,NEy=154.,NEdisplayMode="open"> {
         width<NEdisplayMode="open"> = 150;
      };

      UIiso_level {
         parent => assist0.assist1.frames[3].br;
         width=100;
         y = 10;
         x = 10;
         visible=1;
      };


      UIradioBoxLabel {
         UIradioBox {
            y = UIiso_level.y+20;
            parent => assist0.assist1.frames[3].br;
            x = UIiso_level.x+160 ;
         };
         UIlabel {
            y = UIiso_level.y;
            parent => assist0.assist1.frames[3].br;
            x = UIradioBox.x;
         };
          visible=1;
          width=125;
      };
      UIoptionBoxLabel {
         UIoptionBox {
            y =  UIiso_level.y+20;
            parent => assist0.assist1.frames[3].br;
            x =  UIiso_level.x+300;
         };
         UIlabel {
            y = UIiso_level.y;
            parent => assist0.assist1.frames[3].br;
            x = UIoptionBox.x;
         };
         visible=1;
      };
   };

};
MODS.Filters.divergence divergence<NEx=11.,NEy=110.> {
   in_field => <-.Read_UCD.field;
   DivUI {
      DVnode_data_labels {
         labels[];
      };
   };
};
MODS.Filters.extract_scalar extract_scalar<NEx=11.,NEy=33.> {
   in_field => <-.Read_UCD.field;
   ExtractScalarUI {
      DVnode_data_labels {
         labels[];
      };
      UIpanel {
         width = 150;
      };
      ComponentSlider {
         parent => assist0.assist1.frames[3].br;
         y = isosurface.IsoUI.UIiso_level.y+60;
         x = isosurface.IsoUI.UIiso_level.x;
      };
   };

};
MODS.Filters.combine_comp combine_comp<NEx=11.,NEy=176.> {
   in_field1 => <-.divergence.out_fld;
   in_field2 => <-.extract_scalar.out_fld;
   CombCompUI {
      DVnode_data_labels1 {
         labels[];
      };
      DVnode_data_labels2 {
         labels[];
      };
   };
};
$link Viewer3D.objs_in isosurface.out_obj

};
!assist0.assist1.frames[3]{old=1;};
