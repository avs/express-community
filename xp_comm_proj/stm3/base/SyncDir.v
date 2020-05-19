module SyncDir<cxx_hdr_dirs="string.h"> {
  string+read+notify+IPort2 filename;
  string+read+notify+req+IPort2 ext;
  string+write+OPort2 srch;
  cxxmethod+req update = <"
     char tmp[128];
	 
	 if(ext.changed(seq_num))
	 {
     char tmp[6];
	 strcpy(tmp+2, (char *)ext);
	 tmp[0] = '*';
	 tmp[1] = '.';
     srch = tmp;
	 return 1;
	 }
     strcpy(tmp, filename);
#ifdef MSDOS
     char *p = strrchr(tmp, '\\');
#else
     char *p = strrchr(tmp, '/');
#endif
     if(p == NULL) p = tmp-1;
     p[1] = '*';
     p[2] = '.';

     strcpy(p+3, ext);
     srch = tmp;
     
     return 1;
  ">;
  cxxmethod+notify_inst+req OnInst = <"
     char tmp[6];
	 strcpy(tmp+2, (char *)ext);
	 tmp[0] = '*';
	 tmp[1] = '.';
     srch = tmp;
	 
	 return 1;
  ">;
};
