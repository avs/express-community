
flibrary ImageFilterMacs <compile_subs=0> {

   macro ImageFilterUI {

      XP_COMM_PROJ.ImageFilter.ImageFilterMods.ImageFilterParams &params<NEportLevels={2,1}>;

      UImod_panel panel {
         title = "Image Filter";
      };

      UIlabel ImgFilterTitle {
         parent => <-.panel;
         x = 0;
         y = 0;
         width => parent.width;
         alignment = "left";
         label => "Image Filter";
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };

      UIradioBoxLabel FilterOp {
         parent => <-.panel;
         title = "Filter Operation";
         labels = {"Add", "Subtract", "Multiply", "Divide"};
         selectedItem => <-.params.filterOp;
         x = 0;
         y => <-.ImgFilterTitle.y + <-.ImgFilterTitle.height + 20;
      };

 
      UIslider AlphaSlider {
         parent => <-.panel;
         min => switch((params.filterOp + 1), -255., -255., 0., 0.);
         max => switch((params.filterOp + 1),  255.,  255., 2., 2.);
         value => <-.params.alphaConst;
         x = 0;
         y => <-.FilterOp.y + <-.FilterOp.height + 20;
         title = "Alpha Constant";
      };
      UIslider RedSlider {
         parent => <-.panel;
         min => switch((params.filterOp + 1), -255., -255., 0., 0.);
         max => switch((params.filterOp + 1),  255.,  255., 2., 2.);
         value => <-.params.redConst;
         x = 0;
         y => <-.AlphaSlider.y + <-.AlphaSlider.height + 5;
         title = "Red Constant";
      };
      UIslider GreenSlider {
         parent => <-.panel;
         min => switch((params.filterOp + 1), -255., -255., 0., 0.);
         max => switch((params.filterOp + 1),  255.,  255., 2., 2.);
         value => <-.params.greenConst;
         x = 0;
         y => <-.RedSlider.y + <-.RedSlider.height + 5;
         title = "Green Constant";
      };
      UIslider BlueSlider {
         parent => <-.panel;
         min => switch((params.filterOp + 1), -255., -255., 0., 0.);
         max => switch((params.filterOp + 1),  255.,  255., 2., 2.);
         value => <-.params.blueConst;
         x = 0;
         y => <-.GreenSlider.y + <-.GreenSlider.height + 5;
         title = "Blue Constant";
      };

   };



   macro ImageFilterFunc {
      Image_ARGB &in<NEportLevels={2,1}>;

      XP_COMM_PROJ.ImageFilter.ImageFilterMods.ImageFilterParams &params<NEportLevels={2,1}>;

      XP_COMM_PROJ.ImageFilter.ImageFilterMods.ImageFilterCore ImageFilterCore {
         input => <-.in.node_data[0].values;
         params => <-.params;
      };

      FLD_MAP.image_field_argb image_field_argb {
         mesh {
            in_dims => <-.<-.in.dims;
         };
         data {
            in_data => <-.<-.ImageFilterCore.output;
         };
         obj {
            name => name_of(<-.<-.<-.<-);
         };
      };

      olink out_image => .image_field_argb.out;
      olink out_obj   => .image_field_argb.obj;
   };


   macro image_filter {
      Image_ARGB &in<NEportLevels={2,1}>;

      XP_COMM_PROJ.ImageFilter.ImageFilterMods.ImageFilterParams params {
         filterOp = 0;
         alphaConst = 0.0;
         redConst = 0.0;
         greenConst = 0.0;
         blueConst = 0.0;
      };

      ImageFilterUI ImageFilterUI {
         params => <-.params;
      };

      ImageFilterFunc ImageFilterFunc {
         in => <-.in;
         params => <-.params;
         image_field_argb {
            obj {
               name => name_of(<-.<-.<-.<-.<-);
            };
         };
      };

      olink out_image => .ImageFilterFunc.out_image;
      olink out_obj   => .ImageFilterFunc.out_obj;
   };



   APPS.SingleWindowApp ImageFilterEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {<-.<-.<-.Read_Image.image, <-.<-.<-.image_filter.out_obj};
            };
         };
      };

      MODS.Read_Image Read_Image {
         read_image_ui {
            file_browser {
               filename = "$XP_PATH<0>/data/image/mandrill.x";
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

      XP_COMM_PROJ.ImageFilter.ImageFilterMacs.image_filter image_filter {
         &in => <-.Read_Image.field;

         ImageFilterFunc {
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
         params {
            filterOp = 2;
            redConst = 1.;
         };
      };
   };

};

