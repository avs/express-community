
flibrary TextureWaveMods <build_dir="xp_comm_proj/twave",
                          out_src_file="gen.cxx",
                          out_hdr_file="gen.hxx">
{

   group+OPort TextureWaveParams {
      float+Port2 freq_value;
      float+Port2 phase_value;
      int+Port2   wave_mode;
      float+Port2 min_amp;
      float+Port2 max_amp;
      float+Port2 contrast;
      int+Port2   animate;
      int+Port2   numsteps;
      int+Port2   cycle;
   };


   module TextureWaveCore <src_file="twave.cxx"> {
      byte+IPort2 input<export=2>[][4];

      int+IPort   dims<export=2>[2];
      int in_x => dims[0];
      int in_y => dims[1];

      TextureWaveParams+IPort2 &params;

      float+IPort freq_value<export=2> => params.freq_value;
      float+IPort phase_value<export=2> => params.phase_value;
      int+IPort   wave_mode<export=2> => params.wave_mode;
      float+IPort min_amp<export=2> => params.min_amp;
      float+IPort max_amp<export=2> => params.max_amp;
      float+IPort contrast<export=2> => params.contrast;

      cxxmethod+req+notify_inst update (
         input+read+notify+req,
         in_x+read+notify+req,
         in_y+read+notify+req,
         freq_value+read+notify+req,
         phase_value+read+notify+req,
         wave_mode+read+notify+req,
         min_amp+read+notify+req,
         max_amp+read+notify+req,
         contrast+read+notify+req,
         out_x+write,
         out_y+write,
         output+write
      );

      int out_x;
      int out_y;
      int+OPort2 out_dims<export=2>[2] => {out_x, out_y};
      byte+OPort2 output[(out_x * out_y)][4];
   };

};

