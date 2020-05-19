

flibrary XrayMacs <compile_subs=0> {

   macro XrayUI {
      XP_COMM_PROJ.Xray.XrayMods.XrayParams &XrayParams<NEportLevels={2,1}>;

      UImod_panel UImod_panel {
         title => "X-Ray Orthographic Volume Renderer";
      };


      int boxWidth => (.UImod_panel.width/2) - 15;

      UIlabel ModeLabel {
         parent => <-.UImod_panel;
         x => 5;
         y => 10;
         width => <-.boxWidth - 5;
         label = "Mode";
         alignment = "left";
      };
      UIradioBox OperationRadioBox {
         parent => <-.UImod_panel;
         x => <-.ModeLabel.x - 5;
         y => <-.ModeLabel.y + <-.ModeLabel.height + 10;
         width => <-.boxWidth;
         cmdList => {
            <-.UIoptionSUM,
            <-.UIoptionMEAN,
            <-.UIoptionMEDIAN,
            <-.UIoptionMIN,
            <-.UIoptionMAX
         };
         selectedItem => <-.XrayParams.mode;
      };

      UIlabel DirectionLabel {
         parent => <-.UImod_panel;
         x => <-.ModeLabel.x + <-.boxWidth + 15;
         y => <-.ModeLabel.y;
         width => <-.boxWidth - 5;
         label = "Direction";
         alignment = "left";
      };
      UIradioBox DirectionRadioBox {
         parent => <-.UImod_panel;
         x => <-.DirectionLabel.x - 5;
         y => <-.OperationRadioBox.y;
         width = <-.boxWidth;
         cmdList => {
            <-.UIoptionXZ,
            <-.UIoptionYZ,
            <-.UIoptionXY
         };
         selectedItem => <-.XrayParams.axis;
      };

      UIoption UIoptionXZ { label => "XZ - axis"; };
      UIoption UIoptionYZ { label => "YZ - axis"; };
      UIoption UIoptionXY { label => "XY - axis"; };

      UIoption UIoptionSUM    { label => "Sum"; };
      UIoption UIoptionMEAN   { label => "Mean"; };
      UIoption UIoptionMEDIAN { label => "Median"; };
      UIoption UIoptionMIN    { label => "Min"; };
      UIoption UIoptionMAX    { label => "Max"; };
   };



   // Functional Macro
   macro Xray_Func {

      Mesh_Struct+Node_Data &in_field<NEportLevels={2,1}>;
      XP_COMM_PROJ.Xray.XrayMods.XrayParams &XrayParams<NEportLevels={2,1}>;

      group extract_data_array_noUI {
         Node_Data+IPort2 &in => <-.in_field;
         int+IPort2 comp=0;
         prim+OPort2 data[]=>in.node_data[comp].values;
      };

      byte+Port array_data[] => .extract_data_array_noUI.data;
      int+Port  dims[3]      => in_field.dims;


      XP_COMM_PROJ.Xray.XrayMods.XrayCore XrayCore {
         input => <-.array_data;
         dims => <-.dims;
         XrayParams => <-.XrayParams;
      };
      int+Port dims_2D[2] => {.XrayCore.nx, .XrayCore.ny};

      FLD_MAP.uniform_scalar_field uniform_scalar_field {
         mesh {
            in_dims => <-.<-.dims_2D;
         };
         data {
            in_data => <-.<-.XrayCore.output;
         };
      };

      olink out_fld => .uniform_scalar_field.out;
      olink out_obj => .uniform_scalar_field.obj;
   };



   // user macro
   macro xray {
      Mesh_Struct+Node_Data &in_field<NEportLevels={2,1}>;

      XP_COMM_PROJ.Xray.XrayMods.XrayParams XrayParams {
         axis = 2;
         mode = 0;
      };

      XrayUI XrayUI {
         XrayParams => <-.XrayParams;
      };

      Xray_Func Xray_Func {
         in_field => <-.in_field;
         XrayParams => <-.XrayParams;
      };

      olink out_fld => .Xray_Func.out_fld;
      olink out_obj => .Xray_Func.out_obj;
   };



   APPS.SingleWindowApp XrayEg {
      MODS.Read_Volume Read_Volume {
         read_volume_ui {
            filename = "$XP_PATH<0>/data/volume/lobster.dat";
         };
      };

      MODS.crop crop {
         in_field => <-.Read_Volume.field;
      };

      xray xray {
         in_field => <-.crop.out_fld;
      };

      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.xray.out_obj
               };
            };
         };
      };

   };


}; // end of lib

