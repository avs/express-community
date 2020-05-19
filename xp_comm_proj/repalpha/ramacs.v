
flibrary ReplaceAlphaMacs <compile_subs=0> {


   macro replace_alpha {

      Image_ARGB &in<NEportLevels={2,1}>;
      Image      &alpha<NEportLevels={2,1}>;


      XP_COMM_PROJ.ReplaceAlpha.ReplaceAlphaMods.ReplaceAlphaCore ReplaceAlphaCore {
         input_image => <-.in.node_data[0].values;
         input_alpha => <-.alpha.node_data[0].values;
      };

      FLD_MAP.image_field_argb image_field_argb {
         mesh {
            in_dims => <-.<-.in.dims;
         };
         data {
            in_data => <-.<-.ReplaceAlphaCore.output;
         };
      };

      olink out_image => .image_field_argb.out;
      olink out_obj   => .image_field_argb.obj;
   };




   APPS.SingleWindowApp ReplaceAlphaEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.extract_alpha_channel.out_obj};
            };
         };
      };


      MODS.Read_Image Read_Image {
         read_image_ui {
            file_browser {
               filename = "$XP_PATH<0>/data/image/mandrill.x";
            };
         };
      };


      MODS.extract_scalar extract_color_channel {
         in_field => <-.Read_Image.field;
         ExtractScalarParam {
            component = 1;
         };
      };
      MODS.clamp clamp {
         in_field => <-.extract_color_channel.out_fld;
         ClampParam {
            min_value = 180;
            max_value = 210;
            below = 1;
            above = 1;
         };
      };


      XP_COMM_PROJ.ReplaceAlpha.ReplaceAlphaMacs.replace_alpha replace_alpha {
         in => <-.Read_Image.field;
         alpha => <-.clamp.out_fld;
      };


      MODS.extract_scalar extract_alpha_channel {
         in_field => <-.replace_alpha.out_image;
      };

   };

};

