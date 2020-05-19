
flibrary BackgroundMods <build_dir="xp_comm_proj/backgrnd",
                         out_src_file="gen.cxx",
                         out_hdr_file="gen.hxx">
{

   group+OPort BackgroundParams {
      float+Port2 Upper_Left_Hue;
      float+Port2 Upper_Left_Sat;
      float+Port2 Upper_Left_Value;
      float+Port2 Upper_Right_Hue;
      float+Port2 Upper_Right_Sat;
      float+Port2 Upper_Right_Value;
      float+Port2 Lower_Left_Hue;
      float+Port2 Lower_Left_Sat;
      float+Port2 Lower_Left_Value;
      float+Port2 Lower_Right_Hue;
      float+Port2 Lower_Right_Sat;
      float+Port2 Lower_Right_Value;
      int+Port2 X_Resolution;
      int+Port2 Y_Resolution;
      boolean+Port2 Dither;
   };

   group+OPort RGB_BackgroundParams {
      float+Port2 Upper_Left_Red;
      float+Port2 Upper_Left_Green;
      float+Port2 Upper_Left_Blue;
      float+Port2 Upper_Right_Red;
      float+Port2 Upper_Right_Green;
      float+Port2 Upper_Right_Blue;
      float+Port2 Lower_Left_Red;
      float+Port2 Lower_Left_Green;
      float+Port2 Lower_Left_Blue;
      float+Port2 Lower_Right_Red;
      float+Port2 Lower_Right_Green;
      float+Port2 Lower_Right_Blue;
   };



   module BackgroundCore <src_file="backgrnd.cxx"> {
      int+IPort2 input_dims<export=2>[];

      BackgroundParams+IPort2 &params;

      float Upper_Left_Hue<export=2> => params.Upper_Left_Hue;
      float Upper_Left_Sat<export=2> => params.Upper_Left_Sat;
      float Upper_Left_Value<export=2> => params.Upper_Left_Value;
      float Upper_Right_Hue<export=2> => params.Upper_Right_Hue;
      float Upper_Right_Sat<export=2> => params.Upper_Right_Sat;
      float Upper_Right_Value<export=2> => params.Upper_Right_Value;
      float Lower_Left_Hue<export=2> => params.Lower_Left_Hue;
      float Lower_Left_Sat<export=2> => params.Lower_Left_Sat;
      float Lower_Left_Value<export=2> => params.Lower_Left_Value;
      float Lower_Right_Hue<export=2> => params.Lower_Right_Hue;
      float Lower_Right_Sat<export=2> => params.Lower_Right_Sat;
      float Lower_Right_Value<export=2> => params.Lower_Right_Value;
      int X_Resolution<export=2> => params.X_Resolution;
      int Y_Resolution<export=2> => params.Y_Resolution;
      boolean Dither<export=2> => params.Dither;

      cxxmethod+req+notify_inst update (
         Upper_Left_Hue+read+notify+req,
         Upper_Left_Sat+read+notify+req,
         Upper_Left_Value+read+notify+req,
         Upper_Right_Hue+read+notify+req,
         Upper_Right_Sat+read+notify+req,
         Upper_Right_Value+read+notify+req,
         Lower_Left_Hue+read+notify+req,
         Lower_Left_Sat+read+notify+req,
         Lower_Left_Value+read+notify+req,
         Lower_Right_Hue+read+notify+req,
         Lower_Right_Sat+read+notify+req,
         Lower_Right_Value+read+notify+req,
         X_Resolution+read+notify+req,
         Y_Resolution+read+notify+req,
         Dither+read+notify+req,
         input_dims+read+notify,
         output+write,
         output_dims+write
      );

      int+OPort2  output_dims[2];
      byte+OPort2 output[(output_dims[0]*output_dims[1])][4];
	};

};

