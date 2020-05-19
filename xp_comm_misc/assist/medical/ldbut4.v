Applications.ModuleStack
   {
   Viewer3D.View3D.View.renderer=0;
   macro field_to_byte<NEx=44.,NEy=209.> 
      {
      link in_field<NEportLevels={2,1},NEx=11.,NEy=22.> => <-.Read_Field.field;
      byte byte<NEportLevels=1,NEx=11.,NEy=66.>[][] => 
           255 * (in_field.node_data[0].values - in_field.node_data[0].min) / 
                 (in_field.node_data[0].max - in_field.node_data[0].min);
      int dims<NEportLevels=1,NEx=11.,NEy=110.>[] => in_field.dims;
      FLD_MAP.Field_Mappers.uniform_scalar_field 
        uniform_scalar_field<NEx=11.,NEy=154.> 
          {
          mesh 
             {
             in_dims => <-.<-.dims;
             };
          data 
             {
             in_data => <-.<-.byte;
             };
          out<NEportLevels={0,2}>;
          obj<NEportLevels={1,2}>;
          };
       link out_field<NEportLevels={1,2}>=>uniform_scalar_field.out;
       link out_geom<NEportLevels={1,2}>=>uniform_scalar_field.obj;
       };

MODS.volume_render volume_render {
      in_mesh => <-.field_to_byte.out_field;
   DataObject {
      VolRenderDatamap {
         DataRange = {
            {
               UIMaxValue=9.24,,,,,,,,,,,,,,,,,,,
            },};
         DatamapValue = {,
            {v1=0.22},,};
      };
      Props {
         fat_ray = 0;
         inherit = 0;
      };
   };
   GDprops_edit {
      voxel_interp = 1;
      ray_algo = 0;
      fat_ray = 0;
      inherit = 0;
   };
   UImod_panel {
      option {
         set = 1;
      };
   };
   modes_frame {
      parent => assist0.assist1.frames[3].br;
      y = 8;
      x = 12;
   };
   surf_rb {
      x = 0;
   };
   vol_rb {
      x = 0;
   };
   props_frame {
      parent => assist0.assist1.frames[3].br;
      y => 124;
      x = 12;
   };
   interp_tri {
      set = 1;
   };
   interp_rb {
      x = 0;
   };
   ray_direct {
      set = 1;
   };
   ray_rb {
      x = 0;
   };
   dist_rb {
      x = 0;
   };
   pos_slider {
      parent => assist0.assist1.frames[3].br;
      y => 312;
      x = 12;
   };
   range_slider {
      parent => assist0.assist1.frames[3].br;
      y => 368;
      x = 12;
   };
   alpha_rb {
      parent => assist0.assist1.frames[3].br;
      y => 328;
      x = 240;
   };
   min_slider {
      parent => assist0.assist1.frames[3].br;
      y => 376;
      x = 244;
   };
   max_slider {
      parent => assist0.assist1.frames[3].br;
      y => 432;
      x = 248;
   };
};

$push
$link Viewer3D.objs_in volume_render.DataObject.obj
$pop
};
!assist0.assist1.frames[3]{old=1;};
