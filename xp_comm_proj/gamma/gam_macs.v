
flibrary GammaCorrectMacs <compile_subs=0> {

   macro GammaCorrectUI {
      XP_COMM_PROJ.GammaCorrect.GammaCorrectMods.GammaCorrectParams &params<NEportLevels={2,1}>;

      UImod_panel panel {
         title = "Gamma Correct Module";
      };

      UIlabel GammaTitle {
         parent => <-.panel;
         x = 0;
         y = 0;
         width => parent.width;
         alignment = "left";
         label => "Gamma Correct Module";
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };

 
      UIslider GammaValueSlider {
        parent => <-.panel;
        min = 0;
        max = 3.0;
        value => <-.params.gamma_value;
        x = 0;
        y => <-.GammaTitle.y + <-.GammaTitle.height + 20;
        title = "Gamma Value (1.0=No Correction)";
      };
   
      UIoption ChromaticOption {
         label => "Chromatic Method";
      };
      UIoption AchromaticOption {
         label => "Achromatic Method";
      };
      UIradioBox MethodRadioBox {
         parent => <-.panel;
         x = 0;
         y => <-.GammaValueSlider.y + GammaValueSlider.height + 20;
         width = 200;
         cmdList => {
            <-.ChromaticOption,
            <-.AchromaticOption
         };
         selectedItem+IPort2 => <-.params.method;
      };
   };


   macro GammaCorrectFunc {
      Image_ARGB &in<NEportLevels={2,1}>;

      XP_COMM_PROJ.GammaCorrect.GammaCorrectMods.GammaCorrectParams &params<NEportLevels={2,1}>;

      XP_COMM_PROJ.GammaCorrect.GammaCorrectMods.GammaCorrectCore GammaCorrectCore {
         input => <-.in.node_data[0].values;
         params => <-.params;
      };

      FLD_MAP.image_field_argb image_field_argb {
         mesh {
            in_dims => <-.<-.in.dims;
         };
         data {
            in_data => <-.<-.GammaCorrectCore.output;
         };
         obj {
            name => name_of(<-.<-.<-.<-);
         };
      };

      olink out_image => .image_field_argb.out;
      olink out_obj   => .image_field_argb.obj;
   };


   macro gamma_correct {
      Image_ARGB &in<NEportLevels={2,1}>;

      XP_COMM_PROJ.GammaCorrect.GammaCorrectMods.GammaCorrectParams params {
         gamma_value = 1.0;
         method = 1;
      };

      GammaCorrectUI GammaCorrectUI {
         params => <-.params;
         panel {
            title => name_of(<-.<-.<-);
         };
      };

      GammaCorrectFunc GammaCorrectFunc {
         in => <-.in;
         params => <-.params;
         image_field_argb {
            obj {
               name => name_of(<-.<-.<-.<-.<-);
            };
         };
      };

      olink out_image => .GammaCorrectFunc.out_image;
      olink out_obj   => .GammaCorrectFunc.out_obj;
   };



   APPS.SingleWindowApp GammaCorrectEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_Image.image,
                  <-.<-.<-.gamma_correct.out_obj
               };
            };
         };
      };


      MODS.Read_Image Read_Image {
         read_image_ui {
            file_browser {
               filename = "$XP_PATH<0>/data/image/marble.x";
            };
         };
         DVread_image {
            out {
               xform {
                  xlate = {0.,300.,0.};
               };
            };
         };
      };


      XP_COMM_PROJ.GammaCorrect.GammaCorrectMacs.gamma_correct gamma_correct {
         in => <-.Read_Image.field;
         params {
            gamma_value = 1.8;
         };

         GammaCorrectFunc {
            image_field_argb {
               mesh {
                  out {
                     xform {
                        xlate = {0.,-300.,0.};
                     };
                  };
               };
            };
         };
      };

   };


};

