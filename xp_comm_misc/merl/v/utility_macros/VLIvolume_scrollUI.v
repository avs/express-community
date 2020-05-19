macro VLIvolume_scrollUI {

   //*************************************************************************

   link parent<NEportLevels={2,1}>;
   int  x<NEportLevels={2,1}> = 0;
   int  y<NEportLevels={2,1}> = 0;
   VLIVolumeInfo &VLIvol_info<NEportLevels={2,1}>;

   //*************************************************************************

   UIscrolledWindow UIscrolledWindow {
      parent => <-.parent;
      x => <-.x;
      y => <-.y;
      width => (parent.width - 5);
      height => (parent.height - 5);
#ifdef MSDOS      
      virtualWidth = 220;
#else
      virtualWidth = 280;
#endif
      virtualHeight = 985;
   };

   //*************************************************************************

   GMOD.copy_on_change UpdateLabel {
      input+nres => VLIvol_info.volume.node_data[0].labels;
      on_inst = 0;
   };
   UIoption Data_Options {
     label => UpdateLabel.output;
   };
   UIoptionMenu Data_Component {
      parent => <-.UIscrolledWindow;
      x = 10;
      y = 8;
      width = 200;
      cmdList => {Data_Options};
      selectedItem => VLIvol_info.data_component;
   };

   //*************************************************************************

   macro Blend {
     VLIVolumeInfo &VLIvol_info<NEportLevels={2,1}> => <-.VLIvol_info;
     link parent<NEportLevels={2,1}> => <-.UIscrolledWindow;

     UIframe UIframe_blend {
       y => <-.<-.Data_Component.y + <-.<-.Data_Component.height;
       width => parent.virtualWidth;
       height => Texture_Type.y + Texture_Type.height + 5;
       parent => <-.parent;
     };

     UItoggle Correct_opacity {
       parent => <-.UIframe_blend;
       x = 5;
       y = 5;
       width => parent.clientWidth;
       label => "Correct Opacity";
       set => VLIvol_info.correct_opacity;
     };
     UItoggle Gradient_diffuse_illum_mod {
       parent => <-.UIframe_blend;
       x = 5;
       y => Correct_opacity.y + Correct_opacity.height;
       width => parent.clientWidth;
       label => "Gradient diffuse illumination modulation";
       set => VLIvol_info.grad_diffuse_illum_mod;
     };
     UItoggle Gradient_specular_illum_mod {
       parent => <-.UIframe_blend;
       x = 5;
       y => Gradient_diffuse_illum_mod.y + Gradient_diffuse_illum_mod.height;
       width => parent.clientWidth;
       label => "Gradient specular illumination modulation";
       set => VLIvol_info.grad_specular_illum_mod;
     };
     UItoggle Gradient_opacity_mod {
       parent => <-.UIframe_blend;
       x = 5;
       y => Gradient_specular_illum_mod.y + Gradient_specular_illum_mod.height;
       width => parent.clientWidth;
       label => "Gradient opacity modulation";
       set => VLIvol_info.grad_opacity_mod;
     };

     string Blend_Types[] = { "FTB", "MIP", "MinIP", "WGTSUM" };
     UIoption Blend_Options[array_size(Blend_Types)] {
       label => Blend_Types[index_of(Blend_Options)];
     };
     UIoptionMenu Blend_Mode {
       x = 5;
       y => Gradient_opacity_mod.y + Gradient_opacity_mod.height + 10;
       parent => <-.UIframe_blend;
       cmdList => <-.Blend_Options;
       selectedItem => VLIvol_info.blend_mode;
     };
     
     string Interp_Types[] = {  "Point" , "Bilinear" };
     UIoption Interp_Options[array_size(Interp_Types)] {
       label => Interp_Types[index_of(Interp_Options)];
     };
     UIoptionMenu Texture_Interp {
       x => <-.Blend_Mode.x;
       y => (<-.Blend_Mode.y + <-.Blend_Mode.height);
       parent => <-.UIframe_blend;
       cmdList => <-.Interp_Options;
       selectedItem => VLIvol_info.texture_interp;
     };
     
     string Texture_Types[] = { "Unfiltered", "MipMap" };
     UIoption Type_Options[array_size(Texture_Types)] {
       label => Texture_Types[index_of(Type_Options)];
     };
     UIoptionMenu Texture_Type {
       parent => <-.UIframe_blend;
       x => <-.Texture_Interp.x;
       y => (<-.Texture_Interp.y + <-.Texture_Interp.height);
       cmdList => <-.Type_Options;
       selectedItem => VLIvol_info.texture_type;
     };
   };

   //*************************************************************************

   macro Slice {
     VLIVolumeInfo &VLIvol_info<NEportLevels={2,1}> => <-.VLIvol_info;
     link parent<NEportLevels={2,1}> => <-.UIscrolledWindow;

     int slice_connected<NEportLevels={0,1}> 
	=> is_valid(VLIvol_info.slice_plane);
     UIframe UIframe_slice {
       parent => <-.parent;
       y => <-.<-.Blend.UIframe_blend.y + 
            <-.<-.Blend.UIframe_blend.height + 5;
       width => parent.virtualWidth;
       height => Slice_flags.y + Slice_flags.height + 5;
       visible => <-.slice_connected;
     };

     string Slice_Types[] = { "Disable", "Inside", "Outside" };
     UIoption Slice_Options[array_size(Slice_Types)] {
       label => Slice_Types[index_of(Slice_Options)];
     };
     UIoptionMenu Slice_flags {
       parent => <-.UIframe_slice;
       x = 5;
       y = 40;
       cmdList => <-.Slice_Options;
       selectedItem => VLIvol_info.slice_flags;
     };

#ifdef MSDOS
       int field_width = 40;
       int height = 20;
#else
       int field_width = 50;
       int height = 30;
#endif

     UIlabel Slice_offset {
       parent => <-.UIframe_slice;
       x = 5;
       y = 10;
#ifdef MSDOS
       width = 55;
#else
       width = 70;
#endif       
       height => <-.height;
       alignment = "left";
     };
     UIfield offset_field {
       parent => <-.UIframe_slice;
       x => (<-.Slice_offset.x + <-.Slice_offset.width);
       y => (<-.Slice_offset.y - 3);
       width => <-.field_width;
       height => <-.height;
       value => VLIvol_info.slice_offset;
       nullString = "?";
     };
     UIlabel Slice_falloff {
       parent => <-.UIframe_slice;
       x => offset_field.x + offset_field.width;
       y = 10;
#ifdef MSDOS
       width = 65;
#else
       width = 80;
#endif       
       height => <-.height;
     };
     UIfield Falloff_field {
       parent => <-.UIframe_slice;
       x => (<-.Slice_falloff.x + <-.Slice_falloff.width);
       y => (<-.Slice_falloff.y - 3);
       width => <-.field_width;
       height => <-.height;
       value => VLIvol_info.slice_falloff;
       nullString = "?";
     };
   };

   //*************************************************************************

   macro Crop {
     VLIVolumeInfo &VLIvol_info<NEportLevels={2,1}> => <-.VLIvol_info;
     link parent<NEportLevels={2,1}> => <-.UIscrolledWindow;

     int crop_connected<NEportLevels={1,1}>
        => is_valid(VLIvol_info.crop_bounds);
     UItoggle Crop_Enable {
       x = 5;
       y = 5;
       parent => <-.UIframe_crop;
       set => VLIvol_info.crop_bounds.crop_enabled;
     };
     UIframe UIframe_crop {
       y => <-.<-.Blend.UIframe_blend.y + 
            <-.<-.Blend.UIframe_blend.height + 5 +
            (<-.<-.Slice.UIframe_slice.visible * 
             (<-.<-.Slice.UIframe_slice.height + 5));
       width => parent.virtualWidth;
       height => <-.UIframe_terms.y + 
                 (<-.UIframe_terms.visible * <-.UIframe_terms.height) + 5;
       visible => <-.crop_connected;
       parent => <-.parent;
     };
     UIslider Min_x {
       x = 5;
       y => ((<-.Crop_Enable.y + <-.Crop_Enable.height) + 5);
       width => parent.clientWidth/2 - .x - 10;
       parent => <-.UIframe_crop;
       min => 0;
       max => Max_x.value;
       value => VLIvol_info.crop_bounds.min_x;
       mode = "integer";
       immediateMode = 1;
     };
     UIslider Min_y {
       x => <-.Min_x.x;
       y => (<-.Min_x.y + <-.Min_x.height);
       width => <-.Min_x.width;
       parent => <-.UIframe_crop;
       min => 0;
       max => Max_y.value;
       value => VLIvol_info.crop_bounds.min_y;
       mode = "integer";
       immediateMode = 1;
     };
     UIslider Min_z {
       x => <-.Min_x.x;
       y => (<-.Min_y.y + <-.Min_y.height);
       width => <-.Min_x.width;
       parent => <-.UIframe_crop;
       min => 0;
       max => Max_z.value;
       value => VLIvol_info.crop_bounds.min_z;
       mode = "integer";
       immediateMode = 1;
     };
     UIslider Max_x {
       x => ((<-.Min_x.x + <-.Min_x.width) + 10);
       y => <-.Min_x.y;
       width => <-.Min_x.width;
       parent => <-.UIframe_crop;
       min => Min_x.value;
       max => (VLIvol_info.volume.dims[0] - 1);
       value => VLIvol_info.crop_bounds.max_x;
       mode = "integer";
       immediateMode = 1;
     };
     UIslider Max_y {
       x => <-.Max_x.x;
       y => (<-.Min_x.y + <-.Min_x.height);
       width => <-.Min_x.width;
       parent => <-.UIframe_crop;
       min => Min_y.value;
       max => (VLIvol_info.volume.dims[1] - 1);
       value => VLIvol_info.crop_bounds.max_y;
       mode = "integer";
       immediateMode = 1;
     };
     UIslider Max_z {
       x => <-.Max_x.x;
       y => (<-.Min_y.y + <-.Min_y.height);
       width => <-.Min_x.width;
       parent => <-.UIframe_crop;
       min => Min_z.value;
       max => (VLIvol_info.volume.dims[2] - 1);
       value => VLIvol_info.crop_bounds.max_z;
       mode = "integer";
       immediateMode = 1;
     };

     string Crop_Types[] = { "Sub_Volume",
                             "3D Cross", "3D Cross Inverted",
                             "3D Fence", "3D Fence Inverted",
			     "User Defined" };
     UIoption Crop_Options[array_size(Crop_Types)] {
       label => Crop_Types[index_of(Crop_Options)];
     };
     UIoptionMenu Crop_Flags {
       x => (<-.Min_z.x + 10);
       y => ((<-.Min_z.y + <-.Min_z.height) + 15);
       width = 180;
       parent => <-.UIframe_crop;
       cmdList => <-.Crop_Options;
       selectedItem => VLIvol_info.crop_bounds.predefined;
     };

     UIframe UIframe_terms {
       x = 0;
       y => (<-.Crop_Flags.y + <-.Crop_Flags.height);
       width => parent.clientWidth;
       height => <-.OR_Select.y + <-.OR_Select.height + 5;
       visible => switch(is_valid(<-.VLIvol_info.crop_bounds.predefined) + 1,
                              0, (<-.VLIvol_info.crop_bounds.predefined == 5));
       parent => <-.UIframe_crop;
     };
     group Term_0 {
       link term_link<NEportLevels={2,1}> => <-.UIframe_terms;
       UIlabel Term_label {
         x = 15;
         y = 5;
         width = 50;
         parent => <-.term_link;
         label => name_of(<-.<-);
       };
       UItoggle En_x {
         x => <-.Term_label.x;
         y => ((<-.Term_label.y + <-.Term_label.height) + 5);
         width = 60;
         parent => <-.term_link;
         set => <-.<-.VLIvol_info.crop_bounds.term0.en_x;
       };
       UItoggle En_y {
         x => <-.Term_label.x;
         y => ((<-.En_x.y + <-.En_x.height) + 5);
         width = 60;
         parent => <-.term_link;
         set => <-.<-.VLIvol_info.crop_bounds.term0.en_y;
       };
       UItoggle En_z {
         x => <-.Term_label.x;
         y => ((<-.En_y.y + <-.En_y.height) + 5);
         width = 60;
         parent => <-.term_link;
         set => <-.<-.VLIvol_info.crop_bounds.term0.en_z;
       };
       UItoggle Inv {
         x => <-.Term_label.x;
         y => ((<-.En_z.y + <-.En_z.height) + 5);
         width => <-.En_x.width;
         parent => <-.term_link;
         set => <-.<-.VLIvol_info.crop_bounds.term0.inv;
       };
     };
     group Term_1 {
       link term_link<NEportLevels={2,1}> => <-.UIframe_terms;
       UIlabel Term_label {
         x => ((<-.<-.Term_0.Term_label.x + <-.<-.Term_0.Term_label.width)+30);
         y => <-.<-.Term_0.Term_label.y;
         width = 50;
         parent => <-.term_link;
         label => name_of(<-.<-);
       };
       UItoggle En_x {
         x => <-.Term_label.x;
         y => ((<-.Term_label.y + <-.Term_label.height) + 5);
         width = 60;
         parent => <-.term_link;
         set => <-.<-.VLIvol_info.crop_bounds.term1.en_x;
       };
       UItoggle En_y {
         x => <-.Term_label.x;
         y => ((<-.En_x.y + <-.En_x.height) + 5);
         width = 60;
         parent => <-.term_link;
         set => <-.<-.VLIvol_info.crop_bounds.term1.en_y;
       };
       UItoggle En_z {
         x => <-.Term_label.x;
         y => ((<-.En_y.y + <-.En_y.height) + 5);
         width = 60;
         parent => <-.term_link;
         set => <-.<-.VLIvol_info.crop_bounds.term1.en_z;
       };
       UItoggle Inv {
         x => <-.Term_label.x;
         y => ((<-.En_z.y + <-.En_z.height) + 5);
         width => <-.En_x.width;
         parent => <-.term_link;
         set => <-.<-.VLIvol_info.crop_bounds.term1.inv;
       };
     };
     group Term_2 {
       link term_link<NEportLevels={2,1}> => <-.UIframe_terms;
       UIlabel Term_label {
         x => ((<-.<-.Term_1.Term_label.x + <-.<-.Term_1.Term_label.width)+30);
         y => <-.<-.Term_0.Term_label.y;
         width = 50;
         parent => <-.term_link;
         label => name_of(<-.<-);
       };
       UItoggle En_x {
         x => <-.Term_label.x;
         y => ((<-.Term_label.y + <-.Term_label.height) + 5);
         width = 60;
         parent => <-.term_link;
         set => <-.<-.VLIvol_info.crop_bounds.term2.en_x;
       };
       UItoggle En_y {
         x => <-.Term_label.x;
         y => ((<-.En_x.y + <-.En_x.height) + 5);
         width = 60;
         parent => <-.term_link;
         set => <-.<-.VLIvol_info.crop_bounds.term2.en_y;
       };
       UItoggle En_z {
         x => <-.Term_label.x;
         y => ((<-.En_y.y + <-.En_y.height) + 5);
         width = 60;
         parent => <-.term_link;
         set => <-.<-.VLIvol_info.crop_bounds.term2.en_z;
       };
       UItoggle Inv {
         x => <-.Term_label.x;
         y => ((<-.En_z.y + <-.En_z.height) + 5);
         width => <-.En_x.width;
         parent => <-.term_link;
         set => <-.<-.VLIvol_info.crop_bounds.term2.inv;
       };
     };

     UItoggle OR_Select {
       x => <-.Min_x.x;
       y => ((<-.Term_0.Inv.y + <-.Term_0.Inv.height) + 15);
       parent => <-.UIframe_terms;
       set => VLIvol_info.crop_bounds.combine;
     };
     UItoggle Invert_Output {
       x => ((<-.OR_Select.x + <-.OR_Select.width) + 20);
       y => <-.OR_Select.y;
       parent => <-.UIframe_terms;
       width = 110;
       set => VLIvol_info.crop_bounds.invert_result;
     };
   };
   
   //*************************************************************************

   macro Fields {
     VLIVolumeInfo &VLIvol_info<NEportLevels={2,1}> => <-.VLIvol_info;
     link parent<NEportLevels={2,1}> => <-.UIscrolledWindow;

     UIframe UIframe_fields {
       y => <-.<-.Blend.UIframe_blend.y + 
            <-.<-.Blend.UIframe_blend.height + 5 +
            (<-.<-.Slice.UIframe_slice.visible * 
             (<-.<-.Slice.UIframe_slice.height + 5)) + 
            (<-.<-.Crop.UIframe_crop.visible * 
             (<-.<-.Crop.UIframe_crop.height + 5));
       width => parent.virtualWidth;
       height => SuperSample_Space.y + SuperSample_Space.height + 5;
       parent => <-.parent;
     };
     
     MERL.Utility_macros.XYZ_UIfields Shear {
       parent => <-.UIframe_fields;
       x_top_left = 0;
       y_top_left = 10;
#ifdef MSDOS
       field_width = 40;
#else
       field_width = 50;
       field_height = 30;
       label_width = 60;
       label_height = 30;
#endif
       x_value => VLIvol_info.x_shear;
       y_value => VLIvol_info.y_shear;
       z_value => VLIvol_info.z_shear;
     };
     
     MERL.Utility_macros.XYZ_UIfields SuperSample {
       parent => <-.UIframe_fields;
       x_top_left => Shear.x_bot_right + 5;
       y_top_left => Shear.y_top_left;
#ifdef MSDOS
       field_width = 40;
       label_width = 80;
#else
       field_width = 50;
       field_height = 30;
       label_width = 100;
       label_height = 30;
#endif
       x_value => VLIvol_info.x_supersample;
       y_value => VLIvol_info.y_supersample;
       z_value => VLIvol_info.z_supersample;
     };

     string Accum_Types[] = { "Grow base plane", "Blend base plane" };
     UIoption Accum_Options[array_size(Accum_Types)] {
       label => Accum_Types[index_of(Accum_Options)];
     };
     UIoptionMenu Base_Plane_Accum {
       x = 5;
       y => <-.SuperSample.y_bot_right + 5;
       parent => <-.UIframe_fields;
       cmdList => <-.Accum_Options;
       selectedItem => VLIvol_info.base_plane_accum;
     };
     string Space_Types[] = { "Camera Space", "Object Space" };
     UIoption Space_Options[array_size(Space_Types)] {
       label => Space_Types[index_of(Space_Options)];
     };
     UIoptionMenu SuperSample_Space {
       x = 5;
       y => <-.Base_Plane_Accum.y + <-.Base_Plane_Accum.height;
       parent => <-.UIframe_fields;
       cmdList => <-.Space_Options;
       selectedItem => VLIvol_info.supersample_space;
     };
    
   };

   //*************************************************************************

   macro Config_Info {
     VLIVolumeInfo &VLIvol_info<NEportLevels={2,1}> => <-.VLIvol_info;
     link parent<NEportLevels={2,1}> => <-.UIscrolledWindow;
     UItoggle Show_Config_Info {
       parent => <-.parent;
       x = 5;
       y => <-.<-.Fields.UIframe_fields.y + 
            <-.<-.Fields.UIframe_fields.height + 5;
     };
     UIframe UIframe_config {
       parent => <-.parent;
       visible => <-.Show_Config_Info.set;
       y => ((<-.Show_Config_Info.y + <-.Show_Config_Info.height) + 5);
       width => parent.virtualWidth;
       height => Available_memory.y + Available_memory.height + 5;
     };
     UIlabel Version {
       parent => <-.UIframe_config;
       x = 5;
       y = 5;
       width = 100;
       alignment = "left";
     };
     UIlabel Version_2 {
       parent => <-.UIframe_config;
       x => (<-.Version.x + <-.Version.width);
       y => <-.Version.y;
       width = 60;
       label => str_format(" %d.%d",VLIvol_info.config_info.major_version,
                           VLIvol_info.config_info.minor_version);
       alignment = "left";
     };
     UIlabel VLI_Version {
       parent => <-.UIframe_config;
       x => <-.Version.x;
       y => <-.Version.y + <-.Version.height + 2;
       width = 100;
       alignment = "left";
     };
     UIlabel VLI_Vers_2 {
       parent => <-.UIframe_config;
       x => (<-.VLI_Version.x + <-.VLI_Version.width);
       y => <-.VLI_Version.y;
       width = 60;
       label => str_format(" %d.%d",VLIvol_info.config_info.vli_major_version,
                           VLIvol_info.config_info.vli_minor_version);
       alignment = "left";
     };
     UIslider Board {
       parent => <-.UIframe_config;
       x => <-.Version.x;
       y => ((<-.VLI_Version.y + <-.VLI_Version.height) + 7);
       width = 75;
       min = 0.;
       max => VLIvol_info.config_info.board_count - 1;
       value = 0.;
       mode = "integer";
       decimalPoints = 0;
       increment = 1.;
     };
     UIlabel Available_memory {
       parent => <-.UIframe_config;
       x => <-.Version.x;
       y => <-.Board.y + <-.Board.height + 5;
       width = 110;
       alignment = "left";
     };
     UIlabel mem_field {
       parent => <-.UIframe_config;
       x => ((<-.Available_memory.x + <-.Available_memory.width) + 5);
       y => <-.Available_memory.y;
       width = 70;
       label => VLIvol_info.config_info.board_info[<-.Board.value].
	        available_memory;
       alignment = "left";
     };
   };
};
