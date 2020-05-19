c***********************************************************************
c  READP_F: main entry point for Read_Phoenics module
c  Author: David Knight, AVS(UK) Ltd
c  Creation Date: 26.8.97
c  Notes:
c***********************************************************************
      integer function READP_F(OBJ_ID, MASK, SEQ_NUM)
      include 'avs/omf.inc'
C **
C PARAMETERS DEFINING INTERNAL ARRAY SIZES, THESE MAY NEED TO BE
C TO BE INCREASED FOR LARGE PROBLEMS. HERE 'BIG GRID' MEANS THE
C ENTIRE MULTIBLOCK MESH
C **
c .. Maximum number of bodies
      PARAMETER(IBMAX=100)
c .. Maximum number of links
      PARAMETER(NLMAX=100)
c .. Maximum number of nodes in the big grid
      PARAMETER(NODMAX=100000)
c .. Maximum number of elements in the big grid
      PARAMETER(IELMAX=100000)
c .. Maximum number of nodal results (multiple of NODMAX)
      PARAMETER(NRMAX = NODMAX * 2)
c .. If NRMAX*3 + IELMAX is greater than 8*IELMAX IWORK must be
c .. dimensioned to   NRMAX*3 + IELMAX
      LOGICAL XDIR,PDIR,BFC,CARTES,LOG(50),NOBLK,COMM,CCC1
      LOGICAL ONEPHS,CRT1,CCC2,CRT2,FVPOR,FPRPS
      CHARACTER*4 NAME(50)
      CHARACTER*40 TITLE
      CHARACTER*80 PHINAM,XYZNAM,PATNAM
      CHARACTER*40 MESSAGE
      INTEGER IJKBLK(6,IBMAX),NMAP(NODMAX),IWORK(8*IELMAX)
      INTEGER LINK(11,NLMAX),LIMBLK(3,IBMAX),NRESV(50)
      REAL WORK(3*NODMAX + 3*NRMAX),XFRAC(256),TEMP(NODMAX)
c .. express objects
      INTEGER OBJ_ID(OIDSIZ), FLD_ID(OIDSIZ), CDA_ID(OIDSIZ),
     .	NDA_ID(OIDSIZ)
      INTEGER MASK, SEQ_NUM
c .. IO units
      IOPAT = 50
c .. end NEP stuff to stdout
      IONEP = 51
      IOPHI = 52
      IOPHDA = 53
      IOGRD = 54   
      IOPRE = 55
      IOCON = 56
      COMM = .TRUE.
      MESSAGE = ' '
c .. get the input parameters
      CALL GET_PARAMS(OBJ_ID,PHINAM,XYZNAM,PATNAM,
     .	PDIR,XDIR,FLD_ID)
c .. assumed size of reals and ints (increase for 64bit)
      LBYTE = 4
      LENREF = 256
c .. open the NEP file
      CALL ONEPNP(IONEP,IERR)
      IF (IERR.GT.0) GOTO 95
c .. open the PATGEO file, if there isn't one, then use
c .. default parameters from the PHI file
      IF(COMM) WRITE(*,*)'Opening PATGEO file'
      CALL OPATNP(IONEP,IOPAT,PATNAM,IERR)
c .. no patgeo file
      IF (IERR.GT.0) THEN
        WRITE(*,*) 'PATGEO file not found. Using defaults.'
      ENDIF
      IERR = 0
c .. Open the PHI file, create a temp DA file if required
      WRITE(*,*)'Opening PHI file'
      IF(PDIR) THEN
        LEN = LBYTE*LENREF     
c ..   Open with correct record length
         OPEN(IOPHDA,FILE=PHINAM,STATUS='OLD',RECL=LEN,
     +        ACCESS='DIRECT',ERR=97)
c ..   ASCII file
      ELSE
c ..   Open the PHI file
        OPEN(IOPHI,FILE=PHINAM,STATUS='OLD',ERR=97) 
        LEN = LBYTE*LENREF
        OPEN(IOPHDA,STATUS='SCRATCH',RECL=LEN,
     +      ACCESS='DIRECT',ERR=97)
c ..   Convert to DA         
        CALL CPHINP(IONEP,IOPHI,IOPHDA,IWORK,WORK,LENREF,IERR)                     
        IF(IERR .GT. 0) GOTO 97
      ENDIF  
c .. read and output the file title
      CALL RPHINP(IOPHDA,TITLE,NX,NY,NZ,LOG,NAME,CARTES,ONEPHS,BFC,
     .            IONEP,IERR)
      IF(IERR .GT. 0) GOTO 99
      WRITE(IONEP,'(A40)') TITLE
c .. Set the title string
      CALL WRITE_TITLE(OBJ_ID,TITLE)
c .. size of the mesh
      NEBIG = NX*NY*NZ
      NNBIG = (NX+1)*(NY+1)*(NZ+1)
c .. read the block and inter block patches
      if (COMM) WRITE(*,*)'Reading PATGEO file'
      call RPATNP(IONEP,IOPAT,IBMAX,NODMAX,NLMAX,NX,NY,NZ,IJKBLK,NMAP,
     +             NBLOCK,NUMNOD,LINK,NLINK,IERR)
      IF(IERR .GT. 0) GOTO 95
c .. write the block definitions
c .. get the model coordinates
      if (COMM) write(*,*) 'Reading the model coordinates'
      if (BFC) then
      	if (XDIR) then
          LEN = LBYTE*LENREF
          OPEN(UNIT=IOGRD,FILE=XYZNAM,STATUS='OLD',ERR=92,RECL=LEN,
     +                   ACCESS='DIRECT')
          LENX = LENREF
        ELSE
          OPEN(UNIT=IOGRD,FILE=XYZNAM,STATUS='OLD',ERR=92)
          LENX = 5
        ENDIF
        CALL RNODNP(IONEP,IOGRD,XDIR,IJKBLK,NBLOCK,LENX,
     +           NODMAX,NX,NY,NZ,WORK(3*NODMAX+1),WORK,IERR)
        IF(IERR .GT. 0) GOTO 92
      ELSE
        KX = NX + 1
        KY = NY + 1
        KZ = NZ + 1
        IWOFF = 3*KX*KY*KZ + 1
        CALL CCALNP(IOPHDA,CARTES,NX,NY,NZ,XFRAC,WORK(IWOFF+LENREF),
     +            WORK(2*LENREF+IWOFF),LENREF,NUMNOD,WORK,IONEP,IERR)
        IF(IERR .GT. 0) GOTO 92
      ENDIF  
      IF(COMM) WRITE(*,*)'Writing the coordinates'
c .. write the nodes
c      CALL WNODNP(IONEP,NMAP,WORK,NUMNOD)
c .. write field coordinates
      CALL WRITE_COORDS(fld_id,NMAP,WORK,NUMNOD)
c .. Calculate the elements definitions
      IF(COMM) WRITE(*,*)'Calculating the element definitions'
      CALL CELENP(IONEP,NMAP,IJKBLK,NBLOCK,IELMAX,IWORK,
     +             NUMELE,IERR)
      IF(IERR .GT. 0) GOTO 91
c .. write the element definitions
      IF(COMM) WRITE(*,*)'Writing the element definitions'
c      CALL WELENP(IONEP,IWORK,NUMELE)
c .. write the element definitions into the field
      CALL WRITE_ELEMENTS(FLD_ID,IWORK,NUMELE)
c .. find the prps numbers
      IF(COMM) WRITE(*,*)'Reading the PRPS numbers'
      CALL PRPSNP(IONEP,IOPHDA,LENREF,LOG,NAME,IJKBLK,
     +     NBLOCK,NX,NY,NZ,WORK(2*NEBIG+1),NUMRES,IWORK,WORK,IERR)
      IF(IERR .GT. 0) GOTO 91
c .. calculate the number of results to output
c .. here, the nnode_data = numres and ncell_data = numres + 1
c .. unless the PRPS or VPOR are already included
      NRES = NUMRES * 2
      FPRPS = .FALSE.
      FVPOR = .FALSE.
      DO 15 IR = 1,NUMRES
        IF(NAME(IR).EQ.'PRPS' )FPRPS = .TRUE.
        IF(NAME(IR).EQ.'VPOR') FVPOR = .TRUE.
 15   CONTINUE
      IF(FPRPS .AND. FVPOR) THEN
        NRES = NRES - 2
      ELSEIF(FPRPS) THEN
        NRES = NRES - 1
      ELSEIF(FVPOR) THEN
        NRES = NRES 
      ELSE
        NRES = NRES + 1
      ENDIF
      WRITE(IONEP,*)NRES
c .. initialise field data
      CALL INIT_FLDDATA(FLD_ID,NUMRES,CDA_ID,NDA_ID)
c .. write the prps numbers out
      IF(COMM) print*,'Writing the PRPS numbers'
      IEOFF = 0
      WRITE(IONEP,*)'PRPS ELEMC'
      DO 16 IB = 1,NBLOCK
        NE = (IJKBLK(2,IB)-IJKBLK(1,IB)+1)*
     +     (IJKBLK(4,IB)-IJKBLK(3,IB)+1)*(IJKBLK(6,IB)-IJKBLK(5,IB)+1)
c       WRITE(IONEP,*)NE
c       WRITE(IONEP,'(3(I8,1X,E13.6,1X))')
c     +      (IEOFF+IE,WORK(IEOFF+IE),IE=1,NE)
        IEOFF = IEOFF + NE
c .. write into the cell data array
      call WRITE_DATA(CDA_ID,0,'PRPS',NUMELE,WORK)
 16   CONTINUE     
c .. find the velocity locations
      CALL FNDVNP(1,NAME,NUMRES,CCC1,CRT1,LOCU1,LOCV1,LOCW1)
      CALL FNDVNP(2,NAME,NUMRES,CCC2,CRT2,LOCU2,LOCV2,LOCW2)
c .. for each result stored, write into the cell and node data arrays
c .. we ignore the velocity components at this stage
      DO 100 IR = 1,NUMRES
C 
        IF(NAME(IR) .NE. 'PRPS' .AND. IR.NE.LOCU1 .AND. IR.NE.LOCV1
     +      .AND. IR.NE.LOCW1 .AND. IR.NE.LOCU2 .AND. IR.NE.LOCV2
     +      .AND. IR.NE.LOCW2) THEN
          NOBLK = (NAME(IR).EQ.'TEM1' .OR. NAME(IR).EQ.'DEN1')
          IF(COMM) WRITE(*,*)'Reading ',NAME(IR)
          WRITE(IONEP,*)NAME(IR),' ELEMC'
c ..     Read the results                    
          CALL RDVLNP(IONEP,IOPHDA,IR,NUMRES,LENREF,IJKBLK,NBLOCK,
     +                NX,NY,NZ,WORK(NEBIG+1),WORK,IERR)
          IF(IERR .GT. 0) GOTO 91
c ..     Write the scalar results        
          IF(COMM) WRITE(*,*)'Writing Cell centre ',NAME(IR)
          IEP = 0
          DO 17 IB = 1,NBLOCK
            CALL CSCANP(IB,IJKBLK,WORK,IWORK,NOBLK,NX,NY,NZ,
     +        WORK(NEBIG+1),IWORK(NEBIG+1),NRES)
c            WRITE(IONEP,*)NRES
c            WRITE(IONEP,'(3(I8,1X,E13.6,1X))')
c     +      (IWORK(NEBIG+I),WORK(NEBIG+I),I=1,NRES)
c .. write into an array
            DO 34 I = 1,NRES
              NE = IWORK(NEBIG+I)
      		TEMP(NE) = WORK(NEBIG+I)
 34         CONTINUE
 17       CONTINUE
c .. write into express object
      	CALL WRITE_DATA(CDA_ID,IR,NAME(IR),NUMELE,TEMP)
      	ICELL=ICELL+1
          IF( NAME(IR) .EQ. 'VPOR') GOTO 25
c ..      
c ..     Nodally average the results     
          IF(COMM) WRITE(*,*)'Writing Nodally Averaged ',NAME(IR)
          WRITE(IONEP,*)NAME(IR),' NODAL'
          IEOFF = 0
          IN = 0
          DO 20 IB = 1,NBLOCK   
            CALL NAVGNP(IWORK,WORK,IJKBLK,IB,NOBLK,NMAP,NBLOCK,
     +        LINK,NLINK,NRMAX,WORK(NEBIG+1),IWORK(NEBIG+1),
     +         IWORK(NEBIG+NRMAX+1),NR,IONEP,IERR)
            IF(IERR .GT. 0) GOTO 91
c ..       Write the nodally averaged results
c            WRITE(IONEP,*)NR
c            WRITE(IONEP,'(3(I8,1X,I3,1X,E13.6,1X))')
c     +      (IWORK(NEBIG+NRMAX+I),IWORK(NEBIG+I),WORK(NEBIG+I),I=1,NR)
c .. write array
            DO 35 I = 1,NR
              NN = IWORK(NEBIG+NRMAX+I)
              TEMP(NN) = WORK(NEBIG+I)
 35	      CONTINUE
 20       CONTINUE    
      	CALL WRITE_DATA(NDA_ID,IR-1,NAME(IR),NUMNOD,TEMP)
 25       CONTINUE       
        ENDIF  
 100  CONTINUE
c .. Write out velocities
      IF(COMM) WRITE(*,*)'Writing First Phase Velocities'
      CALL VELONP(LOCU1,LOCV1,LOCW1,IONEP,IOPHDA,LENREF,IJKBLK,NX,
     +    NY,NZ,NEBIG,1,NUMRES,IWORK,WORK,CCC1,CRT1,NBLOCK,NRMAX,NRESV,
     +    NUMNOD,NAME,NMAP,LINK,NLINK,IERR,
     +    NUMELE,TEMP,CDA_ID,NDA_ID,OIDSIZ) 
      IF(IERR .GT. 0) GOTO 93
c .. Second phase velocity if required
      IF(.NOT. ONEPHS) THEN
        IF(COMM) WRITE(*,*)'Writing Second Phase Velocities'
        CALL VELONP(LOCU2,LOCV2,LOCW2,IONEP,IOPHDA,LENREF,IJKBLK,NX,
     +    NY,NZ,NEBIG,2,NUMRES,IWORK,WORK,CCC2,CRT2,NBLOCK,NMRES,NRESV,
     +    NUMNOD,NAME,NMAP,LINK,NLINK,IERR,
     +	NUMELE,TEMP,CDA_ID,NDA_ID,OIDSIZ) 
        IF(IERR .GT. 0) GOTO 93
      ENDIF
c .. write links (unused at present)
c      IF(NLINK .GT. 0)CALL WRTLNK(IONEP,LINK,NLINK,IJKBLK,NBLOCK)
c .. normal exit
      readp_f = 1
      goto 90
c .. abnormal exits
98	MESSAGE = 'Error opening PATGEO file.'
      GOTO 99
97	MESSAGE = 'Error opening PHI file'
      GOTO 99
96	MESSAGE = 'Error opening XYZ file'
      GOTO 99
95	MESSAGE = 'Error opening NEP file'
      GOTO 99
94	MESSAGE = 'Error reading PATGEO file'
      GOTO 99
93	MESSAGE = 'Error writing velocities'
      GOTO 99
92	MESSAGE = 'Error getting coordinates'
      GOTO 99
91	MESSAGE = 'Error writing nodally averaged data'	
99	readp_f = 0
90	CONTINUE
      WRITE(*,*) MESSAGE
      CALL WRITE_MESSAGE(OBJ_ID,MESSAGE)
c .. close all files
      CALL FCLSNP(IOPHI,IOPHDA,IONEP,IOPAT,IOGRD)
      RETURN
      END
      SUBROUTINE GET_PARAMS(OBJ_ID,PHINAM,XYZNAM,PATNAM,
     .    PDIR,XDIR,FLD_ID)
c**********************************************************************
c Get the input parameters from the Express object
c**********************************************************************
      INCLUDE 'avs/omf.inc'
      INTEGER OBJ_ID(OIDSIZ),FLD_ID(OIDSIZ),TMP_ID(OIDSIZ)
      CHARACTER*80 PHINAM,XYZNAM,PATNAM
      LOGICAL PDIR,XDIR
      INTEGER PDIR_ID(OIDSIZ), XDIR_ID(OIDSIZ)
      INTEGER ISTAT, PDIR_VAL, XDIR_VAL
c .. filename length
      NLEN = 80
c .. PHI filename (required)
c .. NB MS Fortran is really dopey, if a filename is blank
c .. it prompts the user for one, which isnt very helpful if
c .. its a Windows app. Hence the 'NULL' below
      ISTAT =  OMFget_name_str_val(OBJ_ID,
     .	OMFstr_to_name('phi_name'),
     .	PHINAM, NLEN)
      IF (ISTAT.LT.1.OR.PHINAM(1:2).EQ.' ') THEN
        PHINAM = 'NULL'
      ENDIF
c .. XYZ filename (optional)
      ISTAT =  OMFget_name_str_val(obj_id,
     .	OMFstr_to_name('xyz_name'),
     .	XYZNAM, NLEN)
      IF (ISTAT.LT.1.OR.XYZNAM(1:2).EQ.' ') THEN
        XYZNAM = 'NULL'
      ENDIF
c .. PATGEO filename (optional)
      ISTAT =  OMFget_name_str_val(obj_id,
     .	OMFstr_to_name('pat_name'),
     .	PATNAM, NLEN)
      IF (ISTAT.LT.1.OR.PATNAM(1:2).EQ.' ') THEN
        PATNAM = 'NULL'
      ENDIF
c .. phi binary ?
      ISTAT = OMFfind_subobj(obj_id, OMFstr_to_name('phi_bin'),
     .	OM_OBJ_RD, pdir_id)
      ISTAT = OMFget_int_val(pdir_id,pdir_val)
c .. xyz binary ?
      ISTAT = OMFfind_subobj(obj_id, OMFstr_to_name('xyz_bin'),
     .	OM_OBJ_RD, xdir_id)
      ISTAT = OMFget_int_val(xdir_id,xdir_val)
c .. set logicals
      pdir = pdir_val
      xdir = xdir_val
c .. get the field object id
      ISTAT = OMFfind_subobj(obj_id,
     .	OMFstr_to_name('out_fld'),
     .	OM_OBJ_RD, tmp_id)
      ISTAT = OMFget_obj_val(tmp_id,fld_id)
      RETURN
      END
      SUBROUTINE WRITE_MESSAGE(OBJ_ID,MESSAGE)
C***********************************************************************
C Write out a termination message, for the benefit of PC
C users who don't see stdout - connected to the UI
C***********************************************************************
      INCLUDE 'avs/omf.inc'
      INTEGER OBJ_ID(OIDSIZ),MESS_ID(OIDSIZ)
      CHARACTER*40 MESSAGE
      ISTAT = OMFfind_subobj(obj_id, OMFstr_to_name('message'),
     .				OM_OBJ_RD, MESS_ID)
      ISTAT = OMFset_str_val(MESS_ID,MESSAGE)
      RETURN
      END
      SUBROUTINE WRITE_TITLE(OBJ_ID,TITLE)
C***********************************************************************
C Write the title 
C***********************************************************************
      INCLUDE 'avs/omf.inc'
      INTEGER OBJ_ID(OIDSIZ),TITLE_ID(OIDSIZ)
      CHARACTER*40 TITLE
      ISTAT = OMFfind_subobj(obj_id, OMFstr_to_name('title'),
     .				OM_OBJ_RD, title_id)
      ISTAT = OMFset_str_val(title_id,title)
      RETURN
      END
      SUBROUTINE WRITE_COORDS(FLD_ID,NMAP,COORD,NUMNOD)
C***********************************************************************
C Write the coordinates into the field structure
C***********************************************************************
      INCLUDE 'avs/omf.inc'
      INTEGER FLD_ID(OIDSIZ),NUMNOD,TMP_ID(OIDSIZ),NMAP(*)
      REAL COORD(3,*)
      REAL RBASE(1)
      INTEGER IBASE(1)
      ISTAT = OMFset_name_int_val(fld_id,
     .	OMFstr_to_name('nnodes'), NUMNOD)
      ISTAT = OMFset_name_int_val(fld_id,
     .	OMFstr_to_name('nspace'), 3)
      ISTAT = OMFfind_subobj(fld_id, OMFstr_to_name('coordinates'),
     .	OM_OBJ_RW, tmp_id)
      IPTR = OMFret_name_array_ptr(tmp_id,
     .	OMFstr_to_name('values'),
     .    OM_GET_ARRAY_WR, isize, itype)
      ioffs = ARRFoffset(IPTR,rbase,DTYPE_FLOAT)
c .. write the coordinates into the array
      DO 30 IN = 1,NUMNOD
        NOD = IN
        IF(IABS(NMAP(IN)) .LT. IN) NOD = -IN
        i = ioffs+(nod-1)*3
        rbase(i+1) = coord(1,NOD)
        rbase(i+2) = coord(2,NOD)
        rbase(i+3) = coord(3,NOD)
30	CONTINUE
      call ARRFfree(IPTR)
      RETURN
      END
      SUBROUTINE WRITE_ELEMENTS(FLD_ID,ELE,NUMELE)
C**********************************************************************
C Write the elements into the field structure
C**********************************************************************
      INCLUDE 'avs/omf.inc'
      INTEGER FLD_ID(OIDSIZ), TMP_ID(OIDSIZ), SET_ID(OIDSIZ),
     .	ELE(8,*), NUMELE
      INTEGER IBASE(1)
      ISTAT = OMFfind_subobj(FLD_ID,
     .    OMFstr_to_name('cell_set'),
     .    OM_OBJ_RD, set_id)
      ISTAT = OMFset_name_int_val(FLD_ID,
     .	OMFstr_to_name('ncell_sets'),1)
c .. init cell set 0
      ISTAT = OMFget_array_val(set_id,0,tmp_id,OM_OBJ_RW)
      ISTAT = OMFset_name_int_val(tmp_id,
     .	OMFstr_to_name('ncells'), NUMELE)
      ISTAT = OMFset_name_int_val(tmp_id,
     .	OMFstr_to_name('cell_ndim'), 3)
      ISTAT = OMFset_name_int_val(tmp_id,
     .	OMFstr_to_name('cell_order'), 1)
      ISTAT = OMFset_name_int_val(tmp_id,
     .	OMFstr_to_name('cell_nnodes'), 8)
      ISTAT = OMFset_name_int_val(tmp_id,
     .	OMFstr_to_name('poly_flag'), 0)
c .. get element pointer
      IPTR = OMFret_name_array_ptr(tmp_id,
     .	OMFstr_to_name('node_connect_list'),
     .	OM_GET_ARRAY_WR, ISIZE, ITYPE)
      ioffs = ARRFoffset(IPTR,ibase,DTYPE_INT)
c .. write the elements (zero-based) into the array
      DO 31 I = 1,NUMELE
      DO 31 J = 1,8
        KK = IOFFS+(I-1)*8+J
        IBASE(KK) = ELE(J,I)-1
31	CONTINUE
      call ARRFfree(IPTR)
      RETURN
      END
      SUBROUTINE INIT_FLDDATA(FLD_ID,NUMRES,CDA_ID,NDA_ID)
C**********************************************************************
C Initialise the cell data structure - set numres and return IDs for
C cell and node data
C**********************************************************************
      INCLUDE 'avs/omf.inc'
      INTEGER FLD_ID(OIDSIZ), NUMRES, CDA_ID(OIDSIZ),
     .    NDA_ID(OIDSIZ), TMP_ID(OIDSIZ), SET_ID(OIDSIZ),
     .    VAL_ID(OIDSIZ)
      INTEGER ISTAT
c .. get the cell set
      ISTAT = OMFfind_subobj(fld_id,
     .	OMFstr_to_name('cell_set'), OM_OBJ_RD, set_id)
c .. set ncell_data
      ISTAT = OMFget_array_val(set_id,0,val_id,OM_OBJ_RW)
      ISTAT = OMFset_name_int_val(val_id,
     .	OMFstr_to_name('ncell_data'),
     .	NUMRES+1)
c .. get the cell data array id
      ISTAT = OMFfind_subobj(val_id,
     .	OMFstr_to_name('cell_data'),
     .	OM_OBJ_RD, cda_id)
c .. set nnode_data
      ISTAT = OMFset_name_int_val(fld_id,
     .	OMFstr_to_name('nnode_data'), NUMRES)
c .. get the node data array id
      ISTAT = OMFfind_subobj(fld_id,
     .	OMFstr_to_name('node_data'),
     .	OM_OBJ_WR, nda_id)
c ..
      RETURN
      END
      SUBROUTINE WRITE_DATA(TMP_ID,INDEX,NAME,NVALS,VALS)
C**********************************************************************
C Write an array into a data object
C**********************************************************************
      INCLUDE 'avs/omf.inc'
      INTEGER TMP_ID(OIDSIZ),INDEX,VAL_ID(OIDSIZ),IPTR,NVALS
	INTEGER	NAME_ID(OIDSIZ)
      REAL RBASE(1),VALS(NVALS)
      CHARACTER*(*) NAME
c .. set array
      ISTAT = OMFget_array_val(TMP_ID,INDEX,VAL_ID,OM_OBJ_RW)
      IPTR = OMFret_name_array_ptr(VAL_ID,
     .	OMFstr_to_name('values'),
     .	OM_GET_ARRAY_WR,
     .	ISIZE, ITYPE)
      IOFFS = ARRFoffset(IPTR,RBASE,DTYPE_FLOAT)
      DO 33 I = 1,NVALS
      	RBASE(IOFFS+I) = VALS(I)
33	CONTINUE
      CALL ARRFfree(IPTR)
c .. set data label
      ISTAT = OMFset_name_str_val(VAL_ID,
     .    OMFstr_to_name('labels'), NAME)
      RETURN
      END
      SUBROUTINE FCLSNP(IOPHI,IOPHDA,IONEP,IOPAT,IOGRD)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
c .. Nature      Date      Author
C
C  Function of Subroutine:  Closes the files.
C
C  Called by: MAINNP
C
C  Limitations
C
C  Source of further information
C
C  Examples of use
C
C  Input Parameters 
C
C  Output Parameters
C
C***********************************************************************
C
      CLOSE(IOPHI)      
      CLOSE(IOPHDA)      
      CLOSE(IONEP)      
      IF (IOPAT.NE.0) THEN
      CLOSE(IOPAT)      
	ENDIF
      CLOSE(IOGRD)      
      
      RETURN
      END
      SUBROUTINE WELENP(IONEP,IELEM,NUMELE)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
c .. Nature      Date      Author
C
C  Function of Subroutine:  Write out the element definition.
C
C  Called by: MAINNP
C
C  Limitations
C
C  Source of further information
C
C  Examples of use
C
C  Input Parameters 
C
C  Output Parameters
C
C***********************************************************************
C
C
      INTEGER IELEM(8,*)
C
      WRITE(IONEP,*)NUMELE
      DO 100 IE = 1,NUMELE
        WRITE(IONEP,'(9(1X,I5))')IE,(IELEM(IN,IE),IN=1,8)
 100  CONTINUE
C
      RETURN
      END 
      SUBROUTINE WNODNP(IONEP,NMAP,COORD,NUMNOD)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
c .. Nature      Date      Author
C
C  Function of Subroutine:  Converts the PHOENICS PHI(DA),PATGEO and
c ..                       XYZ(DA) files into the NEP file.
C
C  Called by: NEPTRN
C
C  Limitations
C
C  Source of further information
C
C  Examples of use
C
C  Input Parameters
C
C  Output Parameters
C
C***********************************************************************
      REAL COORD(3,*)
      INTEGER NMAP(*)
C
      WRITE(IONEP,*)NUMNOD
      DO 100 IN = 1,NUMNOD
        NOD = IN
        IF(IABS(NMAP(IN)) .LT. IN) NOD = -IN
c ..    IF(NMAP(IN) .GT. 0 .AND. NMAP(IN) .NE. IN) NOD = -IN
        WRITE(IONEP,*)NOD,COORD(1,IN),COORD(2,IN),COORD(3,IN)
 100  CONTINUE
C
      RETURN
      END
      SUBROUTINE OPHINP(IONEP,IOPHI,IOPHDA,DIRECT,LBYTE,INT,PHI,LENREF,
     +                    BFC,CARTES,NUMRES,LOG,NAME,IERR)
C***********************************************************************
C
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
c .. Nature      Date      Author
C
C  Function of Subroutine:  Open the phi file. If ASCII convert to DA
C
C  Called by: MAINPN
C
C  Limitations
C
C  Source of further information
C
C  Examples of use
C
C  Input Parameters
c .. IONEP  - NEP file unit number
c .. IOPHI  - PHI file unit number
c .. IOPHDA - PHIDA unit number
c .. DIRECT - PHI file type
c .. LBYTE  - Number of bytes / words
c .. INT     - Work array for reading Integers
c .. PHI     - Work array for reading Reals
c .. LENREF  - Number of reals / record
C
C  Output Parameters
c .. BFC    - Grid is BFC flag
c .. CARTES  - Polar or Cartesian flag
c .. NUMRES  - Number of results found
c .. LOG     - Array of pointers to results found
c .. NAME    - Name of results found
c .. IERR    - Error flag
C
C***********************************************************************
      EXTERNAL ERRNP
C
      REAL REC(6)                                                       
      REAL PHI(LENREF)                                                  
      LOGICAL LOG(50),LOG1,LOG2,LOG3,LOG4
      CHARACTER*40 TITLE
      CHARACTER*4 NAME(50)
      INTEGER INT(LENREF)
C
      LOGICAL DIRECT,BFC,CARTES
C
      LEN = LBYTE*LENREF     
      IF(DIRECT) THEN
c ..   Open with correct record length 
        OPEN(IOPHDA,FILE='PHIDA',STATUS='OLD',RECL=LEN,
     +        ACCESS='DIRECT',ERR=900)
C
c ..   ASCII file
      ELSE
c ..   Open the PHI file
        OPEN(IOPHI,FILE='PHI',STATUS='OLD',ERR=800)                    
c ..   Convert this to a direct access file
        OPEN(IOPHDA,STATUS='SCRATCH',RECL=LEN,
     +       ACCESS='DIRECT',ERR=900)
C
        READ (IOPHI,1,ERR=700)TITLE
 1      FORMAT(A40)
        WRITE(IOPHDA,REC=1)TITLE
C
        READ(IOPHI,2,ERR=700)LOG1,LOG2,LOG3,LOG4
 2      FORMAT(1X,4L1)
        WRITE(IOPHDA,REC=2)LOG1,LOG2,LOG3,LOG4
C
        READ(IOPHI,3,ERR=700)(INT(J),J=1,11)
 3      FORMAT(1X,8I10,/,1X,4I10)
        WRITE(IOPHDA,REC=3)INT
        NX = INT(1)
        NY = INT(2)
        NZ = INT(3)
C
        READ(IOPHI,4,ERR=700)PHI(1)
 4      FORMAT(E13.2)
        WRITE(IOPHDA,REC=4)PHI(1)
C
        READ(IOPHI,5,ERR=700)NAME
 5      FORMAT(1X,19A4,/,1X,19A4,/,1X,12A4)
        WRITE(IOPHDA,REC=5)NAME
C
        READ(IOPHI,6,ERR=700)(PHI(I),I=1,NX)
 6      FORMAT(43(6(E13.0:),/))
        WRITE(IOPHDA,REC=6)(PHI(I),I=1,NX)
C
        READ(IOPHI,7,ERR=700)(PHI(I),I=1,NY)
 7      FORMAT(43(6(E13.0:),/))
        WRITE(IOPHDA,REC=7)(PHI(I),I=1,NY)
C
        READ(IOPHI,8,ERR=700)(PHI(I),I=1,NZ)
 8      FORMAT(43(6(E13.0:),/))
        WRITE(IOPHDA,REC=8)(PHI(I),I=1,NZ)
C
        READ(IOPHI,9,ERR=700)(PHI(I),I=1,NZ)
 9      FORMAT(43(6(E13.0:),/))
        WRITE(IOPHDA,REC=9)(PHI(I),I=1,NZ)
C
        READ(IOPHI,10,ERR=700)LOG
 10     FORMAT(1X,50L1)
        WRITE(IOPHDA,REC=10)LOG
C
C This is a do while loop
C
        IREC = 10                                                         
        NLAYER = NX*NY                                                    
        NUMREC = NLAYER/6                                                 
        NREST  = MOD(NLAYER,6)                                            
 100    CONTINUE                                                          
        IPHI = 1                                                          
c ..   For All the ascii records but one for this layer
        DO 110 NASC = 1,NUMREC                                       
          READ(IOPHI,115,END=200,ERR=700)REC                                     
 115      FORMAT(6E13.0)                                                  
          DO 120 IN = 1,6                                                 
            PHI(IPHI) = REC(IN)                                           
c ..       Full binary record write it out                               
            IF (IPHI .LT. 256) THEN                                       
              IPHI = IPHI + 1                                             
            ELSE                                                          
              IREC = IREC + 1                                             
              WRITE(IOPHDA,REC=IREC)PHI                                    
              IPHI = 1                                                    
            ENDIF                                                         
 120      CONTINUE                                                        
 110    CONTINUE                                                          
c ..                   
c ..   Last record for the layer
        IF (NREST .NE. 0) THEN                                            
          READ(IOPHI,125,END=200,ERR=700)(REC(I),I=1,NREST)                          
 125      FORMAT(6E13.0:)                                                 
          DO 130 IN = 1,NREST                                             
            PHI(IPHI) = REC(IN)                                           
            IF (IPHI .LT. 256) THEN                                       
              IPHI = IPHI + 1                                             
            ELSE                                                          
              IREC = IREC + 1                                             
              WRITE(IOPHDA,REC=IREC)PHI                                    
              IPHI = 1                                                    
            ENDIF                                                         
 130      CONTINUE                                                        
        ENDIF                                                             
c ..   Write out incomplete record
        IF (IPHI .NE. 1)THEN                                              
          IREC = IREC + 1                                                 
          WRITE(IOPHDA,REC=IREC)(PHI(I),I=1,(IPHI-1))                      
        ENDIF                                                             
C
        GOTO 100
 200    CONTINUE
        CLOSE(IOPHI)
      ENDIF 
      READ(IOPHDA,REC=1,ERR=750)TITLE
      READ(IOPHDA,REC=2,ERR=750)CARTES,LOG2,BFC,LOG4
      READ(IOPHDA,REC=5,ERR=750)NAME
      READ(IOPHDA,REC=10,ERR=750)LOG
      GOTO 999
 700  CONTINUE
      IERR = 5
      CALL ERRNP(IONEP,IERR,' ')
      GOTO 999
 750  CONTINUE
      IERR = 6
      CALL ERRNP(IONEP,IERR,' ')
      GOTO 999
 800  CONTINUE
      IERR = 3
      CALL ERRNP(IONEP,IERR,' ')
      GOTO 999
 900  CONTINUE
      IERR = 4
      CALL ERRNP(IONEP,IERR,' ')
 999  CONTINUE
      RETURN
      END

      SUBROUTINE OPATNP(IONEP,IOPAT,PATNAM,IERR)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
c .. Nature      Date      Author
C
C  Function of Subroutine:  Open the PATGEO file
C
C  Called by: NEPTRN
C
C  Limitations
C
C  Source of further information
C
C  Examples of use
C
C  Input Parameters
c ..  IONEP - NEP File Unit number
c ..  IOPAT - PATGEO File Unit number
C
C  Output Parameters
c ..  IERR  - File opening error flag
C
C***********************************************************************
      CHARACTER*(*) PATNAM
      IERR= 0 
      OPEN(UNIT=IOPAT,FILE=PATNAM,STATUS='OLD',ERR=10)
      GOTO 20
 10   IERR = 2
	IOPAT = 0
      CALL ERRNP(IONEP,IERR,' ')
 20   CONTINUE
      RETURN
      END          
      SUBROUTINE WRTLNK(IONEP,LINK,NLINK,IJKBLK,NBLOCK)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
c .. Nature      Date      Author
C
C  Function of Subroutine:  Writes links in local IJK to end of NEP file.
C
C  Called by: NEPTRN
C
C  Limitations
C
C  Source of further information
C
C  Examples of use
C
C  Input Parameters
C    IONEP  - NEP file unit number 
C    LINK   - Link definitions
C    NLINK  - Number of links
C    IJKBLK - Block limits
C    NBLOCK - Number of blocks
C
C  Output Parameters
C
C***********************************************************************
      INTEGER IJKBLK(6,NBLOCK)
      INTEGER LINK(11,NLINK)
C
c .. For each link
c ..  
      WRITE(IONEP,*)NLINK
      DO 100 IL = 1,NLINK
c ..   Find element offset for first block in link
        IB1 = IABS(LINK(1,IL))
        IEOF = IEOFF(IB1,IJKBLK)
c ..   Write link definition
        IB2 = IABS(LINK(2,IL))
        ID1 = LINK(3,IL)
        ID1S = LINK(4,IL) - IJKBLK(ID1*2-1,IB1) + 1
        ID1F = LINK(5,IL) - IJKBLK(ID1*2-1,IB1) + 1
        ID2 = LINK(6,IL)
        ID2S = LINK(7,IL) - IJKBLK(ID2*2-1,IB1) + 1 
        ID2F = LINK(8,IL) - IJKBLK(ID2*2-1,IB1) + 1 
        ID3 = LINK(9,IL)
        ID3S = LINK(10,IL) - IJKBLK(ID3*2-1,IB1) + 1
        ID3F = LINK(11,IL) - IJKBLK(ID3*2-1,IB1) + 1 
        WRITE(IONEP,10)IB1,IB2,ID1,ID1S,ID1F,ID2,ID2S,ID2F,ID3,ID3S,ID3F
 10     FORMAT(2(I8,1X),3(I1,1X,I8,1X,I8,1X))
C
 100  CONTINUE       
      RETURN
      END                      
