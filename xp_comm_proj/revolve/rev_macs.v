flibrary RevolveMacs <compile_subs=0>{

   macro RevolveUI {
      UImod_panel UImod_panel {
         title => "Revolve";
      };
      XP_COMM_PROJ.Revolve.RevolveMods.RevolveParams+IPort2 &params;
      UIslider UIslider {
         parent => <-.UImod_panel;
         value => <-.params.resolution;
         title => "Resolution";
         width => parent.clientWidth * 11 / 12;
         max = 120.;
         min = 3.;
         mode="integer";
      };
      UItoggle UItoggle {
         parent => <-.UImod_panel;
         label => "Smooth";
         set => <-.params.smooth;
         y => <-.UIslider.y + UIslider.height + 10;
         width => parent.clientWidth * 11 / 12;
         active = 1;
      };
   };

   macro RevolveFunc {
      ilink in;
      XP_COMM_PROJ.Revolve.RevolveMods.RevolveParams &params;
      XP_COMM_PROJ.Revolve.RevolveMods.revolve revolve {
         profile => <-.in;
         params => <-.params;
      };
      olink out => .revolve.surface;
   };
   
   macro revolve {
      ilink in;
      XP_COMM_PROJ.Revolve.RevolveMods.RevolveParams RevolveParams {
         resolution = 1;
         smooth = 1;
      };
      XP_COMM_PROJ.Revolve.RevolveMacs.RevolveUI RevolveUI {
         params => <-.RevolveParams;
      };
      XP_COMM_PROJ.Revolve.RevolveMacs.RevolveFunc RevolveFunc {
         in => <-.in;
         params => <-.RevolveParams;
      };
      olink out => RevolveFunc.out;
      GDM.DataObject DataObject {
         in => <-.RevolveFunc.out;
      };
      olink obj => DataObject.obj;
   };

};


