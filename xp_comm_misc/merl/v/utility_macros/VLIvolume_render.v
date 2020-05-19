macro VLIvolume_render {

/*  Diagnostics
  int   base_plane_dims[2];
  int   image_dims[2];
  int   npixels => base_plane_dims[0] * base_plane_dims[1];
  byte  base_plane_argb[npixels][4];
  float hex_coords[6][3];
  float tex_coords[6][2];
*/  

  Mesh_Unif+Dim3+Space3+Node_Data+Scalar &in_volume <NEportLevels={2,1}>;
  
  VLIVolume VLIVolume {
    volume => in_volume;
    cursor       <NEportLevels={3,0}>;
    slice_plane  <NEportLevels={3,0}>;
    crop_bounds  <NEportLevels={3,0}>;
    lookup_table <NEportLevels={3,0}>;
  };
  
  VLIvolume_renderUI VLIvolume_renderUI {
    VLIvol_info => VLIVolume;
  };
  
  GDM.DataObject VolumeDataObject {
    in => <-.VLIVolume;
  };

  // Stick some points in to get the autonormalize to work
  FLD_MAP.point_mesh point_mesh {
    coord => in_volume.points;
    DataObject {
      Props {
	col = {0.,0.,0.};
      };
    };
  };
  
  GDM.GroupObject VolumeGroupObject {
    child_objs => {<-.VolumeDataObject.obj, <-.point_mesh.obj };
  };
 
  link obj<NEportLevels={1,2}> => VolumeGroupObject.obj; 
};
