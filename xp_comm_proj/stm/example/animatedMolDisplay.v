APPS.SingleWindowApp MolDisplay_AnimatedDisplay {
   UI {
      shell {
         x = 217;
         y = 59;
         clientHeight = 623;
      };
      Modules {
         IUI {
            optionList {
               cmdList => {
                  <-.<-.<-.<-.Coord_To_Geom_Display.MolNet.residue_name_text.UImod_panel.option,
                  <-.<-.<-.<-.Coord_To_Geom_Display.MolNet.atom_name_text.UImod_panel.option};
               selectedItem = 0;
            };
         };
      };
      Windows {
         IUI {
            panel {
               width = 475;
               height = 547;
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
   GDM.Uviewer3D Uviewer3D<NEx=165.,NEy=77.> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.Coord_To_Geom_Display.MolNet.atomic_positions.atomic_field.obj,
               <-.<-.<-.Coord_To_Geom_Display.MolNet.atomic_bonds.bond_field.obj,
               <-.<-.<-.Coord_To_Geom_Display.MolNet.residue_name_text.out_obj,
               <-.<-.<-.Coord_To_Geom_Display.MolNet.atom_name_text.out_obj};
            Props {
               subdiv = 15;
               line_aa = 1;
               material = {0.28,0.3,0.47,
26.07};
               dir_pt_size = 0.3199999928;
            };
            Modes {
               outline = "Off";
            };
            Top {
               cache_size = 10;
            };
            Xform {
               mat = {
                  -0.679693,0.185988,-0.709524,0.,0.517507,0.80711,-0.284181,0.,
0.519809,-0.56034,-0.644837,0.,0.,0.,0.,1.
               };
            };
         };
         View {
            View {
               trigger = 1;
            };
         };
         Lights {
            Lights = {,
               {
                  state=1,type="BiDirectional"
               },
               {
                  state=1,type="Point"
               },
               {
                  type="Point"
               }};
            LightXform = {
               {
                  mat=
                  {
                     0.725421,0.375623,-0.576778,0.,-0.192824,0.915317,0.353572,
0.,0.660746,-0.145272,0.736419,0.,0.,0.,0.,1.
                  }
               },
               {
                  mat=
                  {
                     0.781173,0.594043,-0.192038,0.,-0.301749,0.0899744,-0.949131,
0.,-0.546546,0.799384,0.249538,0.,0.,0.,0.,1.
                  }
               },
               {
                  mat=
                  {
                     0.585503,0.611109,0.532667,0.,-0.76973,0.625251,0.128753,
0.,-0.254369,-0.485395,0.836474,0.,0.,0.,0.,1.
                  }
               },
               };
         };
         Camera {
            Camera {
               auto_norm = "None";
               norm_scale = 0.9499999881;
            };
            CameraXform {
               xlate = {0.,0.,-12.};
            };
         };
      };
      Scene_Editor {
         Object_Editor {
            GDprops_edit {
               inherit = 0;
               subdiv = 15;
               line_style = "Solid";
               draw_mode = "Copy";
               line_width = 0;
               line_aa = 1;
               ambient = 0.2800000012;
               diffuse = 0.3000000119;
               specular = 0.4699999988;
               spec_exp = 26.06999969;
               trans = 1.;
               cull = "Normal";
               dir_pt_size = 0.3199999928;
               metal = 0.;
            };
            GDobj_edit {
               visible = 1;
               cache_size = 10;
               chunk = 1;
            };
            IUI {
               optionList {
                  selectedItem = 3;
               };
               General {
                  IUI {
                     AltSpace {
                        OPcmdList = {
                           {
                              set=1,,,,,,,,,,,,,,,,,,,,
                           },
                           };
                     };
                     TransformMode {
                        OPcmdList = {
                           {
                              set=1,,,,,,,,,,,,,,,,,,,,
                           },
                           ,,};
                     };
                  };
               };
               Modes {
                  IUI {
                     EditAltModes {
                        OPcmdList = {
                           {
                              set=1,,,,,,,,,,,,,,,,,,,,
                           },
                           };
                     };
                     Point {
                        OPcmdList = {
                           
                           {
                              set=1,,,,,,,,,,,,,,,,,,,,
                           },
                           ,,,,,
                        };
                     };
                     Surface {
                        OPcmdList = {
                           
                           {
                              set=1,,,,,,,,,,,,,,,,,,,,
                           },
                           ,,,,
                        };
                     };
                     Volume {
                        OPcmdList = {
                           {
                              set=1,,,,,,,,,,,,,,,,,,,,
                           },
                           ,,};
                     };
                     Bounds {
                        OPcmdList = {
                           {
                              set=1,,,,,,,,,,,,,,,,,,,,
                           },
                           ,};
                     };
                     Normals {
                        OPcmdList = {
                           {
                              set=1,,,,,,,,,,,,,,,,,,,,
                           },
                           ,};
                     };
                  };
               };
               Properties {
                  IUI {
                     ObjectOptions = {
                        {
                           set=1,,,,,,,,,,,,,,,,,,,,
                        },
                        };
                     EditAltProps {
                        y = 0;
                        x = 0;
                     };
                     Type {
                        IUI {
                           optionList {
                              selectedItem = 1;
                           };
                           General {
                              IUI {
                                 ColorOptions = {
                                    {
                                       set=1,,,,,,,,,,,,,,,,,,,,
                                    },
                                    ,};
                                 ColorEditor {
                                    ColorEcho {
                                       ColorView {
                                          trigger = 1;
                                       };
                                    };
                                 };
                              };
                           };
                           PointLine {
                              IUI {
                                 LineStyle {
                                    OPcmdList = {
                                       {
                                          set=1,,,,,,,,,,,,,,,,,,,,
                                       },
                                       ,,};
                                 };
                                 DrawingMode {
                                    OPcmdList = {
                                       {
                                          set=1,,,,,,,,,,,,,,,,,,,,
                                       },
                                       };
                                 };
                                 panel {
                                    visible = 1;
                                 };
                              };
                           };
                           Surface {
                              IUI {
                                 CullMode {
                                    OPcmdList = {
                                       {
                                          set=1,,,,,,,,,,,,,,,,,,,,
                                       },
                                       ,,};
                                 };
                              };
                           };
                        };
                     };
                     panel {
                        visible = 1;
                     };
                  };
               };
               Field_Conversion {
                  IUI {
                     Type {
                        IUI {
                           optionList {
                              selectedItem = 0;
                           };
                        };
                     };
                  };
               };
            };
            GDmodes_edit {
               points = "Inherit";
               lines = "Inherit";
               surf = "Inherit";
               outline = "Off";
            };
         };
         View_Editor {
            IUI {
               optionList {
                  selectedItem = 0;
               };
               General {
                  IUI {
                     panel {
                        visible = 1;
                     };
                     Renderer {
                        OPcmdList = {
                           ,
                           {
                              set=1,,,,,,,,,,,,,,,,,,,,
                           },
                           ,,
                        };
                     };
                     Timer {
                        y = 0;
                     };
                     Color {
                        ColorEcho {
                           ColorView {
                              trigger = 2;
                           };
                        };
                        rgb_or_hsv {
                           h = 0.;
                           s = 0.;
                           v = 0.;
                        };
                     };
                  };
               };
               Options {
                  IUI {
                     DoubleBuffer {
                        OPcmdList = {,,
                           {
                              set=1,,,,,,,,,,,,,,,,,,,,
                           }};
                     };
                  };
               };
            };
            GDobj_sel {
               curobj_name = "Top";
            };
            InfoPanel {
               curobj_sel {
                  selectedItem = 0;
                  x = 60;
                  y = 492;
                  width = 238;
                  height = 245;
                  ok = 1;
                  cancel = 1;
               };
            };
            GDview_edit {
               timer = 0;
               buffer = "Double (Pixmap)";
            };
         };
         Track_Editor {
            IUI {
               optionList {
                  selectedItem = 0;
               };
               Interactor_Behavior {
                  IUI {
                     panel {
                        visible = 1;
                     };
                     RotateMode {
                        OPcmdList = {
                           {
                              set=1,,,,,,,,,,,,,,,,,,,,
                           },
                           ,};
                     };
                     XYTranslateMode {
                        OPcmdList = {
                           {
                              set=1,,,,,,,,,,,,,,,,,,,,
                           },
                           ,,};
                     };
                  };
               };
            };
         };
         Light_Editor {
            GDlight_edit {
               state = 1;
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
                     LightType {
                        OPcmdList = {
                           {
                              set=1,,,,,,,,,,,,,,,,,,,,
                           },
                           ,,};
                     };
                     VUIColorEditor {
                        ColorEcho {
                           ColorView {
                              trigger = 1;
                           };
                        };
                     };
                  };
               };
            };
            GDlinfo_edit {
               show = 0;
            };
         };
         Datamap_Editor {
            IUI {
               Options {
                  IUI {
                     optionList {
                        selectedItem = 0;
                     };
                     Edit_Color {
                        IUI {
                           panel {
                              visible = 1;
                           };
                           ColorRB {
                              OPcmdList = {
                                 {
                                    set=1,,,,,,,,,,,,,,,,,,,,
                                 },
                                 };
                           };
                        };
                     };
                  };
               };
               DmapEcho {
                  DmapView {
                     trigger = 1;
                  };
               };
               ModelOptions = {
                  {
                     set=1,,,,,,,,,,,,,,,,,,,,
                  },
                  };
            };
         };
         Camera_Editor {
            IUI {
               optionList {
                  selectedItem = 0;
               };
               General {
                  IUI {
                     panel {
                        visible = 1;
                     };
                     CameraType {
                        OPcmdList = {
                           {
                              set=1,,,,,,,,,,,,,,,,,,,,
                           },
                           };
                     };
                     Extents {
                        OPcmdList = {
                           {
                              set=1,,,,,,,,,,,,,,,,,,,,
                           },
                           };
                     };
                     Mapping {
                        OPcmdList = {
                           {
                              set=1,,,,,,,,,,,,,,,,,,,,
                           },
                           };
                     };
                     Normalize {
                        OPcmdList = {
                           {
                              set=1,,,,,,,,,,,,,,,,,,,,
                           },
                           ,,};
                     };
                  };
               };
            };
            GDcamera_edit {
               norm_scale = 0.9499999881;
            };
         };
      };
   };
   macro Coord_To_Geom_Display<NEx=165.,NEy=11.,NEscalingFactor=1.,NExOffset=0.,NEyOffset=0.> {
      APPS.ModuleStack MolDisplayUI<NEx=11.,NEy=11.,NEscalingFactor=1.023114324,NExOffset=-11.,NEyOffset=83.90013885> {
         ModuleStackUI {
            mod_shell {
               y = 59;
               width => 391;
               height => 800;
               visible = 0;
            };
         };
         UIlabel File_Type_Label<NEy=132.,NEx=11.> {
            x = 10;
            y = 2;
            active = 1;
            parent => <-.File_Type_Frame;
            message<NEportLevels={2,0}> => <-.File_Type_Frame.message;
            label<NEportLevels={1,0}> => "Select File Type";
            width = 145;
            color {
               foregroundColor => <-.<-.fontColor;
               backgroundColor => <-.<-.bgColor;
            };
            fontAttributes {
               height = 14;
               weight = "bold";
            };
            height => 15;
         };
         UIlabel Units_Label<NEx=165.,NEy=132.> {
            x = 10;
            y = 3;
            parent => <-.Units_Frame;
            message<NEportLevels={2,0}> => <-.Units_Frame.message;
            label<NEportLevels={1,0}> => "Units";
            color {
               foregroundColor => <-.<-.fontColor;
               backgroundColor => <-.<-.bgColor;
            };
            fontAttributes {
               height = 14;
               weight = "bold";
            };
            width = 105;
            height => 20;
         };
         UIlabel Scale_Label<NEx=264.,NEy=264.> {
            x = 10;
            y = 3;
            parent => <-.Scale_Frame;
            message<NEportLevels={2,0}> => <-.Scale_Frame.message;
            label<NEportLevels={1,0}> => "Set Sphere Size";
            height => 20;
            width = 290;
            color {
               foregroundColor => <-.<-.fontColor;
               backgroundColor => <-.<-.bgColor;
            };
            fontAttributes {
               height = 14;
               weight = "bold";
            };
            alignment = "left";
         };
         macro Scale_Factor<NEx=264.,NEy=297.> {
            UIslider UIslider<NEx=484.,NEy=253.> {
               y = 25;
               height => 33;
               visible<NEportLevels={1,0}>;
               color {
                  foregroundColor = "black";
               };
               parent<NEportLevels={3,0}> => <-.<-.Scale_Frame;
               message<NEportLevels={3,0}> => <-.<-.Scale_Frame.message;
               min<NEportLevels={2,0}> => <-.Min;
               max<NEportLevels={2,0}> => <-.Max;
               value<NEportLevels={2,4}> = 1.00;
               decimalPoints<NEportLevels={2,0}> => <-.Decimal_Points;
               title<NEportLevels={1,0}> => ;
               width = 223;
            };
            int Decimal_Points<NEportLevels=1,NEx=11.,NEy=231.> = 2;
            float Min<NEportLevels=1,NEx=0.,NEy=143.> = 0.;
            float Max<NEportLevels=1,NEx=242.,NEy=143.> = 4.;
            UIfield UIfield<NEx=121.,NEy=319.> {
               x = 230;
               y = 30;
               visible<NEportLevels={1,0}>;
               color {
                  foregroundColor = "black";
               };
               parent<NEportLevels={3,0}> => <-.<-.Scale_Frame;
               message<NEportLevels={3,0}> => <-.<-.Scale_Frame.message;
               min<NEportLevels={2,0}> => <-.Min;
               max<NEportLevels={2,0}> => <-.Max;
               value<NEdisplayMode="NEopened"> => <-.UIslider.value;
               decimalPoints<NEportLevels={2,0},NEdisplayMode="open"> => <-.Decimal_Points;
               width = 70;
            };
         };
         UIlist File_Type<NEx=11.,NEy=165.,NEwidth=506.,NEheight=220.> {
            x = 4;
            y = 20;
            width = 155;
            height => 250;
            active = 1;
            color {
               foregroundColor = "black";
            };
            parent => <-.File_Type_Frame;
            message<NEportLevels={2,0}> => <-.File_Type_Frame.message;
            selectedItem<NEportLevels={0,3}> = 0;
            stringdims => 14;
            strings = {
               "Set PlainCoord","Set Coordinate","Set Cube","Set pdb",
               "Set PGamess","Set Gamess Cube","Set Dcar","Set DMOL_XYZ",
               "Set Mopac ESP","Set Mopac XYZ","Set mol2","Set mol","Set Chem3D",
               "Set mm3"
            };
            selectedText = "Set PlainCoord";
         };
         UIdynamic_toggle Show_Hydrogens<NEx=88.,NEy=297.> {
            y = 35;
            width = 160;
            color {
               foregroundColor = "black";
            };
            parent => <-.Show_Frame;
            message = "Toggle to diplay hydrogen atoms";
            set<NEportLevels={0,3}> = 1;
         };
         UIdynamic_toggle Show_Atom_Names<NEx=88.,NEy=330.> {
            y = 65;
            width = 160;
            color {
               foregroundColor = "black";
            };
            parent => <-.Show_Frame;
            message = "Toggle to display atom names";
            set<NEportLevels={0,3}>;
         };
         UIdynamic_toggle Show_Residue_Names<NEx=88.,NEy=363.> {
            y = 95;
            width = 160;
            visible<NEdisplayMode="open">;
            color {
               foregroundColor = "black";
            };
            parent => <-.Show_Frame;
            message = "Toggle to display residue names";
            set<NEportLevels={0,3}>;
         };
         UIlist Representation_List<NEx=484.,NEy=165.> {
            y = 25;
            width = 140;
            height => 100;
            active = 1;
            color {
               foregroundColor = "black";
            };
            parent => <-.Representation_Frame;
            message<NEportLevels={2,0}> => <-.Representation_Frame.message;
            selectedItem<NEportLevels={0,3}> = 0;
            stringdims => 5;
            strings<NEportLevels={1,0}> = {
               "Ball and Stick","CPK","Colored Stick","Colored Residue",
               "Stick"
            };
            selectedText = "Ball and Stick";
         };
         UIlabel Representation_Label<NEx=484.,NEy=132.> {
            x = 3;
            y = 3;
            width = 135;
            parent => <-.Representation_Frame;
            message<NEportLevels={2,0}> => <-.Representation_Frame.message;
            label<NEportLevels={1,0}> => "View Representation";
            color {
               foregroundColor => <-.<-.fontColor;
               backgroundColor => <-.<-.bgColor;
            };
            fontAttributes {
               height = 13;
               weight = "bold";
            };
            height => 20;
            alignment = "left";
         };
         UIfileBrowser UIfileBrowser<NEx=319.,NEy=165.> {
            UIfileDialog<NEx=44.,NEy=22.> {
               x = 393;
               y = 282;
               width = 300;
               height = 390;
               cancel = 1;
               filename<NEportLevels={1,2}> = "";
               dirMaskCache = "xp_comm_proj/stm/data/*";
               searchPattern = "*";
            };
            copy_on_change<NEx=220.,NEy=231.> {
               on_inst<NEdisplayMode="open"> = 0;
            };
            File_Browser_Panel {
               Browse<NEx=88.,NEy=165.> {
                  x => 10;
                  width = 100;
                  height = 30;
                  message<NEportLevels={2,0}> => <-.UIpanel.message;
                  label<NEportLevels={1,0}>;
                  parent<NEdisplayMode="open"> => File_Select_Frame;
                  y = 60;
                  fontAttributes<NEdisplayMode="open"> {
                     weight = "bold";
                  };
               };
               File_Name_Str<NEx=242.,NEy=231.> {
                  x => 4;
                  width => 324;
                  y = 335;
                  color {
                     foregroundColor = "cyan";
                     backgroundColor = "black";
                  };
                  message<NEportLevels={2,0}> => <-.UIpanel.message;
                  height = 33;
                  rows = 1;
                  resizeToText = 0;
                  parent => <-.<-.<-.Molecular_Display_UI;
                  fontAttributes {
                     weight = "bold";
                  };
               };
               File_Name_Label<NEx=242.,NEy=165.> {
                  width = 133;
                  height = 16;
                  message<NEportLevels={2,0}> => <-.UIpanel.message;
                  label<NEportLevels={1,0}> = "Press Browse to";
                  parent => File_Select_Frame;
                  color {
                     foregroundColor = "Yellow";
                  };
                  fontAttributes {
                     height = 14;
                     weight = "bold";
                  };
               };
               UIpanel<NEx=330.,NEy=110.> {
                  message<NEportLevels={4,2}> => <-.<-.<-.File_Select_Frame.message;
                  y = 170;
                  width<NEdisplayMode="open"> => 340;
                  height<NEdisplayMode="open"> = 40;
                  clientHeight = 40;
                  visible<NEdisplayMode="open">;
                  x = 151;
               };
               width<NEx=473.,NEy=154.>;
               parent<NEx=132.,NEy=55.> => <-.<-.File_Select_Frame;
            };
            visibility = 1;
            file_name<NEportLevels={1,3}>;
         };
         UIlist Units<NEx=165.,NEy=165.> {
            y = 35;
            width = 120;
            height => 50;
            color {
               foregroundColor = "black";
            };
            parent => <-.Units_Frame;
            message<NEportLevels={2,0}> => <-.Units_Frame.message;
            selectedText<NEportLevels={0,1}> = "Angstroms";
            selectedItem<NEportLevels={0,3}> = 1;
            strings<NEportLevels={1,0}> = {"Bohrs",
               "Angstroms"};
         };
         UIlabel File_Label<NEx=319.,NEy=132.> {
            x = 10;
            y = 30;
            width = 110;
            parent => <-.File_Select_Frame;
            message<NEportLevels={2,0}> => <-.File_Select_Frame.message;
            label<NEportLevels={1,0}> => "Select a File...";
            height => 20;
            color {
               foregroundColor => <-.<-.fontColor;
               backgroundColor = "light gray";
            };
            fontAttributes {
               height = 14;
               weight = "bold";
            };
         };
         UIframeEtchedIn File_Type_Frame<NEx=11.,NEy=99.> {
            x = 7;
            y = 45;
            width = 180;
            height = 290;
            color<NEdisplayMode="open"> {
               foregroundColor = "blue";
               backgroundColor = "dark gray";
            };
            parent => <-.Molecular_Display_UI;
            message<NEportLevels={0,2}> = "Select the type of file";
            shadowThickness = 8;
         };
         UIframeEtchedIn Units_Frame<NEx=165.,NEy=99.> {
            x = 190;
            y = 45;
            width = 140;
            height = 120;
            color {
               foregroundColor = "blue";
               backgroundColor = "dark gray";
            };
            parent => <-.Molecular_Display_UI;
            message<NEportLevels={0,2}> = "Select the units";
            shadowThickness = 8;
         };
         UIlabel Molecular_Display<NEx=330.,NEy=55.> {
            x = -1;
            y = 3;
            width = 307;
            fontAttributes {
               height = 18;
            };
            parent => <-.UI_Label_Frame;
            message<NEportLevels={2,0}> => <-.UI_Label_Frame.message;
            label<NEportLevels={1,0}>;
            height => 28;
            color<NEdisplayMode="open"> {
               foregroundColor = "Red";
               backgroundColor = "Grey";
            };
            alignment = "center";
         };
         UIframeEtchedIn File_Select_Frame<NEx=319.,NEy=99.,NEwidth=484.,NEheight=231.> {
            x = 190;
            y = 165;
            width = 140;
            clientHeight = 94;
            color {
               foregroundColor = "blue";
               backgroundColor = "dark gray";
            };
            parent => <-.Molecular_Display_UI;
            message<NEportLevels={0,2}> = "Select a data file";
            shadowThickness = 8;
            height = 110;
         };
         UIframeEtchedOut UI_Label_Frame<NEx=330.,NEy=22.> {
            x = 7;
            y = 2;
            width = 330;
            height = 45;
            color {
               foregroundColor = "cyan";
               backgroundColor = "Grey";
            };
            parent => <-.Molecular_Display_UI;
            message<NEportLevels={0,2}> = "Display a molecular geometry";
            shadowThickness = 10;
         };
         UIframeEtchedIn Representation_Frame<NEx=484.,NEy=99.> {
            x = 4;
            y = 370;
            width = 160;
            height = 145;
            color {
               foregroundColor = "blue";
               backgroundColor = "dark gray";
            };
            parent => <-.Molecular_Display_UI;
            message<NEportLevels={0,2}> = "Select representation for the display";
            shadowThickness = 8;
         };
         UIframeEtchedIn Show_Frame<NEx=88.,NEy=220.> {
            x = 164;
            y = 370;
            width = 165;
            height = 145;
            color {
               foregroundColor = "blue";
               backgroundColor = "dark gray";
            };
            parent => <-.Molecular_Display_UI;
            message<NEportLevels={0,2}> = "Select display options";
            shadowThickness = 8;
         };
         UIlabel Display_Options<NEx=88.,NEy=253.> {
            x = 10;
            y = 3;
            parent => <-.Show_Frame;
            message<NEportLevels={2,0}> => <-.Show_Frame.message;
            label<NEportLevels={1,0}>;
            color {
               foregroundColor => <-.<-.fontColor;
               backgroundColor => <-.<-.bgColor;
            };
            fontAttributes {
               height = 14;
               weight = "bold";
            };
            height => 25;
            width = 130;
            alignment = "left";
         };
         UIframeEtchedIn Scale_Frame<NEx=264.,NEy=231.> {
            x = 4;
            y = 520;
            width = 325;
            height = 85;
            color {
               foregroundColor = "blue";
               backgroundColor = "dark gray";
            };
            parent => <-.Molecular_Display_UI;
            message<NEportLevels={0,2}> = "Select sphere scale factor";
            shadowThickness = 8;
         };
         UIshell Molecular_Display_UI<NEx=44.,NEy=-33.> {
            x = 7;
            y = -6;
            width = 345;
            height = 800;
            color<NEdisplayMode="open"> {
               backgroundColor = "dark gray";
            };
         };
         UIbutton ReadFile_Toggle<NEx=682.,NEy=143.,NEwidth=286.,NEheight=220.> {
            label => "Start";
            y = 280;
            width = 80;
            x = 190;
            color {
               foregroundColor<NEdisplayMode="open"> = "cyan";
               backgroundColor = "black";
            };
            fontAttributes {
               height = 18;
               weight = "bold";
            };
            message = "Click here to read the file with the selected attributes";
            active = 1;
            do<NEportLevels={2,2}> = 1;
            parent => <-.Molecular_Display_UI;
         };
         UIlabel HScale_Label<NEx=572.,NEy=264.> {
            y = 2;
            parent => <-.HScale_Frame;
            message<NEportLevels={2,0}> => <-.HScale_Frame.message;
            label<NEportLevels={1,0},NEdisplayMode="open"> => "Set Hydrogen Bond Scale Factor";
            width = 290;
            height => 18;
            color {
               foregroundColor => <-.<-.fontColor;
               backgroundColor => <-.<-.bgColor;
            };
            fontAttributes {
               height = 14;
               weight = "bold";
            };
            alignment = "left";
         };
         macro HScale_Factor<NEx=572.,NEy=297.> {
            UIslider UIslider<NEx=484.,NEy=253.> {
               y = 20;
               height => 35;
               visible<NEportLevels={1,0}>;
               color {
                  foregroundColor = "black";
               };
               parent<NEportLevels={3,0}> => <-.<-.HScale_Frame;
               message<NEportLevels={3,0}> => <-.<-.HScale_Frame.message;
               min<NEportLevels={2,0}> => <-.Min;
               max<NEportLevels={2,0}> => <-.Max;
               value<NEportLevels={2,4}> = 0.8;
               decimalPoints<NEportLevels={2,0}> => <-.Decimal_Points;
               title<NEportLevels={1,0}> => ;
               width = 223;
            };
            int Decimal_Points<NEportLevels=1,NEx=11.,NEy=231.,NEdisplayMode="open"> = 2;
            float Min<NEportLevels=1,NEx=0.,NEy=143.> = 0.;
            float Max<NEportLevels=1,NEx=242.,NEy=143.> = 4.;
            UIfield UIfield<NEx=121.,NEy=319.> {
               x = 233;
               y = 27;
               visible<NEportLevels={1,0}>;
               color {
                  foregroundColor = "black";
               };
               parent<NEportLevels={3,0}> => <-.<-.HScale_Frame;
               message<NEportLevels={3,0}> => <-.<-.HScale_Frame.message;
               min<NEportLevels={2,0}> => <-.Min;
               max<NEportLevels={2,0}> => <-.Max;
               value<NEdisplayMode="NEopened"> => <-.UIslider.value;
               decimalPoints<NEportLevels={2,0},NEdisplayMode="open"> => <-.Decimal_Points;
               width = 70;
            };
         };
         UIframeEtchedIn HScale_Frame<NEx=572.,NEy=231.> {
            x = 4;
            y = 700;
            width = 325;
            height = 85;
            color {
               foregroundColor = "blue";
               backgroundColor = "dark gray";
            };
            parent => <-.Molecular_Display_UI;
            message<NEportLevels={0,2}> = "Select sphere scale factor";
            shadowThickness = 8;
         };
         UIlabel BScale_Label<NEx=418.,NEy=264.> {
            x = 10;
            y = 3;
            parent => <-.BScale_Frame;
            message<NEportLevels={2,0}> => <-.BScale_Frame.message;
            label<NEportLevels={1,0}> => "Set Bond Scale Factor";
            width = 290;
            height => 17;
            color<NEdisplayMode="open"> {
               foregroundColor => <-.<-.fontColor;
               backgroundColor => <-.<-.bgColor;
            };
            fontAttributes {
               height = 14;
               weight = "bold";
            };
            alignment = "left";
         };
         macro BScale_Factor<NEx=418.,NEy=297.> {
            UIslider UIslider<NEx=484.,NEy=253.> {
               y = 0;
               visible<NEportLevels={1,0}>;
               color {
                  foregroundColor = "black";
               };
               parent<NEportLevels={3,0}> => <-.<-.BScale_Frame;
               message<NEportLevels={3,0}> => <-.<-.BScale_Frame.message;
               min<NEportLevels={2,0}> => <-.Min;
               max<NEportLevels={2,0}> => <-.Max;
               value<NEportLevels={2,4}> = 1.15;
               decimalPoints<NEportLevels={2,0}> => <-.Decimal_Points;
               title<NEportLevels={1,0}> => ;
               width = 226;
            };
            int Decimal_Points<NEportLevels=1,NEx=11.,NEy=231.> = 2;
            float Min<NEportLevels=1,NEx=0.,NEy=143.> = 0.;
            float Max<NEportLevels=1,NEx=242.,NEy=143.> = 4.;
            UIfield UIfield<NEx=121.,NEy=319.> {
               x = 230;
               y = 20;
               visible<NEportLevels={1,0}>;
               color {
                  foregroundColor = "black";
               };
               parent<NEportLevels={3,0}> => <-.<-.BScale_Frame;
               message<NEportLevels={3,0}> => <-.<-.BScale_Frame.message;
               min<NEportLevels={2,0}> => <-.Min;
               max<NEportLevels={2,0}> => <-.Max;
               value<NEdisplayMode="NEopened"> => <-.UIslider.value;
               decimalPoints<NEportLevels={2,0},NEdisplayMode="open"> => <-.Decimal_Points;
               width = 70;
            };
         };
         UIframeEtchedIn BScale_Frame<NEx=418.,NEy=231.> {
            x = 4;
            y = 610;
            width = 325;
            height = 85;
            color {
               foregroundColor = "blue";
               backgroundColor = "dark gray";
            };
            parent => <-.Molecular_Display_UI;
            message<NEportLevels={0,2}> = "Select sphere scale factor";
            shadowThickness = 8;
         };
         UItoggle Pause_Display<NEx=682.,NEy=110.> {
            parent => <-.Molecular_Display_UI;
            set<NEdisplayMode="open">;
            x = 190;
            y = 310;
            width = 150;
            color {
               foregroundColor = "Red";
            };
            fontAttributes {
               height = 16;
               weight = "bold";
            };
         };
         int Trigger<NEportLevels={1,2},NEx=671.,NEy=176.,NEwidth=869.,NEheight=55.> => ((.Pause_Display.set - 1) & ReadFile_Toggle.do);
         string fontColor<NEportLevels=1,NEx=682.,NEy=33.> = "Brown";
         string bgColor<NEportLevels=1,NEx=682.,NEy=66.> = "light gray";
         XP_COMM_PROJ.STM.MACRO.animFileUI animFileUI<NEx=660.,NEy=341.> {
            Animate_File_Name_UI {
               x = 126;
               y = 415;
               width = 279;
               height = 550;
            };
            animFile_Frame<NEx=77.,NEy=77.> {
               x = 4;
               y = 20;
               height = 510;
            };
            Root_File_Name<NEy=143.> {
               text = "data/DWB";
               y = 70;
               height = 40;
            };
            Number_of_digits<NEy=231.> {
               y = 125;
               value = 4.;
               height = 65;
            };
            File_Root_Name<NEx=506.,NEy=132.> {
               y = -4;
            };
            Animate_File_Name_<NEx=385.,NEy=209.> {
               visible => ;
               y = 20;
            };
            MODS.Loop Loop<NEx=374.,NEy=33.,NEscalingFactor=1.035714269,NExOffset=-14.,NEyOffset=43.68965912> {
               start = 1.;
               end = 28.;
               incr = 1.;
               count<NEportLevels={0,3}> = 1.;
               UIpanel<NEx=550.,NEy=286.> {
                  visible => 1;
                  option<NEdisplayMode="open"> {
                     set = 1;
                  };
                  parent => <-.<-.animFile_Frame;
                  y = 200;
                  active = 1;
                  height = 300;
               };
               cycle = 2;
               LoopUI<NEscalingFactor=1.007185578,NExOffset=-11.,NEyOffset=13.5737505,NEx=440.,NEy=374.> {
                  Once {
                     set = 0;
                  };
                  Bounce {
                     set = 1;
                  };
                  UIpanel<NEx=11.,NEy=44.>;
                  cycle_toggle {
                     x = 0;
                  };
                  start_typein {
                     x = 0;
                  };
                  end_typein {
                     x = 0;
                  };
                  incr_typein {
                     x = 0;
                  };
                  count_typein {
                     x = 0;
                  };
               };
               loop<NEx=253.,NEy=275.>;
               instancer<NEx=429.,NEy=231.>;
            };
         };
         XP_COMM_PROJ.STM.MODULE.animFile animFile<NEx=660.,NEy=385.,NEwidth=363.,NEheight=176.> {
            Root_Name => <-.animFileUI.Root_File_Name.text;
            num => <-.animFileUI.Number_of_digits.value;
            time_step => <-.animFileUI.Loop.count;
            FileName<NEportLevels={0,3}>;
         };
      };
      macro MolNet<NExOffset=0.,NEyOffset=0.,NEscalingFactor=1.,NEx=11.,NEy=220.> {
         macro atomic_positions<NEwidth=484.,NEheight=198.> {
            FLD_MAP.point_mesh atomic_point_mesh<NEx=33.,NEy=88.> {
               coord<NEportLevels={4,0}> => <-.<-.<-.Mol_c.a_lst;
               out<NEportLevels={0,3}>;
            };
            FLD_MAP.radius_data atomic_radii<NEx=220.,NEy=121.> {
               in_data<NEportLevels={4,0}> => <-.<-.<-.Mol_c.r_lst;
            };
            FLD_MAP.node_colors atomic_mesh_colors<NEx=418.,NEy=121.> {
               in_data<NEportLevels={4,0}> => <-.<-.<-.Mol_c.rgb_lst;
            };
            FLD_MAP.combine_node_datas combine_node_datas<NEx=253.,NEy=198.> {
               in => {
                  <-.atomic_mesh_colors.out,<-.atomic_radii.out};
            };
            FLD_MAP.combine_mesh_data atomic_field<NEx=143.,NEy=341.> {
               in_mesh => <-.atomic_point_mesh.out;
               in_nd => <-.combine_node_datas.out;
               obj<NEportLevels={1,5}>;
               DataObject {
                  Props {
                     line_width = 1;
                     inherit = 0;
                     subdiv = 20;
                     material = {0.3,0.4,0.95,
50.};
                     line_aa = 1;
                  };
               };
            };
         };
         macro atomic_bonds<NEwidth=330.,NEheight=154.> {
            FLD_MAP.line_mesh bond_mesh<NEx=0.,NEy=11.> {
               coord<NEportLevels={4,0},NEdisplayMode="open"> => <-.<-.<-.Mol_c.a_lst;
               connect<NEportLevels={4,0}> => <-.<-.<-.Mol_c.connect_lst;
               out {
                  xform {
                     ocenter = {-0.0114274,-0.00678647,
0.039991};
                     dcenter = {-0.0114274,-0.00678647,
0.039991};
                     center = {-0.0114274,-0.00678647,
0.039991};
                  };
               };
            };
            FLD_MAP.node_colors bond_colors<NEx=154.,NEy=11.> {
               in_data<NEportLevels={4,0},NEdisplayMode="open"> => <-.<-.<-.Mol_c.rgb_lst;
            };
            FLD_MAP.combine_mesh_data bond_field<NEx=77.,NEy=77.> {
               in_mesh => <-.bond_mesh.out;
               in_nd => <-.bond_colors.out;
               obj<NEportLevels={1,5}>;
               DataObject {
                  Props {
                     line_width = 4;
                     inherit = 0;
                     line_aa = 1;
                     dir_pt_size = 2.5;
                     subdiv = 17;
                  };
                  Modes {
                     outline = "Off";
                     mode = {0,3,0,0,0};
                  };
               };
            };
         };
         MODS.text_glyph residue_name_text {
            in_mesh => <-.atomic_positions.atomic_point_mesh.out;
            in_text<NEportLevels={3,0}> => <-.<-.Mol_c.residue_name;
            UImod_panel {
               clientHeight = 1500;
               parent<NEportLevels={2,0}>;
               option {
                  set = 1;
               };
            };
            TextUI<NEscalingFactor=0.6597014666,NExOffset=221.809967,NEyOffset=-22.> {
               Font {
                  text = "-adobe-times-*-r-*-*-18-*-*-*-*-*-*-*";
               };
               AlignHorizMenu<NEx=660.,NEy=594.> {
                  x = 0;
                  selectedItem<NEdisplayMode="open"> = 1;
               };
               UIoption {
                  set = 0;
               };
               UIoption#1 {
                  set = 1;
               };
               AlignVertMenu<NEx=352.,NEy=594.> {
                  x = 0;
                  selectedItem = 2;
                  alignment<NEdisplayMode="open">;
               };
               UIoption#3 {
                  set = 0;
               };
               UIoption#5 {
                  set = 1;
               };
               Offset {
                  set = 1;
               };
               OffsetXValue {
                  fval => 0.03;
                  field {
                     value = 0.01;
                  };
               };
               OffsetYValue {
                  fval => 0.01;
                  field {
                     value = -0.01;
                  };
               };
               UPpath<NEx=517.,NEy=440.>;
            };
            TextValues<NEx=297.,NEy=473.> {
               offset<NEdisplayMode="open">;
            };
            obj {
               Props {
                  col<NEportLevels={0,0}> = {1.,0.,1.};
                  font<NEdisplayMode="open">;
               };
               AltObject<NEx=440.,NEy=242.>;
            };
            out_obj<NEportLevels={1,4}>;
         };
         MODS.text_glyph atom_name_text {
            in_mesh => <-.atomic_positions.atomic_point_mesh.out;
            in_text<NEportLevels={3,0}> => <-.<-.Mol_c.atom_name;
            TextUI {
               Font {
                  text = "-adobe-times-bold-r-*-*-18-*-*-*-*-*-*-*";
               };
               AlignHorizMenu {
                  x = 0;
               };
               AlignVertMenu {
                  x = 0;
                  selectedItem = 2;
               };
               UIoption#3 {
                  set = 0;
               };
               UIoption#5 {
                  set = 1;
               };
               Offset {
                  set = 1;
               };
               OffsetXValue {
                  field {
                     value = 0.01;
                  };
               };
               OffsetYValue {
                  field {
                     value = 0.06;
                  };
               };
            };
            out_obj<NEportLevels={1,4}>;
         };
      };
      XP_COMM_PROJ.STM.MODULE.Mol_c Mol_c<NEx=11.,NEy=110.,NEwidth=275.,NEheight=253.> {
         Scale => <-.MolDisplayUI.Scale_Factor.UIslider.value;
         Units => <-.MolDisplayUI.Units.selectedItem;
         Show_Hydrogens => <-.MolDisplayUI.Show_Hydrogens.set;
         File_Name => <-.MolDisplayUI.animFile.FileName;
         Show_Atom_Names => <-.MolDisplayUI.Show_Atom_Names.set;
         Show_Residue_Names => <-.MolDisplayUI.Show_Residue_Names.set;
         FType => <-.MolDisplayUI.File_Type.selectedItem;
         Rep => <-.MolDisplayUI.Representation_List.selectedItem;
         HScale => <-.MolDisplayUI.HScale_Factor.UIslider.value;
         BScale => <-.MolDisplayUI.BScale_Factor.UIslider.value;
         trigger => <-.MolDisplayUI.Trigger;
         residue_name<NEportLevels={0,2}>;
      };
   };
};
