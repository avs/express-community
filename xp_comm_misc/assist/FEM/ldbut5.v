Applications.ModuleStack
{
macro numbering<NEx=99.,NEy=242.> 
   {
   int nnodes<NEx=44.,NEy=11.> => Read_UCD.DVread_ucd.out.nnodes;
   int iarr<NEx=209.,NEy=143.>[] => init_array(sum,(from + 1),(to + 1));
   string string<NEx=209.,NEy=187.>[] => .iarr;

   MODS.Mappers.text_glyph text_glyph<NEx=165.,NEy=253.> {
      in_mesh => <-.point_mesh.out;
      in_text => <-.string;
      TextUI.UIslider in 
         {
         parent => assist0.assist1.frames[4].br;
         title<NEdisplayMode="open"> = "Font Size ";
         y = 10;
         x=  250; 
         width=150;     
         mode = 1;
         decimalPoints<NEdisplayMode="open"> = 0;
         min<NEdisplayMode="open"> => 6;
         max<NEdisplayMode="open"> => 20;
         value=6;
         };
      out_obj<NEportLevels={1,3}>;
      };
   int from<NEx=44.,NEy=55.> => (.UI_first.value - 1);
   int to<NEx=44.,NEy=99.> => (.UI_end.value - 1);
   int sum<NEx=44.,NEy=143.> => ((to - from) + 1);
   FLD_MAP.Mesh_Mappers.point_mesh point_mesh<NEx=44.,NEy=187.> 
      {
      coord => Read_UCD.DVread_ucd.out.coordinates.values[from:to][0:2];
      };

   UIslider UI_first<NEx=209.,NEy=55.> 
      {
      parent => assist0.assist1.frames[4].br;
      title = "First Node";
      y = 10;
      x = 10;
      mode = 1;
      decimalPoints = 0;
      min = 1.;
      max = 4118.;
      value=50;
      };
   UIslider UI_end<NEx=209.,NEy=99.> {
      parent => assist0.assist1.frames[4].br;
      title = " End Node";
      y = 70;
      x = 10;      
      mode = 1;
      decimalPoints = 0;
      min => UI_first.value;
      max => <-.nnodes;
      value=100;
   };
};


$link Viewer3D.objs_in numbering.text_glyph.out_obj
#define BUT5
};

!assist0.assist1.frames[4]{old=1;};
