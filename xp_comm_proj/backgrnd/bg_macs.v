
flibrary BackgroundMacs <compile_subs=0> {

   macro BackgroundUI {

      XP_COMM_PROJ.Background.BackgroundMods.BackgroundParams &params<NEportLevels={2,1}>;

      UImod_panel panel {
         title = "Background Generator";
      };
   
      UIlabel BackgroundTitle {
         parent => <-.panel;
         y = 0;
         width => parent.width;
         alignment = "left";
         label => "Background Generator";
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };


      //    Upper Left HSV panel
      //------------------------------------

      UIframe ULframe {
         parent => <-.panel;
         x = 0;
         y => <-.BackgroundTitle.y + <-.BackgroundTitle.height + 10;
         width => (parent.width / 2) - 5;
         height => <-.ULVslider.y + <-.ULVslider.height + 10;
      };
      UIlabel ULtitle {
         parent => <-.ULframe;
         x = 5;
         y = 5;
         width => parent.width - 12;
         alignment = "left";
         label => "Upper Left";
      };
      UIslider ULHslider {
        parent => <-.ULframe;
        min = 0;
        max = 1.0;
        value => <-.params.Upper_Left_Hue;
        x => <-.ULtitle.x;
        y => <-.ULtitle.y + <-.ULtitle.height + 5;
        width => <-.ULtitle.width;
        title = "Hue";
      };
      UIslider ULSslider {
        parent => <-.ULframe;
        min = 0;
        max = 1.0;
        value => <-.params.Upper_Left_Sat;
        x => <-.ULtitle.x;
        y => <-.ULHslider.y + <-.ULHslider.height + 5;
        width => <-.ULtitle.width;
        title = "Saturation";
      };
      UIslider ULVslider {
        parent => <-.ULframe;
        min = 0;
        max = 1.0;
        value => <-.params.Upper_Left_Value;
        x => <-.ULtitle.x;
        y => <-.ULSslider.y + <-.ULSslider.height + 5;
        width => <-.ULtitle.width;
        title = "Value";
      };



      //    Upper Right HSV panel
      //------------------------------------

      UIframe URframe {
         parent => <-.panel;
         x => <-.ULframe.x + <-.ULframe.width + 5;
         y => <-.ULframe.y;
         width => <-.ULframe.width;
         height => <-.ULframe.height;
      };
      UIlabel URtitle {
         parent => <-.URframe;
         x = 5;
         y = 5;
         width => parent.width - 12;
         alignment = "left";
         label => "Upper Right";
      };
      UIslider URHslider {
        parent => <-.URframe;
        min = 0;
        max = 1.0;
        value => <-.params.Upper_Right_Hue;
        x => <-.URtitle.x;
        y => <-.URtitle.y + <-.URtitle.height + 5;
        width => <-.URtitle.width;
        title = "Hue";
      };
      UIslider URSslider {
        parent => <-.URframe;
        min = 0;
        max = 1.0;
        value => <-.params.Upper_Right_Sat;
        x => <-.URtitle.x;
        y => <-.URHslider.y + <-.URHslider.height + 5;
        width => <-.URtitle.width;
        title = "Saturation";
      };
      UIslider URVslider {
        parent => <-.URframe;
        min = 0;
        max = 1.0;
        value => <-.params.Upper_Right_Value;
        x => <-.URtitle.x;
        y => <-.URSslider.y + <-.URSslider.height + 5;
        width => <-.URtitle.width;
        title = "Value";
      };



      //    Lower Left HSV panel
      //------------------------------------

      UIframe LLframe {
         parent => <-.panel;
         x => <-.ULframe.x;
         y => <-.ULframe.y + <-.ULframe.height + 10;
         width => <-.ULframe.width;
         height => <-.ULframe.height;
      };
      UIlabel LLtitle {
         parent => <-.LLframe;
         x = 5;
         y = 5;
         width => parent.width - 12;
         alignment = "left";
         label => "Lower Left";
      };
      UIslider LLHslider {
        parent => <-.LLframe;
        min = 0;
        max = 1.0;
        value => <-.params.Lower_Left_Hue;
        x => <-.LLtitle.x;
        y => <-.LLtitle.y + <-.LLtitle.height + 5;
        width => <-.LLtitle.width;
        title = "Hue";
      };
      UIslider LLSslider {
        parent => <-.LLframe;
        min = 0;
        max = 1.0;
        value => <-.params.Lower_Left_Sat;
        x => <-.LLtitle.x;
        y => <-.LLHslider.y + <-.LLHslider.height + 5;
        width => <-.LLtitle.width;
        title = "Saturation";
      };
      UIslider LLVslider {
        parent => <-.LLframe;
        min = 0;
        max = 1.0;
        value => <-.params.Lower_Left_Value;
        x => <-.LLtitle.x;
        y => <-.LLSslider.y + <-.LLSslider.height + 5;
        width => <-.LLtitle.width;
        title = "Value";
      };


      //    Lower Right HSV panel
      //------------------------------------

      UIframe LRframe {
         parent => <-.panel;
         x => <-.URframe.x;
         y => <-.LLframe.y;
         width => <-.ULframe.width;
         height => <-.ULframe.height;
      };
      UIlabel LRtitle {
         parent => <-.LRframe;
         x = 5;
         y = 5;
         width => parent.width - 12;
         alignment = "left";
         label => "Lower Right";
      };
      UIslider LRHslider {
        parent => <-.LRframe;
        min = 0;
        max = 1.0;
        value => <-.params.Lower_Right_Hue;
        x => <-.LRtitle.x;
        y => <-.LRtitle.y + <-.LRtitle.height + 5;
        width => <-.LRtitle.width;
        title = "Hue";
      };
      UIslider LRSslider {
        parent => <-.LRframe;
        min = 0;
        max = 1.0;
        value => <-.params.Lower_Right_Sat;
        x => <-.LRtitle.x;
        y => <-.LRHslider.y + <-.LRHslider.height + 5;
        width => <-.LRtitle.width;
        title = "Saturation";
      };
      UIslider LRVslider {
        parent => <-.LRframe;
        min = 0;
        max = 1.0;
        value => <-.params.Lower_Right_Value;
        x => <-.LRtitle.x;
        y => <-.LRSslider.y + <-.LRSslider.height + 5;
        width => <-.LRtitle.width;
        title = "Value";
      };



      UIslider Xres_slider {
        parent => <-.panel;
        min = 2;
        max = 1280;
        mode = 1;
        value => <-.params.X_Resolution;
        x = 0;
        y => <-.LLframe.y + <-.LLframe.height + 10;
        title = "X Resolution";
      };

      UIslider Yres_slider {
        parent => <-.panel;
        min = 2;
        max = 1280;
        mode = 1;
        value => <-.params.Y_Resolution;
        x = 0;
        y => <-.Xres_slider.y + <-.Xres_slider.height + 10;
        title = "Y Resolution";
      };

      UItoggle DitherToggle {
        parent => <-.panel;
        set => <-.params.Dither;
        x = 0;
        y => <-.Yres_slider.y + <-.Yres_slider.height + 10;
        label = "Dither";
      };

   };


   macro RGB_BackgroundUI {

      XP_COMM_PROJ.Background.BackgroundMods.RGB_BackgroundParams &params<NEportLevels={2,1}>;

      UImod_panel panel {
         title = "Background Fade";
      };
   
      UIlabel BackgroundTitle {
         parent => <-.panel;
         y = 0;
         width => parent.width;
         alignment = "left";
         label => "Background Fade";
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };


      //    Upper Left RGB panel
      //------------------------------------

      UIframe ULframe {
         parent => <-.panel;
         x = 0;
         y => <-.BackgroundTitle.y + <-.BackgroundTitle.height + 10;
         width => (parent.width / 2) - 5;
         height => <-.ULBslider.y + <-.ULBslider.height + 10;
      };
      UIlabel ULtitle {
         parent => <-.ULframe;
         x = 5;
         y = 5;
         width => parent.width - 12;
         alignment = "left";
         label => "Upper Left";
      };
      UIslider ULRslider {
        parent => <-.ULframe;
        min = 0;
        max = 1.0;
        value => <-.params.Upper_Left_Red;
        x => <-.ULtitle.x;
        y => <-.ULtitle.y + <-.ULtitle.height + 5;
        width => <-.ULtitle.width;
        title = "Red";
      };
      UIslider ULGslider {
        parent => <-.ULframe;
        min = 0;
        max = 1.0;
        value => <-.params.Upper_Left_Green;
        x => <-.ULtitle.x;
        y => <-.ULRslider.y + <-.ULRslider.height + 5;
        width => <-.ULtitle.width;
        title = "Green";
      };
      UIslider ULBslider {
        parent => <-.ULframe;
        min = 0;
        max = 1.0;
        value => <-.params.Upper_Left_Blue;
        x => <-.ULtitle.x;
        y => <-.ULGslider.y + <-.ULGslider.height + 5;
        width => <-.ULtitle.width;
        title = "Blue";
      };



      //    Upper Right RGB panel
      //------------------------------------

      UIframe URframe {
         parent => <-.panel;
         x => <-.ULframe.x + <-.ULframe.width + 5;
         y => <-.ULframe.y;
         width => <-.ULframe.width;
         height => <-.ULframe.height;
      };
      UIlabel URtitle {
         parent => <-.URframe;
         x = 5;
         y = 5;
         width => parent.width - 12;
         alignment = "left";
         label => "Upper Right";
      };
      UIslider URRslider {
        parent => <-.URframe;
        min = 0;
        max = 1.0;
        value => <-.params.Upper_Right_Red;
        x => <-.URtitle.x;
        y => <-.URtitle.y + <-.URtitle.height + 5;
        width => <-.URtitle.width;
        title = "Red";
      };
      UIslider URGslider {
        parent => <-.URframe;
        min = 0;
        max = 1.0;
        value => <-.params.Upper_Right_Green;
        x => <-.URtitle.x;
        y => <-.URRslider.y + <-.URRslider.height + 5;
        width => <-.URtitle.width;
        title = "Green";
      };
      UIslider URBslider {
        parent => <-.URframe;
        min = 0;
        max = 1.0;
        value => <-.params.Upper_Right_Blue;
        x => <-.URtitle.x;
        y => <-.URGslider.y + <-.URGslider.height + 5;
        width => <-.URtitle.width;
        title = "Blue";
      };



      //    Lower Left RGB panel
      //------------------------------------

      UIframe LLframe {
         parent => <-.panel;
         x => <-.ULframe.x;
         y => <-.ULframe.y + <-.ULframe.height + 10;
         width => <-.ULframe.width;
         height => <-.ULframe.height;
      };
      UIlabel LLtitle {
         parent => <-.LLframe;
         x = 5;
         y = 5;
         width => parent.width - 12;
         alignment = "left";
         label => "Lower Left";
      };
      UIslider LLRslider {
        parent => <-.LLframe;
        min = 0;
        max = 1.0;
        value => <-.params.Lower_Left_Red;
        x => <-.LLtitle.x;
        y => <-.LLtitle.y + <-.LLtitle.height + 5;
        width => <-.LLtitle.width;
        title = "Red";
      };
      UIslider LLGslider {
        parent => <-.LLframe;
        min = 0;
        max = 1.0;
        value => <-.params.Lower_Left_Green;
        x => <-.LLtitle.x;
        y => <-.LLRslider.y + <-.LLRslider.height + 5;
        width => <-.LLtitle.width;
        title = "Green";
      };
      UIslider LLBslider {
        parent => <-.LLframe;
        min = 0;
        max = 1.0;
        value => <-.params.Lower_Left_Blue;
        x => <-.LLtitle.x;
        y => <-.LLGslider.y + <-.LLGslider.height + 5;
        width => <-.LLtitle.width;
        title = "Blue";
      };


      //    Lower Right RGB panel
      //------------------------------------

      UIframe LRframe {
         parent => <-.panel;
         x => <-.URframe.x;
         y => <-.LLframe.y;
         width => <-.ULframe.width;
         height => <-.ULframe.height;
      };
      UIlabel LRtitle {
         parent => <-.LRframe;
         x = 5;
         y = 5;
         width => parent.width - 12;
         alignment = "left";
         label => "Lower Right";
      };
      UIslider LRRslider {
        parent => <-.LRframe;
        min = 0;
        max = 1.0;
        value => <-.params.Lower_Right_Red;
        x => <-.LRtitle.x;
        y => <-.LRtitle.y + <-.LRtitle.height + 5;
        width => <-.LRtitle.width;
        title = "Red";
      };
      UIslider LRGslider {
        parent => <-.LRframe;
        min = 0;
        max = 1.0;
        value => <-.params.Lower_Right_Green;
        x => <-.LRtitle.x;
        y => <-.LRRslider.y + <-.LRRslider.height + 5;
        width => <-.LRtitle.width;
        title = "Green";
      };
      UIslider LRBslider {
        parent => <-.LRframe;
        min = 0;
        max = 1.0;
        value => <-.params.Lower_Right_Blue;
        x => <-.LRtitle.x;
        y => <-.LRGslider.y + <-.LRGslider.height + 5;
        width => <-.LRtitle.width;
        title = "Blue";
      };

   };



   macro BackgroundFunc {
      Image_ARGB &in<NEportLevels={2,1}> {
         dims+OPort2;
      };

      XP_COMM_PROJ.Background.BackgroundMods.BackgroundParams &params<NEportLevels={2,1}>;

      XP_COMM_PROJ.Background.BackgroundMods.BackgroundCore BackgroundCore {
         input_dims => <-.in.dims;
         params => <-.params;
      };

      FLD_MAP.image_field_argb image_field_argb {
         mesh {
            in_dims => <-.<-.BackgroundCore.output_dims;
         };
         data {
            in_data => <-.<-.BackgroundCore.output;
         };
      };

      olink out_fld => .image_field_argb.out;
      olink out_obj => .image_field_argb.obj;
   };



   macro background_generator {
      Image_ARGB &in<NEportLevels={2,1}>;

      XP_COMM_PROJ.Background.BackgroundMods.BackgroundParams params {
         Upper_Left_Hue = 0.67;
         Upper_Left_Sat = 1.0;
         Upper_Left_Value = 1.0;
         Upper_Right_Hue = 0.67;
         Upper_Right_Sat = 1.0;
         Upper_Right_Value = 1.0;
         Lower_Left_Hue = 0.0;
         Lower_Left_Sat = 1.0;
         Lower_Left_Value = 0.0;
         Lower_Right_Hue = 0.0;
         Lower_Right_Sat = 1.0;
         Lower_Right_Value = 0.0;
         X_Resolution = 128;
         Y_Resolution = 128;
         Dither = 0;
      };

      BackgroundUI BackgroundUI {
         params => <-.params;
      };

      BackgroundFunc BackgroundFunc {
         in => <-.in;
         params => <-.params;
      };

      olink out_fld => .BackgroundFunc.out_fld;
      olink out_obj => .BackgroundFunc.out_obj;
   };



   macro textured_background {
      ilink texture_in;
      float depth = -0.98;

      Quad quad1 {
         ncells = 1;
         node_connect_list = {0,1,2,3};
      };

      Mesh+Node_Data+Vector+Float+Space3+OPort quad_texture {
         nnodes = 4;
		
         coordinates {
            values[nvals][veclen]=> {
               {-1,-1,<-.<-.depth},
               {-1,1,<-.<-.depth},
               {1,1,<-.<-.depth},
               {1,-1,<-.<-.depth}
            };
         };
			
         ncell_sets = 1;
         cell_set[ncell_sets]=>{quad1};
			
         nnode_data=1;
         node_data[nnode_data];

         // Use texture coordinate values
         !node_data[0] {
            nvals => <-.nnodes;
            veclen = 2;
            values[nvals][veclen] =>
                                   { 0.0, 0.0,
                                     0.0, 1.0,
                                     1.0, 1.0,
                                     1.0, 0.0 };
            id = 670; // float texture 0-1 range data
         };
   	};

      GDM.DataObject DataObject {
         in => <-.quad_texture;
         Obj {
            pickable = 0;
            xform_mode = "Locked";
            name => "BackgroundTexture";
         };
         texture_in => <-.texture_in;
      };

      olink out_fld => quad_texture;
      olink out_obj => DataObject.obj;
   };



   macro coloured_background {
      float depth = -0.98;

      XP_COMM_PROJ.Background.BackgroundMods.RGB_BackgroundParams params {
         Upper_Left_Red = 0.0;
         Upper_Left_Green = 0.0;
         Upper_Left_Blue = 0.7;
         Upper_Right_Red = 0.0;
         Upper_Right_Green = 0.0;
         Upper_Right_Blue = 0.7;
         Lower_Left_Red = 0.6;
         Lower_Left_Green = 0.6;
         Lower_Left_Blue = 0.7;
         Lower_Right_Red = 0.6;
         Lower_Right_Green = 0.6;
         Lower_Right_Blue = 0.7;
      };

      RGB_BackgroundUI RGB_BackgroundUI {
         params => <-.params;
      };

      Quad quad1 {
         ncells = 1;
         node_connect_list = {0,1,2,3};
      };

      Mesh+Node_Data+Vector+Float+Space3+OPort quad_rgb {
         nnodes = 4;
		
         coordinates {
            values[nvals][veclen]=> {
               {-1,-1,<-.<-.depth},
               {-1,1,<-.<-.depth},
               {1,1,<-.<-.depth},
               {1,-1,<-.<-.depth}
            };
         };
			
         ncell_sets = 1;
         cell_set[ncell_sets]=>{quad1};
			
         nnode_data=1;
         node_data[nnode_data];

         // Use RGB coordinate values
         !node_data[0] {
            nvals => <-.nnodes;
            veclen = 3;
            values[nvals][veclen] =>
                                   { <-.<-.params.Lower_Left_Red,  <-.<-.params.Lower_Left_Green,  <-.<-.params.Lower_Left_Blue,
                                     <-.<-.params.Upper_Left_Red,  <-.<-.params.Upper_Left_Green,  <-.<-.params.Upper_Left_Blue,
                                     <-.<-.params.Upper_Right_Red, <-.<-.params.Upper_Right_Green, <-.<-.params.Upper_Right_Blue,
                                     <-.<-.params.Lower_Right_Red, <-.<-.params.Lower_Right_Green, <-.<-.params.Lower_Right_Blue };
            id = 667; // float RGB 0-1 range data
         };
   	};

      GDM.DataObject DataObject {
         in => <-.quad_rgb;
         Obj {
            pickable = 0;
            xform_mode = "Locked";
            name => "BackgroundColour";
         };
      };

      olink out_fld => quad_rgb;
      olink out_obj => DataObject.obj;
   };


};

