

flibrary JoanArcMacs <compile_subs=0>{


   macro  JoanArcUI {
      ilink nseg;

      UImod_panel panel {
         title = "Joan of Arc Module";
      };

      UIlabel Title {
         parent => <-.panel;
         x = 0;
         y = 0;
         width => parent.width;
         alignment = "left";
         label => "Joan of Arc Module";
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };

 
      UIslider UIslider {
        parent => <-.panel;
        min = 2;
        max = 64;
        mode = 1;
        value => <-.nseg;
        x => <-.Title.x;
        y => <-.Title.y + <-.Title.height + 15;
        title = "Number of Segments";
      };
   };


 
   macro JoanArcFunc {

      float &start<NEportLevels={2,1}>[][3];
      float &end<NEportLevels={2,1}>[][3];
      float &vert<NEportLevels={2,1}>[];
      int   &nseg<NEportLevels={2,1}>;

      XP_COMM_PROJ.JoanArc.JoanArcMods.JoanArcCore JoanArcCore {
         start => <-.start;
         end => <-.end;
         vert => <-.vert;
         nseg => <-.nseg;
      };
      FLD_MAP.polyline_mesh polyline_mesh {
         coord => <-.JoanArcCore.polyline_out;
         connect => <-.JoanArcCore.connections_out;
         obj {
            name => name_of(<-.<-.<-.<-);
         };
      };

      olink out_fld => polyline_mesh.out;
      olink out_obj => polyline_mesh.obj;
   };


   macro joan_arc {
      float start<NEportLevels={2,1}>[][3];
      float end<NEportLevels={2,1}>[][3];
      float vert<NEportLevels={2,1}>[];
      int   nseg<NEportLevels={2,1}> = 20;

      JoanArcUI JoanArcUI {
         nseg => <-.nseg;
      };

      JoanArcFunc JoanArcFunc {
         start => <-.start;
         end => <-.end;
         vert => <-.vert;
         nseg => <-.nseg;
         polyline_mesh {
            obj {
               name => name_of(<-.<-.<-.<-.<-);
            };
         };
      };

      olink out_fld => JoanArcFunc.out_fld;
      olink out_obj => JoanArcFunc.out_obj;
   };

};

