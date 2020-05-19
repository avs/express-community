
flibrary CombineDataMacs <compile_subs=0> {

   macro CMBD_ComponentBox {
      UIframe UIframe {
         parent<NEportLevels={3,0}>;
         width => .parent.clientWidth;
         height => ((<-.Invert_Data.y + <-.Invert_Data.height) + 10);
      };
      UIradioBoxLabel Component {
         parent => <-.UIframe;
         labels<NEportLevels={3,1}>;
         selectedItem<NEportLevels={3,0}>;
      };
      UItoggle Constant {
         parent => <-.UIframe;
         set<NEportLevels={3,2}>;
         y => (<-.Component.height + 10);
      };
      UIslider Constant_Value {
         parent => <-.UIframe;
         value<NEportLevels={3,2}>;
         y => ((<-.Constant.y + <-.Constant.height) + 10);
         active<NEportLevels={2,1}> => <-.Constant.set;
         min = 0.;
         max = 1.;
         immediateMode = 1;
      };
      UItoggle Invert_Data {
         parent => <-.UIframe;
         set<NEportLevels={3,2}>;
         y => ((<-.Constant_Value.y + <-.Constant_Value.height) + 10);
      };
   };


   macro Combine_Datasets_To_HSV_UI {
      UImod_panel UImod_panel {
         title = "Combine Datasets to HSV";
      };

      XP_COMM_PROJ.CombineData.CombineDataMods.CombineDataParams &params<NEportLevels={2,1}>;
      string data_labels<NEportLevels={2,1}>[];

      CMBD_ComponentBox Component_0 {
         UIframe {
            parent => <-.<-.UImod_panel;
            y = 0;
         };
         Component {
            labels => <-.<-.data_labels;
            selectedItem => <-.<-.params.comp0;
            title => "Hue Component";
         };
         Constant {
            set => <-.<-.params.constant0on;
         };
         Constant_Value {
            value => <-.<-.params.constant0val;
         };
         Invert_Data {
            set => <-.<-.params.invert0on;
         };
      };
      CMBD_ComponentBox Component_1 {
         UIframe {
            parent => <-.<-.UImod_panel;
            y => <-.<-.Component_0.UIframe.y + <-.<-.Component_0.UIframe.height + 10;
         };
         Component {
            labels => <-.<-.data_labels;
            selectedItem => <-.<-.params.comp1;
            title => "Saturation Component";
         };
         Constant {
            set => <-.<-.params.constant1on;
         };
         Constant_Value {
            value => <-.<-.params.constant1val;
         };
         Invert_Data {
            set => <-.<-.params.invert1on;
         };
      };
      CMBD_ComponentBox Component_2 {
         UIframe {
            parent => <-.<-.UImod_panel;
            y => <-.<-.Component_1.UIframe.y + <-.<-.Component_1.UIframe.height + 10;
         };
         Component {
            labels => <-.<-.data_labels;
            selectedItem => <-.<-.params.comp2;
            title => "Value/Light Component";
         };
         Constant {
            set => <-.<-.params.constant2on;
         };
         Constant_Value {
            value => <-.<-.params.constant2val;
         };
         Invert_Data {
            set => <-.<-.params.invert2on;
         };
      };

   };


   Combine_Datasets_To_HSV_UI Combine_Datasets_To_RGB_UI {
      UImod_panel {
         title = "Combine Datasets to RGB";
      };

      Component_0 {
         Component {
            title => "Red Component";
         };
      };
      Component_1 {
         Component {
            title => "Blue Component";
         };
      };
      Component_2 {
         Component {
            title => "Green Component";
         };
      };
   };



   macro Combine_Datasets_To_HSV_Func {
      ilink in_fld;

      XP_COMM_PROJ.CombineData.CombineDataMods.CombineDataParams &params<NEportLevels={2,1}>;

      XP_COMM_PROJ.CombineData.CombineDataMods.CMBD_extract_data_array extract_data_array#1 {
         in => <-.in_fld;
         comp => <-.params.comp0;
      };
      XP_COMM_PROJ.CombineData.CombineDataMods.CMBD_extract_data_array extract_data_array#2 {
         in => <-.in_fld;
         comp => <-.params.comp1;
      };
      XP_COMM_PROJ.CombineData.CombineDataMods.CMBD_extract_data_array extract_data_array#3 {
         in => <-.in_fld;
         comp => <-.params.comp2;
      };

      XP_COMM_PROJ.CombineData.CombineDataMods.DVcombine_data_to_HSV DVcombine_data_to_HSV {
         data0 => <-.extract_data_array#1.data;
         data1 => <-.extract_data_array#2.data;
         data2 => <-.extract_data_array#3.data;
         params => <-.params;
      };
      FLD_MAP.interleave_3_arrays interleave_3_arrays {
         in1 => <-.DVcombine_data_to_HSV.r;
         in2 => <-.DVcombine_data_to_HSV.g;
         in3 => <-.DVcombine_data_to_HSV.b;
      };
      FLD_MAP.node_colors node_colors {
         in_data => <-.interleave_3_arrays.out;
         out {
            node_data = {
               {
                  labels="NODE COLOR",,,,,,,,,
               }};
         };
      };

      XP_COMM_PROJ.CombineData.CombineDataMods.DVadd_node_data DVadd_node_data {
         in_fld => <-.in_fld;
         in_nd => <-.node_colors.out;
      };

      olink out_fld => .DVadd_node_data.out_fld;
      olink out_obj => .DVadd_node_data.out_obj;
   };


   Combine_Datasets_To_HSV_Func Combine_Datasets_To_RGB_Func {
      -DVcombine_data_to_HSV;

      XP_COMM_PROJ.CombineData.CombineDataMods.DVcombine_data_to_RGB DVcombine_data_to_RGB {
         data0 => <-.extract_data_array#1.data;
         data1 => <-.extract_data_array#2.data;
         data2 => <-.extract_data_array#3.data;
         params => <-.params;
      };
      interleave_3_arrays {
         in1 => <-.DVcombine_data_to_RGB.r;
         in2 => <-.DVcombine_data_to_RGB.g;
         in3 => <-.DVcombine_data_to_RGB.b;
      };
   };



   macro Combine_Datasets_To_HSV {
      ilink in_fld;

      XP_COMM_PROJ.CombineData.CombineDataMods.CombineDataParams params;

      Combine_Datasets_To_HSV_Func Combine_Datasets_To_HSV_Func {
         in_fld => <-.in_fld;
         params => <-.params;
      };

      string+nres+Port data_labels[] => in_fld.node_data.labels;

      Combine_Datasets_To_HSV_UI Combine_Datasets_To_HSV_UI {
         params => <-.params;
         data_labels => <-.data_labels;
      };

      olink out_fld => .Combine_Datasets_To_HSV_Func.out_fld;
      olink out_obj => .Combine_Datasets_To_HSV_Func.out_obj;
   };


   Combine_Datasets_To_HSV Combine_Datasets_To_RGB {
      -Combine_Datasets_To_HSV_Func;
      Combine_Datasets_To_RGB_Func Combine_Datasets_To_RGB_Func {
         in_fld => <-.in_fld;
         params => <-.params;
      };

      -Combine_Datasets_To_HSV_UI;
      Combine_Datasets_To_RGB_UI Combine_Datasets_To_RGB_UI {
         params => <-.params;
         data_labels => <-.data_labels;
      };

      out_fld => .Combine_Datasets_To_RGB_Func.out_fld;
      out_obj => .Combine_Datasets_To_RGB_Func.out_obj;
   };



   APPS.MultiWindowApp CombineDataEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.bounds.out_obj,
                  <-.<-.<-.Combine_Datasets_To_HSV.out_obj
               };
               Xform {
                  ocenter = {12.,12.,15.5};
                  dcenter = {3.96127,3.89111,4.9922};
                  mat = {
                     -0.118174,-0.0280091,0.179378,0.,
                     0.181241,-0.0307174,0.114605,0.,
                     0.0106176,0.212599,0.0401911,0.,
                     0.,0.,0.,1.
                  };
                  xlate = {-3.96127,-3.89111,-4.9922};
               };
            };
            Lights {
               Lights = {
                  {
                     type="BiDirectional"
                  },
                  ,,};
            };
            Camera {
               Camera {
                  perspec = 1;
                  front = 1.;
               };
            };
         };
         Scene_Selector {
            input_views => {Scene.View.View, <-.RGBScene.View.View};
         };
         GDM.Uscene3D RGBScene {
            Top {
               child_objs<NEportLevels={4,1}> => {
                  <-.<-.<-.Combine_Datasets_To_RGB.out_obj,
                  <-.<-.<-.bounds.out_obj
               };
               Xform {
                  ocenter = {12.,12.,15.5};
                  dcenter = {3.96127,3.89111,4.9922};
                  mat = {
                     -0.118174,-0.0280091,0.179378,0.,
                     0.181241,-0.0307174,0.114605,0.,
                     0.0106176,0.212599,0.0401911,0.,
                     0.,0.,0.,1.
                  };
                  xlate = {-3.96127,-3.89111,-4.9922};
               };
            };
            Lights {
               Lights = {
                  {
                     type="BiDirectional"
                  },
                  ,,};
            };
            Camera {
               Camera {
                  perspec = 1;
                  front = 1.;
               };
            };
         };
      };


      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>/data/field/wind.fld";
         };
      };
      MODS.bounds bounds {
         in_field => <-.Read_Field.field;
      };
      MODS.orthoslice orthoslice {
         in_field => <-.Read_Field.field;
      };

      XP_COMM_PROJ.CombineData.CombineDataMacs.Combine_Datasets_To_HSV Combine_Datasets_To_HSV {
         in_fld => <-.orthoslice.out_fld;
         params {
            comp0 = 0;
            comp1 = 1;
            comp2 = 2;
         };
      };
      XP_COMM_PROJ.CombineData.CombineDataMacs.Combine_Datasets_To_RGB Combine_Datasets_To_RGB {
         in_fld => <-.orthoslice.out_fld;
         params {
            comp0 = 0;
            comp1 = 1;
            comp2 = 2;
         };
      };

   };


};

