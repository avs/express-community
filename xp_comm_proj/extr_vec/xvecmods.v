
flibrary ExtractVectorMods {

  // define the parameter block group

  group ExtractVectorParams <NEportLevels={0,1}> {
   int veclen <NEportLevels={2,2},export=2>;
   int components <NEportLevels={2,2},NEx=561.,NEy=187.,export=2>[veclen];
   int sub_components <NEportLevels={2,2},export=2>[veclen];
  };

  // no low-level modules
};
