VLILookupTable VLILookupTable<NEportLevels={0,1}> {
  red => init_array(.size,0,(.size - 1));
  size = 256;
  green => init_array(.size,0,(.size - 1));
  blue => init_array(.size,0,(.size - 1));
  alpha => concat_array(init_array(85,0,0),init_array(171,4095,4095));
};
