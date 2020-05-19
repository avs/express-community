
flibrary ReadRawApps<compile_subs=0>
{

   APPS.SingleWindowApp ReadRawEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.read_raw.out_obj
               };
            };
            Camera {
               Camera {
                  perspec = 1;
                  front = 1.;
               };
            };
         };
      };


      XP_COMM_PROJ.ReadRaw.ReadRawMacs.read_raw read_raw {
         filename = "$XP_PATH<1>/xp_comm_proj/rd_raw/data/f14nocan.raw";
      };
   };



   APPS.ModuleStack RAW_2_UCD {
      XP_COMM_PROJ.ReadRaw.ReadRawMacs.read_raw read_raw {
         filename = "$XP_PATH<1>/xp_comm_proj/rd_raw/data/f14nocan.raw";
      };

      Node_Data+OPort Empty_Node_Data {
         nnodes => <-.read_raw.out_fld.nnodes;
         nnode_data = 0;
         node_data {
            veclen = 0;
         };
      };
      FLD_MAP.combine_mesh_data combine_mesh_data {
         in_mesh => <-.read_raw.out_fld;
         in_nd => <-.Empty_Node_Data;
      };

      XP_COMM_PROJ.WriteUCD.WriteUCDMacs.write_ucd write_ucd {
         in_fld => <-.combine_mesh_data.out;
         filename = "$XP_PATH<1>/raw2ucd.inp";
      };
   };



   APPS.SingleWindowApp ReadUCDEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_UCD.out_obj
               };
            };
         };
      };


      MODS.Read_UCD Read_UCD {
         read_ucd_ui {
            filename = "$XP_PATH<1>/raw2ucd.inp";
         };
      };
   };

};

