C  Fortran AVS/Express module

C
C  Source for fortran function readStar_f
C
      integer function readStar_f(obj_id, mask, seq_num)
c#include "avs/omf.inc"
      include "avs/omf.inc"
      integer obj_id(OIDSIZ), mask, seq_num
C
C  Place your module's source code here
C
      integer tmp_id(OIDSIZ), fld_id(OIDSIZ)
C
C SET PARAMETERS ACCORDING TO PREPROCESSING PARAMETERS (SEE FILE PARAM.PRP)
C
C MAX NO. OF VERTICES
      PARAMETER (MAXVRT=    240000)
C MAX NO. OF CELLS
      PARAMETER (MAXCEL=    200000)
C MAX NO. OF SAMM CELLS
      PARAMETER (MAXSAM=      1)
C MAX NO. OF USER APPLIED BOUNDARIES
      PARAMETER (MAXNBU=     100000)
C MAX NO. OF CELL TABLE ENTRIES
      PARAMETER (MAXTAB=      600)
C MAX NO. OF COUPLED SETS
      PARAMETER (MAXNCP=      4000)
C THE LARGER BETWEEN MAXCEL AND MAXVRT
      PARAMETER (MAXPST=    500000)
C MAX NO. OF BOUNDARIES FOR ENTIRE MODEL (USER + DEFAULT)
      PARAMETER (MAXBND=     400000)
C MAX NO. OF BOUNDARY REGIONS
      PARAMETER (MAXREG=       500)
C MAX NO. OF ELEMENTS FOR SCRATCH ARRAY1
      PARAMETER (MAXSC1=    560000)
C MAX NO. OF ELEMENTS FOR SCRATCH ARRAY2
      PARAMETER (MAXSC2=   2732048)
C
      PARAMETER (NCYDIM=  51)
      PARAMETER (NCPDIM=  51)
C

      COMMON/NODES/C(3,MAXVRT)
      COMMON/CELLS/NE(9,MAXCEL)
      COMMON/SAMML/LSAMM,NLSAMM(MAXCEL)
      COMMON/SAMMC/NESAMM(13,MAXSAM)
      COMMON/BOUND/IBDS(6,0:MAXNBU)
      COMMON/REGTYP/IRTYPE(-1:MAXREG)
      COMMON/CTABLE/ICTAB(10,MAXTAB)
      COMMON/NTABLE/CTNAME(MAXTAB)
      COMMON/CYCLIC/ISCYC(NCYDIM,MAXNCP)
      COMMON/COUPLE/ICOUP(NCPDIM,MAXNCP)
      COMMON/PDATA/VECTOR(14,MAXPST)
      COMMON/POSTC/DENM(99),VISM(99),CPHM(99),CONDM(99)
C TWO SCRATCH BLOCKS SCRT1 AND SCRT2
      COMMON/SCRT1/DUM1(MAXSC1)
      COMMON/SCRT2/DUM2(MAXSC2)
C SCRT3 IS NCMAP
      COMMON/SCRT3/DUM3(MAXCEL)
      COMMON/PRINTS/NPRSRF(3),NPROBS
      COMMON/ARSIZE/MAXC,MAXV,MAXCP,MAXTB,MAXBU,MAXRU,MXPST
      COMMON/CASEN/CASE,TITLE
      COMMON/KEY/MAXN,MAXE,ICSYS,MAXSYS,ITYPE,PFIRST,IVERS,NUMW,
     1           NI,NJ,NK,NF,MAXB,IBFILL,NOVICE,KEYS(10),LW,LR,
     2           LECHO,LNUT,MXTB,LTURBI,LTURBP,SETADD,NSENS,NPART,
     4           SCALE8,KEYS2(18)
      COMMON/POSTD/ITERU,TIMEU,ITERS,TIMES,VMING,VMAXG,VMINL,VMAXL,
     1             SMING,SMAXG,SMINL,SMAXL,NDATA(200,3),LPOST,
     2             NCELL,NBC,NBW,NBS,NBB,NNODE,WPOST,LDCELL,LDVERT,
     3             LSORT,LUNMOD,IPSYS,LOPER,LRSKIP,TREF,LDSURF,
     4             ISRTAB,ICSH1,NSITEM,LSRF(200),NBCUT,NBCYC,PREF1,
     5             PREPP(2,99),NTCELL,LCNPST,LDPPRS,
     6             LPCORD,NUMCON,NSC,TREFM(99),LVERS,LDFLUX,
     7             IREFRM,IRESYS,OMEREF,ADDREL,PGRAV(7,99),PSPIN(7,99),
     8             PMW(99),PMWS(50,99),SCALE9,LPTYP,LADJST,LPINTP,TIMEP,
     9             ITERP,LRELA,LMVGR,NDATAP(200,3),IRETW,NWSET,SPCYC,
     *             LSTAR(100),NBNCST,NBNCEN,NBSI,NSCL,NREG,
     1             MRECMM,NBP23,NCLPS,NWPRSM,NPRSM,NCLP,NCLPF,ITYPEN
      DIMENSION ARR(2048),LARR(2048),NDUM(9)
      LOGICAL LSTAR,SPCYC,LRSKIP
      LOGICAL LARR,WPOST,FMTOUT,WELEM,WWALL,WBOUND
      CHARACTER CASE*70,ANS*1,OFILE*4,TITLE*52,ITEM*4,FNAME*80
      EQUIVALENCE (ARR,IARR,LARR)
c
      integer ndoffs(maxvrt)
      logical optset, lstset
      character pans*1, tans*1, trtype*1 
      integer trstep
      double precision trtime
c
C
C FOR COMMON/ARSIZE/ USE
C
      LRSKIP=.FALSE.
      MAXC=MAXCEL
      MAXV=MAXVRT
      MAXTB=MAXTAB
      MAXCP=MAXNCP
      MAXBU=MAXNBU
      MAXRU=MAXREG
      MXPST=MAXPST
C
C RECORD LENGTH FOR DIFFERENT MACHINES
C
C*VAX  LREC=2048
C*IRIS LREC=2048
C*TITA LREC=2048
C*APOL LREC=8192
C*IBM  LREC=8192
C*HP   LREC=8192
C*SUN  LREC=8192
C*CRAY LREC=16384
C*FPS  LREC=16384
c      LREC=2048
c ** EDIT THIS LINE TO REFLECT YOUR TARGET MACHINE **
       LREC=8192
c **
C DEFAULTS
      LR=5
      LW=6
      LPOST=0
      IERROR=0
      FMTOUT=.FALSE.
      WELEM=.FALSE.
C
C VERSION
C
C     IVERS=2300
C     new version: 3000 (HJ)
      IVERS=3000
      RVERS=FLOAT(IVERS)/1000.
c
      readStar_f = 1
c
C
C GET CASENAME
C
      OFILE='star'
      LCASE=70
      NCASE=70
c
      istat = OMFget_name_str_val(obj_id,
     .                            OMFstr_to_name('case'),
     .                            CASE,
     .                            NCASE)
c
      CALL STRTRM(CASE,CASE,LCASE)
      IF (LCASE.GT.NCASE) LCASE=NCASE
      IF (LCASE.EQ.0) THEN
        CASE='star'
        LCASE=4
      ENDIF
C
C RESUME PREPROCESSING INFORMATION FROM FILE 16
C
      CALL RESUME(16,IERROR,NCPDIM,NCYDIM,ISCYC,ICOUP)
      IF (IERROR.EQ.1) THEN
         readStar_f = 0
         return
      ENDIF
c
      istat = OMFfind_subobj(obj_id,
     .                       OMFstr_to_name('out_fld'),
     .                       OM_OBJ_RD,
     .                       tmp_id)
c
      istat = OMFget_obj_val(tmp_id,fld_id)
C
C DECIDE LOADING WHICH POST DATA
C
      LPOST = 9
c
      ltype = 0
      if (optset(obj_id,'load_type',ltype)) return
      if (ltype .eq. 1) then
         LPOST = 9
      else if (ltype .eq. 2) then
         LPOST = 29
      endif
c
C
C LOAD POST DATA FROM FILE 9 OR 29
C
      IF (LPOST.EQ.9) THEN
        CALL LOAD9(LREC,IERROR)
      ELSEIF (LPOST.EQ.29) THEN
chj
        istat = OMFget_name_int_val(obj_id,
     .                              OMFstr_to_name('trtype'),
     .                              itoggv)
        trtype = 'F'
        if (itoggv .eq. 1) trtype = 'L'
        if (itoggv .eq. 2) trtype = 'S'
        if (itoggv .eq. 3) trtype = 'I'
chj
chj
        istat = OMFget_name_int_val(obj_id,
     .                              OMFstr_to_name('trstep'),
     .                              trstep)
chj
chj
        istat = OMFget_name_real8_val(obj_id,
     .                              OMFstr_to_name('trtime'),
     .                              trtime)
chj
        CALL TRLOAD(LREC,IERROR,trtype,trstep,trtime)
chj
      ENDIF
      IF (IERROR.EQ.1) THEN
         readStar_f = 0
         return
      ENDIF
c
      scale8 = scale9
      call co2fld(fld_id,ndoffs)
      call ce2fld(fld_id,ndoffs)
c
C
      IF (NCELL+NBC.GT.MAXCEL) THEN
        WRITE(LW,1014)NCELL
1014    FORMAT(/,' **THIS PROGRAM IS NOT DIMENSIONED LARGE ENOUGH.',/,
     1           '   SET PARAMETERS MAXCEL > ',I7,' AND RECOMPILE.')
        readStar_f = 0
        return
      ENDIF
      IF (NBC.GT.MAXBND) THEN
        WRITE(LW,1016)NBC
1016    FORMAT(/,' **THIS PROGRAM IS NOT DIMENSIONED LARGE ENOUGH.',/,
     1          '   SET PARAMETER MAXBND > ',I7,' AND RECOMPILE.')
        readStar_f = 0
        return
      ENDIF
C
C DECIDE ELEMENT CONVERSION
C
      WELEM = .FALSE.
c
      lcd = 0
      if (optset(obj_id,'cdconv',lcd)) goto 570
      if (lcd .gt. 0) WELEM = .TRUE.
c
C
C CELL DATA (14 OF THEM) IS STORED IN THE ARRAY VECTOR IN ORDER: 
C   U, V, W, PRESSURE, TURBULENT ENERGY, DISSIP, VISCOSITY, TEMP, 
C   DENSITY, LAM VISCOSITY, SPEC HEAT, CONDUCTIVITY, CONCENTRATION
C
      if (lcd .eq. 1) then
         ITEM = 'SCAL'
      else if (lcd .eq. 2) then
         ITEM = 'FLUX'
      endif
c
C
      IF (WELEM) THEN
c
        istat = OMFget_name_int_val(obj_id,
     .                              OMFstr_to_name('pconv'),
     .                              itoggv)
        pans = 'R'
        if (itoggv .eq. 1) pans = 'A'
c
        istat = OMFget_name_int_val(obj_id,
     .                              OMFstr_to_name('tconv'),
     .                              itoggv)
        tans = 'R'
        if (itoggv .eq. 1) tans = 'A'
c
        CALL CELGET(ITEM,NCPDIM,ICOUP,pans,tans)
C ELEMENT/CELL  CONVERSION PORTION
        call fldelm(obj_id,fld_id)
      ENDIF
C
C PROMPT FOR WALL CONVERSION
C
C SKIP IF NO WALL DATA
570   IF (NDATA(1,3).EQ.0) GO TO 700
C
      WWALL = .FALSE.
c
      lwd = 1
      if (optset(obj_id,'wdconv',lwd)) WWALL = .TRUE.
c
C
C WALL DATA (14 OF THEM) IS STORED IN THE ARRAY VECTOR IN SUCH ORDER:
C   SFX, SFY, SFZ, YPLUS, DIST, TEMP, HEAT TRANSFER, 
C   MASS TRANSFER, HEAT FLUX, MASS FLUX, THERMAL RADISTION, 
C   THERMAL INCIDENT RADIOSITY, SOLAR RADIATION, 
C   SOLAR INCIDENT RADIOSITY
C
      IF (WWALL) THEN
        CALL SRFGET(IERROR,DUM2(2048+MAXCEL+1),ICOUP)
        IF (IERROR.EQ.1) THEN
           readStar_f = 0
           return
        ENDIF
C WALL CONVERSION
cfix        CALL PATWAL(FMTOUT)
      ENDIF
 700  CONTINUE
C
      readStar_f = 1
      return
      end







      logical function optset(obj_id,obj_name,id)
c#include "avs/omf.inc"
      include "avs/omf.inc"
      integer obj_id(OIDSIZ)
      character*(*) obj_name
c
      optset = .false.
c
      istat = OMFget_name_int_val(obj_id,
     .                            OMFstr_to_name(obj_name),
     .                            ival)
      if (istat .ne. 1) return
c
      if (ival .eq. id) optset = .true.
      id = ival
c
      return
      end








      logical function lstset(obj_id,obj_name,id)
c#include "avs/omf.inc"
      include "avs/omf.inc"
      integer obj_id(OIDSIZ)
      character*(*) obj_name
      integer ibase(1)
c
      iptr = OMFret_name_array_ptr(obj_id,
     .                             OMFstr_to_name(obj_name),
     .                             OM_GET_ARRAY_RD,
     .                             isize,
     .                             itype)
      ioffs = ARRFoffset(iptr,ibase,DTYPE_INT)
c
      lstset = .true.
      do 10 i=1,isize
      if (ibase(ioffs+i) .eq. id) return
 10   continue
c
      lstset = .false.
c
      return
      end








      subroutine co2fld(fld_id,ndoffs)
c#include "avs/omf.inc"
      include "avs/omf.inc"
      integer fld_id(OIDSIZ), ndoffs(1), tmp_id(OIDSIZ)
      real rbase(1)
      COMMON/NODES/C(3,1)
      COMMON/KEY/MAXN,MAXE,ICSYS,MAXSYS,ITYPE,PFIRST,IVERS,NUMW,
     1           NI,NJ,NK,NF,MAXB,IBFILL,NOVICE,KEYS(10),LW,LR,
     2           LECHO,LNUT,MXTB,LTURBI,LTURBP,SETADD,NSENS,NPART,
     4           SCALE8,KEYS2(18)
C
      NC=0
      DO 100 N=1,MAXN
        IF (C(1,N).GT.1.E29) GO TO 100
        NC=NC+1
        ndoffs(n) = n - nc
100   CONTINUE
      istat = OMFset_name_int_val(fld_id,
     .                            OMFstr_to_name('nnodes'),
     .                            NC)
      istat = OMFset_name_int_val(fld_id,
     .                            OMFstr_to_name('nspace'),
     .                            3)
c
      istat = OMFfind_subobj(fld_id,
     .                       OMFstr_to_name('coordinates'),
     .                       OM_OBJ_RW,
     .                       tmp_id)
c
      iptr = OMFret_name_array_ptr(tmp_id,
     .                             OMFstr_to_name('values'),
     .                             OM_GET_ARRAY_WR,
     .                             isize,
     .                             itype)
      ioffs = ARRFoffset(iptr,rbase,DTYPE_FLOAT)
c
      NC=0
      DO 200 N=1,MAXN
        IF (C(1,N).GT.1.E29) GO TO 200
        i = ioffs + nc*3
        rbase(i+1) = scale8 * c(1,n)
        rbase(i+2) = scale8 * c(2,n)
        rbase(i+3) = scale8 * c(3,n)
        NC=NC+1
200   CONTINUE
c
      call ARRFfree(iptr)
c
      return
      end










      subroutine ce2fld(fld_id,ndoffs)
c#include "avs/omf.inc"
      include "avs/omf.inc"
      integer fld_id(OIDSIZ), ndoffs(1)
      integer tmp_id(OIDSIZ), set_id(OIDSIZ)
      integer ibase(1)
      COMMON/CELLS/NE(9,1)
      COMMON/CTABLE/ICTAB(10,1)
      COMMON/NTABLE/CTNAME(1)
      COMMON/ARSIZE/MAXC,MAXV,MAXCP,MAXTB,MAXBU,MAXRU,MXPST
      COMMON/KEY/MAXN,MAXE,ICSYS,MAXSYS,ITYPE,PFIRST,IVERS,NUMW,
     1           NI,NJ,NK,NF,MAXB,IBFILL,NOVICE,KEYS(10),LW,LR,
     2           LECHO,LNUT,MXTB,LTURBI,LTURBP,SETADD,NSENS,NPART,
     4           SCALE8,KEYS2(18)
      character*4 ctname, dname*15
c
      istat = OMFfind_subobj(fld_id,
     .                       OMFstr_to_name('cell_set'),
     .                       OM_OBJ_RD,
     .                       set_id)
c
      iset = 0
      do 200 m=1,maxtb
      if (ictab(1,m) .le. 0) goto 200
      if (ictab(1,m) .gt. 2) goto 200
c
      nc = 0
      do 210 n=1,maxe
      if (ne(1,n) .lt. 1) goto 210
      if (ne(9,n) .ne. m) goto 210
      nc = nc + 1
 210  continue
      if (nc .eq. 0) goto 200
c
      nsets = iset + 1
      istat = OMFset_name_int_val(fld_id,
     .                            OMFstr_to_name('ncell_sets'),
     .                            nsets)
c
      istat = OMFget_array_val(set_id,iset,tmp_id,OM_OBJ_RW)
      iset = iset + 1
c
      istat = OMFset_name_int_val(tmp_id,
     .                            OMFstr_to_name('ncells'),
     .                            nc)
      istat = OMFset_name_int_val(tmp_id,
     .                            OMFstr_to_name('cell_ndim'),
     .                            3)
      istat = OMFset_name_int_val(tmp_id,
     .                            OMFstr_to_name('cell_order'),
     .                            1)
      istat = OMFset_name_int_val(tmp_id,
     .                            OMFstr_to_name('cell_nnodes'),
     .                            8)
      istat = OMFset_name_int_val(tmp_id,
     .                            OMFstr_to_name('poly_flag'),
     .                            0)
c
      dname(12:15) = ctname(m)
      if (dname(12:15) .eq. '    ') then
         dname(12:12) = '#'
         write(dname(13:15),'(i3)') m
         if (ictab(1,m) .eq. 1) dname(1:11) = 'Fluid      '
         if (ictab(1,m) .eq. 2) dname(1:11) = 'Solid      '
         if (ictab(1,m) .eq. 3) dname(1:11) = 'Baffle     '
         if (ictab(1,m) .eq. 4) dname(1:11) = 'Shell      '
         if (ictab(1,m) .eq. 5) dname(1:11) = 'Line       '
         if (ictab(1,m) .eq. 6) dname(1:11) = 'Point      '
      endif
      istat = OMFset_name_str_val(tmp_id,
     .                            OMFstr_to_name('name'),
     .                            dname)
c
      iptr = OMFret_name_array_ptr(tmp_id,
     .                             OMFstr_to_name('node_connect_list'),
     .                             OM_GET_ARRAY_WR,
     .                             isize,
     .                             itype)
      ioffs = ARRFoffset(iptr,ibase,DTYPE_INT)
c
      nc = 0
      do 220 n=1,maxe
      if (ne(1,n) .lt. 1) goto 220
      if (ne(9,n) .ne. m) goto 220
      do 221 k=1,8
      kk = ioffs + nc*8 + k
      ibase(kk) = ne(k,n) - ndoffs(ne(k,n)) - 1
 221  continue
      nc = nc + 1
 220  continue
c
      call ARRFfree(iptr)
c
 200  continue
c
      return
      end












      subroutine fldelm(obj_id,fld_id)
c#include "avs/omf.inc"
      include "avs/omf.inc"
      integer obj_id(OIDSIZ), fld_id(OIDSIZ), set_id(OIDSIZ)
      integer tmp_id(OIDSIZ), val_id(OIDSIZ), cda_id(OIDSIZ)
C
C CONVERT TO ELEMENT DATA
C
      COMMON/CELLS/NE(9,1)
      COMMON/CTABLE/ICTAB(10,1)
      COMMON/NTABLE/CTNAME(1)
      COMMON/ARSIZE/MAXC,MAXV,MAXCP,MAXTB,MAXBU,MAXRU,MXPST
      COMMON/KEY/MAXN,MAXE,ICSYS,MAXSYS,ITYPE,PFIRST,IVERS,NUMW,
     1           NI,NJ,NK,NF,MAXB,IBFILL,NOVICE,KEYS(10),LW,LR,
     2           LECHO,LNUT,MXTB,LTURBI,LTURBP,SETADD,NSENS,NPART,
     4           SCALE8,KEYS2(18)
      COMMON/PDATA/VECTOR(14,1)
      COMMON/CASEN/CASE,TITLE
      COMMON/POSTD/ITERU,TIMEU,ITERS,TIMES,VMING,VMAXG,VMINL,VMAXL,
     1             SMING,SMAXG,SMINL,SMAXL,NDATA(200,3),LPOST,
     2             NCELL,NBC,NBW,NBS,NBB,NNODE,WPOST,LDCELL,LDVERT,
     3             LSORT,LUNMOD,IPSYS,LOPER,LRSKIP,TREF,LDSURF,
     4             ISRTAB,ICSH1,NSITEM,LSRF(200),NBCUT,NBCYC,PREF1,
     5             PREPP(2,99),NTCELL,LCNPST,LDPPRS,
     6             LPCORD,NUMCON,NSC,TREFM(99),LVERS,LDFLUX,
     7             IREFRM,IRESYS,OMEREF,ADDREL,PGRAV(7,99),PSPIN(7,99),
     8             PMW(99),PMWS(50,99),SCALE9,LPTYP,LADJST,LPINTP,TIMEP,
     9             ITERP,LRELA,LMVGR,NDATAP(200,3),IRETW,NWSET,SPCYC,
     *             LSTAR(100),NBNCST,NBNCEN,NBSI,NSCL,NREG,
     1             MRECMM,NBP23,NCLPS,NWPRSM,NPRSM,NCLP,NCLPF,ITYPEN
      DIMENSION ISUB(80)
      CHARACTER CASE*70,FNAME*80,TITLE*52,SUB1*50,SUB2*50
      CHARACTER CITER*6
      DIMENSION DTYPE(15)
      CHARACTER DTYPE*4
      DATA DTYPE/'FLUX',' U  ',' V  ',' W  ',' P  ',' TE ',' ED ',
     1           'VIS ',' T  ','DENS','LAMV',' CP ','COND','ENTH',
     2           'CONC'/
c
      integer ibase(1)
      real rbase(1)
c
      jptr = OMFret_name_array_ptr(obj_id,
     .                             OMFstr_to_name('cdsel'),
     .                             OM_GET_ARRAY_RD,
     .                             jsize,
     .                             jtype)
      joffs = ARRFoffset(jptr,ibase,DTYPE_INT)
c
      istat = OMFget_name_int_val(fld_id,
     .                            OMFstr_to_name('ncell_sets'),
     .                            ncsets)
      istat = OMFfind_subobj(fld_id,
     .                       OMFstr_to_name('cell_set'),
     .                       OM_OBJ_RD,
     .                       set_id)
c
      do 100 n=1,ncsets
      istat = OMFget_array_val(set_id,n-1,val_id,OM_OBJ_RW)
      istat = OMFset_name_int_val(val_id,
     .                            OMFstr_to_name('ncell_data'),
     .                            jsize)
C
      istat = OMFfind_subobj(val_id,
     .                       OMFstr_to_name('cell_data'),
     .                       OM_OBJ_RD,
     .                       cda_id)
      do 100 jt=1,jsize
      istat = OMFget_array_val(cda_id,jt-1,tmp_id,OM_OBJ_RW)
      istat = OMFset_name_int_val(tmp_id,
     .                            OMFstr_to_name('veclen'),
     .                            1)
 100  continue
      print*,'Cell Data initialized'
c
      iset = 0
      do 200 m=1,maxtb
      if (ictab(1,m) .le. 0) goto 200
      if (ictab(1,m) .gt. 2) goto 200
      istat = OMFget_array_val(set_id,iset,val_id,OM_OBJ_RW)
      istat = OMFget_name_int_val(val_id,
     .                            OMFstr_to_name('ncells'),
     .                            ncells)
      istat = OMFfind_subobj(val_id,
     .                       OMFstr_to_name('cell_data'),
     .                       OM_OBJ_RD,
     .                       cda_id)
c
      do 210 jt=1,jsize
      j = ibase(jt+joffs) + 1
      istat = OMFget_array_val(cda_id,jt-1,tmp_id,OM_OBJ_RW)
c
      istat = OMFset_name_str_val(tmp_id,
     .                            OMFstr_to_name('labels'),
     .                            dtype(j+1))
c
      iptr = OMFret_name_array_ptr(tmp_id,
     .                             OMFstr_to_name('values'),
     .                             OM_GET_ARRAY_RW,
     .                             isize,
     .                             itype)
      ioffs = ARRFoffset(iptr,rbase,DTYPE_FLOAT)
c
      nc = 0
      do 210 n=1,maxe
      if (ne(1,n) .lt. 1) goto 210
      if (ne(9,n) .ne. m) goto 210
      kk = ioffs + nc + 1
      rbase(kk) = vector(j,n)
      nc = nc + 1
 210  continue
      if (nc .eq. 0) goto 200
      iset = iset + 1
 200  continue
c
      return
      end
