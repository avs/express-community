#ifndef ReadVis5D_h
#define ReadVis5D_h

#include "v5d.h"

//
//	Structure that contains the global reader status
//
struct reader_status_struct {
	v5dstruct      *v5d;					//	Vis5D file descriptor
	int             totl;					//	Total number of levels
	bool	        caching;				//	True if the timesteps are read all at once
	unsigned char **ucache;					//	All the data for all the timesteps (in compressed format)
	float          *ga;						//	Used by the DecompressGrid routine
	float          *gb;						//	Used by the DecompressGrid routine
	float		   *tmp;					//	Temporary area used during decompression
	int				nvars2d;				//	Number of variables with only one level
	int				nvars3d;				//	Number of variables with more than one level
};

typedef struct reader_status_struct ReaderStatus;

#endif
