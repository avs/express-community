flibrary Krig_Apps <compile_subs=0> {

 APPS.SingleWindowApp KrigEg0<NEdisplayMode="maximized"> {
   GDM.Uviewer3D Uviewer3D {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.Bubbleviz.out_obj,
               <-.<-.<-.viz.out_obj
            };
         };
      };
   };


   macro data {
      float+OPort x_val[10] = {1.,3.,7.,5.,2.,9.,4.,2.,6.,5.};
      float+OPort y_val[10] = {2.,4.,5.,6.,8.,2.,4.,5.,8.,10.};
      float+OPort z_val[10] = {2.,4.,6.,4.,8.,2.,5.,3.,8.,1.};
      float+OPort data_val[10] = {3.,4.,5.,7.,4.,8.,5.,2.,2.,10.};

      //float+OPort x_val[] = {1.,9.};
      //float+OPort y_val[] = {5.,5.};
      //float+OPort z_val[] = {5.,5.};
      //float+OPort data_val[] = {1.,9.};

      FLD_MAP.interleave_3_arrays interleave_3_arrays {
         in1 => <-.x_val;
         in2 => <-.y_val;
         in3 => <-.z_val;
      };
      FLD_MAP.node_scalar node_scalar {
         in_data => {<-.data_val};
      };

      FLD_MAP.point_mesh point_mesh {
         coord => <-.interleave_3_arrays.out;
      };
      FLD_MAP.combine_mesh_data combine_mesh_data {
         in_mesh => <-.point_mesh.out;
         in_nd => <-.node_scalar.out;
      };

      olink out_fld => combine_mesh_data.out;
   };

   HLM.Bubbleviz Bubbleviz {
      glyph {
         in_field => <-.<-.data.out_fld;
      };
   };

   int+OPort dims[3] = {30,30,30};

   XP_COMM_PROJ.Krig.KrigMacs.Krig Krig {
      in_fld => <-.data.out_fld;
      XuKrigging3D{
         nx => <-.<-.dims[2];
         ny => <-.<-.dims[1];
         nz => <-.<-.dims[0];
         radius = 20.;
         trigger<NEdisplayMode="open"> = 1;
      };
   };


   macro map_field {
      ilink in_dims => <-.dims;
      ilink in_data => <-.Krig.zest;
      
      FLD_MAP.uniform_mesh uniform_mesh {
         in_dims => <-.in_dims;
         out {
            points => {
               {0,0,0},
               {10,10,10}
            };
         };
      };
      FLD_MAP.node_scalar node_scalar {
         in_data => <-.in_data;
      };
      FLD_MAP.combine_mesh_data combine_mesh_data {
         in_mesh => <-.uniform_mesh.out;
         in_nd => <-.node_scalar.out;
      };
      macro set_null {
         ilink in_field<export_all=1,NEportLevels={2,1}> => <-.combine_mesh_data.out;

         DV_Param_set_null SetNullParam<export_all=2> {
            component = 0;
            unset = 0;
            null_value = 999.999;
         };
         DVset_null DVset_null {
            in => <-.in_field;
            DV_Param_set_null+OPort2 &param => <-.SetNullParam;
            &component => param.component;
            &unset => param.unset;
            &null_value => param.null_value;
         };
         DVcomb_mesh_and_data DVcomb_mesh_and_data {
            in_mesh => <-.in_field;
            in_nd => <-.DVset_null.out;
         };
         GDM.DataObject DataObject {
            in => <-.DVcomb_mesh_and_data.out;
         };

         olink out_fld<export_all=2> => .DVcomb_mesh_and_data.out;
         olink out_obj => .DataObject.obj;
      };

      olink out_fld => set_null.out_fld;
      olink out_obj => set_null.out_obj;
   };


   macro viz {
      ilink in_fld => <-.map_field.out_fld;

      MODS.bounds bounds{
         in_field => <-.in_fld;
      };
      MODS.orthoslice orthoslice#1 {
         in_field => <-.in_fld;
      };
      MODS.orthoslice orthoslice#2 {
         in_field => <-.in_fld;
         OrthoSliceParam {
            axis = 1;
            plane = 12;
         };
      };

      MODS.orthoslice orthoslice_line {
         in_field => <-.in_fld;
         OrthoSliceParam {
            plane => 9;
         };
      };
      MODS.isoline isoline {
         in_field => <-.orthoslice_line.out_fld;
      };
      GDM.GroupObject GroupObject{
         child_objs => {
            <-.isoline.out_obj,
            <-.bounds.out_obj,
            <-.orthoslice#1.out_obj,
            <-.orthoslice#2.out_obj
         };
      };

      olink out_obj => GroupObject.obj;
   };

};



 APPS.SingleWindowApp Krig2DEg {
   GDM.Uviewer2D Uviewer2D {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.Bubbleviz.out_obj,
               <-.<-.<-.viz.out_obj
            };
         };
      };
   };


   macro data {
      //float+OPort x_val[10] = {1.,3.,7.,5.,2.,9.,4.,2.,6.,5.};
      //float+OPort y_val[10] = {2.,4.,5.,6.,8.,2.,4.,5.,8.,10.};
      //float+OPort z_val[10] = {2.,4.,6.,4.,8.,2.,5.,3.,8.,1.};
      //float+OPort data_val[10] = {3.,4.,5.,7.,4.,8.,5.,2.,2.,10.};

      float+OPort x_val[] = {1.,9.};
      float+OPort y_val[] = {1.,9.};
      float+OPort data_val[] = {1.,9.};

      FLD_MAP.interleave_2_arrays interleave_2_arrays {
         in1 => <-.x_val;
         in2 => <-.y_val;
      };
      FLD_MAP.node_scalar node_scalar {
         in_data => {<-.data_val};
      };

      FLD_MAP.point_mesh point_mesh {
         coord => <-.interleave_2_arrays.out;
      };
      FLD_MAP.combine_mesh_data combine_mesh_data {
         in_mesh => <-.point_mesh.out;
         in_nd => <-.node_scalar.out;
      };

      olink out_fld => combine_mesh_data.out;
   };

   HLM.Bubbleviz Bubbleviz {
      glyph {
         in_field => <-.<-.data.out_fld;
      };
   };

   int+OPort dims[2] = {30,30};

   XP_COMM_PROJ.Krig.KrigMacs.Krig2D Krig {
      in_fld => <-.data.out_fld;
      XuKrigging2D{
         nx => <-.<-.dims[1];
         ny => <-.<-.dims[0];
         radius = 20.;
         poly_x = ;
         poly_y = ;
         trigger<NEdisplayMode="open"> = 1;
      };
   };


   macro map_field {
      ilink in_dims => <-.dims;
      ilink in_data => <-.Krig.zest;
      
      FLD_MAP.uniform_mesh uniform_mesh {
         in_dims => <-.in_dims;
         out {
            points => {
               {0,0},
               {10,10}
            };
         };
      };
      FLD_MAP.node_scalar node_scalar {
         in_data => <-.in_data;
      };
      FLD_MAP.combine_mesh_data combine_mesh_data {
         in_mesh => <-.uniform_mesh.out;
         in_nd => <-.node_scalar.out;
      };
      macro set_null {
         ilink in_field<export_all=1,NEportLevels={2,1}> => <-.combine_mesh_data.out;

         DV_Param_set_null SetNullParam<export_all=2> {
            component = 0;
            unset = 0;
            null_value = 999.999;
         };
         DVset_null DVset_null {
            in => <-.in_field;
            DV_Param_set_null+OPort2 &param => <-.SetNullParam;
            &component => param.component;
            &unset => param.unset;
            &null_value => param.null_value;
         };
         DVcomb_mesh_and_data DVcomb_mesh_and_data {
            in_mesh => <-.in_field;
            in_nd => <-.DVset_null.out;
         };
         GDM.DataObject DataObject {
            in => <-.DVcomb_mesh_and_data.out;
         };

         olink out_fld<export_all=2> => .DVcomb_mesh_and_data.out;
         olink out_obj => .DataObject.obj;
      };

      olink out_fld => set_null.out_fld;
      olink out_obj => set_null.out_obj;
   };


   macro viz {
      ilink in_fld => <-.map_field.out_fld;

      MODS.bounds bounds{
         in_field => <-.in_fld;
      };

/*
      MODS.orthoslice orthoslice#1 {
         in_field => <-.in_fld;
      };
      MODS.orthoslice orthoslice#2 {
         in_field => <-.in_fld;
         OrthoSliceParam {
            axis = 1;
            plane = 12;
         };
      };

      MODS.orthoslice orthoslice_line {
         in_field => <-.in_fld;
         OrthoSliceParam {
            plane => 9;
         };
      };
      MODS.isoline isoline {
         in_field => <-.orthoslice_line.out_fld;
      };
*/
      GDM.GroupObject GroupObject{
         child_objs => {
            <-.bounds.out_obj,
            <-.<-.map_field.out_obj
         };
      };

      olink out_obj => GroupObject.obj;
   };

};





APPS.SingleWindowApp KrigEg1<NEdisplayMode="maximized"> {
   HLM.Bubbleviz Bubbleviz{
      glyph {
         in_field => <-.<-.data.combine_mesh_data.out;
         GlyphUI {
            DVnode_data_labels {
               labels[];
            };
            UIradioBoxLabel_map {
               label_cmd {
                  cmd[];
               };
            };
         };
      };
      UIpanel {
         height = 780;
         option {
            set = 0;
         };
      };
   };
   macro data{
      FLD_MAP.point_mesh point_mesh{
         coord => <-.interleave_3_arrays.out;
      };
      FLD_MAP.combine_mesh_data combine_mesh_data{
         in_mesh => <-.point_mesh.out;
         in_nd => <-.node_scalar.out;
         out<NEportLevels={0,3}>;
      };
      FLD_MAP.node_scalar node_scalar{
         in_data => {<-.float#3};
      };
      FLD_MAP.interleave_3_arrays interleave_3_arrays {
         in1 => <-.float;
         in2 => <-.float#1;
         in3 => <-.float#2;
      };
      float float<NEportLevels={1,1}>[10] = {1.,3.,7.,5.,2.,9.,4.,2.,6.,5.};
      float float#1<NEportLevels={1,1}>[10] = {2.,4.,5.,6.,8.,2.,4.,5.,8.,10.};
      float float#2<NEportLevels={1,1}>[10] = {2.,4.,6.,4.,8.,2.,5.,3.,8.,1.};
      float float#3<NEportLevels={1,1}>[10] = {3.,4.,5.,7.,4.,8.,5.,2.,2.,10.};
   };
   int dims<NEportLevels={1,1}>[3] = {10,10,10};
   GDM.Uviewer3D Uviewer3D{
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.Bubbleviz.out_obj,<-.<-.<-.viz.GroupObject.obj,
               <-.<-.<-.cropper.line_mesh.obj};
            Xform {
               ocenter = {5.,5.11306,5.};
               dcenter = {4.12273,0.474254,
7.75557};
               mat = {
                  0.420303,0.363297,-0.360142,0.,-0.503576,0.211845,-0.374001,
0.,-0.0899875,0.511351,0.410811,0.,0.,0.,0.,1.
               };
               xlate = {-4.16466,-0.456618,
-7.7867};
            };
         };
      };
      Scene_Editor {
         View_Editor {
            InfoPanel {
               curobj_sel {
                  width = 238;
                  height = 245;
                  x = 0;
                  y = 0;
               };
            };
         };
      };
   };
   macro map_field{
      FLD_MAP.uniform_mesh uniform_mesh{
         in_dims<NEportLevels={3,0}> => <-.<-.dims;
         out<NEdisplayMode="open"> {
            points => {{0,0,0},
               {10,10,10}};
         };
      };
      FLD_MAP.node_scalar node_scalar{
         in_data<NEportLevels={3,0}> => <-.<-.Krig.zest;
      };
      FLD_MAP.combine_mesh_data combine_mesh_data{
         in_mesh => <-.uniform_mesh.out;
         in_nd => <-.node_scalar.out;
      };
      macro set_null{
         ilink in_field<export_all=1,NEportLevels={2,1}> => <-.combine_mesh_data.out;
         DV_Param_set_null SetNullParam<export_all=2,NEx=341.,NEy=22.> {
            component = 0;
            unset = 0;
            null_value = 999.999;
         };
         DVset_null DVset_null {
            in => <-.in_field;
            &component => param.component;
            &unset => param.unset;
            &null_value => param.null_value;
            DV_Param_set_null &param<NEportLevels={2,0}> => <-.SetNullParam;
         };
         DVcomb_mesh_and_data DVcomb_mesh_and_data {
            in_mesh => <-.in_field;
            in_nd => <-.DVset_null.out;
         };
         DataObjectNoTexture obj {
            in => <-.DVcomb_mesh_and_data.out;
         };
         olink out_fld<export_all=2,NEportLevels={1,3}> => .DVcomb_mesh_and_data.out;
         olink out_obj<NEportLevels={1,2}> => .obj.obj;
      };
   };
   macro viz{
      MODS.bounds bounds{
         in_field => <-.link;
         BoundsUI {
            DVnode_data_labels {
               labels[];
            };
            UIradioBoxLabel {
               label_cmd {
                  cmd[];
               };
            };
         };
      };
      MODS.orthoslice orthoslice{
         in_field => <-.link;
      };
      MODS.orthoslice orthoslice#1{
         in_field => <-.link;
         OrthoSliceParam {
            axis = 1;
            plane => 4;
         };
         OrthoSliceUI {
            UIpanel {
               option {
                  set = 1;
               };
            };
         };
      };
      MODS.orthoslice orthoslice#2{
         in_field => <-.link;
         OrthoSliceParam {
            plane => 1;
            axis = 1;
         };
      };
      MODS.isoline isoline{
         in_field => <-.orthoslice#2.out_fld;
      };
      link link<NEportLevels={2,1}> => <-.map_field.set_null.out_fld;
      GDM.GroupObject GroupObject{
         child_objs => {<-.isoline.out_obj,
            <-.bounds.out_obj,<-.orthoslice.out_obj,<-.orthoslice#1.out_obj};
         obj<NEportLevels={1,3}>;
      };
   };
   float polyx<NEportLevels=1>[] = {5.,8.,8.,1.};
   float polyy<NEportLevels=1>[] = {2.5,1.,6.,6.};
   int num<NEportLevels=1> => array_size(.polyx);
   macro cropper{
      FLD_MAP.line_mesh line_mesh{
         coord[8][3] => <-.interleave_3_arrays.out;
         connect => {
            0,1,1,2,2,3,3,0,4,5,5,6,6,7,7,4,0,4,1,5,2,6,3,7
         };
         DataObject {
            Obj {
               props<NEdisplayMode="open">;
            };
            Props {
               col = {1.,0.,0.};
            };
         };
         obj<NEdisplayMode="open",NEportLevels={1,3}>;
      };
      float z0<NEportLevels={1,1}>[<-.num] = {0.,0.,0.,0.};
      float z1<NEportLevels={1,1}>[<-.num] => 10;
      FLD_MAP.concat_2_arrays concat_2_arrays {
         in1<NEportLevels={3,0}> => <-.<-.polyx;
         in2<NEportLevels={3,0}> => <-.<-.polyx;
      };
      FLD_MAP.concat_2_arrays concat_2_arrays#1{
         in1<NEportLevels={3,0}> => <-.<-.polyy;
         in2<NEportLevels={3,0}> => <-.<-.polyy;
      };
      FLD_MAP.concat_2_arrays concat_2_arrays#2{
         in1 => <-.z0;
         in2 => <-.z1;
      };
      FLD_MAP.interleave_3_arrays interleave_3_arrays{
         in1 => <-.concat_2_arrays.out;
         in2 => <-.concat_2_arrays#1.out;
         in3 => <-.concat_2_arrays#2.out;
      };
   };
   XP_COMM_PROJ.Krig.KrigMacs.Krig Krig{
      in_fld => <-.data.combine_mesh_data.out;
      XuKrigging3D{
         nx => dims[2];
         ny => dims[1];
         nz => dims[0];
         poly_x => polyx;
         poly_y => polyy;
         radius = 10.;
         trigger<NEdisplayMode="open"> = 1;
      };
   };
};


};

