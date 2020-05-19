
library rp_examples <compile_subs=0> {


APPS.SingleWindowApp Cavity {
   UI {
      shell {
         x = -4;
         y = -4;
      };
      Windows {
         IUI {
            panel {
               width = 692;
               height = 602;
            };
         };
      };
      Modules {
         IUI {
            optionList {
               cmdList => {
                  <-.<-.<-.<-.Read_Phoenics.ReadPhoenicsUI.mod_panel.option,
                  <-.<-.<-.<-.glyph.UIpanel.option,
                  <-.<-.<-.<-.slice_plane.UIpanel.option,
                  <-.<-.<-.<-.external_edges.ExtEdgeUI.UIpanel.option};
               selectedItem = 0;
            };
         };
      };
      Editors {
         IUI {
            optionList {
               selectedItem = 0;
            };
         };
      };
   };
   GDM.Uviewer3D Uviewer3D<NEx=506.,NEy=352.> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.TextTitle.DefaultObject,<-.<-.<-.glyph.out_obj,
               <-.<-.<-.external_edges.out_obj,
               <-.<-.<-.isoline.out_obj};
            Xform {
               mat = {
                  67.4744,0.,0.,0.,0.,67.4744,0.,0.,0.,0.,67.4744,0.,0.,
0.,0.,1.
               };
               xlate = {-4.5814,-4.12987,
-40.};
            };
         };
      };
      Scene_Editor {
         View_Editor {
            InfoPanel {
               curobj_sel {
                  width = 238;
                  height = 245;
                  x = 0;
                  y = 0;
               };
            };
         };
         Object_Editor {
            GDmodes_edit {
               points = "Inherit";
               lines = "Inherit";
               surf = "Inherit";
            };
            IUI {
               optionList {
                  selectedItem = 0;
               };
               General {
                  IUI {
                     panel {
                        visible = 1;
                     };
                  };
               };
               Modes {
                  IUI {
                     Point {
                        OPcmdList = {
                           
                           {
                              set=1
                           },
                           ,,,,,
                        };
                     };
                     Line {
                        OPcmdList = {
                           
                           {
                              set=1
                           },
                           ,,,,
                        };
                     };
                     Surface {
                        OPcmdList = {
                           
                           {
                              set=1
                           },
                           ,,,,
                        };
                     };
                  };
               };
            };
         };
      };
   };
   WORKSPACE_1.Read_Phoenics Read_Phoenics<NEx=11.,NEy=11.> {
      ReadPhoenicsUI {
         mod_panel {
            option {
               set = 1;
            };
         };
         phi_dialog {
            filename = "data/phi";
            ok = 1;
         };
         pat_dialog {
            filename = "data/patgeo";
            ok = 1;
         };
      };
   };
   macro combine_vect<NEx=341.,NEy=44.> {
      ilink in_field<export_all=1> => <-.slice_plane.out_fld;
      DV_Param_combine_vect CombineVectParam<NEx=407.,NEy=22.,NEportLevels=1,export_all=2> {
         veclen<NEdisplayMode="open"> = 2;
         components<NEdisplayMode="open"> = {1,2};
         sub_components[4] => {0,0,0,0};
      };
      CombineVect CombineVect<NEy=143.,NEx=253.> {
         in_fld => <-.in_field;
         param => <-.CombineVectParam;
      };
      DataObjectNoTexture obj<NEy=231.,NEx=99.> {
         in => <-.CombineVect.out_fld;
      };
      olink out_fld<export_all=2> => .CombineVect.out_fld;
      olink out_obj<NEy=308,NEx=385> => .obj.obj;
   };
   macro TextTitle<NEx=55.,NEy=209.> {
      Xform text<NEx=44.,NEy=165.,export_all=2> {
         xform<NEportLevels={0,2}>;
         int align_horiz<NEportLevels={2,0},NEdisplayMode="open"> = 1;
         int align_vert<NEportLevels={2,0},NEdisplayMode="open"> = 1;
         int drop_shadow<NEportLevels={2,0}>;
         int background<NEportLevels={2,0}>;
         int bounds<NEportLevels={2,0}>;
         int underline<NEportLevels={2,0}>;
         int lead_line<NEportLevels={2,0}>;
         int radial<NEportLevels={2,0}>;
         int do_offset<NEportLevels={2,0}>;
         float offset<NEportLevels={2,0}>[3];
         int xform_mode;
         string str<NEportLevels={3,0}> => <-.<-.Read_Phoenics.read_phoenics.title;
         int nspace = 3;
         float position<NEdisplayMode="open">[.nspace] = {0.,0.9,0.99};
         int stroke = 0;
         group StrokeTextAttribs {
            int font_type;
            int style;
            int plane;
            int orient;
            int path;
            int space_mode;
            float spacing;
            float angle;
            float height;
            float expansion;
            float width;
         };
         float+write min_vec[.nspace];
         float+write max_vec[.nspace];
      };
      DefaultObject DefaultObject<NEx=330,NEy=341,NEportLevels={1,2}> {
         input => <-.text;
         xform => <-.text.xform;
         props => <-.DefaultProps;
         xform_mode = "Locked";
         name => name_of(<-.<-);
         pick_info => <-.DefaultPickInfo;
      };
      DefaultProps DefaultProps<NEx=451,NEy=220> {
         font<NEportLevels={2,0}> => ;
         inherit = 0;
      };
      GDpick_info DefaultPickInfo<NEx=495,NEy=176>;
      GMOD.instancer instancer {
         active = 2;
      };
   };
   MODS.glyph glyph<NEx=473.,NEy=121.> {
      in_field => <-.combine_vect.out_fld;
      in_glyph => <-.Arrow2.out_fld;
      GlyphParam<NEx=308.,NEy=187.> {
         scale<NEdisplayMode="open"> = 0.05;
      };
   };
   MODS.isoline isoline {
       in_field => <-.slice_plane.out_fld;
       IsoParam {
       	  level_min = 0.0;
       	  level_max = 1.0;
       	  ncontours = 20;
       };
   };
   GEOMS.Arrow2 Arrow2<NEx=528.,NEy=11.>;
   MODS.slice_plane slice_plane<NEx=121.,NEy=66.> {
      in_field => <-.Read_Phoenics.out_fld;
      SliceParam {
         component = {0,1,2,3};
      };
      DVcell_data_labels {
         labels[];
      };
      DVnode_data_labels {
         labels[];
      };
      SliceUI {
         UIoptionBoxLabel {
            label_cmd {
               cmd[4] = {
                  {
                     set=1,,,,,,,,,,,,,,,,,,,,
                  },
                  {
                     set=1,,,,,,,,,,,,,,,,,,,,
                  },
                  {
                     set=1,,,,,,,,,,,,,,,,,,,,
                  },
                  {
                     set=1,,,,,,,,,,,,,,,,,,,,
                  }};
            };
         };
      };
      slice_obj {
         Modes {
            mode = {0,0,1,0,0};
         };
      };
   };
   MODS.external_edges external_edges<NEx=275.,NEy=143.> {
      in_field => <-.slice_plane.out_fld;
   };
};



APPS.SingleWindowApp Pipe<NEdisplayMode="maximized"> {
   UI {
      shell {
         x = -4;
         y = -4;
      };
      Windows {
         IUI {
            panel {
               width = 692;
               height = 602;
            };
         };
      };
      Editors {
         IUI {
            optionList {
               selectedItem = 0;
            };
         };
      };
      Modules {
         IUI {
            optionList {
               cmdList => {
                  <-.<-.<-.<-.Read_Phoenics.ReadPhoenicsUI.mod_panel.option,
                  <-.<-.<-.<-.slice_plane.UIpanel.option,
                  <-.<-.<-.<-.external_edges.ExtEdgeUI.UIpanel.option,
                  <-.<-.<-.<-.streamlines.UIpanel.option};
               selectedItem = 0;
            };
         };
      };
   };
   GDM.Uviewer3D Uviewer3D<NEx=506.,NEy=352.> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.TextTitle.DefaultObject,<-.<-.<-.external_edges.out_obj,
               <-.<-.<-.slice_plane.out_obj_slice,<-.<-.<-.streamlines.out_obj};
            Xform {
               mat = {
                  1.90197,9.87991,7.11053,0.,8.32668,-6.30557,6.53415,0.,
8.87908,3.79694,-7.6508,0.,0.,0.,0.,1.
               };
               xlate = {2.47548,0.497747,
-0.9594};
            };
         };
         Camera {
            Camera {
               perspec = 1;
               front = 1.;
            };
         };
      };
      Scene_Editor {
         View_Editor {
            GDobj_sel {
               curobj_name = "Top";
            };
            InfoPanel {
               curobj_sel {
                  x = 0;
                  y = 0;
                  width = 238;
                  height = 245;
                  ok = 1;
                  selectedItem = 4;
               };
            };
         };
         Camera_Editor {
            GDcamera_edit {
               front = 1.;
            };
         };
         Object_Editor {
            GDmodes_edit {
               points = "Inherit";
               lines = "Inherit";
               surf = "Inherit";
            };
            IUI {
               optionList {
                  selectedItem = 2;
               };
               Modes {
                  IUI {
                     panel {
                        visible = 1;
                     };
                  };
               };
            };
         };
      };
   };
   WORKSPACE_1.Read_Phoenics Read_Phoenics<NEx=11.,NEy=11.> {
      ReadPhoenicsUI {
         mod_panel {
            option {
               set = 1;
            };
         };
         phi_dialog {
            ok = 1;
            filename = "data/pipe.phi";
         };
         pat_dialog {
            ok = 1;
            filename = "";
         };
      };
   };
   macro combine_vect<NEx=341.,NEy=44.> {
      ilink in_field<export_all=1> => <-.Read_Phoenics.out_fld;
      DV_Param_combine_vect CombineVectParam<NEx=407.,NEy=22.,NEportLevels=1,export_all=2> {
         veclen<NEdisplayMode="open"> = 3;
         components<NEdisplayMode="open"> = {1,2,3};
         sub_components[4] => {0,0,0,0};
      };
      CombineVect CombineVect<NEy=143.,NEx=253.> {
         in_fld => <-.in_field;
         param => <-.CombineVectParam;
      };
      DataObjectNoTexture obj<NEy=231.,NEx=99.> {
         in => <-.CombineVect.out_fld;
      };
      olink out_fld<export_all=2> => .CombineVect.out_fld;
      olink out_obj<NEy=308,NEx=385> => .obj.obj;
   };
   macro TextTitle<NEx=55.,NEy=209.> {
      Xform text<NEx=44.,NEy=165.,export_all=2> {
         xform<NEportLevels={0,2}>;
         int align_horiz<NEportLevels={2,0},NEdisplayMode="open"> = 1;
         int align_vert<NEportLevels={2,0},NEdisplayMode="open"> = 1;
         int drop_shadow<NEportLevels={2,0}>;
         int background<NEportLevels={2,0}>;
         int bounds<NEportLevels={2,0}>;
         int underline<NEportLevels={2,0}>;
         int lead_line<NEportLevels={2,0}>;
         int radial<NEportLevels={2,0}>;
         int do_offset<NEportLevels={2,0}>;
         float offset<NEportLevels={2,0}>[3];
         int xform_mode;
         string str<NEportLevels={3,0}> => <-.<-.Read_Phoenics.read_phoenics.title;
         int nspace = 3;
         float position<NEdisplayMode="open">[.nspace] = {0.,0.9,0.99};
         int stroke = 0;
         group StrokeTextAttribs {
            int font_type;
            int style;
            int plane;
            int orient;
            int path;
            int space_mode;
            float spacing;
            float angle;
            float height;
            float expansion;
            float width;
         };
         float+write min_vec[.nspace];
         float+write max_vec[.nspace];
      };
      DefaultObject DefaultObject<NEx=330,NEy=341,NEportLevels={1,2}> {
         input => <-.text;
         xform => <-.text.xform;
         props => <-.DefaultProps;
         xform_mode = "Locked";
         name => name_of(<-.<-);
         pick_info => <-.DefaultPickInfo;
      };
      DefaultProps DefaultProps<NEx=451,NEy=220> {
         font<NEportLevels={2,0}> => ;
         inherit = 0;
      };
      GDpick_info DefaultPickInfo<NEx=495,NEy=176>;
      GMOD.instancer instancer {
         active = 2;
      };
   };
   MODS.slice_plane slice_plane<NEx=132.,NEy=88.> {
      in_field => <-.Read_Phoenics.out_fld;
      SliceParam {
         dist = 0.48;
         cell_data = {8};
      };
      DVnode_data_labels {
         labels[];
      };
      DVcell_data_labels {
         labels[];
      };
      SliceUI {
         UIoptionBoxLabel {
            label_cmd {
               cmd[8];
            };
         };
         UIoptionBoxLabel_cell {
            label_cmd {
               cmd[9] = {
                  ,,
                  {
                     set=0
                  },,,,,,
                  {
                     set=1
                  }
               };
            };
         };
      };
   };
   MODS.external_edges external_edges<NEx=242.,NEy=165.> {
      in_field => <-.Read_Phoenics.out_fld;
      edge_angle = 0.;
   };
   MODS.streamlines streamlines<NEx=198.,NEy=308.> {
      in_field => <-.combine_vect.out_fld;
      in_probe => <-.slice_plane.out_fld;
      StreamParam {
         forw_back = 0;
         ribbons = 1;
         rib_width<NEdisplayMode="open"> = 0.01;
      };
      StreamUI {
         DVnode_data_labels {
            labels[];
         };
         UIradioBoxLabel {
            label_cmd {
               cmd[];
            };
         };
         UIradioBoxLabel_dir {
            label_cmd {
               cmd[] = {
                  {
                     set=1,
                  },};
            };
         };
      };
   };
};


APPS.SingleWindowApp Rocket<NEdisplayMode="maximized"> {
   UI {
      shell {
         x = -4;
         y = -4;
      };
      Windows {
         IUI {
            panel {
               width = 692;
               height = 602;
            };
         };
      };
      Editors {
         IUI {
            optionList {
               selectedItem = 0;
            };
         };
      };
      Modules {
         IUI {
            optionList {
               cmdList => {
                  <-.<-.<-.<-.Read_Phoenics.ReadPhoenicsUI.mod_panel.option,
                  <-.<-.<-.<-.external_edges.ExtEdgeUI.UIpanel.option,
                  <-.<-.<-.<-.isosurface.UIpanel.option,
                  <-.<-.<-.<-.glyph.UIpanel.option,
                  <-.<-.<-.<-.extract_data_array.UIpanel.option,
                  <-.<-.<-.<-.extract_data_array#1.UIpanel.option
               };
               selectedItem = 0;
            };
         };
      };
   };
   GDM.Uviewer3D Uviewer3D<NEx=638.,NEy=407.> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.TextTitle.DefaultObject,<-.<-.<-.external_edges.out_obj,
               <-.<-.<-.isosurface.out_obj,<-.<-.<-.glyph.out_obj};
            Xform {
               mat = {
                  2.76363,0.357728,1.86125,0.,-0.536215,3.304,0.161167,0.,
-1.81788,-0.430735,2.78202,0.,0.,0.,0.,1.
               };
               xlate = {26.3423,2.47915,
6.98253};
            };
         };
         View {
            View {
               trigger = 1;
            };
         };
      };
      Scene_Editor {
         View_Editor {
            GDobj_sel {
               curobj_name = "Top";
            };
            InfoPanel {
               curobj_sel {
                  x = 0;
                  y = 0;
                  width = 238;
                  height = 245;
                  ok = 1;
                  selectedItem = 4;
               };
            };
         };
         Light_Editor {
            GDlinfo_edit {
               show = 0;
            };
            GDlight_edit {
               type = "Directional";
            };
            IUI {
               optionList {
                  selectedItem = 0;
               };
               General {
                  IUI {
                     panel {
                        visible = 1;
                     };
                     AllFrame {
                        y = 0;
                     };
                     CurrentLight {
                        y => 80;
                     };
                     VUIColorEditor {
                        Imm {
                           set = 0;
                        };
                        rgb_or_hsv {
                           s = 0.;
                        };
                     };
                  };
               };
            };
         };
         Camera_Editor {
            GDcamera_edit {
               front = -88.;
            };
            IUI {
               optionList {
                  selectedItem = 0;
               };
            };
         };
         Object_Editor {
            GDmodes_edit {
               points = "Inherit";
               lines = "Inherit";
               surf = "Inherit";
            };
            IUI {
               optionList {
                  selectedItem = 2;
               };
               Modes {
                  IUI {
                     panel {
                        visible = 1;
                     };
                  };
               };
            };
         };
      };
   };
   WORKSPACE_1.Read_Phoenics Read_Phoenics<NEx=11.,NEy=11.> {
      ReadPhoenicsUI {
         mod_panel {
            option {
               set = 1;
            };
         };
         phi_dialog {
            x = 33;
            y = 363;
            width = 296;
            height = 359;
            ok = 1;
            filename = "data/pho.phi";
            dirMaskCache = "/usr/people/david/phoenics/readp/data/*";
         };
         xyz_dialog {
            x = 33;
            y = 363;
            width = 296;
            height = 359;
            ok = 1;
            filename = "data/pho.xyz";
            dirMaskCache = "/usr/people/david/phoenics/readp/data/*";
         };
         pat_dialog {
            ok = 1;
            filename = "";
         };
         Read_Files {
            do<NEportLevels={0,4}>;
         };
      };
   };
   macro TextTitle<NEx=66.,NEy=297.> {
      Xform text<NEx=44.,NEy=165.,export_all=2> {
         xform<NEportLevels={0,2}>;
         int align_horiz<NEportLevels={2,0},NEdisplayMode="open"> = 1;
         int align_vert<NEportLevels={2,0},NEdisplayMode="open"> = 1;
         int drop_shadow<NEportLevels={2,0}>;
         int background<NEportLevels={2,0}>;
         int bounds<NEportLevels={2,0}>;
         int underline<NEportLevels={2,0}>;
         int lead_line<NEportLevels={2,0}>;
         int radial<NEportLevels={2,0}>;
         int do_offset<NEportLevels={2,0}>;
         float offset<NEportLevels={2,0}>[3];
         int xform_mode;
         string str<NEportLevels={3,0}> => <-.<-.Read_Phoenics.read_phoenics.title;
         int nspace = 3;
         float position<NEdisplayMode="open">[.nspace] = {0.,0.9,0.99};
         int stroke = 0;
         group StrokeTextAttribs {
            int font_type;
            int style;
            int plane;
            int orient;
            int path;
            int space_mode;
            float spacing;
            float angle;
            float height;
            float expansion;
            float width;
         };
         float+write min_vec[.nspace];
         float+write max_vec[.nspace];
      };
      DefaultObject DefaultObject<NEx=330,NEy=341,NEportLevels={1,2}> {
         input => <-.text;
         xform => <-.text.xform;
         props => <-.DefaultProps;
         xform_mode = "Locked";
         name => name_of(<-.<-);
         pick_info => <-.DefaultPickInfo;
      };
      DefaultProps DefaultProps<NEx=451,NEy=220> {
         font<NEportLevels={2,0}> => ;
         inherit = 0;
      };
      GDpick_info DefaultPickInfo<NEx=495,NEy=176>;
      GMOD.instancer instancer {
         active = 2;
      };
   };
   MODS.external_edges external_edges<NEx=528.,NEy=209.> {
      in_field => <-.Read_Phoenics.out_fld;
   };
   MODS.isosurface isosurface<NEx=253.,NEy=297.> {
      in_field => <-.Read_Phoenics.out_fld;
      IsoParam {
         iso_component = 1;
         iso_level => 76.87;
      };
      DVnode_data_labels {
         labels[];
      };
      DVcell_data_labels {
         labels[];
      };
      IsoUI {
         UIradioBoxLabel {
            label_cmd {
               cmd[];
            };
         };
         UIoptionBoxLabel {
            label_cmd {
               cmd[6];
            };
         };
         UIoptionBoxLabel_cell {
            label_cmd {
               cmd[];
            };
         };
      };
   };
   GEOMS.Arrow1 Arrow1<NEx=649.,NEy=33.>;
   MODS.glyph glyph<NEx=627.,NEy=121.> {
      in_field => <-.combine_mesh_data.out;
      in_glyph => <-.Arrow1.out_fld;
      GlyphParam {
         scale<NEdisplayMode="open"> = 0.0001;
      };
      GlyphUI {
         DVnode_data_labels {
            labels[];
         };
         UIradioBoxLabel_glyph {
            label_cmd {
               cmd[];
            };
         };
         UIradioBoxLabel_map {
            label_cmd {
               cmd[];
            };
         };
         UIradioBoxLabel_scale {
            label_cmd {
               cmd[];
            };
         };
         UIradioBoxLabel_mode {
            label_cmd {
               cmd[];
            };
         };
      };
   };
   FLD_MAP.extract_data_array extract_data_array<NEx=165.,NEy=44.> {
      in => <-.Read_Phoenics.out_fld;
      comp = 5;
      DVnode_data_labels {
         labels[];
      };
      UIradioBoxLabel_mode2 {
         label_cmd {
            cmd[] = {
               ,,,,,
               {
                  set=1,
               }
            };
         };
      };
   };
   FLD_MAP.extract_data_array extract_data_array#1<NEx=352.,NEy=44.> {
      in => <-.Read_Phoenics.out_fld;
      comp = 4;
      DVnode_data_labels {
         labels[];
      };
      UIradioBoxLabel_mode2 {
         label_cmd {
            cmd[] = {
               ,,,,
               {
                  set=1,
               },
            };
         };
      };
   };
   float float<NEportLevels=1,NEx=88.,NEy=121.>[] => init_array(.Read_Phoenics.out_fld.nnodes,0.,0.);
   FLD_MAP.node_vector node_vector<NEx=308.,NEy=187.> {
      in_data => <-.interleave_3_arrays.out;
   };
   FLD_MAP.interleave_3_arrays interleave_3_arrays<NEx=275.,NEy=143.> {
      in1 => <-.float;
      in2 => <-.extract_data_array.data;
      in3 => <-.extract_data_array#1.data;
   };
   FLD_MAP.combine_mesh_data combine_mesh_data<NEx=275.,NEy=242.> {
      in_mesh => <-.Read_Phoenics.out_fld;
      in_nd => <-.node_vector.out;
   };
};


APPS.SingleWindowApp Plume<NEdisplayMode="maximized"> {
   UI {
      shell {
         x = 35;
         y = 25;
      };
      Modules {
         IUI {
            optionList {
               selectedItem = 0;
            };
         };
      };
   };
   GDM.Uviewer3D Uviewer3D<NEx=600,NEy=450> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.advector.out_obj,<-.<-.<-.external_edges.out_obj,
               <-.<-.<-.TextTitle.DefaultObject};
         };
      };
   };
   WORKSPACE_1.Read_Phoenics Read_Phoenics<NEx=99.,NEy=44.> {
      ReadPhoenicsUI {
         mod_panel {
            option {
               set = 1;
            };
         };
         Read_Files {
            do<NEportLevels={0,4}>;
         };
      };
   };
   GEOMS.Arrow2 Arrow2<NEx=561.,NEy=33.>;
   MODS.advector advector<NEx=429.,NEy=297.> {
      in_field => <-.slice_plane.out_fld;
      in_probe => <-.slice_plane.out_fld;
      in_glyph => <-.Arrow2.out_fld;
      AdvectParam<NEdisplayMode="open",NEx=429.,NEy=22.> {
         run<NEportLevels={3,0}> => <-.<-.copy_on_change.output;
         end = 50.27;
         incr = 0.58;
         count = 0.;
      };
      AdvectUI {
         UIradioBoxLabel_dir {
            label_cmd {
               cmd[];
            };
         };
         UIradioBoxLabel_mode {
            label_cmd {
               cmd[];
            };
         };
      };
   };
   MODS.slice_plane slice_plane<NEx=385.,NEy=143.> {
      in_field => <-.combine_vect.out_fld;
      SliceParam {
         cell_data = {3};
      };
      DVcell_data_labels {
         labels[];
      };
      DVnode_data_labels {
         labels[];
      };
      SliceUI {
         UIoptionBoxLabel {
            label_cmd {
               cmd[1];
            };
         };
         UIoptionBoxLabel_cell {
            label_cmd {
               cmd = {
                  ,,,
                  {
                     set=1,,,
                  },,
               };
            };
         };
      };
   };
   MODS.combine_vect combine_vect<NEx=308.,NEy=77.> {
      in_field => <-.Read_Phoenics.out_fld;
      CombineVectParam {
         veclen<NEdisplayMode="open"> = 2;
         components<NEdisplayMode="open"> = {1,2};
      };
      CombVectUI {
         DVnode_data_labels {
            labels[];
         };
         UIoptionBoxLabel {
            label_cmd {
               cmd[] = {
                  {
                     active=1
                  },
                  {
                     active=1
                  },
                  {
                     set=1,,,active=0
                  },
                  {
                     active=0
                  },
                  {
                     active=0
                  }
               };
            };
         };
      };
   };
   MODS.external_edges external_edges<NEx=132.,NEy=165.> {
      in_field => <-.Read_Phoenics.out_fld;
   };
   GEOMS.TextTitle TextTitle<NEx=55.,NEy=341.> {
      text {
         str<NEportLevels={3,0}> => <-.<-.Read_Phoenics.read_phoenics.title;
      };
      TextUI {
         OffsetXValue {
            x = 0;
         };
         OffsetYValue {
            x = 0;
         };
         OffsetZValue {
            x = 0;
         };
         Font {
            text = "-adobe-times-bold-r-*-*-20-*-*-*-*-*-*-*";
         };
         UIoption {
            set = 0;
         };
         UIoption#1 {
            set = 1;
         };
      };
   };
   GMOD.copy_on_change copy_on_change<NEx=264.,NEy=308.> {
      trigger => <-.Read_Phoenics.ReadPhoenicsUI.Read_Files.do;
      input<NEdisplayMode="open"> = 1;
      output = 0;
   };
};


APPS.SingleWindowApp Multi_Block<NEdisplayMode="maximized"> {
   UI {
      shell {
         x = 24;
         y = 63;
      };
      Modules {
         IUI {
            optionList {
               selectedItem = 0;
            };
         };
      };
   };
   GDM.Uviewer3D Uviewer3D<NEx=462.,NEy=330.> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.external_edges.out_obj,<-.<-.<-.isosurface.out_obj,
               <-.<-.<-.LegendVert.GroupObject.obj,
               <-.<-.<-.TextTitle.DefaultObject};
            Xform {
               mat = {
                  3.59756,-0.116809,1.39474,0.,-0.330468,3.66711,1.15952,0.,
-1.36005,-1.20002,3.40758,0.,0.,0.,0.,1.
               };
               xlate = {-0.933278,-2.66667,
-5.33333};
            };
         };
      };
      Scene_Editor {
         View_Editor {
            InfoPanel {
               curobj_sel {
                  width = 238;
                  height = 245;
                  x = 0;
                  y = 0;
               };
            };
         };
      };
   };
   WORKSPACE_1.Read_Phoenics Read_Phoenics<NEx=99.,NEy=33.> {
      ReadPhoenicsUI {
         mod_panel {
            option {
               set = 1;
            };
         };
         phi_dialog {
            filename = "data\\phida.212";
            ok = 1;
         };
         xyz_dialog {
            filename = "data\\xyzda.212";
            ok = 1;
         };
         pat_dialog {
            filename = "data\\Patgeo.212";
            ok = 1;
         };
         phi_toggle {
            set = 1;
         };
         xyz_toggle {
            set = 1;
         };
      };
   };
   MODS.external_edges external_edges<NEx=132.,NEy=154.> {
      in_field => <-.Read_Phoenics.out_fld;
      edge_angle = 0.;
   };
   MODS.isosurface isosurface<NEx=330.,NEy=132.> {
      in_field => <-.Read_Phoenics.out_fld;
      IsoParam {
         iso_component = 1;
         iso_level => -0.02;
         map_component = {1,2};
      };
      DVcell_data_labels {
         labels[];
      };
      DVnode_data_labels {
         labels[];
      };
      IsoUI {
         UIradioBoxLabel {
            label_cmd {
               cmd[] = {
                  ,
                  {
                     set=1,
                  },,,,,,,,,,
               };
            };
         };
         UIoptionBoxLabel {
            label_cmd {
               cmd[] = {
                  ,
                  {
                     set=1
                  },
                  {
                     set=1
                  },,,,,,,,,
               };
            };
         };
         UIoptionBoxLabel_cell {
            label_cmd {
               cmd[];
            };
         };
      };
   };
   GEOMS.LegendVert LegendVert<NEx=88.,NEy=264.> {
      obj_in => <-.isosurface.out_obj;
      y_max = 0.;
   };
   GEOMS.TextTitle TextTitle<NEx=506.,NEy=209.> {
      text {
         str<NEportLevels={3,0}> => <-.<-.Read_Phoenics.read_phoenics.title;
      };
      TextUI {
         Font {
            text = "-adobe-times-bold-r-*-*-20-*-*-*-*-*-*-*";
         };
      };
   };
};


