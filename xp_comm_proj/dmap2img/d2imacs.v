
flibrary Dmap2ImageMacs <compile_subs=0>
{
   macro Dmap2ImageUI {
      XP_COMM_PROJ.Dmap2Image.Dmap2ImageMods.Dmap2ImageParams+IPort2 &params;
      UImod_panel UImod_panel {
         option {
            set = 1;
         };
         title => name_of(<-.<-.<-,1);
      };
      UIslider X_Resolution {
         parent => <-.UImod_panel;
         value => <-.params.X_Resolution;
         y = 0;
         max = 512.;
         width => parent.clientWidth;
         mode = "integer";
      };
      UIslider Y_Resolution {
         parent => <-.UImod_panel;
         value => <-.params.Y_Resolution;
         y => ((<-.X_Resolution.y + <-.X_Resolution.height) + 10);
         max = 512.;
         width => parent.clientWidth;
         mode = "integer";
      };
      UIradioBoxLabel Mode {
         parent => <-.UImod_panel;
         labels => {"Square","Legend"};
         selectedItem => <-.params.Mode;
         y => ((<-.Y_Resolution.y + <-.Y_Resolution.height) + 10);
         width => parent.clientWidth;
      };
   };

   macro Dmap2ImageFunc {
      ilink dmap_in;
      XP_COMM_PROJ.Dmap2Image.Dmap2ImageMods.Dmap2ImageParams+IPort2 &params;

      XP_COMM_PROJ.Dmap2Image.Dmap2ImageMods.Dmap2ImageCore Dmap2ImageCore {
         params => <-.params;
         dmap => <-.dmap_in;
      };

      FLD_MAP.image_field_argb image_field_argb {
         mesh {
            in_dims => <-.<-.Dmap2ImageCore.output_dims;
         };
         data {
            in_data => <-.<-.Dmap2ImageCore.output;
         };
      };

      olink out_fld => .image_field_argb.out;
      olink out_obj => .image_field_argb.obj;
   };

   macro Dmap2Image {

      ilink dmap_in;

      XP_COMM_PROJ.Dmap2Image.Dmap2ImageMods.Dmap2ImageParams params{
          X_Resolution = 256;
          Y_Resolution = 256;
          Mode = 0;
      };
	  
      XP_COMM_PROJ.Dmap2Image.Dmap2ImageMacs.Dmap2ImageUI Dmap2ImageUI {
         params => <-.params;
      };

      XP_COMM_PROJ.Dmap2Image.Dmap2ImageMacs.Dmap2ImageFunc Dmap2ImageFunc {
         params => <-.params;
         dmap_in => <-.dmap_in;
      };
	  
      olink out_fld => .Dmap2ImageFunc.out_fld ;
      olink out_obj => .Dmap2ImageFunc.out_obj;
   };

   // simple square image, no alpha blending

   macro Dmap2ImageSquare {

      ilink dmap_in;

      XP_COMM_PROJ.Dmap2Image.Dmap2ImageMods.Dmap2ImageParams params{
          X_Resolution = 256;
          Y_Resolution = 256;
          Mode = 0;
      };

      XP_COMM_PROJ.Dmap2Image.Dmap2ImageMacs.Dmap2ImageFunc Dmap2ImageFunc {
         params => <-.params;
         dmap_in => <-.dmap_in;
      };
	  
      olink out_fld => .Dmap2ImageFunc.out_fld ;
      olink out_obj => .Dmap2ImageFunc.out_obj;
   };


   // split image, checkerboard, alpha blending legend mode

   macro Dmap2ImageLegend {
      ilink dmap_in;
      XP_COMM_PROJ.Dmap2Image.Dmap2ImageMods.Dmap2ImageParams params {
         X_Resolution = 350;
         Y_Resolution = 60;
         Mode = 1;
      };
      XP_COMM_PROJ.Dmap2Image.Dmap2ImageMacs.Dmap2ImageFunc Dmap2ImageFunc {
         params => <-.params;
         dmap_in => <-.dmap_in;
      };
      olink out_fld => .Dmap2ImageFunc.out_fld ;
      olink out_obj => .Dmap2ImageFunc.out_obj;
   };

};

