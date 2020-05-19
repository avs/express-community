Applications.ModuleStack{

MODS.Mappers.slice_plane slice_plane<NEx=176.,NEy=110.> {
   in_field => <-.Read_UCD.field;
   UIpanel.visible=>1;
   SliceUI<NEx=330.,NEy=44.,NEdisplayMode="open"> {
      DVnode_data_labels {
         labels[];
      };
      UIpanel<NEx=11.,NEy=143.> {
         width = 150;
      };
      plane_dist<NEx=11.,NEy=231.> {
         min<NEdisplayMode="open">;
         parent => assist0.assist1.frames[1].br;
         y = 10;
         x = 10;
      };
      xform_toggle {
         parent => assist0.assist1.frames[1].br;
         y = plane_dist.y+60;
         x = plane_dist.x-3;
      };
      plane_xform {
         XformEditorUI {
            trans_panel<NEdisplayMode="open",NEx=11.,NEy=55.> {
               width<NEdisplayMode="open"> = 450;
            };
         };
      };

   };

};
MODS.Mappers.glyph glyph<NEx=209.,NEy=231.> {
   in_field => <-.slice_plane.out_fld;
   in_glyph => <-.Arrow1.out_fld;
   UIpanel.visible=>1;
   GlyphUI {
      DVnode_data_labels {
         labels[];
      };
      UIpanel {
         width = 248;
      };
      norm_toggle {
         height = 36;
         y = 271;
         x = 465;
      };
      scale_slider {
         parent => assist0.assist1.frames[1].br;
         y => slice_plane.SliceUI.plane_dist.y;
         x =slice_plane.SliceUI.plane_dist.x+160;
      };
   };

};
GEOMS.Arrow1 Arrow1<NEx=242.,NEy=165.>;

$link Viewer3D.objs_in glyph.out_obj
};
!assist0.assist1.frames[1]{old=1;};
