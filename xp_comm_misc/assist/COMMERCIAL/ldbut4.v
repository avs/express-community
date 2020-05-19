Applications.ModuleStack{

macro PickVal<NEx=484.,NEy=209.> {
   GD.GDpick_process GDpick_process<NEx=55.,NEy=121.> {
      obj_in<NEportLevels={3,0}> => <-.<-.city_plot.out_obj;
      use_pick_data<NEdisplayMode="open"> = 1;
   };
   GMOD.parse_v parse_v<NEx=121.,NEy=66.> {
      v_commands<NEdisplayMode="open"> = "src_field<NEportLevels={3,0}> => <-.<-.in_field;";
      relative<NEdisplayMode="open"> => Applications.ModuleStack.city_plot.obj.PickInfo;
   };
   macro annotation<NEx=154.,NEy=319.> {
      float coor<NEportLevels={2,1},NEx=77.,NEy=55.>[] => <-.GDpick_process.point;
      float xcoor<NEportLevels={1,1},NEx=77.,NEy=121.> => (coor[0][0] + 0.5);
      float ycoor<NEportLevels={1,1},NEx=77.,NEy=165.> => (coor[0][1] + 0.5);
      float cooran<NEportLevels=1,NEx=286.,NEy=143.>[1][3] => {xcoor,ycoor,.zcoor};
      string string<NEportLevels={2,1},NEx=275.,NEy=22.> => <-.GDpick_process.value;
      MODS.Mappers.text_glyph text_glyph<NEx=308.,NEy=220.> {
         in_mesh<NEx=209.,NEy=22.> => <-.point_mesh.out;
         in_text => {<-.string};
         out_obj<NEportLevels={1,4}>;
      };
      float zcoor<NEportLevels={2,1},NEx=88.,NEy=231.> => (<-.GDpick_process.value[0] * city_plot.CityUI.scale_slider.value);
      FLD_MAP.Mesh_Mappers.point_mesh point_mesh<NEx=517.,NEy=165.> {
         coord => <-.cooran;
      };
   };
   macro print_value<NEx=209.,NEy=165.> {
      Controls.UIlabel UIlabel<NEx=0.,NEy=33.> {
         parent => assist0.assist1.frames[3].br;
         label<NEdisplayMode="open"> = "Pick Value =";
         y = 16;
         x = 16;
      };
      Controls.UItext UItext<NEx=0.,NEy=11.> {
         parent => assist0.assist1.frames[3].br;
         text<NEportLevels={3,2},NEdisplayMode="open"> => <-.<-.GDpick_process.value;
         x = 108;
         y = 12;
      };
   };
};
$link Viewer3D.objs_in PickVal.annotation.text_glyph.out_obj

};
!assist0.assist1.frames[3]{old=1;};

