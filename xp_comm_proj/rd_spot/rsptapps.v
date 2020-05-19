
flibrary ReadSpotApps <compile_subs=0> {

   APPS.SingleWindowApp ReadSpotEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.read_spot.out_obj
               };
            };
         };
      };

      XP_COMM_PROJ.ReadSpot.ReadSpotMacs.read_spot read_spot {
         input_params {
            HeaderFileName = "$XP_PATH<1>/xp_comm_proj/rd_spot/data/lead_05.dat";
            ImageFileName = "$XP_PATH<1>/xp_comm_proj/rd_spot/data/imag_05.dat";
            BandNumberToRead = 1;
            StartRow = 0;
            StartColumn = 0;
            NumberOfRows = 500;
            NumberOfColumns = 500;
            XScaleFactor = 1;
            YScaleFactor = 1;
            ActivateFlag = 1;
         };
      };
   };



   APPS.SingleWindowApp ReadSpotEgAlt {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.read_spot_alt.out_obj
               };
            };
         };
      };

      XP_COMM_PROJ.ReadSpot.ReadSpotMacs.read_spot_alt read_spot_alt {
         input_params {
            HeaderFileName = "$XP_PATH<1>/xp_comm_proj/rd_spot/data/lead_05.dat";
            ImageFileName = "$XP_PATH<1>/xp_comm_proj/rd_spot/data/imag_05.dat";
            BandNumberToRead = 1;
            StartRow = 0;
            StartColumn = 0;
            NumberOfRows = 500;
            NumberOfColumns = 500;
            XScaleFactor = 1;
            YScaleFactor = 1;
            ActivateFlag = 1;
         };
      };
   };

};

