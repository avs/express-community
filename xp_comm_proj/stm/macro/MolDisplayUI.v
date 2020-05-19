      APPS.ModuleStack MolDisplayUI<NEx=11.,NEy=11.,NEscalingFactor=0.8803418875,NExOffset=-11.,NEyOffset=52.72815704> {
         ModuleStackUI {
            mod_shell {
               y = 59;
               width => 391;
               height => 703;
               visible = 0;
            };
         };
         UIlabel File_Type_Label<NEy=132.,NEx=11.> {
            y = 0;
            active = 1;
            parent => <-.File_Type_Frame;
            message<NEportLevels={2,0}> => <-.File_Type_Frame.message;
            label<NEportLevels={1,0}> => "Select File Type";
            width = 170;
            color {
               foregroundColor => <-.<-.fontColor;
               backgroundColor => <-.<-.bgColor;
            };
            fontAttributes {
               height = 14;
               weight = "bold";
            };
            height => 20;
         };
         UIlabel Units_Label<NEx=165.,NEy=132.> {
            y = 1;
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
            width = 140;
            height => 20;
         };
         UIlabel Scale_Label<NEx=264.,NEy=264.> {
            x = 4;
            y = 0;
            parent => <-.Scale_Frame;
            message<NEportLevels={2,0}> => <-.Scale_Frame.message;
            label<NEportLevels={1,0}> => "Set Sphere Size";
            height => 20;
            width = 320;
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
               y = 20;
               height = 33;
               visible<NEportLevels={1,0}>;
               color {
                  foregroundColor = "black";
               };
               parent<NEportLevels={3,0}> => <-.<-.Scale_Frame;
               message<NEportLevels={3,0}> => <-.<-.Scale_Frame.message;
               min<NEportLevels={2,0}> => <-.Min;
               max<NEportLevels={2,0}> => <-.Max;
               value<NEportLevels={2,4}> = 0.82;
               decimalPoints<NEportLevels={2,0}> => <-.Decimal_Points;
               title<NEportLevels={1,0}> => ;
               width = 223;
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
               parent<NEportLevels={3,0}> => <-.<-.Scale_Frame;
               message<NEportLevels={3,0}> => <-.<-.Scale_Frame.message;
               min<NEportLevels={2,0}> => <-.Min;
               max<NEportLevels={2,0}> => <-.Max;
               value<NEdisplayMode="NEopened"> => <-.UIslider.value;
               decimalPoints<NEportLevels={2,0},NEdisplayMode="open"> => <-.Decimal_Points;
               width = 88;
            };
         };
         UIlist File_Type<NEx=11.,NEy=165.,NEwidth=506.,NEheight=220.> {
            x = 4;
            y = 20;
            width = 170;
            height => 215;
            active = 1;
            color {
               foregroundColor = "black";
            };
            parent => <-.File_Type_Frame;
            message<NEportLevels={2,0}> => <-.File_Type_Frame.message;
            selectedItem<NEportLevels={0,3}> = 3;
            stringdims => 14;
            strings = {
               "Set PlainCoord","Set Coordinate","Set Cube","Set pdb",
               "Set PGamess","Set Gamess Cube","Set Dcar","Set DMOL_XYZ",
               "Set Mopac ESP","Set Mopac XYZ","Set mol2","Set mol","Set Chem3D",
               "Set mm3"
            };
            selectedText = "Set pdb";
         };
         UIdynamic_toggle Show_Hydrogens<NEx=88.,NEy=297.> {
            y = 28;
            width = 125;
            color {
               foregroundColor = "black";
            };
            parent => <-.Show_Frame;
            message = "Toggle to diplay hydrogen atoms";
            set<NEportLevels={0,3}> = 1;
            x = 2;
         };
         UIdynamic_toggle Show_Atom_Names<NEx=88.,NEy=330.> {
            y = 55;
            width = 125;
            color {
               foregroundColor = "black";
            };
            parent => <-.Show_Frame;
            message = "Toggle to display atom names";
            set<NEportLevels={0,3}>;
            x = 2;
         };
         UIdynamic_toggle Show_Residue_Names<NEx=88.,NEy=363.> {
            y = 85;
            width = 158;
            visible<NEdisplayMode="open">;
            color {
               foregroundColor = "black";
            };
            parent => <-.Show_Frame;
            message = "Toggle to display residue names";
            set<NEportLevels={0,3}>;
            x = 2;
         };
         UIlist Representation_List<NEx=484.,NEy=165.> {
            y = 20;
            width = 154;
            height => 90;
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
            y = 0;
            width = 160;
            parent => <-.Representation_Frame;
            message<NEportLevels={2,0}> => <-.Representation_Frame.message;
            label<NEportLevels={1,0}> => "Select Representation";
            color {
               foregroundColor => <-.<-.fontColor;
               backgroundColor => <-.<-.bgColor;
            };
            fontAttributes {
               height = 14;
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
                  x => 0;
                  width = 135;
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
                  y = 282;
                  color {
                     foregroundColor = "cyan";
                     backgroundColor = "white";
                  };
                  message<NEportLevels={2,0}> => <-.UIpanel.message;
                  height = 35;
                  rows = 1;
                  resizeToText = 0;
                  parent => <-.<-.<-.Molecular_Display_UI;
                  fontAttributes {
                     weight = "bold";
                  };
               };
               File_Name_Label<NEx=231.,NEy=165.> {
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
            width = 140;
            height => 46;
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
            y = 30;
            width<NEdisplayMode="open"> = 135;
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
            y = 41;
            width = 180;
            height = 239;
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
            y = 43;
            width = 140;
            height = 88;
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
            color {
               foregroundColor = "Red";
               backgroundColor = "Grey";
            };
            alignment = "center";
         };
         UIframeEtchedIn File_Select_Frame<NEx=319.,NEy=99.,NEwidth=484.,NEheight=231.> {
            x = 190;
            y = 136;
            width = 140;
            clientHeight = 91;
            color {
               foregroundColor = "blue";
               backgroundColor = "dark gray";
            };
            parent => <-.Molecular_Display_UI;
            message<NEportLevels={0,2}> = "Select a data file";
            shadowThickness = 8;
            height = 95;
         };
         UIframeEtchedOut UI_Label_Frame<NEx=330.,NEy=22.> {
            x = 7;
            y = 0;
            width = 318;
            height = 38;
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
            y = 321;
            width = 160;
            height = 119;
            color {
               foregroundColor = "blue";
               backgroundColor = "dark gray";
            };
            parent => <-.Molecular_Display_UI;
            message<NEportLevels={0,2}> = "Select representation for the display";
            shadowThickness = 8;
         };
         UIframeEtchedIn Show_Frame<NEx=88.,NEy=220.> {
            x = 169;
            y = 321;
            width = 160;
            height = 119;
            color {
               foregroundColor = "blue";
               backgroundColor = "dark gray";
            };
            parent => <-.Molecular_Display_UI;
            message<NEportLevels={0,2}> = "Select display options";
            shadowThickness = 8;
         };
         UIlabel Display_Options<NEx=88.,NEy=253.> {
            y = 0;
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
            width = 155;
            alignment = "left";
         };
         UIframeEtchedIn Scale_Frame<NEx=264.,NEy=231.> {
            x = 4;
            y = 445;
            width = 325;
            height = 65;
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
            height = 700;
            color<NEdisplayMode="open"> {
               backgroundColor = "dark gray";
            };
         };
         UIbutton ReadFile_Toggle<NEx=682.,NEy=143.,NEwidth=286.,NEheight=220.> {
            label => "Start";
            y = 233;
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
            width = 320;
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
               height = 35;
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
               width = 89;
            };
         };
         UIframeEtchedIn HScale_Frame<NEx=572.,NEy=231.> {
            x = 4;
            y = 584;
            width = 325;
            height = 65;
            color {
               foregroundColor = "blue";
               backgroundColor = "dark gray";
            };
            parent => <-.Molecular_Display_UI;
            message<NEportLevels={0,2}> = "Select sphere scale factor";
            shadowThickness = 8;
         };
         UIlabel BScale_Label<NEx=418.,NEy=264.> {
            x = 2;
            y = 3;
            parent => <-.BScale_Frame;
            message<NEportLevels={2,0}> => <-.BScale_Frame.message;
            label<NEportLevels={1,0}> => "Set Bond Scale Factor";
            width = 320;
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
               y = 20;
               visible<NEportLevels={1,0}>;
               color {
                  foregroundColor = "black";
               };
               parent<NEportLevels={3,0}> => <-.<-.BScale_Frame;
               message<NEportLevels={3,0}> => <-.<-.BScale_Frame.message;
               min<NEportLevels={2,0}> => <-.Min;
               max<NEportLevels={2,0}> => <-.Max;
               value<NEportLevels={2,4}> = 0.76;
               decimalPoints<NEportLevels={2,0}> => <-.Decimal_Points;
               title<NEportLevels={1,0}> => ;
               width = 226;
            };
            int Decimal_Points<NEportLevels=1,NEx=11.,NEy=231.> = 2;
            float Min<NEportLevels=1,NEx=0.,NEy=143.> = 0.;
            float Max<NEportLevels=1,NEx=242.,NEy=143.> = 40.;
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
               width = 90;
            };
         };
         UIframeEtchedIn BScale_Frame<NEx=418.,NEy=231.> {
            x = 4;
            y = 515;
            width = 325;
            height = 63;
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
            set<NEdisplayMode="open"> = 1;
            x = 190;
            y = 260;
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
      };
