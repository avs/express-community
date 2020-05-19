
flibrary TextureWaveMacs <compile_subs=0> {


   macro TextureWaveUI {

      XP_COMM_PROJ.TextureWave.TextureWaveMods.TextureWaveParams &params<NEportLevels={2,1}>;

      UImod_panel panel {
         title = "Texture Wave Module";
      };

      UIlabel TwaveTitle {
         parent => <-.panel;
         y = 0;
         width => parent.width;
         alignment = "left";
         label => "Texture Wave Module";
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };

 
      UIslider UIslider_freq {
        parent => <-.panel;
        min = 0;
        max = 25.0;
        value => <-.params.freq_value;
        x = 0;
        y => <-.TwaveTitle.y + <-.TwaveTitle.height + 20;
        title = "Frequency";
      };
      UIslider UIslider_phase {
        parent => <-.panel;
        min = 0;
        max = 1.0;
        value => <-.params.phase_value;
        x = 0;
        y => <-.UIslider_freq.y + <-.UIslider_freq.height + 10;
        title = "Phase";
      };
      UIslider UIslider_min_amp {
        parent => <-.panel;
        min = 0;
        max = 1.0;
        value => <-.params.min_amp;
        x = 0;
        y => <-.UIslider_phase.y + <-.UIslider_phase.height + 10;
        title = "Minimum Amplitude";
      };
      UIslider UIslider_max_amp {
        parent => <-.panel;
        min = 0;
        max = 1.0;
        value => <-.params.max_amp;
        x = 0;
        y => <-.UIslider_min_amp.y + <-.UIslider_min_amp.height + 10;
        title = "Maximum Amplitude";
      };
      UIslider UIslider_contrast {
        parent => <-.panel;
        min = 0.2;
        max = 2.0;
        value => <-.params.contrast;
        x = 0;
        y => <-.UIslider_max_amp.y + <-.UIslider_max_amp.height + 10;
        title = "RGB Contrast";
      };

      UIslider UIslider_numsteps {
        parent => <-.panel;
        min = 2;
        max = 50;
        mode = 1;
        value => <-.params.numsteps;
        x = 0;
        y => <-.UIslider_contrast.y + UIslider_contrast.height + 10;
        title = "Number of Steps to Animate";
      };
 
      UItoggle UItoggle_animate {
         parent => <-.panel;
         label => "Animate Texture";
         set<NEportLevels={3,2},export=2> => <-.params.animate;
         x = 0;
         y => <-.UIslider_numsteps.y + UIslider_numsteps.height + 10;
         width => (parent.width * 0.75) - 5;
      };
 
      UItoggle UItoggle_cycle {
         parent => <-.panel;
         label => "Cycle";
         set<NEportLevels={3,2},export=2> => <-.params.cycle;
         x => <-.UItoggle_animate.x + (parent.width * 0.75) + 5;
         y => <-.UIslider_numsteps.y + UIslider_numsteps.height + 10;
         width => (parent.width * 0.25) - 5;
      };
 

      UIradioBoxLabel LineTypeRadioBox {
         parent => <-.panel;
         x = 0;
         y => <-.UItoggle_animate.y + UItoggle_animate.height + 10;
         labels => {"Full Sine Wave", "Cosine Squared", "Cosine Medium Pulse", "Cosine Short Pulse", "50% Square Wave"};
         selectedItem => <-.params.wave_mode;
         title => "Pulse Type...";
      };

   };


   macro TextureWaveFunc {
      ilink in_image;

      Mesh_Struct+Node_Data+Byte &in<NEportLevels={1,1}> => in_image;

      XP_COMM_PROJ.TextureWave.TextureWaveMods.TextureWaveParams &params<NEportLevels={2,1}>;

      XP_COMM_PROJ.TextureWave.TextureWaveMods.TextureWaveCore TextureWaveCore {
         input+nres => <-.in.node_data[0].values;
         dims => <-.in.dims;
         params => <-.params;
      };

      FLD_MAP.image_field_argb image_field_argb {
         mesh {
            in_dims+notify => <-.<-.TextureWaveCore.out_dims;
         };
         data {
            in_data+notify => <-.<-.TextureWaveCore.output;
         };
         obj {
            name => name_of(<-.<-.<-.<-);
         };
      };

      olink out_image => image_field_argb.out;
      olink out_obj   => image_field_argb.obj;
   };



   macro texture_wave {
      ilink in_image;

      XP_COMM_PROJ.TextureWave.TextureWaveMods.TextureWaveParams params {
         freq_value = 10.0;
         phase_value = 0.0;
         wave_mode = 0;
         min_amp = 0.0;
         max_amp = 1.0;
         contrast = 1.0;
         animate = 0;
         numsteps = 10;
         cycle = 1;
      };

      GMOD.loop loop {
         run+Port2 => <-.params.animate;
         cycle => <-.params.cycle;
         done = 1;
         start_val = 0.;
         float+OPort rns => 1.0 / <-.params.numsteps;
         end_val => 1.0 - rns;
         incr => rns;
         count+Port2<export=2> => <-.params.phase_value;
      };

      TextureWaveUI TextureWaveUI {
         &params => <-.params;
      };

      TextureWaveFunc TextureWaveFunc {
         in_image => <-.in_image;
         &params => <-.params;
         image_field_argb {
            obj {
               name => name_of(<-.<-.<-.<-.<-);
            };
         };
      };

      olink out_image => .TextureWaveFunc.out_image;
      olink out_obj => .TextureWaveFunc.out_obj;
   };



   macro texture_wave_field {
      ilink in_fld;
      ilink in_image;

      texture_wave texture_wave {
         in_image => <-.in_image;
         TextureWaveFunc {
            image_field_argb {
               obj {
                  name => name_of(<-.<-.<-.<-.<-.<-) + "_image";
               };
            };
         };
      };

      XP_COMM_PROJ.Scalar2Texture.Scl2TexMacs.Scalar2Texture Scalar2Texture {
         in_fld => <-.in_fld;
         in_image => <-.texture_wave.out_image;
         data_obj {
            Texture {
               alpha = "Modulate";
               filter = "Bilinear";
            };
            Obj {
               name => name_of(<-.<-.<-.<-);
            };
         };
      };

      olink out_fld => .Scalar2Texture.out_fld;
      olink out_image => .texture_wave.out_image;
      olink out_obj => .Scalar2Texture.out_obj;
   };

};

