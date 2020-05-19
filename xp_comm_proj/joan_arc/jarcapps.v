

flibrary JoanArcApps <compile_subs=0> {


   APPS.SingleWindowApp JoanArcEg1 {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.joan_arc.out_obj
               };
            };
         };
      };

      float+Port start[][3] => {{0,0,0},
                                {1,0,0},
                                {2,0,0},
                                {3,0,0},
                                {4,0,0},
                                {5,0,0},
                                {6,0,0},
                                {7,0,0},
                                {8,0,0},
                                {9,0,0}};

      float+Port end[][3] =>   {{0,5,0},
                                {1,5,0},
                                {2,5,0},
                                {3,5,0},
                                {4,5,0},
                                {5,5,0},
                                {6,5,0},
                                {7,5,0},
                                {8,5,0},
                                {9,5,0}};

      float+Port height[][3] => {2,
                                 3,
                                 4,
                                 5,
                                 6,
                                 5,
                                 4,
                                 3,
                                 2,
                                 1};


      XP_COMM_PROJ.JoanArc.JoanArcMacs.joan_arc joan_arc {
         start => <-.start;
         end => <-.end;
         vert => <-.height;
      };
   };



   APPS.SingleWindowApp JoanArcEg2 {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.joan_arc.out_obj,
                  <-.<-.<-.bounds.out_obj,
                  <-.<-.<-.orthoslice#1.out_obj,
                  <-.<-.<-.orthoslice#2.out_obj
               };
            };
         };
      };

      MODS.Read_Volume Read_Volume {
         read_volume_ui {
            filename = "$XP_PATH<0>/data/volume/hydrogen.dat";
         };
      };
      MODS.bounds bounds {
         in_field => <-.Read_Volume.field;
      };

      MODS.orthoslice orthoslice {
         in_field => <-.Read_Volume.field;
         OrthoSliceParam {
            axis = 2;
         };
      };

      MODS.Loop Loop {
         run = 0;
         cycle = 2;
         end = 63.;
         incr = 1.;
         count = 30.;
      };

      MODS.orthoslice orthoslice#1 {
         in_field => <-.orthoslice.out_fld;
         OrthoSliceParam {
            plane<NEportLevels={3,2},export=3> => <-.<-.Loop.count;
         };
      };
      MODS.orthoslice orthoslice#2 {
         in_field => <-.orthoslice.out_fld;
         OrthoSliceParam {
            axis = 1;
            plane<NEportLevels={3,2},export=3> => <-.<-.Loop.count;
         };
      };


      FLD_MAP.extract_coordinate_array extract_coordinate_array#1 {
         in => <-.orthoslice#1.out_fld;
         comp = {0,1,2};
      };
      FLD_MAP.extract_coordinate_array extract_coordinate_array#2 {
         in => <-.orthoslice#2.out_fld;
         comp = {0,1,2};
      };

      MODS.data_math data_math {
         in_field1 => <-.orthoslice#1.out_fld;
         in_field2 => <-.orthoslice#2.out_fld;
         expres = "(#1 + #2) * 0.05";
      };
      FLD_MAP.extract_data_array extract_data_array {
         in => <-.data_math.out_fld;
      };


      XP_COMM_PROJ.JoanArc.JoanArcMacs.joan_arc joan_arc {
         start => <-.extract_coordinate_array#1.coord;
         end => <-.extract_coordinate_array#2.coord;
         vert => <-.extract_data_array.data;
      };
   };


};

