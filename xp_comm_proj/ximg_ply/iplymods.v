
flibrary AVSImagePlayerMods <cxx_name=""> {

   group ImagePlayerParams {
      string file_format<NEportLevels={2,2}>;
      int nb_images<NEportLevels={2,2}>;
      int first_image<NEportLevels={2,2}>;
      int step<NEportLevels={2,2}>;
      int access_mode<NEportLevels={2,2}>;
      int run_mode<NEportLevels={2,2}>;
      float fps<NEportLevels={2,2}>;
      int nsec_before<NEportLevels={2,2}>;
      int nsec_after<NEportLevels={2,2}>;
      int count<NEportLevels={2,2}>;
      string real_fps<NEportLevels={2,2}>;
   };


   module AVSImagePlayer < src_file="ximg_ply.cxx",
                           build_dir="xp_comm_proj/ximg_ply",
                           out_src_file="gen.cxx",
                           out_hdr_file="gen.hxx",
                           cxx_name="xImgPlay",
                           cxx_members="private:
                                        friend void AVSImagePlayer_LoopFuncTmp(char *arg);
                                        void LoopFunc(void);
                                        int  ReadImages(void);
                                        int  ReadImage(int image_no, int data_array_index,
                                                       char *module_name); ",
                           cxx_hdr_files="fld/Xfld.h"> {

      ImagePlayerParams+read+write+req &ImagePlayerParams<NEportLevels={2,0}>;

      int trigger_run<NEportLevels={2,0}>;
      int trigger_stop<NEportLevels={2,0}>;

      int+read+req data_array_size = 1;

      Data_Array+write+nonotify+nosave Data_Array[data_array_size] {
         veclen = 4;
         id = 669;
         values+byte;
         null_value+byte;
         min+byte;
         max+byte;
         min_vec+byte;
         max_vec+byte;
      };

      int node_data_index => switch((ImagePlayerParams.access_mode + 1),0,ImagePlayerParams.count);

      Mesh_Unif+Node_Data+write+nonotify out<NEportLevels={0,2}> {
         ndim = 2;
         nspace = 2;
         points => { {0,0}, {(dims[0] - 1),(dims[1] - 1)} };
         nnode_data = 1;
         &node_data => Data_Array[node_data_index];
      };

      GDM.DataObject DataObject {
         in => <-.out;
      };

      olink obj<NEportLevels={0,2}> => DataObject.obj;

      cxxmethod+req image_player_run( trigger_run+read+notify+req );
      cxxmethod+req image_player_stop( trigger_stop+read+notify+req );
   };
};

