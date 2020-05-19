
flibrary ExcavateBrickMods <build_dir="xp_comm_proj/exbrk",
					out_hdr_file="gen.h"> {

  // define the parameter block group
  group ExcavateBrickParams <NEportLevels={0,1}> {
    int+Port2 X;
    int+Port2 Y;
    int+Port2 Z;
    boolean+Port2 belowX;
    boolean+Port2 belowY;
    boolean+Port2 belowZ;
   };

    module ExcavateBrick<src_file="exbrk.c"> {
    omethod+notify_inst Create() = "ExcavateBrickCreate";
    omethod+notify_deinst Delete() = "ExcavateBrickDelete";

    ExcavateBrickParams+IPort2 &ExcavateBrickParams;
    Mesh_Unif+Node_Data+IPort2 &Input_Field;

    omethod+req UpdateField(.X+read,
                            .Y+read,
                            .Z+read,
                            .belowX+read+notify,
                            .belowY+read+notify,
                            .belowZ+read+notify,
                            .Input_Field+read+notify+req,
                            .Output_Field+write
                            ) = "ExcavateBrickUpdFld";

    omethod+req UpdateXYZ(.X+read+notify,
                          .Y+read+notify,
                          .Z+read+notify,
                          .belowX+read,
                          .belowY+read,
                          .belowZ+read,
                          .Input_Field+read+req,
                          .Output_Field+write
                          ) = "ExcavateBrickUpdXYZ";

    int X =>ExcavateBrickParams.X;
    int Y =>ExcavateBrickParams.Y;
    int Z =>ExcavateBrickParams.Z;
    boolean belowX =>ExcavateBrickParams.belowX;
    boolean belowY =>ExcavateBrickParams.belowY;
    boolean belowZ =>ExcavateBrickParams.belowZ;
    Mesh_Unif+Node_Data+OPort2 Output_Field;
    ptr localptr<NEvisible=0> = 0;
  };
};
