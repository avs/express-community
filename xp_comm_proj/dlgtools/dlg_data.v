
group DlgData <NEcolor0=10039312,
               NEcolor1=10066329,
               NEcolor2=7379200,
               NEnumColors=3,
               NEportLevels={1,1}>
{
  
   group header {
      int have_header;
      string banner0;
      string banner1;
      int scale;
      int dlg_level;
      int planimetric_zone;
      int planimetric_system;
      int planimetric_units;
      float resolution;
      int n_ftm_trans_params;
      int n_accuracy_records;
      int n_control_points;
      int n_categories;
      float projection_params[15];
      float file2map_params[4];
      string corner_names[4];
      float corner_data[4][4];
   };



   group pb {
      int have_political_boundaries;
      int att_code;
      int n_nodes_ref;
      int n_nodes;
      int node_to_area_flag;
      int node_to_line_flag;
      int n_areas_ref;
      int n_areas;
      int area_to_node_flag;
      int area_to_line_flag;
      int area_coordinates_flag;
      int n_lines_ref;
      int n_lines;
      int line_coordinates_flag;

      group nodes[n_nodes] {
         float xy[2];
         int   n_lines;
         int   lines[n_lines];
      };

      group lines[n_lines] {
         int   end_nodes[2];
         int   side_areas[2];
         int   n_nodes;
         float nodes[n_nodes][2];
         int   n_attributes;
         int   attributes[n_attributes][2];
      };

      group areas[n_areas] {
         float xy[2];
         int   n_lines = 0;
         int   lines[n_lines];
         int   n_attributes = 0;
         int   attributes[n_attributes][2];
      };
   };


   group polys {
     int    n_polys;
     int    poly_area[n_polys];
     int    poly_limits[n_polys][2];
     int    total_nodes;
     float  poly_xy[total_nodes][2];
     
     string names[n_polys];

     int   n_data;
     float data[n_data][n_polys];
   };

};

