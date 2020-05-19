

flibrary BinnerApps<compile_subs=0> {

   APPS.SingleWindowApp BinnerEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Bubbleviz.out_obj,
                  <-.<-.<-.bounds.out_obj,
                  <-.<-.<-.orthoslice.out_obj
               };
            };
            Lights {
               Lights = {
                  {
                     type="BiDirectional"
                  },,,
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

      MODS.Read_Column_File Read_Column_File {
         param {
            filename = "$XP_PATH<1>/xp_comm_proj/binner/small.dat";
            header = 1;
            columns = {
               {name="ENERGY",type=4,use=1,null_value=0.},
               {name="IQ",type=2,use=1,null_value=0.},
               {name="X",type=4,use=1,null_value=0.},
               {name="Y",type=4,use=1,null_value=0.},
               {name="U",type=4,use=1,null_value=0.},
               {name="V",type=4,use=1,null_value=0.},
               {name="W",type=4,use=1,null_value=0.},
               {name="ZLAST",type=4,use=1,null_value=0.},
               {name="WEIGHT",type=4,use=1,null_value=0.},
               {name="LATCH1",type=2,use=1,null_value=0.},
               {name="LATCH2",type=2,use=1,null_value=0.}
            };
            ncol = 11;
         };
      };


      MODS.table_to_scatter_field table_to_scatter_field {
         columns => <-.Read_Column_File.table;
         xyz_select {
            xyz_set = {1,1,1};
            xyz_axis = {2,3,7};
         };
         data_select {
            data = {0};
         };
         xyz_create {
            ncol = 11;
            xyz_names[11] = {
               "ENERGY","IQ","X","Y","U","V","W","ZLAST","WEIGHT","LATCH1","LATCH2"
            };
            field_xyz[11];
            names[11] = {
               "ENERGY","IQ","X","Y","U","V","W","ZLAST","WEIGHT","LATCH1","LATCH2"
            };
            reset = 0;
         };
         data_create {
            data_names[11] = {
               "ENERGY","IQ","X","Y","U","V","W","ZLAST","WEIGHT","LATCH1","LATCH2"
            };
            names[11] = {
               "ENERGY","IQ","X","Y","U","V","W","ZLAST","WEIGHT","LATCH1","LATCH2"
            };
            reset = 0;
         };
         xyz = {
            {
               values=>Read_Column_File.DVtable_read.out[0].values,null_flag=>
               Read_Column_File.DVtable_read.out[0].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[0].null_value,,,name=>
               Read_Column_File.DVtable_read.out[0].labels,discrete=0,normalize=1,set_labels=0,label_step=1,index_labels=0,bin_size=1.1525
            },
            {
               values=>Read_Column_File.DVtable_read.out[1].values,null_flag=>
               Read_Column_File.DVtable_read.out[1].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[1].null_value,,,name=>
               Read_Column_File.DVtable_read.out[1].labels,discrete=1,normalize=1,set_labels=1,label_step=1,index_labels=1,bin_size=0.1
            },
            {
               values=>Read_Column_File.DVtable_read.out[2].values,null_flag=>
               Read_Column_File.DVtable_read.out[2].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[2].null_value,,,name=>
               Read_Column_File.DVtable_read.out[2].labels,discrete=0,normalize=1,set_labels=0,label_step=1,index_labels=0,bin_size=8.1884
            },
            {
               values=>Read_Column_File.DVtable_read.out[3].values,null_flag=>
               Read_Column_File.DVtable_read.out[3].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[3].null_value,,,name=>
               Read_Column_File.DVtable_read.out[3].labels,discrete=0,normalize=1,set_labels=0,label_step=1,index_labels=0,bin_size=7.9252
            },
            {
               values=>Read_Column_File.DVtable_read.out[4].values,null_flag=>
               Read_Column_File.DVtable_read.out[4].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[4].null_value,,,name=>
               Read_Column_File.DVtable_read.out[4].labels,discrete=0,normalize=1,set_labels=0,label_step=1,index_labels=0,bin_size=0.1512
            },
            {
               values=>Read_Column_File.DVtable_read.out[5].values,null_flag=>
               Read_Column_File.DVtable_read.out[5].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[5].null_value,,,name=>
               Read_Column_File.DVtable_read.out[5].labels,discrete=0,normalize=1,set_labels=0,label_step=1,index_labels=0,bin_size=0.1534
            },
            {
               values=>Read_Column_File.DVtable_read.out[6].values,null_flag=>
               Read_Column_File.DVtable_read.out[6].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[6].null_value,,,name=>
               Read_Column_File.DVtable_read.out[6].labels,discrete=0,normalize=1,set_labels=0,label_step=1,index_labels=0,bin_size=0.0632
            },
            {
               values=>Read_Column_File.DVtable_read.out[7].values,null_flag=>
               Read_Column_File.DVtable_read.out[7].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[7].null_value,,,name=>
               Read_Column_File.DVtable_read.out[7].labels,discrete=0,normalize=1,set_labels=0,label_step=1,index_labels=0,bin_size=9.8822
            },
            {
               values=>Read_Column_File.DVtable_read.out[8].values,null_flag=>
               Read_Column_File.DVtable_read.out[8].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[8].null_value,,,name=>
               Read_Column_File.DVtable_read.out[8].labels,discrete=0,normalize=1,set_labels=0,label_step=1,index_labels=0,bin_size=0.
            },
            {
               values=>Read_Column_File.DVtable_read.out[9].values,null_flag=>
               Read_Column_File.DVtable_read.out[9].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[9].null_value,,,name=>
               Read_Column_File.DVtable_read.out[9].labels,discrete=1,normalize=1,set_labels=1,label_step=1,index_labels=1,bin_size=111.
            },
            {
               values=>Read_Column_File.DVtable_read.out[10].values,null_flag=>
               Read_Column_File.DVtable_read.out[10].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[10].null_value,,,name=>
               Read_Column_File.DVtable_read.out[10].labels,discrete=1,normalize=1,set_labels=1,label_step=1,index_labels=1,bin_size=295632895.9
            }
         };
         data = {
            {
               values=>Read_Column_File.DVtable_read.out[0].values,null_flag=>
               Read_Column_File.DVtable_read.out[0].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[0].null_value,,,name=>
               Read_Column_File.DVtable_read.out[0].labels,count_records=0,operation=0,valid=1
            },
            {
               values=>Read_Column_File.DVtable_read.out[1].values,null_flag=>
               Read_Column_File.DVtable_read.out[1].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[1].null_value,,,name=>
               Read_Column_File.DVtable_read.out[1].labels,count_records=0,operation=0,valid=1
            },
            {
               values=>Read_Column_File.DVtable_read.out[2].values,null_flag=>
               Read_Column_File.DVtable_read.out[2].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[2].null_value,,,name=>
               Read_Column_File.DVtable_read.out[2].labels,count_records=0,operation=0,valid=1
            },
            {
               values=>Read_Column_File.DVtable_read.out[3].values,null_flag=>
               Read_Column_File.DVtable_read.out[3].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[3].null_value,,,name=>
               Read_Column_File.DVtable_read.out[3].labels,count_records=0,operation=0,valid=1
            },
            {
               values=>Read_Column_File.DVtable_read.out[4].values,null_flag=>
               Read_Column_File.DVtable_read.out[4].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[4].null_value,,,name=>
               Read_Column_File.DVtable_read.out[4].labels,count_records=0,operation=0,valid=1
            },
            {
               values=>Read_Column_File.DVtable_read.out[5].values,null_flag=>
               Read_Column_File.DVtable_read.out[5].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[5].null_value,,,name=>
               Read_Column_File.DVtable_read.out[5].labels,count_records=0,operation=0,valid=1
            },
            {
               values=>Read_Column_File.DVtable_read.out[6].values,null_flag=>
               Read_Column_File.DVtable_read.out[6].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[6].null_value,,,name=>
               Read_Column_File.DVtable_read.out[6].labels,count_records=0,operation=0,valid=1
            },
            {
               values=>Read_Column_File.DVtable_read.out[7].values,null_flag=>
               Read_Column_File.DVtable_read.out[7].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[7].null_value,,,name=>
               Read_Column_File.DVtable_read.out[7].labels,count_records=0,operation=0,valid=1
            },
            {
               values=>Read_Column_File.DVtable_read.out[8].values,null_flag=>
               Read_Column_File.DVtable_read.out[8].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[8].null_value,,,name=>
               Read_Column_File.DVtable_read.out[8].labels,count_records=0,operation=0,valid=1
            },
            {
               values=>Read_Column_File.DVtable_read.out[9].values,null_flag=>
               Read_Column_File.DVtable_read.out[9].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[9].null_value,,,name=>
               Read_Column_File.DVtable_read.out[9].labels,count_records=0,operation=0,valid=1
            },
            {
               values=>Read_Column_File.DVtable_read.out[10].values,null_flag=>
               Read_Column_File.DVtable_read.out[10].null_flag,null_value=>
               Read_Column_File.DVtable_read.out[10].null_value,,,name=>
               Read_Column_File.DVtable_read.out[10].labels,count_records=0,operation=0,valid=1
            }
         };
      };

      HLM.Bubbleviz Bubbleviz {
         glyph {
            in_field => <-.<-.table_to_scatter_field.out_fld;
            GlyphParam {
               scale = 2.5;
            };
         };
      };

      XP_COMM_PROJ.Binner.BinnerMacs.binner binner {
         in_fld => <-.table_to_scatter_field.out_fld;
         BinnerUI {
            coord_fields = {
               {
                  value=10.,,,,,,
               },
               {
                  value=10.,,,,,,
               },
               {
                  value=15.,,,,,,
               }
            };
         };
      };

      MODS.extract_component extract_component {
         in_field => <-.binner.out_fld;
      };

      MODS.bounds bounds {
         in_field => <-.extract_component.out_fld;
      };
      MODS.orthoslice orthoslice {
         in_field => <-.extract_component.out_fld;
      };
   };




   XP_COMM_PROJ.Binner.BinnerApps.BinnerEg BinnerCityPlotEg {
      -orthoslice;

      Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Bubbleviz.out_obj,
                  <-.<-.<-.bounds.out_obj,
   				   <-.<-.<-.city_plot.out_obj
               };
            };
         };
      };


      table_to_scatter_field {
         xyz_select {
            xyz_set = {1,1,0};
            xyz_axis = {2,3,0};
         };
      };

      binner {
         BinnerUI {
            coord_fields = {
               {
                  value=20.,,,,,,
               },
               {
                  value=20.,,,,,,
               }
            };
            data_field.value = 4;
         };
      };


      MODS.city_plot city_plot {
         in_field => <-.extract_component.out_fld;
         CityParam {
            scale = 10.;
         };
      };

   };



   XP_COMM_PROJ.Binner.BinnerApps.BinnerCityPlotEg BinnerPolarEg {
      -city_plot;

      Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Bubbleviz.out_obj,
                  <-.<-.<-.bounds.out_obj,
   				   <-.<-.<-.coordinate_math.out_obj
               };
            };
         };
      };

      binner {
         BinnerParams {
            coordinate_system = "polar";
         };
         BinnerUI {
            coord_fields = {
               {
                  value=10.,,,,,,
               },
               {
                  value=20.,,,,,,
               }
            };
         };
      };

      MODS.coordinate_math coordinate_math {
         in_field1 => <-.extract_component.out_fld;
         operation_x = "#1x * cos(#1y)";
         operation_y = "#1x * sin(#1y)";
      };
      bounds {
         in_field => <-.coordinate_math.out_fld;
      };
   };

};

