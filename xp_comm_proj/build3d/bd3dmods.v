
flibrary Build3DMods {

  // define the parameter block group

   group+OPort Build3DImageParams {
      int+Port2 resx<export=1>;
      int+Port2 resy<export=1>;
      int+Port2 start_layer<export=1>;
      int+Port2 end_layer<export=1>;
      int+Port2 start<export=1> = 0;
      int+Port2 component<export=1> = 1;
      int+Port2 count<export=1> = 0;
   };



   // define the low-level modules

   group Build3DMakeDims {
      Build3DImageParams+IPort2 &ImageParams;
      int+OPort2 dims[3] => {ImageParams.resx, ImageParams.resy, (ImageParams.end_layer - ImageParams.start_layer + 1) };
   };

   macro Build3DExtractScalar {
      ilink in_field<export_all=1>;
      ilink component;

      DV_Param_extract_scalar+Port ExtractScalarParam<export_all=2> {
         vector = 0;
         component => <-.component;
      };

      ExtractScalar ExtractScalar {
         in_fld => <-.in_field;
         param => <-.ExtractScalarParam;
      };

      olink out_fld<export_all=2> => .ExtractScalar.out_fld;
   };



   // Build3DAssemble
   //   Functional Macro (Build3DMakeDims low level module and Build3DImageParams)
   //   Loads a specified file into a specified layer in the resulting data set
   //   Used in conjunction with Build3DCreateFilename and an external loop to load a complete data set.

   macro Build3DAssemble {
      ilink filename;
      ilink index;

      Build3DImageParams &ImageParams<export=1,NEportLevels={2,1}>;

      Build3DMakeDims Build3DMakeDims {
         &ImageParams => <-.ImageParams;
      };

      DVM.DVread_image DVread_image {
         filename => <-.filename;
         flip = 0;
      };

      XP_COMM_PROJ.Build3D.Build3DMods.Build3DExtractScalar extract_scalar {
         in_field => <-.DVread_image.out;
         component => <-.ImageParams.component;
      };

      int+OPort  in_data_dims[2] => {4, (Build3DMakeDims.dims[0] * Build3DMakeDims.dims[1] * Build3DMakeDims.dims[2])};
      char+OPort in_data<export=1>[in_data_dims[1]][in_data_dims[0]];

      FLD_MAP.uniform_vector_field uniform_vector_field {
         mesh {
            in_dims => <-.<-.Build3DMakeDims.dims;
         };
         data {
            in_data => <-.<-.in_data;
            dims+IPort3 => <-.<-.in_data_dims;
         };
      };

      DVM.DVset_slice_data DVset_slice_data {
         inField => <-.uniform_vector_field.out;
         sliceField => <-.DVread_image.out;
         slice => <-.index;
      };

      olink out => .DVset_slice_data.inField;

   };


};

