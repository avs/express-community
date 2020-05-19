
flibrary CopyOnTriggerMods {

  // define the low-level module

  module modCopyOnTrigger <compile_subs = 0> {
    prim trigger <NEportLevels={2,0}>;
    prim input <NEportLevels={2,0}>;
    prim output <NEportLevels={0,2}>;
    boolean on_inst = 1;

    omethod+notify_inst+req Update(
      .trigger +req+notify,
      .input   +read+req,
      .output  +write
    ) = "GMODcopy_on_change";
  };

  module modCopyOnNonReqTrigger <compile_subs = 0> {
    prim trigger <NEportLevels={2,0}>;
    prim input <NEportLevels={2,0}>;
    prim output <NEportLevels={0,2}>;
    boolean on_inst = 1;

    omethod+notify_inst+req Update(
      .trigger +notify,
      .input   +read,
      .output  +write
    ) = "GMODcopy_on_change";
  };
};
