
flibrary EnhanceContrastApps {

   APPS.SingleWindowApp EnhanceContrastEg {
     GDM.Uviewer2D Uviewer2D {
        Scene {
           Top {
              child_objs => {
                 <-.<-.<-.IPEnhanceContrast.out_obj
              };
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

     MODS.extract_scalar extract_scalar {
        in_field => <-.Read_Image.field;
        ExtractScalarParam {
           component = 1;
        };
     };
   
     IPfldToImage IPfldToImage {
        In => <-.extract_scalar.out_fld;
     };

     XP_COMM_PROJ.EnhanceContrast.EnhanceContrastMacs.IPEnhanceContrast IPEnhanceContrast {
        in_image => <-.IPfldToImage.Image;
        params {
           level = 3.0;
        };
     };
  };

};

