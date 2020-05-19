/*
 */

/*			Copyright (c) 1993, 1997 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.
	
*/

/*
 *  R E A D      P L O T 3 D  
 *
 *  AVS module reader for NASA plot3d files
 *
 *  Version 3.0   
 *
 *  31 August    1994   Michael Rangitsch, DOW
 *          Will only read multiblock files, (IAC version)
 *
 *   4 July      1995   ALEXY
 *          Added to AVS/Express 2.1 release for multiblock
 *          Reads all blocks into memory
 *
 *  15 Feb       1997   Ian Curington
 *          Re-work of reader code from the AVS5 UNSUPP/read_plot3d
 *
 *          Use the preprocessor flag DEBUG_PRINT to print out more information 
 *          
 *  12 March     1997   Ian Curington
 *          new min-max computation, avoiding OM calls 
 *          fixed Q file read bug with binary padding, thanks Aldo!
 *
 *  20 March     1997   Aldo Bergamini
 *          updated q file formatting
 *  19 June      1997   Aldo Bergamini
 *          fix nblocks number read for binary format, both xyz file and qfile.
 *  24 November  1997   Paul G. Lever
 *          modifications for IAC repository
 */

#include "xp_comm_proj/rdplot3d/gen.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <avs/util.h>
#include <avs/err.h>
#include <avs/om.h>
#include <avs/fld.h>
#include <avs/math.h>

#define  linelen  100
#define  true       1
#define  false      0

#define  MAX_BLOCKS  200
/*
 * To Enable Error reporting, uncomment the following symbol:
 */

/*
 * #define   DEBUG_PRINT 1
 */

/*
 * Macro to reset files, free temp memory, print an error message, and exit
 */
#define   READ_ERROR(str) { fclose(xyzfile); fclose(qfile); \
	  if (fields) free((char *)fields);  \
	  if (idim)   free((char *)idim);  \
	  if (idimq)  free((char *)idimq);  \
	  if (jdim)   free((char *)jdim);  \
	  if (jdimq)  free((char *)jdimq);  \
	  if (kdim)   free((char *)kdim);  \
	  if (kdimq)  free((char *)kdimq);  \
	  if (IBLANK) free((char *)IBLANK);  \
	  if (tint)   free((char *)tint);  \
	  if (tfloat) free((char *)tfloat);  \
          ERRerror(str,1, ERR_CHAIN, "read plot3D: "); return(0); \
          }

/*
 * Compound data types and macros
 */
typedef  float   XYZ[3];

typedef  int     VERTS[8]; /* for 1 hexahedral brick element */

#define  CELL(nb,i,j,k) \
          ((blk_offset[(nb)]) + (jdim[(nb)]*idim[(nb)]*(k)) + \
           (idim[(nb)]*(j)) + (i))

/*
 * bigendian - littlendian swap macro (assume IEEE floating point)
 */
#define  BYTE_SWAP(a) { tbyte=(char *) &(a);   \
			tb=tbyte[0];           \
			tbyte[0]=tbyte[3];     \
			tbyte[3]=tb;           \
			tb=tbyte[1];           \
                        tbyte[1]=tbyte[2];     \
			tbyte[2]=tb; } 

char *plot3d_labels[] = {"density","u-momentum","v-momentum", "w-momentum", "stagnation"};
			
char *plot3d_units[] = {"kg/m**3","kgm**2/s**2","kgm**2/s**2","kgm**2/s**2","N/m**2"};

char *plot3d_labels2[] = {"density","u-momentum","v-momentum", "stagnation"};
			
char *plot3d_units2[] = {"kg/m**3","kgm**2/s**2","kgm**2/s**2","N/m**2"};

#define FMT_FORMATTED   0
#define FMT_BINARY      1
#define FMT_UNFORMATTED 2

/******************************
 *                            *
 * Read a  PLOT3D data set    *
 *                            *
 ******************************/

/* Module description routine (3-arg version) */

int
read_plot3d_update(OMobj_id elem_id, OMevent_mask event_mask, int seq_num)
{
	OMobj_id e, out; 
	char *str1, *str2;
	int    iblanks, stat;
	int    fmt, status, use_q;
	int    trigger, dims3d, multiblock;

	/*
	 * only do anything if trigger is the last button press
	 */
	e = OMfind_subobj(elem_id, OMstr_to_name("trigger"), OM_OBJ_RD);
        if( OMchanged( e, seq_num ) )
		trigger = 1;
	else
		trigger = 0;
		

#ifdef DEBUG_PRINT
	printf("\nRead PLOT3D trigger = %d...\n", trigger);
#endif
	if (trigger == 0) return(1);

	e = OMfind_subobj(elem_id, OMstr_to_name("filename_xyz"), OM_OBJ_RD);
	if (OMget_str_val(e, &str1, 0) != 1) str1 = NULL;

	e = OMfind_subobj(elem_id, OMstr_to_name("filename_q"), OM_OBJ_RD);
	status = OMget_str_val(e, &str2, 0);
        if( status != OM_STAT_SUCCESS )	str2 = NULL;
        if( status == OM_STAT_UNDEF )   str2 = NULL;

	e = OMfind_subobj(elem_id, OMstr_to_name("iblanks"), OM_OBJ_RD);
	OMget_int_val(e, &iblanks);

	e = OMfind_subobj(elem_id, OMstr_to_name("format"), OM_OBJ_RD);
	OMget_int_val(e, &fmt);

	e = OMfind_subobj(elem_id, OMstr_to_name("use_q"), OM_OBJ_RD);
	OMget_int_val(e, &use_q);

	e = OMfind_subobj(elem_id, OMstr_to_name("dims3d"), OM_OBJ_RD);
	OMget_int_val(e, &dims3d);

	e = OMfind_subobj(elem_id, OMstr_to_name("multiblock"), OM_OBJ_RD);
	OMget_int_val(e, &multiblock);

	out = OMfind_subobj(elem_id, OMstr_to_name("out"), OM_OBJ_RW);

	stat = read_plot3d_entry(str1, str2,
                                 iblanks, fmt, use_q, dims3d, multiblock, out);

	if (str1) free(str1);
	if (str2) free(str2);

	return(stat);
}
		
/* ====================== */
/* module compute section */
/* ====================== */

int
read_plot3d_entry(XYZFileName, QFileName,
				iblanks, fmt, use_q, dims3d, multiblock, out)
    
    OMobj_id          out;
    char             *XYZFileName;
    char             *QFileName;
    int               iblanks, fmt, use_q;
    int	              dims3d, multiblock;
    
{
	int             data_veclen, name_flag, util_flag, error_flag;
	int             cell_tsize, cell_veclen;
	int             node_csize, node_veclen;
	int             conn_list[8];
	int            *comp_list=NULL;
	
	FILE 	*xyzfile=NULL, *qfile=NULL, *FILEfopen();
	char  line[linelen],c,buffer[20],blockname[34],patchname[67];
	
	/* most of the variables in all caps are read in from the PLOT3D 
	   data set.  the dead int and dead float variables are used to 
	   catch and discard information (like the cell face pointers and
	   cell face data values) which are not used to generate the Express Field
	   structure.  the cell face variables may be useful later....   */
	
	int i, j, k, l, m, nvals, nlines;
        int HaveQ;
	
	char  VARIABLENAME[28];
	
	int idata[10], nvar, nphase, npatch, nglue, ncells;
	int nun1, nnodes, ntime, nun2, nstored;
	
	int *ACTIVE_VARIABLES=NULL, *IBLK=NULL, *INFPCH=NULL, *INFGLU=NULL;
	int *LASTSET=NULL;
	int CYLINDRICAL=0;
	char first_word[linelen], dead_word[linelen], dead_word1[linelen];
	int deadint;
	
	float *XNN=NULL, *DATA=NULL;
	float deadfloat;
	
	float rad, theta, vrad, vtheta;
	int   node_numb, last_node_numb, iblk, *blk_offset=NULL;
	int   icell;
	VERTS *vertices=NULL;
	int   init_data;
	int   *node_index=NULL, *cell_used=NULL, *node_used=NULL;
	int   new_nodes, new_cells;
	
	float  min_extent[3], max_extent[3];
	float *node_minimum=NULL, *node_maximum=NULL;
	
	/* now define the parameters needed to remove the glued vertices */
	
	int   af,al,astep,bf,bl,bstep,cf,cl,cstep;
	int   i1,j1,k1,i2,j2,k2,iuse,juse,kuse;
	int   patch1,patch2,nwl1,nwl2,nblk1,nblk2;
	int   iptr,jptr,kptr;
	int   nrow1,ncol1;
	int   *cell1=NULL, *cell2=NULL, cell_no1, cell_no2, icelln;
	int   nrows,ncols,swd1,swd2;
	int   ni1,nj1,nk1,ni2,nj2,nk2;
	int   pointi[7],pointj[7],pointk[7];
	int   indices[7][4];
	int   iglue;
	int   vert1[4],vert2[4],tmp1,tmp2;
	int   lowi1,highi1,lowj1,highj1,lowk1,highk1;
	int   lowi2,highi2,lowj2,highj2,lowk2,highk2;
	int   switch_called[24],mcell;
	
	/* define the percent variables for status display */
	
	int   percent;
	char  message[30];

	/* define the geometry objects for internals */
	
	int            glues;
	
	char     name[10];
	int      *wall=NULL;
	int      *type=NULL;
	int      nintern, status;
	int      ifst,ilst,jfst,jlst,kfst,klst,niw;
	XYZ      verts[4];
	char     tname[40];
	
	/*  add additional parameters to read unformatted files */
	
	int      ftype;     /* flag for file type */
	char     circ[2],tmp_char[1];
	int      fint,lint,lena,size, ipad, byte_swap;
	
	
	int      nblocks, nblocksq, inode, noffset, itmpnode;
	int      *idim=NULL,*jdim=NULL,*kdim=NULL, *tint=NULL;
	int      *idimq=NULL,*jdimq=NULL,*kdimq=NULL;
	int      *IBLANK=NULL;
	float    *tfloat=NULL, *Q=NULL;
	float    fsmach, alpha, re, time;
	
	/* EXPRESS variables, used for array of fields and FLD API calls */
	OMobj_id          elm, *fields = NULL;
	int dims[3], nspace, ndim, ncomp, veclen, dtype, stat;
	float *coord, *node_data;
	float min, max, rmin[10], rmax[10];
	int   interrupt, dsz, padding, rd_size;
	char  *tbyte, tb;
	int   *ti;
	
#ifdef DEBUG_PRINT
	printf("Read PLOT3D starting...\n");
#endif

	/* check to see if we have a formatted or unformatted file */
	ftype = fmt;
	if ( ftype == FMT_BINARY )
		padding = 0;
	else
		padding = 1;

	/* only execute if a valid file names are passed */
	/* Must have a geometric grid at least! */	
	if ( XYZFileName == NULL || (int)strlen(XYZFileName) < 2) {
#ifdef DEBUG_PRINT
	        printf(" silent return because of no valid XYZ filename yet\n");
#endif
		return(1);
	}
	
	if ( ftype == FMT_FORMATTED )
	{
	    if( (xyzfile = (FILE *)FILEfopen(XYZFileName,SIO_R_TXT)) == (FILE *) NULL  ) {
		READ_ERROR("could not open Grid file for reading");
	    }
	} else {
	    if( (xyzfile = (FILE *)FILEfopen(XYZFileName,SIO_R_BIN)) == (FILE *) NULL  ) {
		READ_ERROR("could not open Grid file for reading");
	    }
	}

	/* Q file is optional */
	if ( QFileName == NULL || (int)strlen(QFileName) < 2)
	    HaveQ = 0;
        else
	    HaveQ = 1;
	
	/* user-interface override - must ignore */
	if ( use_q == 0 ) HaveQ = 0;

        if ( HaveQ ) {
	    if ( ftype == FMT_FORMATTED )
	    {
	        if( (qfile = (FILE *)FILEfopen(QFileName,SIO_R_TXT)) == (FILE *) NULL  ) {
#ifdef DEBUG_PRINT
		    printf("Couldn't open ASCII Q Result File: %s for reading.\n", QFileName);
#endif
		    HaveQ = 0;
	        }
	    } else {
	        if( (qfile = (FILE *)FILEfopen(QFileName,SIO_R_BIN)) == (FILE *) NULL  ) {
#ifdef DEBUG_PRINT
		    printf("Couldn't open BINARY Q Result File: %s for reading.\n", QFileName);
#endif
		    HaveQ = 0;
	        }
	    }
	}

	
#ifdef DEBUG_PRINT
	printf("file type = %d, HaveQ = %d, padding = %d, iblanks = %d, dims3d = %d\n",
                ftype,HaveQ,padding,iblanks, dims3d);
#endif

	
	byte_swap = 0;
	fint = lint = 0;

	if( ftype == FMT_FORMATTED)
	{
                /*
                 * ASCII formated file type
                 */
		if (multiblock)
		    fscanf(xyzfile,"%d",&nblocks);
		else
		    nblocks = 1;
	}
	else if ( ftype == FMT_BINARY || ftype == FMT_UNFORMATTED)
	{
                /*
                 * BINARY unformated file type
                 */
		if (multiblock)
		{
			if (padding)
			{
		    	   stat = fread( &ipad,    sizeof(int),1,xyzfile);
				if (stat != 1) READ_ERROR("single word padding fread failed 4");
			}
			stat = fread( &nblocks, sizeof(int),1,xyzfile);
			if (stat != 1) READ_ERROR("single word fread failed 2");
			if (padding)
			{
		    	   stat = fread( &ipad,    sizeof(int),1,xyzfile);
				if (stat != 1) READ_ERROR("single word padding fread failed 4");
			}
		} else {
			nblocks = 1;
		}
		/*
		 * Automatic test if byte swap needed Big-Little Endian swap
		 */
		if ( nblocks < 1 || nblocks > 100*MAX_BLOCKS ) byte_swap = 1;

		if (byte_swap) BYTE_SWAP( fint );
		if (byte_swap) BYTE_SWAP( nblocks );
		if (byte_swap) BYTE_SWAP( lint );


		if (padding)
		{
		    stat = fread( &ipad,    sizeof(int),1,xyzfile);
			if (stat != 1) READ_ERROR("single word padding fread failed 4");
		}
	}
	else
		READ_ERROR("invalid format parameter");

#ifdef DEBUG_PRINT
	if (byte_swap) printf(" automatic byte swap mode enabled\n");
	printf("xyz nblocks = %d\n",nblocks);
	if ( ftype == FMT_BINARY || ftype == FMT_UNFORMATTED)
	{
		printf("xyz fint = %d\n",fint);
		printf("xyz lint = %d\n",lint);
	}
#endif
	if ( nblocks > MAX_BLOCKS )
		READ_ERROR("xyz nblocks is too large, conversion error");

	elm = OMfind_subobj(out, OMstr_to_name("nblocks"), OM_OBJ_RD);
	OMset_int_val(elm, nblocks);

	if (nblocks == 0)
	{
		READ_ERROR("nblocks is interpreted as zero n xyz file, no work to do!");
	}

	fields = (OMobj_id *)malloc(nblocks*sizeof(OMobj_id));

	elm = OMfind_subobj(out, OMstr_to_name("fields"), OM_OBJ_RD);
	for (i=0; i< nblocks; i++) 
		if ((stat = OMget_array_val(elm, i, fields+i, OM_OBJ_RD)) != 1)
		    READ_ERROR("could not obtain obj id of sub-field in mblock");


#ifdef DEBUG_PRINT
	printf("allocd storage and filled array with field OMids\n");
#endif
	
	
	/*  now allocate and read in the dimensions array */
	
	
	idim = (int *)malloc( nblocks*sizeof(int) );
	
	jdim = (int *)malloc( nblocks*sizeof(int) );
	
	kdim = (int *)malloc( nblocks*sizeof(int) );
	
	if( ftype == FMT_FORMATTED) {
                /*
                 * ASCII formated file type
                 */
		for( i = 0; i < nblocks; i++ )	{
		    if ( dims3d )
			fscanf(xyzfile,"%d %d %d",&idim[i],&jdim[i],&kdim[i]);
		    else {
			fscanf(xyzfile,"%d %d",&idim[i],&jdim[i]);
		 	kdim[i] = 1;
		    }
#ifdef DEBUG_PRINT
			printf(" block %d, ni %d, nj %d, nk %d\n",i,idim[i],jdim[i],kdim[i]);
#endif
		}
	}
	else if ( ftype == FMT_BINARY || ftype == FMT_UNFORMATTED)
	{
                /*
                 * BINARY unformated file type
                 */
		/* IAC CODE CHANGE :      if( tint ) free(tint); */
		if( tint )  free(tint);
		tint = (int *)malloc( ((dims3d+2)*nblocks+2)*sizeof(int) );
		if( tint == NULL )
			READ_ERROR("failed to allocate memory for dims arrays");


		stat = fread( tint,sizeof(int),(dims3d+2)*nblocks+padding,xyzfile); 
		if ( stat != (dims3d+2)*nblocks+padding )
			READ_ERROR("fread failed on xyzfile on nblocks dims");

                /*
		 * automatic byte swap test for single block binary
		 */
		if ((tint[0] < 1 || tint[0] > 500000 ))
		    byte_swap = 1;
#ifdef DEBUG_PRINT
		if (byte_swap) printf(" automatic byte swap mode enabled\n");
#endif

		if (byte_swap) for( i = 0; i < (dims3d+2)*nblocks; i++ ) 
			BYTE_SWAP(tint[i]);

		j = 0;
		for( i = 0; i < nblocks; i++ ) {
			idim[i] = tint[j];
			jdim[i] = tint[j+1];
			if ( dims3d ) {
				kdim[i] = tint[j+2];
				j = j + 3;
			} else {
				kdim[i] = 1;
				j = j + 2;
			}
#ifdef DEBUG_PRINT
			printf(" block %d, ni %d, nj %d, nk %d\n ",i,idim[i],jdim[i],kdim[i]);
#endif
		}
	}
	else
	{
#ifdef DEBUG_PRINT
			printf(" should never get here if UI and parameter passing are OK\n ");
#endif
		READ_ERROR("invalid format parameter");
	}
	
	/*
	 * check for silly conversion errors
	 */
	for( i = 0; i < nblocks; i++ ) {
#ifdef DEBUG_PRINT
		printf(" sanity check dimensions for block %d\n ",i);
#endif
		if (idim[i] < 1 || idim[i] > 50000000 ||
		    jdim[i] < 1 || jdim[i] > 50000000 ||
		    kdim[i] < 1 || kdim[i] > 50000000 )
		READ_ERROR("block ijk dimensions found to be unreasonable");
	}


	/* now set the sizes and read in the coordinate arrays */
	
	nnodes = 0;
	nspace = 3;
	for( i = 0; i < nblocks; i++) {
		nnodes = nnodes + idim[i]*jdim[i]*kdim[i];
#ifdef DEBUG_PRINT
		printf(" setting nspace = %d on output field no. %d\n ",nspace, i);
#endif
		if (FLDset_nspace(fields[i], nspace) != 1) {
			READ_ERROR("Error setting nspace");
		}
		if (dims3d)	ndim = 3;
		else 		ndim = 2;

#ifdef DEBUG_PRINT
		printf(" setting ndim = %d on output field no. %d\n ",ndim, i);
#endif
		if (FLDset_ndim(fields[i], ndim) != 1) {
			READ_ERROR("Error setting ndim");
		}

#ifdef DEBUG_PRINT
		printf(" setting dims on output field no. %d, ni %d, nj %d, nk %d\n ",
			i,idim[i],jdim[i],kdim[i]);
#endif
		dims[0] = idim[i];
		dims[1] = jdim[i];
		dims[2] = kdim[i];
		if (FLDset_dims (fields[i], dims) != 1) {
			READ_ERROR("Error setting dims");
		}
	}

	if(iblanks) 
	{
#ifdef DEBUG_PRINT
		printf(" allocate IBLANK memory for %d ints\n ",nnodes);
#endif
		IBLANK = (int *)malloc( nnodes*sizeof(int) );
	}

	if( ftype == FMT_FORMATTED) {
                /*
                 * ASCII formated file type
                 */
		noffset = 0;
		for( l = 0; l < nblocks; l++ )	{
			OMstatus_check(50*l/nblocks,"read_mesh_update",&interrupt);
			if (interrupt) READ_ERROR("user interrupt from middle of plot3d");

			if (FLDget_coord(fields[l],(float **)&coord, &size, OM_GET_ARRAY_WR) != 1) {
				READ_ERROR("Error setting coordinate array");
			}
			for(i = 0; i < idim[l]*jdim[l]*kdim[l]; i++ )
				fscanf( xyzfile, "%f", coord+i*nspace );
			
			for(i = 0; i < idim[l]*jdim[l]*kdim[l]; i++ )
				fscanf( xyzfile, "%f",  coord+i*nspace+1 );
			
			if (dims3d)
				for(i = 0; i < idim[l]*jdim[l]*kdim[l]; i++ )
					fscanf( xyzfile, "%f", coord+i*nspace+2 );
			else
				for(i = 0; i < idim[l]*jdim[l]*kdim[l]; i++ )
					*(coord+i*nspace+2) = 0.0;

			
			if( iblanks ) { 
				for(i = 0; i < idim[l]*jdim[l]*kdim[l]; i++ )
					fscanf( xyzfile, "%d", &IBLANK[noffset+i] ); 
			}
			noffset = noffset + idim[l]*jdim[l]*kdim[l];
			ARRfree((char *)coord);
		}
	}
	else if ( ftype == FMT_BINARY || ftype == FMT_UNFORMATTED)
	{
                /*
                 * BINARY unformated file type
                 */
		noffset = 0;
		inode = 0;
		
		for( l = 0; l < nblocks; l++ )	{

			OMstatus_check(50*l/nblocks,"read_mesh_update",&interrupt);
			if (interrupt) READ_ERROR("user interrupt from middle of plot3d");


			if (FLDget_coord(fields[l],(float **)&coord, &size, OM_GET_ARRAY_WR) != 1) {
				READ_ERROR("Error setting coordinate array");
			}
			
			dsz = idim[l]*jdim[l]*kdim[l];
			rd_size = (iblanks+dims3d+2)*dsz;

#ifdef DEBUG_PRINT
		printf(" number of nodes in  block %d, is = %d, rd_size = %d\n ", l, dsz, rd_size);
#endif
			if(tfloat) {
#ifdef DEBUG_PRINT
		printf(" free-ing previously allocated tfloat array");
#endif
				  free(tfloat);
			}

			tfloat = (float *)malloc( (dsz + padding)*sizeof(float) );
			if (tfloat == NULL) 
				READ_ERROR("Error allocating mem for temp coord array");

			if (padding)
			{
			    stat = fread(tfloat,sizeof(float),(padding),xyzfile);
			    if ( stat != (padding) )
				READ_ERROR("fread failed on padding");
			}
#ifdef DEBUG_PRINT
		printf(" got the memory, try binary read for X block %d\n ", l);
#endif

			stat = fread(tfloat,sizeof(float),(dsz),xyzfile);
			if ( stat != (dsz) )
				READ_ERROR("fread failed on binary xyzfile coordinates");
#ifdef DEBUG_PRINT
		printf("                     binary read for X block %d OK\n ", l);
#endif

			if (byte_swap) for( i = 0; i < dsz; i++ ) 
				BYTE_SWAP( tfloat[i] );

			for ( i = 0; i < dsz; i++ ) 
				coord[i*nspace]   = tfloat[i];

#ifdef DEBUG_PRINT
		printf(" try binary read for Y block %d\n ", l);
#endif

			stat = fread(tfloat,sizeof(float),(dsz),xyzfile);
			if ( stat != (dsz) )
				READ_ERROR("fread failed on binary xyzfile coordinates");
#ifdef DEBUG_PRINT
		printf("     binary read for Y block %d OK\n ", l);
#endif

			if (byte_swap) for( i = 0; i < dsz; i++ ) 
				BYTE_SWAP( tfloat[i] );

			for ( i = 0; i < dsz; i++ )  
				coord[i*nspace+1] = tfloat[i];

			if (dims3d)
			{
#ifdef DEBUG_PRINT
		printf(" try binary read for Z block %d\n ", l);
#endif
			    stat = fread(tfloat,sizeof(float),(dsz),xyzfile);
			    if ( stat != (dsz) )
				READ_ERROR("fread failed on binary xyzfile coordinates");
#ifdef DEBUG_PRINT
		printf("     binary read for Z block %d OK\n ", l);
#endif

			    if (byte_swap) for( i = 0; i < dsz; i++ ) 
				BYTE_SWAP( tfloat[i] );

			    for ( i = 0; i < dsz; i++ )  
					coord[i*nspace+2] = tfloat[i];
			}
			else
			{
			    for ( i = 0; i < dsz; i++ ) 
					coord[i*nspace+2] = 0.0;
			}

			if(iblanks)
			{
#ifdef DEBUG_PRINT
		printf(" read and load iblank array for  block %d\n ", l);
#endif
			    stat = fread(tfloat,sizeof(float),(dsz),xyzfile);
			    if ( stat != (dsz) )
				READ_ERROR("fread failed on binary xyzfile coordinates");
#ifdef DEBUG_PRINT
		printf("     binary read for iblank  block %d OK\n ", l);
#endif

			    if (byte_swap) for( i = 0; i < dsz; i++ ) 
				BYTE_SWAP( tfloat[i] );

			    for ( i = 0; i < dsz; i++ )  
				    IBLANK[i+noffset] = (int)tfloat[i];
			}
			noffset = noffset + dsz;  

			if (padding)
			{
			    stat = fread(tfloat,sizeof(float),(padding),xyzfile);
			    if ( stat != (padding) )
				READ_ERROR("fread failed on padding");
			}

			ARRfree((char *)coord);
			if(tfloat)  {
				free(tfloat);
				tfloat = NULL;
			}
		}
	}
	else
		READ_ERROR("invalid format parameter");

	/* ====================== */
	/* End of XYZ file read   */
	/* ====================== */

#ifdef DEBUG_PRINT
		printf(" mesh read finished, file closed.\n ");
#endif


	/* ====================== */
	/* now read in the Q file */
	/* ====================== */
	if ( HaveQ )  /* optional, will work with grid only */
	{
	
	
#ifdef DEBUG_PRINT
	printf("ftype = %d\n",ftype);
#endif
	
	/*
	 * assume type is same as mesh
	 */
	if( ftype == FMT_FORMATTED) {
                /*
                 * ASCII formated file type
                 */
		if (multiblock)
		    fscanf(qfile,"%d",&nblocksq);
		else
		    nblocksq = 1;
	}
	else if ( ftype == FMT_BINARY || ftype == FMT_UNFORMATTED)
	{
                /*
                 * BINARY unformated file type
                 */
		if (multiblock)
		{
			if (padding)
			{
		    	   stat = fread( &ipad,    sizeof(int),1,qfile);
				if (stat != 1) READ_ERROR("single word padding fread failed 4");
			}
			stat = fread( &nblocksq,sizeof(int),1,qfile);
			if (stat != 1) READ_ERROR("single word fread failed");
			if (padding)
			{
		    	   stat = fread( &ipad,    sizeof(int),1,qfile);
				if (stat != 1) READ_ERROR("single word padding fread failed 4");
			}

			if (byte_swap) BYTE_SWAP( fint );
			if (byte_swap) BYTE_SWAP( nblocksq );
			if (byte_swap) BYTE_SWAP( lint );
		}
		else
		    nblocksq = 1;

		if (padding)
		{
		    stat = fread( &ipad,    sizeof(int),1,qfile);
			if (stat != 1) READ_ERROR("single word fread failed");
		}
	}
	else
		READ_ERROR("invalid format parameter");

#ifdef DEBUG_PRINT
	printf("q nblocks = %d\n",nblocksq);
	if ( ftype == FMT_BINARY || ftype == FMT_UNFORMATTED)
	{
		printf("q fint = %d\n",fint);
		printf("q lint = %d\n",lint);
	}
#endif
	if ( nblocksq > MAX_BLOCKS )
		READ_ERROR("q file nblocks is too large, conversion error");
	
	/*  now allocate and read in the dimensions array */
	
	
	idimq = (int *)malloc( nblocksq*sizeof(int) );
	
	jdimq = (int *)malloc( nblocksq*sizeof(int) );
	
	kdimq = (int *)malloc( nblocksq*sizeof(int) );
	
	if( ftype == FMT_FORMATTED)
	{
                /*
                 * ASCII formated file type
                 */
		for( i = 0; i < nblocksq; i++ )
		{
		    if (dims3d)
			fscanf(qfile,"%d %d %d",&idimq[i],&jdimq[i],&kdimq[i]);
		    else {
			fscanf(qfile,"%d %d",&idimq[i],&jdimq[i]);
			kdimq[i] = 1;
		    }
#ifdef DEBUG_PRINT
			printf(" ASCII block %d, ni %d, nj %d, nk %d\n",i,idimq[i],jdimq[i],kdimq[i]);
#endif
		}
	}
	else if ( ftype == FMT_BINARY || ftype == FMT_UNFORMATTED)
	{
                /*
                 * BINARY unformated file type
                 */
		
		/* IAC CODE CHANGE :      if( tint ) free(tint); */
		if( tint )  free(tint);
		tint = (int *)malloc( ((dims3d + 2)*nblocksq+2)*sizeof(int) );

		stat = fread( tint, sizeof(int), (dims3d+2)*nblocksq+padding,qfile); 
		if ( stat != (dims3d+2)*nblocksq+padding )
			READ_ERROR("fread failed on qfile on nblocks dims");

		if (byte_swap) for( i = 0; i < (dims3d+2)*nblocksq; i++ ) 
			BYTE_SWAP(tint[i]);

		j = 0;
		for( i = 0; i < nblocksq; i++ )
		{
			idimq[i] = tint[j];
			jdimq[i] = tint[j+1];
			if (dims3d) {
				kdimq[i] = tint[j+2];
				j = j + 3;
			} else {
				kdimq[i] = 1;
				j = j + 2;
			}
#ifdef DEBUG_PRINT
			printf(" BINARY block %d, ni %d, nj %d, nk %d\n ",i,idimq[i],jdimq[i],kdimq[i]);
#endif
		}
	}
	else
		READ_ERROR("invalid format parameter");
   
	/* check for file consistency */
	
	if( nblocks != nblocksq )  {
		READ_ERROR("xyz and q files have different block counts");
	}
	
	for( l = 0; l < nblocksq; l++ )
	{
		if( idim[l] != idimq[l] ||
		    jdim[l] != jdimq[l] ||
		    kdim[l] != kdimq[l] )
		{
			READ_ERROR("xyz and q files have inconsistant block dimensions");
		}
	}
	
	/*****************/
	/** READ Q DATA **/
	/*****************/
	ncomp  = 4 + dims3d;
	veclen = 1;

	if( ftype == FMT_FORMATTED)
	{
                /*
                 * ASCII formatted file type
                 */
		for ( l = 0; l < nblocksq; l++ )	{

			OMstatus_check(50 + 50*l/nblocks,"read_result_update",&interrupt);
			if (interrupt) READ_ERROR("user interrupt from middle of plot3d");

			if (FLDset_node_data_ncomp (fields[l], ncomp) != 1) {
					READ_ERROR("Error setting nnode_data");
			} 			

			fscanf(qfile,"%f %f %f %f",&fsmach,&alpha,&re,&time);
			for( m = 0; m < ncomp; m++ ) {
				if (dims3d)
				{
				    if ( FLDset_node_data_comp (fields[l], m, veclen,
						 plot3d_labels[m], plot3d_units[m]) != 1) {
					READ_ERROR("error setting 3D node data component");
				    }
				}
				else
				{
				    if ( FLDset_node_data_comp (fields[l], m, veclen,
						 plot3d_labels2[m], plot3d_units2[m]) != 1) {
					READ_ERROR("error setting 2D node data component");
				    }
				}
				dtype = DTYPE_FLOAT;
				if (FLDget_node_data(fields[l], m, &dtype, (char **)&node_data, &size,
						     OM_GET_ARRAY_WR) != 1) {
					READ_ERROR("Error getting node data");
				} 
				inode = 0;
				for( k = 0; k < kdimq[l]; k++ )
					for( j = 0; j < jdimq[l]; j++ )
						for( i = 0; i < idimq[l]; i++ ) 
							fscanf(qfile,"%f",&node_data[inode++]);
				/*
				 * estimate min-max of each component
				 */
				if (l==0) {
					rmin[m] = rmax[m] = node_data[1];
				}
				dsz = idimq[l]*jdimq[l]*kdimq[l];
				for( i = 0; i < dsz; i++ ) 
				{
					if (node_data[i] < rmin[m])
						rmin[m] = node_data[i];
					if (node_data[i] > rmax[m])
						rmax[m] = node_data[i];
				}

				ARRfree((char *)node_data);
			}
		}
	}
	else if ( ftype == FMT_BINARY || ftype == FMT_UNFORMATTED)
	{
                /*
                 * BINARY unformated file type
                 */

		for( l = 0; l < nblocksq; l++ ) 	{


		/* IAC CODE CHANGE :       if(tfloat) free(tfloat); */
		if(tfloat)  free(tfloat);
		tfloat = (float *)malloc( (4+2)*sizeof(float) );
/* change from Aldo Bergamini to check padding 13/3/1997   */
		if (padding)
		{
#ifdef DEBUG_PRINT
			printf("    ...padding read...\n ");
#endif
    			stat = fread( &ipad,    sizeof(int),1,qfile);
			if (stat != 1) READ_ERROR("single padding fread failed");
		}
		stat = fread(tfloat,sizeof(float),4,qfile);
			if (stat != 4) READ_ERROR("4 vars fread failed");
		if (padding)
		{
#ifdef DEBUG_PRINT
			printf("    ...padding read...\n ");
#endif
    			stat = fread( &ipad,    sizeof(int),1,qfile);
			if (stat != 1) READ_ERROR("single padding fread failed");
		}

		if (byte_swap) for( i = 0; i < 4; i++ ) 
			BYTE_SWAP( tfloat[i] );

#ifdef DEBUG_PRINT
		printf("q block header, %f %f %f %f %f %f\n",
			tfloat[0],tfloat[1],tfloat[2],tfloat[3],tfloat[4],tfloat[5]);
#endif

		inode = 0;
		noffset = 0;
/* changed from Aldo Bergamini 13/3/1997 : padding check is done
   before and after reading all components for each block         */
		/*
		 * is padding needed before and after each read
		 */
		if (padding)
		{
#ifdef DEBUG_PRINT
		printf("    ...padding read...\n ");
#endif
    			stat = fread( &ipad,    sizeof(int),1,qfile);
			if (stat != 1) READ_ERROR("single padding fread failed");
		}

		OMstatus_check(50 + 50*l/nblocks,"read_result_update",&interrupt);
		if (interrupt) READ_ERROR("user interrupt from middle of plot3d");

		if (FLDset_node_data_ncomp (fields[l], ncomp) != 1) {
				READ_ERROR("Error setting nnode_data");
		} 			

		dsz = idimq[l]*jdimq[l]*kdimq[l];
			
		itmpnode = 0;
		for( m = 0; m < ncomp; m++ ) {
			if (dims3d)
			{
			    if ( FLDset_node_data_comp (fields[l], m, veclen,
				 plot3d_labels[m], plot3d_units[m]) != 1) {
					READ_ERROR("error setting 3D node data component");
			    }
			}
			else
			{
			    if ( FLDset_node_data_comp (fields[l], m, veclen,
				 plot3d_labels2[m], plot3d_units2[m]) != 1) {
				READ_ERROR("error setting 2D node data component");
			    }
			}
			dtype = DTYPE_FLOAT;
			if (FLDget_node_data(fields[l], m, &dtype, (char **)&node_data, &size,
				     OM_GET_ARRAY_WR) != 1) {
				READ_ERROR("Error getting node data");
			} 



#ifdef DEBUG_PRINT
		printf(" Reading Q BINARY block %d, component %d, size %d\n ",l,m,dsz);
#endif

			stat = fread(node_data,sizeof(float),dsz,qfile);
			if ( stat != dsz )
			{
#ifdef DEBUG_PRINT
		printf("    read failed, only got %d words when expected %d\n",stat,dsz);
#endif
				READ_ERROR("fread failed on qfile values");
			}

			if (byte_swap) for( i = 0; i < dsz; i++ ) 
				BYTE_SWAP( node_data[i] );

			/*
			 * estimate min-max of each component
			 */
			if (l==0) {
				rmin[m] = rmax[m] = node_data[1];
			}
			for( i = 0; i < dsz; i++ ) 
			{
				if (node_data[i] < rmin[m])
					rmin[m] = node_data[i];
				if (node_data[i] > rmax[m])
					rmax[m] = node_data[i];
			}

			ARRfree((char *)node_data);
		}
		noffset = noffset + dsz; 

		if(tfloat) {
			free(tfloat);
			tfloat = NULL;
		}
		/*
		 * is padding needed before and after each read
		 */
		if (padding)
		{
#ifdef DEBUG_PRINT
		printf("    ...padding read...\n ");
#endif
    			stat = fread( &ipad,    sizeof(int),1,qfile);
			if (stat != 1) READ_ERROR("single padding fread failed");
		}
		}
	}
	else
		READ_ERROR("invalid format parameter");


	OMstatus_check(95,"plot3d min-max set",&interrupt);

	/*
	 * Set the Min-Max ranges on all components of all blocks for consistancy
	 */
	for ( l = 0; l < nblocksq; l++ )	{
		if (FLDget_node_data_ncomp (fields[l], &ncomp) != 1) {
			READ_ERROR("Error setting nnode_data");
		}
		for( m = 0; m < ncomp; m++ ) {
			if (FLDset_node_data_minmax(fields[l], m, 
						    (char *)&rmin[m], (char *)&rmax[m], DTYPE_FLOAT) != 1) {
				READ_ERROR("Error setting minmax data");
			}
		}
	}

	fclose (qfile);

	}
	else
	{
		/*
		 * Did not read any Q file, set components to zero
		 */
#ifdef DEBUG_PRINT
		printf(" Q file skipped, remove node data from field\n");
#endif
		for( l = 0; l < nblocks; l++ ) 	{
			if (FLDset_node_data_ncomp (fields[l], 0) != 1) {
				READ_ERROR("Error setting nnode_data to zero");
			} 			
		}

	}  /* end of Q-file processing option test */

	/* ================ */
	/* end of data read */
	/* ================ */

	fclose (xyzfile);

	OMstatus_check(99,"plot3d final",&interrupt);

	
#ifdef DEBUG_PRINT
	printf(" all data has been read from plot3d files\n");
#endif

	free((char *)fields);
	free((char *)idim);
	free((char *)idimq);
	free((char *)jdim);
	free((char *)jdimq);
	free((char *)kdim);
	free((char *)kdimq);
	if (IBLANK)  free((char *)IBLANK);
	if (tint)    free((char *)tint);
	if (tfloat)  free((char *)tfloat);

	return(1); /* fault free return */
}

