
flibrary CopyXXXOnTriggerMods {

  // define the low-level modules

  module modCopyOnTrigger <compile_subs = 0,
                           need_objs = "GMOD.copy_on_change"> {
    prim trigger<NEportLevels={2,0}>;
    prim input<NEportLevels={2,0}>;
    prim output<NEportLevels={0,2}>;
    boolean on_inst = 1;
    omethod+notify_inst+req Update(
      .trigger+req+notify,
      .input+req+read,
      .output+write
    ) = "GMODcopy_on_change";
  };

  module modCopyArrayOnTrigger <compile_subs = 0,
                                need_objs = "GMOD.copy_on_change"> {
    prim trigger<NEportLevels={2,0}>;
    prim input<NEportLevels={2,0}> [];
    prim output<NEportLevels={2,0}> [];
    boolean on_inst = 1;
    omethod+notify_inst+req Update(
      .trigger+req+notify,
      .input+read,
      .output+write
    ) = "GMODcopy_on_change";
  };

  module modCopyGroupOnTrigger <compile_subs = 0,
                                need_objs = "GMOD.copy_on_change"> {
    prim trigger<NEportLevels={2,0}>;
    group &input<NEportLevels={2,0}>;
    group &output<NEportLevels={2,0}>;
    boolean on_inst = 1;
    omethod+notify_inst+req Update(
      .trigger+req+notify,
      .input+req+read,
      .output+req+write
    ) = "GMODcopy_on_change";
  };

  module modCopyGroupArrayOnTrigger <compile_subs = 0,
                                     need_objs = "GMOD.copy_on_change"> {
    prim trigger<NEportLevels={2,0}>;
    group &input<NEportLevels={2,0}> [];
    group &output<NEportLevels={2,0}> [];
    boolean on_inst = 1;
    omethod+notify_inst+req Update(
      .trigger+req+notify,
      .input+req+read,
      .output+write
    ) = "GMODcopy_on_change";
  };

  module modCopyOnPtrTrigger <compile_subs = 0,
                              need_objs = "GMOD.copy_on_change"> {
    group *trigger<NEportLevels={2,0}>;
    prim input<NEportLevels={2,0}>;
    prim output<NEportLevels={0,2}>;
    boolean on_inst = 1;
    omethod+notify_inst+req Update(
      .trigger+req+notify,
      .input+req+read,
      .output+write
    ) = "GMODcopy_on_change";
  };
};
