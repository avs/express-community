/*
changes for PC lowlevel i/o functions
*/
#ifdef WIN32

#include <io.h>

#define lseek		_lseek
#define open		_open
#define close		_close
#define read		_read
#define write		_write


#endif

