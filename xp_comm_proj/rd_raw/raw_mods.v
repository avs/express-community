
flibrary ReadRawMods <build_dir="xp_comm_proj/rd_raw",
                      out_src_file="gen.cxx",
                      out_hdr_file="gen.hxx">
{ 

    module ReadRawCore<src_file="rd_raw.cxx"> {
        string+IPort2 filename;

        cxxmethod+req+notify_inst update(
            .filename+read+notify+req,
            .nnodes+read+write,
            .coords+read+write,
            .ntri+read+write,
            .conn+read+write
        );

        int nnodes;
        float+nres+OPort2 coords[nnodes][3] ;
        int ntri;
        int+nres+OPort2 conn[ntri][3];
    };

};

