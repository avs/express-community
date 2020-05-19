
flibrary TextureVplaneMods <cxx_name=""> {

  // Parameter Block

  group TextureVPlaneParams <NEportLevels={0,1}> {
    float start_x <NEportLevels={2,2}>;
    float start_y <NEportLevels={2,2}>;
    float start_z <NEportLevels={2,2}>;
    float end_x <NEportLevels={2,2}>;
    float end_y <NEportLevels={2,2}>;
    float end_z <NEportLevels={2,2}>;
    int flip <NEportLevels={2,2}>;
  };

  // Parameter Block Reference

  TextureVPlaneParams &TextureVPlaneParams_ref <NEportLevels={1,1}> {
    float start_x <NEportLevels={0,2}>;
    float start_y <NEportLevels={0,2}>;
    float start_z <NEportLevels={0,2}>;
    float end_x <NEportLevels={0,2}>;
    float end_y <NEportLevels={0,2}>;
    float end_z <NEportLevels={0,2}>;
    int flip <NEportLevels={0,2}>;
  };
};

