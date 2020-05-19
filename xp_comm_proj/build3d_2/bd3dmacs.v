
flibrary Build3D_2_Macs<compile_subs=0> {

   macro Build3D_2_UI {
      XP_COMM_PROJ.Build3D_2.Build3D_2_Mods.Build3D_2_Params &params<NEportLevels={2,1}>;
      string info_string = "";

      UImod_panel panel {
         title = "Build3D 2";
         message = "Select Build3D 2 control panel.";
         parent<NEportLevels={4,0}>;
      };


      UIlabel FilenameHeadLabel {
         parent => panel;
         label = "Filename Head";
         alignment = 2;
         y = 0;
         width => parent.width * (1./3.) - 3;
      };

      UItext FilenameHeadText {
         parent => panel;
         text => <-.params.filename_head;
         x => <-.FilenameHeadLabel.x + <-.FilenameHeadLabel.width + 3;
         y => <-.FilenameHeadLabel.y;
         width => parent.width * (2./3.) - 3;
      };

      UIlabel FilenameTailLabel {
         parent => panel;
         label = "Filename Tail";
         alignment = 2;
         y => <-.FilenameHeadText.y + <-.FilenameHeadText.height + 5;
         width => parent.width * (1./3.) - 3;
      };

      UItext FilenameTailText {
         parent => panel;
         text => <-.params.filename_tail;
         x => <-.FilenameTailLabel.x + <-.FilenameTailLabel.width + 3;
         y => <-.FilenameTailLabel.y;
         width => parent.width * (2./3.) - 3;
      };

      UIlabel MinLengthLabel {
         parent => panel;
         label = "Minimum Index Length";
         alignment = 2;
         y => <-.FilenameTailText.y + <-.FilenameTailText.height + 5;
         width => parent.width * (2./3.) - 3;
      };

      UIfield MinLengthField {
         parent => panel;
         min = 0;
         mode = "integer";
         value => <-.params.min_length;
         x => <-.MinLengthLabel.x + <-.MinLengthLabel.width + 3;
         y => <-.MinLengthLabel.y;
         width => parent.width * (1./3.) - 3;
      };


      string+Port formatEg => "%0" + params.min_length + "d";

      string+Port filenameEg => params.filename_head + str_format(.formatEg, 1) + params.filename_tail;



      UIlabel FilenameEgLabel {
         parent => panel;
         label = "Filename Eg.";
         alignment = 2;
         y => <-.MinLengthField.y + <-.MinLengthField.height + 10;
         width => parent.width * (1./3.) - 3;
      };

      UItext FilenameEgText {
         parent => panel;
         text => <-.filenameEg;
         outputOnly = 1;
         x => <-.FilenameEgLabel.x + <-.FilenameEgLabel.width + 3;
         y => <-.FilenameEgLabel.y;
         width => parent.width * (2./3.) - 3;
      };




      UIlabel StartIndexLabel {
         parent => panel;
         label = "Starting Image Index";
         alignment = 2;
         y => <-.FilenameEgText.y + <-.FilenameEgText.height + 25;
         width => parent.width * (2./3.) - 3;
      };

      UIfield StartIndexField {
         parent => panel;
         min = 0;
         mode = "integer";
         value => <-.params.start;
         x => <-.StartIndexLabel.x + <-.StartIndexLabel.width + 3;
         y => <-.StartIndexLabel.y;
         width => parent.width * (1./3.) - 3;
      };

      UIlabel EndIndexLabel {
         parent => panel;
         label = "Ending Image Index";
         alignment = 2;
         y => <-.StartIndexField.y + <-.StartIndexField.height + 3;
         width => parent.width * (2./3.) - 3;
      };

      UIfield EndIndexField {
         parent => panel;
         min = 0;
         value => <-.params.end;
         mode = "integer";
         x => <-.EndIndexLabel.x + <-.EndIndexLabel.width + 3;
         y => <-.EndIndexLabel.y;
         width => parent.width * (1./3.) - 3;
      };


      UIframe format_frame {
         parent => <-.panel;
         visible => <-.panel.visible;
         y => <-.EndIndexField.y + <-.EndIndexField.height + 10;
         width => <-.panel.width;
         height => <-.format_rb.y + <-.format_rb.height + 6;
      };
      UIlabel format_label {
         parent => format_frame;
         label = "File Format";
         x = 4;
         y = 0;
         width => <-.panel.width;
      };
      UIoption format_from {
         label = "(from file)";
         message = "determine format from file";
         active = 1;
      };
      UIoption format_avsx {
         label = "avs .x";
         message = "AVS .x format";
         active = 1;
      };
      UIoption format_bmp {
         label = "BMP";
         message = "Microsoft BMP format";
         active = 1;
      };
      UIoption format_gif {
         label = "GIF";
         message = "Graphics Interchange Format";
         active = 1;
      };
      UIoption format_jpeg {
         label = "JPEG";
         message = "JFIF: JPEG File Interchange Format";
         active = 1;
      };
      UIoption format_pbm {
         label = "PBM";
         message = "Portable Bitmap Utilities";
         active = 1;
      };
      UIoption format_sgi {
         label = "SGI Image";
         message = "SGI Image format";
         active = 1;
      };
      UIoption format_sun {
         label = "Sun Raster";
         message = "Sun Rasterfile format";
         active = 1;
      };
      UIoption format_tiff {
         label = "TIFF";
         message = "Tag Image File Format";
         active = 1;
      };
      UIradioBox format_rb {
         parent => format_frame;
         cmdList => {format_from, format_avsx, format_bmp, format_gif, format_jpeg, format_pbm, format_sgi, format_sun, format_tiff};
         visible => <-.panel.visible;
         selectedItem => <-.params.format;
         x = 4;
         y => <-.format_label.y + <-.format_label.height + 4;
         width => <-.panel.width;
      };



      UIframe out_format_frame {
         parent => <-.panel;
         visible => <-.panel.visible;
         y => <-.format_frame.y + <-.format_frame.height + 10;
         width => <-.panel.width;
         height => <-.out_format_rb.y + <-.out_format_rb.height + 6;
      };
      UIlabel out_format_label {
         parent => out_format_frame;
         label = "Output Format";
         x = 4;
         y = 0;
         width => <-.panel.width;
      };
      UIoption out_format_argb {
         label = "ARGB Byte Data";
         message = "Output 4-byte ARGB vector.";
         active = 1;
      };
      UIoption out_format_alpha {
         label = "Alpha Data";
         message = "Output alpha channel byte data.";
         active = 1;
      };
      UIoption out_format_red {
         label = "Red Data";
         message = "Output red channel byte data.";
         active = 1;
      };
      UIoption out_format_green {
         label = "Green Data";
         message = "Output green channel byte data.";
         active = 1;
      };
      UIoption out_format_blue {
         label = "Blue Data";
         message = "Output blue channel byte data.";
         active = 1;
      };
      UIoption out_format_lumi {
         label = "Luminance Data";
         message = "Output luminance float data.";
         active = 1;
      };
      UIoption out_format_rgb {
         label = "RGB Float Data";
         message = "Output 3-float RGB vector.";
         active = 1;
      };
      UIradioBox out_format_rb {
         parent => out_format_frame;
         cmdList => {out_format_argb, out_format_alpha, out_format_red, out_format_green, out_format_blue, out_format_lumi, out_format_rgb};
         visible => <-.panel.visible;
         selectedItem => <-.params.out_format;
         x = 4;
         y => <-.out_format_label.y + <-.out_format_label.height + 4;
         width => <-.panel.width;
      };



      UItext file_info {
         parent => <-.panel;
         visible => <-.panel.visible;
         y => <-.out_format_frame.y + <-.out_format_frame.height + 10;
         width => <-.panel.width;
         outputOnly = 1;
         text => <-.info_string;
      };
   };



   macro Build3D_2_Func {
      XP_COMM_PROJ.Build3D_2.Build3D_2_Mods.Build3D_2_Params &params<NEportLevels={2,1}>;

      string+Port filename => params.filename_head + "%0" + params.min_length + "d" + params.filename_tail;

      XP_COMM_PROJ.Build3D_2.Build3D_2_Mods.Build3D_2_Core Build3D_2_Core {
      	filename => <-.filename;
         format => <-.params.format;
         start => <-.params.start;
         end => <-.params.end;
         out_format => <-.params.out_format;
      };
      GDM.DataObject DataObject {
         in => <-.Build3D_2_Core.out;
         Obj.name => name_of(<-.<-.<-);
      };

      olink out_fld => .Build3D_2_Core.out;
      olink out_obj => .DataObject.obj;
      olink info    => .Build3D_2_Core.info;
   };



   macro build3d_2 {
      XP_COMM_PROJ.Build3D_2.Build3D_2_Mods.Build3D_2_Params params {
         min_length = 0;
         format = 0;
         out_format = 5;
      };

      Build3D_2_Func Build3D_2_Func {
  			params => <-.params;
      	DataObject.Obj.name => name_of(<-.<-.<-.<-);
      };

      Build3D_2_UI Build3D_2_UI {
   		params => <-.params;
   		info_string => <-.Build3D_2_Func.info;
         panel {
            title => name_of(<-.<-.<-,1);
         };
      };

      olink out_fld => .Build3D_2_Func.out_fld;
      olink out_obj => .Build3D_2_Func.out_obj;
   };




   macro adjust_slice_spacing {
      Mesh_Unif+Node_Data &in_fld<NEportLevels={2,1}>;
      float &slice_position<NEportLevels={2,1}>[];

      int+OPort spacing_axis = 2;

      XP_COMM_PROJ.Build3D_2.Build3D_2_Mods.AdjustSliceSpacingCore AdjustSliceSpacingCore {
         in => <-.in_fld;
         spacing_axis => <-.spacing_axis;
         slice_position => <-.slice_position;
      };
      
      Mesh+Node_Data+OPort2 &out_fld => merge(AdjustSliceSpacingCore.out, in_fld);
   };


   group node_2_color_data {
      ilink in_fld;

      FLD_MAP.node_colors node_colors {
         in_data+nres => <-.in_fld.node_data[0].values;
      };
      FLD_MAP.combine_mesh_data combine_mesh_data {
         in_mesh => <-.in_fld;
         in_nd => <-.node_colors.out;
      };

      olink out_fld => .combine_mesh_data.out;
      olink out_obj => .combine_mesh_data.obj;
   };


   group node_2_argb_data {
      ilink in_fld;

      FLD_MAP.argb_data argb_data {
         in_data+nres => <-.in_fld.node_data[0].values;
      };
      FLD_MAP.combine_mesh_data combine_mesh_data {
         in_mesh => <-.in_fld;
         in_nd => <-.argb_data.out;
      };

      olink out_fld => .combine_mesh_data.out;
      olink out_obj => .combine_mesh_data.obj;
   };

};

