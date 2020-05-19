APPS.SingleWindowApp KrigEg0<NEdisplayMode="maximized"> {
   HLM.Bubbleviz Bubbleviz<NEx=143.,NEy=176.> {
      glyph {
         in_field => <-.<-.data.combine_mesh_data.out;
         GlyphUI {
            DVnode_data_labels {
               labels[];
            };
            UIradioBoxLabel_map {
               label_cmd {
                  cmd[];
               };
            };
         };
      };
      UIpanel {
         height = 780;
      };
      Sphere {
         subdiv = 21;
      };
   };
   macro data {
      FLD_MAP.point_mesh point_mesh<NEx=165.,NEy=198.> {
         coord => <-.interleave_2_arrays.out;
      };
      FLD_MAP.combine_mesh_data combine_mesh_data<NEx=286.,NEy=253.> {
         in_mesh => <-.point_mesh.out;
         in_nd => <-.node_scalar.out;
         out<NEportLevels={0,3}>;
      };
      FLD_MAP.node_scalar node_scalar<NEx=462.,NEy=187.> {
         in_data => {<-.float#3};
      };
      float float<NEportLevels={1,1}>[10] = {1.,3.,7.,5.,
2.,9.,4.,2.,6.,5.};
      float float#1<NEportLevels={1,1}>[10] = {2.,4.,5.,6.,
8.,2.,4.,5.,8.,10.};
      float float#3<NEportLevels={1,1},NEx=495.,NEy=66.>[10] = {3.,4.,5.,7.,
4.,8.,5.,2.,2.,10.};
      FLD_MAP.interleave_2_arrays interleave_2_arrays<NEx=231.,NEy=132.> {
         in1 => <-.float;
         in2 => <-.float#1;
      };
   };
   int dims<NEportLevels={1,1},NEx=605.,NEy=121.>[2] = {30,30};
   macro map_field<NEx=616.,NEy=209.> {
      FLD_MAP.uniform_mesh uniform_mesh<NEx=121.,NEy=99.> {
         in_dims<NEportLevels={3,0},NEdisplayMode="open">;
         out<NEdisplayMode="open"> {
            points => ;
         };
      };
      FLD_MAP.node_scalar node_scalar<NEx=462.,NEy=55.> {
         in_data<NEportLevels={3,0}>;
      };
      FLD_MAP.combine_mesh_data combine_mesh_data {
         in_mesh => <-.uniform_mesh.out;
         in_nd => <-.node_scalar.out;
      };
      macro set_null {
         ilink in_field<export_all=1,NEportLevels={2,1}> => <-.combine_mesh_data.out;
         DV_Param_set_null SetNullParam<export_all=2> {
            component = 0;
            unset = 0;
            null_value = 999.999;
         };
         DVset_null DVset_null {
            in => <-.in_field;
            &component => param.component;
            &unset => param.unset;
            &null_value => param.null_value;
            DV_Param_set_null &param<NEportLevels={2,0}> => <-.SetNullParam;
         };
         DVcomb_mesh_and_data DVcomb_mesh_and_data {
            in_mesh => <-.in_field;
            in_nd => <-.DVset_null.out;
         };
         DataObjectNoTexture obj {
            in => <-.DVcomb_mesh_and_data.out;
         };
         olink out_fld<export_all=2,NEportLevels={1,3}> => .DVcomb_mesh_and_data.out;
         olink out_obj<NEportLevels={1,2}> => .obj.obj;
      };
   };
   UI {
      shell {
         x = 88;
         y = 88;
      };
      Windows {
         IUI {
            optionList {
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
      Modules {
         IUI {
            optionList {
               cmdList => {
                  <-.<-.<-.<-.Bubbleviz.UIpanel.option,
                  <-.<-.<-.<-.isoline.UIpanel.option};
               selectedItem = 0;
            };
         };
      };
   };
   GDM.Uviewer2D Uviewer2D<NEx=440.,NEy=352.> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.Bubbleviz.out_obj};
            Xform {
               ocenter = {5.,6.,0.};
               dcenter = {5.53791,6.78598,
-0.763669};
               mat = {
                  0.941176,0.,0.,0.,0.,0.941176,0.,0.,0.,0.,0.941176,0.,
0.,0.,0.,1.
               };
               xlate = {-5.53791,-6.78598,
0.763669};
            };
         };
      };
      Scene_Editor {
         View_Editor {
            InfoPanel {
               curobj_sel {
                  width = 235;
                  height = 249;
                  x = 0;
                  y = 0;
               };
            };
         };
      };
   };
   XP_COMM_PROJ.Krig.KrigMacs.Krig2D Krig2D<NEx=407.,NEy=121.> {
      in_fld<NEdisplayMode="open"> => <-.data.combine_mesh_data.out;
   };
   MODS.isoline isoline<NEx=616.,NEy=275.>;
};
