
flibrary ReadDXFUnsupported <compile_subs=0> {

   XP_COMM_PROJ.ReadDXF.ReadDXFMacs.readDXF readAdjustDXF {
      ReadDXFFunc {
         GeomGroupObject {
            Top {
               name<NEportLevels={4,0}> => <-.<-.<-.ReadDXF_UI.object_name;
            };
         };
      };


      ReadDXF_UI {
         panel{
            //title<NEportLevels={2,0}> => <-.mod_name;
            title => "DXF File Read & Transform Module";
         };

         //string+Port mod_name => name_of(<-.<-,1);

         UIbutton transformButton {
            parent => <-.panel;
            label = "Transform...";
            do+OPort3;
            x => <-.fillPolyToggle.x;
            y => <-.layersOptionBox.y + <-.layersOptionBox.height + 20;
         };
         UIlabel objectNameLabel {
            parent => <-.panel;
            label => "Object Name";
            x => <-.fillPolyToggle.x;
            y => <-.transformButton.y + <-.transformButton.height + 5;
            width => (parent.width / 3) - 2;
            alignment = 0;
         };
         UItext objectNameText {
            parent => <-.panel;
            x => <-.objectNameLabel.x + <-.objectNameLabel.width + 2;
            y => <-.objectNameLabel.y;
            width => (2 * parent.width / 3) - 2;
         };

         olink object_name => objectNameText.text;
      };

      XformEditor XformEditor {
         obj_in => <-.ReadDXFFunc.out_geom;
         vis => <-.ReadDXF_UI.transformButton.do;
      };

   };



   macro Read_DXF_Vertical {

      macro read_dxf_ui<NEx=11.,NEy=11.,NEwidth=253.,NEheight=440.> {
         UIfileSB file_browser<NEx=352.,NEy=22.,NEwidth=242.,NEheight=451.> {
            visible => <-.UIbutton.do;
            title = "Read GEOSOFT Filename";
            searchPattern = "*.dxf";
            GMOD.copy_on_change copy_on_change;
         };
         olink filename<NEx=297.,NEy=154.,NEportLevels={1,2}> => .file_browser.filename;
         UImod_panel UImodPanel<NEx=77.,NEy=88.,NEwidth=418.,NEheight=264.> {
            visible<NEportLevels={2,2}>;
            title<NEdisplayMode="open",NEportLevels={2,0}> => <-.string;
            width = 300;
         };
         string string<NEportLevels=1,NEx=22.,NEy=33.> => name_of(<-.<-);
         Panels.UIpanel UIpanel<NEx=77.,NEy=176.> {
            parent => <-.UImodPanel;
            y = 0;
            width = 300;
            height = 550;
         };
         Controls.UIlabel UIlabel<NEx=77.,NEy=286.> {
            parent => <-.UIpanel;
            label => "Filename:";
            y = 40;
            alignment = 0;
         };
         Controls.UIbutton UIbutton<NEx=77.,NEy=242.> {
            parent => <-.UIpanel;
            label => "Show file browser";
            y = 10;
            width = 250;
         };
         Controls.UIlabel UIlabel#1<NEx=77.,NEy=330.> {
            parent => <-.UIpanel;
            label => <-.filename;
            y = 70;
            width = 300;
            alignment = 0;
         };
         Controls.UIlabel UIlabel#2<NEx=77.,NEy=374.> {
            parent => <-.UIpanel;
            label => "Vertical offset";
            y = 124;
            alignment = 0;
         };
         Controls.UIfield UIfield<NEx=77.,NEy=418.> {
            parent => <-.UIpanel;
            value<NEportLevels={2,2}>;
            y = 148;
         };
         link vertical_offset<NEportLevels={1,2},NEx=153.,NEy=489.> => .UIfield.value;
         Controls.UIlabel UIlabel#3<NEx=473.,NEy=264.> {
            parent => <-.UIpanel;
            label => "First axis offset";
            y = 204;
            alignment = 0;
         };
         Controls.UIlabel UIlabel#4<NEx=473.,NEy=385.> {
            parent => <-.UIpanel;
            label => "Second axis offset";
            y = 294;
            alignment = 0;
            width = 130;
         };
         Controls.UIfield UIfield#1<NEx=473.,NEy=319.> {
            parent => <-.UIpanel;
            value<NEportLevels={2,2}>;
            y = 230;
         };
         Controls.UIfield UIfield#2<NEx=473.,NEy=429.> {
            parent => <-.UIpanel;
            value<NEportLevels={2,2}>;
            y = 318;
         };
         link first_offset<NEportLevels={1,2},NEx=292.,NEy=445.> => .UIfield#1.value;
         link second_offset<NEportLevels={1,2},NEx=548.,NEy=477.> => .UIfield#2.value;
         link selected<NEportLevels={1,2},NEx=660.,NEy=198.> => .UIradioBox.selectedItem;
         Controls.UIradioBox UIradioBox<NEx=627.,NEy=132.> {
            parent => <-.UIpanel;
            cmdList => {<-.UIoption,
               <-.UIoption#1};
            selectedItem = 0;
            y = 388;
            width = 200;
         };
         Commands.UIoption UIoption<NEx=627.,NEy=55.> {
            label => "Axis 1 (X) is depth";
         };
         Commands.UIoption UIoption#1<NEx=627.,NEy=99.> {
            label => "Axis 2 (Y) is depth";
         };
         Controls.UIlabel UIlabel#5<NEx=627.,NEy=264.> {
            parent => <-.UIpanel;
            label => "Rotation";
            y = 450;
            alignment = 0;
         };
         Controls.UIfield UIfield#3<NEx=627.,NEy=319.> {
            parent => <-.UIpanel;
            value<NEportLevels={2,2}>;
            y = 474;
         };
         link rotation<NEportLevels={1,2},NEx=682.,NEy=418.> => .UIfield#3.value;
      };


      DefaultXform Xfm<NEx=594.,NEy=66.>;

      Editors.GDxform_edit GDxform_edit<NEx=583.,NEy=319.> {
         shell_vis<NEportLevels={1,0}>;
         xform => <-.Xfm;
         z_trans<NEportLevels={2,0}> => <-.read_dxf_ui.vertical_offset;
         absolute = 1;
         reset<NEportLevels={2,0}> => <-.trigger;
         x_trans<NEportLevels={2,0}> => <-.read_dxf_ui.first_offset;
         y_trans<NEportLevels={2,0}> => <-.read_dxf_ui.second_offset;
         x_rot<NEportLevels={1,0}> = 0.;
         y_rot<NEportLevels={1,0}> = 0.;
         z_rot<NEportLevels={2,0}> => <-.read_dxf_ui.rotation;
      };

      macro flip_points<NEx=374.,NEy=44.> {
         int switch<NEportLevels={2,1},NEx=132.,NEy=121.> => <-.read_dxf_ui.selected;
         DefaultXform Flip_xform<NEx=99.,NEy=341.,NEportLevels={0,2}>;
         int trigger<NEportLevels={2,1},NEx=110.,NEy=220.> => <-.trigger;
         Editors.GDxform_edit GDxform_edit<NEx=462.,NEy=275.> {
            shell_vis<NEportLevels={1,0}>;
            xform => <-.Flip_xform;
            absolute = 1;
         //   reset<NEportLevels={2,0}> => <-.trigger;
            x_rot = 90.;
            y_rot = 0.;
            z_rot = 0.;
            x_trans<NEportLevels={2,0}> => (-1 * <-.xmin);
            y_trans<NEportLevels={2,0}> => (-1 * <-.ymin);
            z_trans = 0;
            x_cent = 0;
            y_cent = 0;
            z_cent = 0;
            scale = 1;
         };
         float xyzmin<NEportLevels={2,1},NEx=319.,NEy=88.>[] => <-.ReadDXFCore.out.coordinates.min_vec;
         float xyzmax<NEportLevels={2,1},NEx=359.,NEy=128.>[] => <-.ReadDXFCore.out.coordinates.max_vec;
         float+nres xmin<NEx=350,NEy=100,NEportLevels={0,1}> => xyzmin[0];
         float+nres ymin<NEx=350,NEy=120,NEportLevels={0,1}> => xyzmin[1];
         float+nres zmin<NEx=350,NEy=140,NEportLevels={0,1}> => xyzmin[2];
         float+nres xmax<NEx=390,NEy=100,NEportLevels={0,1}> => xyzmin[0];
         float+nres ymax<NEx=390,NEy=120,NEportLevels={0,1}> => xyzmin[1];
         float+nres zmax<NEx=390,NEy=140,NEportLevels={0,1}> => xyzmin[2];
      };

      int trigger<NEportLevels={0,1},NEx=374.,NEy=319.> => ReadDXFCore.out.nnodes;

      DV.DVconcat_xform DVconcat_xform<NEx=627.,NEy=154.> {
         xform_in1 => <-.flip_points.Flip_xform;
         xform_in2 => <-.Xfm;
      };
      XP_COMM_PROJ.ReadDXF.ReadDXFMods.ReadDXFCore ReadDXFCore<NEx=418.,NEy=209.> {
         file_name => <-.read_dxf_ui.filename;
         out {
            coordinates {
               min_vec<NEportLevels={0,4}>;
               max_vec<NEportLevels={0,4}>;
            };
            &xform<NEportLevels={3,1}> => <-.<-.DVconcat_xform.xform_out;
         };
      };
      GDM.DataObject DataObject<NEx=231.,NEy=231.> {
         in => <-.ReadDXFCore.out;
      };

      olink field<NEx=22.,NEy=242.,NEportLevels={1,2}> => .ReadDXFCore.out;
      olink obj<NEportLevels={1,2},NEx=209.,NEy=363.> => .DataObject.obj;

   };


};

