      FUNCTION NCONNP(IN,NMAP)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Find lowest numbered coincident node.
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
      INTEGER NMAP(*)
      
      NC = IN
 10   CONTINUE     
      IF(IABS(NMAP(NC)) .LT. NC ) THEN
        NC = IABS(NMAP(NC))
        GOTO 10
      ENDIF
      NCONNP = NC
      RETURN
      END
      SUBROUTINE ONEPNP(IONEP,IERR)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Open the NEP file
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
C    IONEP - NEP file unit number
C
C  Output Parameters
C    IERR  - Error Flag
C
C***********************************************************************
      EXTERNAL ERRNP
C
      IERR= 0 
      OPEN(UNIT=IONEP,FILE='NEP',STATUS='UNKNOWN',ERR=10)
      GOTO 20
 10   IERR = 1
      CALL ERRNP(IONEP,IERR,' ')
 20   CONTINUE
      RETURN
      END          
      SUBROUTINE LNODNP(IN1,IN2,NMAP)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Stores and updates cooincident node information
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
      INTEGER NMAP(*)
C
C     Ensure IN1 is greater than IN2
      IF(IN1 .LT. IN2) THEN
        IT  = IN1
        IN1 = IN2
        IN2 = IT
      ELSE
        IT = IN2
      ENDIF
C
 10   CONTINUE
C     Find the NMAP value for IT
      NMREF = NMAP(IABS(IT))
C     Find base node      
      IF(IABS(NMREF) .LT. IABS(IT)) THEN
        IT = NMREF
        GOTO 10
      ENDIF                      
C
C     Does the base node point to a node BELOW this one
      IF(IABS(NMREF) .LT. IN1) THEN
C       Let this node point to base
        NMAP(IN1) = NMREF      
C       The base node points to this one
        NMAP(IABS(IT)) = IN1
      ELSEIF(NMREF .NE. IN1) THEN
C       Find the correct place for the node
 20     CONTINUE
        IT = NMAP(IABS(NMREF))
        IF(IABS(IT) .LT. IN1 .AND. IABS(NMREF) .GT. IN1) THEN
          NMAP(IABS(NMREF)) = IN1
          NMAP(IN1) = IT
        ELSE
          NMREF = IT
          GOTO 20
        ENDIF
      ENDIF
C
      RETURN
      END 
      SUBROUTINE VELSNP(IBREF,IJKBLK,IPRPS,NOBLK,CCC,UVEL,VVEL,WVEL,
     +                  NX,NY,NZ,UVAL,VVAL,WVAL,IELE,NRES)
C***********************************************************************
C
C  Author: CYC
C
C  Creation Date: 18th July 1994
C
C  Amendment History
C     Nature                    Date      Author
C     ------                    ----      ------
C   Correct Z vel typing error  29/10/94  CYC
C   Correct Z vel typing error  23/11/94  CYC
C
C  Function of Subroutine:  
C     Calculates the cell centre values for a given velocity direction
C     If cell is block/does not exist do not use value
C     If colocated variables then treat as scalar
C
C  Called by: 
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
      REAL UVEL(NX*NY*NZ),VVEL(NX*NY*NZ),WVEL(NX*NY*NZ)
      REAL UVAL(NX*NY*NZ),VVAL(NX*NY*NZ),WVAL(NX*NY*NZ)
      INTEGER IJKBLK(6,*),IPRPS(NX*NY*NZ),IELE(NX*NY*NZ)
      LOGICAL NOBLK,CCC
C
      IF (CCC) THEN 
        CALL CSCANP(IBREF,IJKBLK,UVEL,IPRPS,NOBLK,NX,NY,NZ,UVAL,IELE,
     +      NRES)
        CALL CSCANP(IBREF,IJKBLK,VVEL,IPRPS,NOBLK,NX,NY,NZ,VVAL,IELE,
     +      NRES)
        CALL CSCANP(IBREF,IJKBLK,WVEL,IPRPS,NOBLK,NX,NY,NZ,WVAL,IELE,
     +      NRES)
      ELSE  
        NRES = 0
        IEP = IEOFF(IBREF,IJKBLK)
        IEOF = IEP
        IBX = IJKBLK(2,IBREF) - IJKBLK(1,IBREF) + 1
        IBY = IJKBLK(4,IBREF) - IJKBLK(3,IBREF) + 1
        IBZ = IJKBLK(6,IBREF) - IJKBLK(5,IBREF) + 1
C       Count the number of valid variables and compress        
        DO 40 IZ = 1,IBZ
          DO 30 IY = 1,IBY
            DO 20 IX = 1,IBX
              IEP = IEP + 1
              IF(IPRPS(IEP) .LT. 100 .OR. 
     +          (IPRPS(IEP).LT.198 .AND. NOBLK)) THEN
                NRES = NRES + 1
                IELE(NRES) = IEP
C               Find element on each side of the cell 
C               UVEL   
		IF(IBX .GT. 1) THEN        
                  IXS = IX - 1
                  IXF = IX 
C                 Ignore value at each end of the row
                  IF(IXS .EQ. 0) IXS = 1
                  IF(IXF .EQ. IBX) IXF = IBX-1
C                 Skip blocked values
                  IES = IXS + (IY-1)*IBX + (IZ-1)*IBX*IBY + IEOF            
                  IF(IPRPS(IES) .LT. 100 .OR. 
     +              (IPRPS(IES).LT.198 .AND. NOBLK)) THEN
                    NUM = 1
                    UVALUE = UVEL(IES)
                  ELSE
                    NUM = 0 
                    UVALUE = 0.0
                  ENDIF
                  IEF = IXF + (IY-1)*IBX + (IZ-1)*IBX*IBY + IEOF            
                  IF(IPRPS(IEF+1) .LT. 100 .OR. 
     +              (IPRPS(IEF+1).LT.198 .AND. NOBLK)) THEN
                    NUM = NUM + 1
                    UVALUE = UVALUE +  UVEL(IEF)
                  ENDIF
                  IF(NUM .GT. 0) THEN
                    UVAL(NRES) = UVALUE / NUM
                  ELSE
                    UVAL(NRES) = 0.0
                  ENDIF   
                ELSE
                  UVAL(NRES)=0.0
                ENDIF     
C                 
C               Find element on each side of the cell 
C               VVEL           
		IF(IBY .GT. 1) THEN        
                  IYS = IY - 1
                  IYF = IY 
C                 Ignore value at each end of the row
                  IF(IYS .EQ. 0) IYS = 1
                  IF(IYF .EQ. IBY) IYF = IBY-1
C                 Skip blocked values
                  IES = IX  + (IYS-1)*IBX + (IZ-1)*IBX*IBY + IEOF            
                  IF(IPRPS(IES) .LT. 100 .OR. 
     +              (IPRPS(IES).LT.198 .AND. NOBLK)) THEN
                    NUM = 1
                    VVALUE = VVEL(IES)
                  ELSE
                    NUM = 0 
                    VVALUE = 0.0
                  ENDIF
                  IEF = IX  + (IYF-1)*IBX + (IZ-1)*IBX*IBY + IEOF            
                  IF(IPRPS(IEF+IBX) .LT. 100 .OR. 
     +              (IPRPS(IEF+IBX).LT.198 .AND. NOBLK)) THEN
                    NUM = NUM + 1
                    VVALUE = VVALUE +  VVEL(IEF)
                  ENDIF
                  IF(NUM .GT. 0) THEN
                    VVAL(NRES) = VVALUE / NUM
                  ELSE
                    VVAL(NRES) = 0.0
                  ENDIF    
                ELSE
                  VVAL(NRES) = 0.0
                ENDIF    
C                 
C               Find element on each side of the cell 
C               WVEL           
		IF(IBZ .GT. 1) THEN        
                  IZS = IZ - 1
                  IZF = IZ 
C                 Ignore value at each end of the row
                  IF(IZS .EQ. 0) IZS = 1
                  IF(IZF .EQ. IBZ) IZF = IBZ-1
C                 Skip blocked values
                  IES = IX  + (IY-1)*IBX + (IZS-1)*IBX*IBY + IEOF            
                  IF(IPRPS(IES) .LT. 100 .OR. 
     +              (IPRPS(IES).LT.198 .AND. NOBLK)) THEN
                    NUM = 1
                    WVALUE = WVEL(IES)
                  ELSE
                    NUM = 0 
                    WVALUE = 0.0
                  ENDIF
                  IEF = IX  + (IY-1)*IBX + (IZF-1)*IBX*IBY + IEOF            
                  IF(IPRPS(IEF+(IBX*IBY)) .LT. 100 .OR. 
     +              (IPRPS(IEF+(IBX*IBY)).LT.198 .AND. NOBLK)) THEN
                    NUM = NUM + 1
                    WVALUE = WVALUE +  WVEL(IEF)
                  ENDIF
                  IF(NUM .GT. 0) THEN
                    WVAL(NRES) = WVALUE / NUM
                  ELSE
                    WVAL(NRES) = 0.0
                  ENDIF    
                ELSE
                  WVAL(NRES) = 0.0
                ENDIF    
              ENDIF  
 20         CONTINUE
 30       CONTINUE
 40     CONTINUE
      ENDIF
      RETURN    
      END
      SUBROUTINE FNDMAT(IMREF,MAT,NUMMAT,NUM,TOTAL,IM)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Adds the result value to the correct material 
C                           number.
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
      INTEGER MAT(*),NUM(*)
      REAL TOTAL(*)
C   
      IF(NUMMAT .EQ. 0) THEN
        NUMMAT = 1
        IM = 1
        TOTAL(IM) = 0
        NUM(IM) = 0
        MAT(IM)   = IMREF
      ELSE
        DO 10 IM = 1,NUMMAT
          IF(MAT(IM) .EQ. IMREF) GOTO 20
 10     CONTINUE
        IM = NUMMAT + 1
        NUMMAT = NUMMAT + 1
        TOTAL(IM) = 0
        NUM(IM) = 0
        MAT(IM)   = IMREF
 20     CONTINUE   
      ENDIF
      RETURN
      END                   
      FUNCTION IEOFF(IBREF,IJKBLK)
C
C     Calculates the nodal offset for a block
C
      INTEGER IJKBLK(6,*)
C
      IEOFF = 0
      IF(IBREF .GT. 1) THEN
        DO 100 IB = 1,IBREF-1
          IEOFF = (IJKBLK(2,IB)-IJKBLK(1,IB)+1) *   
     +            (IJKBLK(4,IB)-IJKBLK(3,IB)+1) *   
     +            (IJKBLK(6,IB)-IJKBLK(5,IB)+1) + IEOFF
 100    CONTINUE
      ENDIF
      RETURN
      END      
      SUBROUTINE VAVGNP(IPRPS,IJKBLK,IBREF,NOBLK,NMAP,NBLOCK,LINK,NLINK
     + ,UVEL,VVEL,WVEL,CCC,NRMAX,UVAL,VVAL,WVAL,MATER,NODE,NRV,IONEP,
     +  IERR)
C***********************************************************************
C
C  Author: CYC
C
C  Creation Date: 18th July 1994
C
C  Amendment History
C     Nature               Date      Author
C     ------               ----      ------
C     Remove IP parameter  29/10/94  CYC
C
C  Function of Subroutine:  
C     Nodally averages velocities 
C     Multiblock grid use co-located variables so no multiblock option
C     is available for these 
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
C
C  Output Parameters
C
C***********************************************************************
C
      REAL UVEL(*),VVEL(*),WVEL(*),UVAL(NRMAX),VVAL(NRMAX),WVAL(NRMAX)
      INTEGER IPRPS(*),IJKBLK(6,*),LINK(11,*),MATER(NRMAX),NODE(NRMAX)
      INTEGER NMAP(*)
      LOGICAL CCC,NOBLK
C
      IERR = 0     
C
      IF(CCC) THEN
        CALL NAVGNP(IPRPS,UVEL,IJKBLK,IBREF,NOBLK,NMAP,NBLOCK,
     +                  LINK,NLINK,NRMAX,UVAL,MATER,NODE,NRV,IONEP,IERR)
        IF(IERR .GT. 0) GOTO 900
        CALL NAVGNP(IPRPS,VVEL,IJKBLK,IBREF,NOBLK,NMAP,NBLOCK,
     +                  LINK,NLINK,NRMAX,VVAL,MATER,NODE,NRV,IONEP,IERR)
        IF(IERR .GT. 0) GOTO 900
        CALL NAVGNP(IPRPS,WVEL,IJKBLK,IBREF,NOBLK,NMAP,NBLOCK,
     +                  LINK,NLINK,NRMAX,WVAL,MATER,NODE,NRV,IONEP,IERR)
        IF(IERR .GT. 0) GOTO 900
      ELSE
        IP = 0
        IN = INOFF(IBREF,IJKBLK)
        IEOF = IEOFF(IBREF,IJKBLK)
C       Find the block limits
        KX = IJKBLK(2,IBREF) - IJKBLK(1,IBREF) + 2
        KY = IJKBLK(4,IBREF) - IJKBLK(3,IBREF) + 2
        KZ = IJKBLK(6,IBREF) - IJKBLK(5,IBREF) + 2
        KLAY = KX * KY
        NX = KX - 1
        NY = KY - 1
        NZ = KZ - 1
        NLAY = NX * NY
        NRV = 0
C       For each node in the block
        DO 90 IZ = 1,KZ
          DO 80 IY = 1,KY
            DO 70 IX = 1,KX
C             Find the node number
              IN = IN + 1
C             U velocity              
              UTOT = 0
              NUMU = 0
C             Find the surrounding elements any one can be missing
              DO 30 K = 1,2
                DO 20 J = 1,2
                  IOFF = IX -  1   
                  IF(IX .EQ. KX) IOFF = IOFF - 1
                  IF(IX .EQ. 1) IOFF = IOFF + 1
                  JOFF = IY - J    
                  KOFF = IZ - K  
                  IF(IOFF.GT.0 .AND. JOFF.GT.-1 .AND. KOFF.GT.-1 .AND.
     +             IOFF.LE.NX .AND. JOFF.LT.NY .AND. KOFF.LT.NZ)THEN
                    IE = IOFF + JOFF*NX + KOFF*NLAY + IEOF
                    IF(IPRPS(IE) .LT. 100 .OR. 
     +                (IPRPS(IE).LT.198 .AND. NOBLK)) THEN
C                     Add this value to the total                    
                      UTOT = UTOT + UVEL(IE)
                      NUMU   = NUMU + 1
                    ENDIF
                  ENDIF
 20             CONTINUE    
 30           CONTINUE 
C 
C             V velocity              
              VTOT = 0
              NUMV = 0
C             Find the surrounding elements any one can be missing
              DO 50 K = 1,2
                DO 40 I = 1,2
                  IOFF = IX - I + 1   
                  JOFF = IY - 2    
                  IF(IY .EQ. KY) JOFF = JOFF - 1
                  IF(IY .EQ. 1) JOFF = JOFF + 1
                  KOFF = IZ - K  
                  IF(IOFF.GT.0 .AND. JOFF.GT.-1 .AND. KOFF.GT.-1 .AND.
     +               IOFF.LE.NX .AND. JOFF.LT.NY .AND. KOFF.LT.NZ)THEN
                    IE = IOFF + JOFF*NX + KOFF*NLAY + IEOF
                    IF(IPRPS(IE) .LT. 100 .OR. 
     +                (IPRPS(IE).LT.198 .AND. NOBLK)) THEN
C                     Add this value to the total                    
                      VTOT = VTOT + VVEL(IE)
                      NUMV   = NUMV + 1
                    ENDIF
                  ENDIF
 40             CONTINUE    
 50           CONTINUE
C 
C             W velocity              
              WTOT = 0
              NUMW = 0
C             Find the surrounding elements any one can be missing
              DO 60 I = 1,2
                DO 55 J = 1,2
                  IOFF = IX - I + 1   
                  JOFF = IY - J    
                  KOFF = IZ - 2 
                  IF(IZ .EQ. KZ) KOFF = KOFF - 1
                  IF(IZ .EQ. 1) KOFF = KOFF + 1
                  IF(IOFF.GT.0 .AND. JOFF.GT.-1 .AND. KOFF.GT.-1 .AND.
     +              IOFF.LE.NX .AND. JOFF.LT.NY .AND. KOFF.LT.NZ)THEN
                    IE = IOFF + JOFF*NX + KOFF*NLAY + IEOF
                    IF(IPRPS(IE) .LT. 100 .OR. 
     +                (IPRPS(IE).LT.198 .AND. NOBLK)) THEN
C                     Add this value to the total                    
                      WTOT = WTOT + WVEL(IE)
                      NUMW   = NUMW + 1
                    ENDIF
                  ENDIF
 55             CONTINUE    
 60           CONTINUE
C
C             Are there are valid velocities for the node
              IF(NUMU.GT.0 .OR. NUMV.GT.0 .OR. NUMW.GT.0) THEN
                NRV = NRV + 1
                IF(NRV .GT. NRMAX) THEN
                  IERR = 15
                  CALL ERRNP(IONEP,IERR,' ')
                  GOTO 900
                ENDIF  
                NODE(NRV) = IN
                IF(NMAP(IN) .LT. IN)NODE(NRV) = -IN
                MATER(NRV) = 0.0
                IF(NUMU .GT. 0) THEN
                  UVAL(NRV) = UTOT / NUMU
                ELSE
                  UVAL(NRV) = 0.0
                ENDIF  
                IF(NUMV .GT. 0) THEN
                  VVAL(NRV) = VTOT / NUMV
                ELSE
                  VVAL(NRV) = 0.0
                ENDIF  
                IF(NUMW .GT. 0) THEN
                  WVAL(NRV) = WTOT / NUMW
                ELSE
                  WVAL(NRV) = 0.0
                ENDIF  
              ENDIF
C                 
 70         CONTINUE
 80       CONTINUE
 90     CONTINUE
C      
      ENDIF
 900  CONTINUE
      RETURN
      END
      SUBROUTINE RPHINP(IOPHDA,TITLE,NX,NY,NZ,LOG,NAME,CARTES,ONEPHS,
     +                                     BFC,IONEP,IERR)
C***********************************************************************
C  Author: CYC
C  Creation Date: 23rd September 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Reads the basic information from the PHI file
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
C   	IOPHDA	-  Integer 	- Unit number PHIDA file is open on.  
C
C Output Parameters
C	TITLE	   - Character*40  	- Model title
C   NX      - Integer		- Number of cells in I for whole model grid      C	NY      - Integer		- Number of cells in J for whole model grid      
C 	NZ      - Integer		- Number of cells in K for whole model grid      
C	LOG(50) - Logical Array 	- Results stored flags
C	NAME(50)- Character*4 Array	- Names of results stored
C	CARTES	 - Logical	    - True if grid is cartesian, false if grid is polar
C	ONEPHS	 - Logical		- True if two phases are present
C	BFC	  - Logical		- True if grid is given in an XYZ file
C	IERR	  - Integer		- Error flag
C					=  6 - Error reading PHIDA file
C			=  0  - No error
C
C***********************************************************************
      LOGICAL CARTES,BFC,ONEPHS,LOG(50)
      CHARACTER*4 NAME(50)
      CHARACTER*40 TITLE
C
      IERR = 0
      READ(IOPHDA,REC=1,ERR=750) TITLE
      READ(IOPHDA,REC=2,ERR=750)CARTES,ONEPHS,BFC,LOG(1)
      READ(IOPHDA,REC=3,ERR=750)NX,NY,NZ
      READ(IOPHDA,REC=5,ERR=750)NAME
      READ(IOPHDA,REC=10,ERR=750)LOG
C
      GOTO 900
 750  CONTINUE
      IERR = 6
      CALL ERRNP(IONEP,IERR,' ')
 900  CONTINUE
      RETURN
      END
      SUBROUTINE CBLKNP(IJKBLK,NBLOCK,LIMBLK)
C***********************************************************************
C  Author: CYC
C  Creation Date: 23rd September 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Calculates the blocks limits for each block
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
C    IJKBLK - Block limits in big grid
C    NBLOCK - Number of blocks
C
C  Output Parameters
C    LIMBLK - Block size in local grid - number of cells
C
C***********************************************************************
C
      INTEGER IJKBLK(6,NBLOCK),LIMBLK(3,NBLOCK)
C
      DO 100 IB = 1,NBLOCK
        LIMBLK(1,IB) = IJKBLK(2,IB) - IJKBLK(1,IB) + 1
        LIMBLK(2,IB) = IJKBLK(4,IB) - IJKBLK(3,IB) + 1
        LIMBLK(3,IB) = IJKBLK(6,IB) - IJKBLK(5,IB) + 1
 100  CONTINUE       
C
      RETURN
      END
      SUBROUTINE CPHINP(IONEP,IOPHI,IOPHDA,INT,PHI,LENREF,IERR)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature                       Date      Author
C     Speed up reading of results  1/11/94   CYC
C
C  Function of Subroutine:   Convert ASCII PHI to DA
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
C     IONEP  - NEP file unit number
C     IOPHI  - PHI file unit number
C     IOPHDA - PHIDA unit number
C     INT     - Work array for reading Integers
C     PHI     - Work array for reading Reals
C     LENREF  - Number of reals / record
C
C  Output Parameters
C     IERR    - Error flag
C
C***********************************************************************
      EXTERNAL ERRNP
C
      REAL PHI(*)                                                  
      LOGICAL LOG(50),LOG1,LOG2,LOG3,LOG4
      CHARACTER*40 TITLE
      CHARACTER*4 NAME(50)
      INTEGER INT(LENREF)
C
C
      READ (IOPHI,1,ERR=700)TITLE
 1    FORMAT(A40)
      WRITE(IOPHDA,REC=1)TITLE
C
      READ(IOPHI,2,ERR=700)LOG1,LOG2,LOG3,LOG4
 2    FORMAT(1X,4L1)
      WRITE(IOPHDA,REC=2)LOG1,LOG2,LOG3,LOG4
C
      READ(IOPHI,3,ERR=700)(INT(J),J=1,11)
 3    FORMAT(1X,8I10,/,1X,4I10)
      WRITE(IOPHDA,REC=3)INT
      NX = INT(1)
      NY = INT(2)
      NZ = INT(3)
C
      READ(IOPHI,4,ERR=700)PHI(1)
 4    FORMAT(E13.2)
      WRITE(IOPHDA,REC=4)PHI(1)
C
      READ(IOPHI,5,ERR=700)NAME
 5    FORMAT(1X,19A4,/,1X,19A4,/,1X,12A4)
      WRITE(IOPHDA,REC=5)NAME
C
      READ(IOPHI,6,ERR=700)(PHI(I),I=1,NX)
 6    FORMAT(43(6(E13.0:),/))
      WRITE(IOPHDA,REC=6)(PHI(I),I=1,NX)
C
      READ(IOPHI,7,ERR=700)(PHI(I),I=1,NY)
 7    FORMAT(43(6(E13.0:),/))
      WRITE(IOPHDA,REC=7)(PHI(I),I=1,NY)
C
      READ(IOPHI,8,ERR=700)(PHI(I),I=1,NZ)
 8    FORMAT(43(6(E13.0:),/))
      WRITE(IOPHDA,REC=8)(PHI(I),I=1,NZ)
C
      READ(IOPHI,9,ERR=700)(PHI(I),I=1,NZ)
 9    FORMAT(43(6(E13.0:),/))
      WRITE(IOPHDA,REC=9)(PHI(I),I=1,NZ)
C
      READ(IOPHI,10,ERR=700)LOG
 10   FORMAT(1X,50L1)
      WRITE(IOPHDA,REC=10)LOG
C
C
C     Find the number of results stored
      NUMRES = 0
      DO 100 IR =1,50
        IF(LOG(IR))NUMRES = NUMRES + 1
 100  CONTINUE   
C     
      IREC = 10                                                         
      NLAYER = NX*NY                                                    
      NUMREC = NLAYER/LENREF                                                 
      NREST  = MOD(NLAYER,LENREF)       
      IF(NREST .NE. 0) THEN
        NUMREC = NUMREC + 1
      ELSE
        NREST = LENREF
      ENDIF                                         
C
      DO 130 IL = 1,NUMRES*NZ
        CALL RDARNP(IOPHI,PHI,NLAYER,IERR)
        IF(IERR .GT. 0) GOTO 700
        IOFF = 1
        DO 120 IR = 1,NUMREC
          NVAL = LENREF
          IF(IR .EQ. NUMREC) NVAL = NREST
          IREC = IREC + 1
          CALL WRARNP(IOPHDA,IREC,PHI(IOFF),NVAL)
          IOFF = IOFF + NVAL
 120    CONTINUE
 130  CONTINUE         
                                                      
      CLOSE(IOPHI)
      GOTO 999
 700  CONTINUE
      IERR = 5
      CALL ERRNP(IONEP,IERR,' ')
      GOTO 999
 999  CONTINUE
      RETURN
      END
      SUBROUTINE WRARNP(IO,IREC,ARR,LEN)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine: Write out real array.
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
      REAL ARR(LEN)
      WRITE(IO,REC=IREC)ARR
      RETURN
      END  
      SUBROUTINE RDARNP(IO,ARR,LEN,IERR)
      REAL ARR(LEN)
      READ(IO,5,ERR = 10)ARR
 5    FORMAT(6E13.0)
      GOTO 20
 10   CONTINUE
      IERR = 1     
 20   CONTINUE     
      RETURN
      END                    
      SUBROUTINE RDVLNP(IONEP,IOPHDA,IVLOC,NUMRES,LENREC,IJKBLK,NBLOCK,
     +                NX,NY,NZ,PBIG,PHI,IERR)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Read results values from the PHIDA file.
C
C  Called by: MAINNP,PRPSNP
C
C  Limitations
C
C  Source of further information
C
C  Examples of use
C
C  Input Parameters 
C    IONEP  - NEP file unit number
C    IOPHDA - PHIDA file unit number
C    IVLOC  - Location of result in array
C    NUMRES - Number of results found
C    LENREC - PHIDA file record length
C    IJKBLK - Block limits
C    NBLOCK - Number of blocks
C    NX     - Number of cells in I
C    NY     - Number of cells in J
C    NZ     - Number of cells in K
C
C  Output Parameters
C    PHI    - Results real values
C    IERR   - Error flag
C
C***********************************************************************
      EXTERNAL ERRNP
C
      REAL PHI(NZ*NY*NX),PBIG(NZ*NY*NX)
      INTEGER IJKBLK(6,NBLOCK)
C
      NLAYER = NY*NX
C     Number of records /layer 
      NRLAY = NLAYER / LENREC
C     Number of values for last layer 
      NRLAST = NLAYER - (NRLAY*LENREC)
      IF(NRLAST .GT. 0) NRLAY = NRLAY + 1
      IF(NRLAST .EQ. 0) NRLAST = LENREC
C     Record offset for each Z value 
      IZROFF = (NUMRES-1) * NRLAY      
C     First record is located at
      IFIRST = 10 + (IVLOC-1)*NRLAY
C     
C     Read the results     
C     
      IR = IFIRST 
C
      DO 50 K = 1,NZ
        INF = (K-1)*NX*NY
        DO 30 IP = 1,NRLAY
          IR = IR + 1
          INS = INF + 1
          INF = INS + LENREC - 1
          IF(IP .EQ. NRLAY)INF = INS + NRLAST - 1
          READ(IOPHDA,REC=IR,ERR=800)(PBIG(IN),IN=INS,INF)
 30     CONTINUE
        IR = IR + IZROFF
 50   CONTINUE
C
      IN = 0
      DO 100 IB = 1,NBLOCK
        NXB = IJKBLK(2,IB) - IJKBLK(1,IB) + 1
        NYB = IJKBLK(4,IB) - IJKBLK(3,IB) + 1
        NZB = IJKBLK(6,IB) - IJKBLK(5,IB) + 1
        DO 90 K = 1,NZB
          DO 80 J = 1,NYB
            DO 70 I = 1,NXB
              IN = IN + 1
              II = IJKBLK(1,IB) + I - 1
              JJ = IJKBLK(3,IB) + J - 1
              KK = IJKBLK(5,IB) + K - 1
              INBIG = JJ + (II-1)*NY + (KK-1)*NX*NY
              PHI(IN) = PBIG(INBIG) 
 70         CONTINUE
 80       CONTINUE
 90     CONTINUE
 100  CONTINUE             
C
      GOTO 900
C
 800  CONTINUE
      IERR = 13
      CALL ERRNP(IONEP,IERR,' ')
 900  CONTINUE                          
C
      RETURN
      END                  
      SUBROUTINE ADDNNP(IX,IY,IZ,KX,KY,KZ,NY,NLAY,IPRPS,NOBLK,MAT,
     +  NUMMAT,NUM,TOTAL,IM,PHI,IEOFF)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Calculate nodal averaged value inside a block.
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
      REAL TOTAL(8),PHI(*)
      INTEGER IPRPS(*),MAT(*),NUM(*)
      LOGICAL NOBLK
C
      NX = KX - 1
      NZ = KZ - 1
      DO 30 K = 1,2
        DO 20 J = 1,2
          DO 10 I = 1,2   
            IOFF = IX - I + 1   
            JOFF = IY - J    
            KOFF = IZ - K  
            IF(IOFF.GT.0 .AND. JOFF.GT.-1 .AND. KOFF.GT.-1 .AND.
     +       IOFF.LE.NX .AND. JOFF.LT.NY .AND. KOFF.LT.NZ)THEN
              IE = IOFF + JOFF*NX + KOFF*NLAY + IEOFF
              IF(IPRPS(IE) .LT. 100 .OR. 
     +           (IPRPS(IE).LT.198 .AND. NOBLK)) THEN
C               Find the material number
                CALL FNDMAT(IPRPS(IE),MAT,NUMMAT,NUM,TOTAL,IM)
C               Add this value to the total                    
                TOTAL(IM) = TOTAL(IM) + PHI(IE)
                NUM(IM)   = NUM(IM) + 1
              ENDIF
            ENDIF
 10       CONTINUE    
 20     CONTINUE    
 30   CONTINUE    
C    
C
      RETURN
      END      
      FUNCTION INOFF(IBREF,IJKBLK)
C
C     Calculates the nodal offset for a block
C
      INTEGER IJKBLK(6,*)
C
      INOFF = 0
      IF(IBREF .GT. 1) THEN
        DO 100 IB = 1,IBREF-1
          INOFF = (IJKBLK(2,IB)-IJKBLK(1,IB)+2) *   
     +            (IJKBLK(4,IB)-IJKBLK(3,IB)+2) *   
     +            (IJKBLK(6,IB)-IJKBLK(5,IB)+2) + INOFF
 100    CONTINUE
      ENDIF
      RETURN
      END      
      SUBROUTINE CELENP(IONEP,NMAP,IJKBLK,NBLOCK,IELMAX,IELEM,
     +                          NUMELE,IERR)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Creates the element definitions
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
C      IONEP  - NEP file unit number
C      NMAP   - Nodal mapping array
C      IJKBLK - Block limits
C      NBLOCK - Number of blocks
C      IBOFF  - Offset to block node number
C      IELMAX - Maximum number of elements
C
C  Output Parameters
C      IELEM  - Element definition array
C      NUMELE - Number of elements created
C      IERR   - Error flag
C
C***********************************************************************
      EXTERNAL NCONNP,ERRNP
C
      INTEGER IELEM(8,IELMAX),IJKBLK(6,NBLOCK),NMAP(*)
C
      IE = 0
      DO 100 IB = 1,NBLOCK
        INO = INOFF(IB,IJKBLK)
        NX = IJKBLK(2,IB) - IJKBLK(1,IB) + 1
        NY = IJKBLK(4,IB) - IJKBLK(3,IB) + 1
        NZ = IJKBLK(6,IB) - IJKBLK(5,IB) + 1
        KX = NX + 1
        KY = NY + 1
        KLAY = KY * KX 
        DO 80 K = 1,NZ
          DO 60 J = 1,NY
            DO 40 I = 1,NX
              IE = IE + 1
              IF(IE .GT. IELMAX) THEN
                IERR = 12
                CALL ERRNP(IONEP,IERR,' ')
                GOTO 900
              ENDIF   
              IEREF = I + (J-1)*KX + (K-1)*KLAY + INO
              IELEM(1,IE) = NCONNP(IEREF,NMAP)
              IELEM(2,IE) = NCONNP(IEREF + 1,NMAP)
              IELEM(3,IE) = NCONNP(IEREF + 1 + KX,NMAP)
              IELEM(4,IE) = NCONNP(IEREF + KX,NMAP)
              IELEM(5,IE) = NCONNP(IEREF + KLAY,NMAP)
              IELEM(6,IE) = NCONNP(IEREF + 1 + KLAY,NMAP)
              IELEM(7,IE) = NCONNP(IEREF + 1 + KX + KLAY,NMAP)
              IELEM(8,IE) = NCONNP(IEREF + KX + KLAY,NMAP)
 40         CONTINUE
 60       CONTINUE
 80     CONTINUE
C
 100  CONTINUE
C
      NUMELE = IE
 900  CONTINUE
      RETURN
      END                    
      SUBROUTINE CSCANP(IBREF,IJKBLK,PHI,IPRPS,NOBLK,NX,NY,NZ,
     +                   VAL,IELE,NRES)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Compressed scalr results for each block
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
C     IBREF  - Block number
C     IJKBLK - Block limits
C     PHI    - Cell centre results
C     IPRPS  - Material numbers
C     NOBLK  - Ignore material numbers flag
C
C  Output Parameters
C     VAL    - Compress results
C     IELE   - Element number for compressed results
C     IEP    - Element pointer
C     NRES   - Number of results after compression
C
C***********************************************************************
C
      REAL PHI(NX*NY*NZ),VAL(NX*NY*NZ)
      INTEGER IJKBLK(6,*),IPRPS(NX*NY*NZ),IELE(NX*NY*NZ)
      LOGICAL NOBLK      
C      
C      
      NRES = 0
      IEP = IEOFF(IBREF,IJKBLK)
C     Count the number of valid variables and compress        
      DO 40 IZ = IJKBLK(5,IBREF),IJKBLK(6,IBREF)
        DO 30 IY = IJKBLK(3,IBREF),IJKBLK(4,IBREF)
          DO 20 IX = IJKBLK(1,IBREF),IJKBLK(2,IBREF)
            IEP = IEP + 1
            IF(IPRPS(IEP) .LT. 100 .OR. 
     +        (IPRPS(IEP).LT.198 .AND. NOBLK)) THEN
              NRES = NRES + 1
              VAL(NRES) = PHI(IEP)
              IELE(NRES) = IEP
            ENDIF  
 20       CONTINUE
 30     CONTINUE
 40   CONTINUE
C
      RETURN
      END
      SUBROUTINE INBLNP(INREF,NMAP,IJKBLK,IPRPS,NOBLK,MAT,
     +             NUMMAT,NUM,TOTAL,IM,PHI,NBLOCK)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Calculate nodal averaged results between blocks.
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
      INTEGER MAT(8),NUM(8),IPRPS(*),IJKBLK(6,*),NMAP(*)
      REAL TOTAL(8),PHI(*)
      LOGICAL NOBLK
C
C     Starting node is INREF
      IN = INREF
      IEOF = 0
C     Find the node this points to 
 10   CONTINUE
      IN = IABS(NMAP(IN))
      IF(IN .NE. INREF) THEN
C       Find the block number for this node
        IB = 1
        ieof = 0
        IF(NBLOCK .GT. 1) THEN
          DO 100 IB = 1,NBLOCK-1
            KX = IJKBLK(2,IB) - IJKBLK(1,IB) + 2                                  
            KY = IJKBLK(4,IB) - IJKBLK(3,IB) + 2                                  
            KZ = IJKBLK(6,IB) - IJKBLK(5,IB) + 2     
            IF (IN .LE. INOFF(IB+1,IJKBLK)) GOTO 110
            IEOF = IEOF + (KX-1)*(KY-1)*(KZ-1)
 100      CONTINUE
          KX = IJKBLK(2,IB) - IJKBLK(1,IB) + 2                                  
          KY = IJKBLK(4,IB) - IJKBLK(3,IB) + 2                                  
          KZ = IJKBLK(6,IB) - IJKBLK(5,IB) + 2     
 110      CONTINUE
        ENDIF
C       Set KX,KY and KZ
        NY = KY - 1
        NX = KX - 1
        NLAY = NY*NX                             
C       Calculate the IJK for this node
        INBLK = IN - INOFF(IB,IJKBLK)
        K = INBLK/(KX*KY) + 1
        INBLK = INBLK - (K-1)*(KX*KY)
        IF(INBLK .EQ. 0) THEN
          K = K - 1
          I = KX
          J = KY
        ELSE  
          J = INBLK / KX  + 1 
          INBLK = INBLK - (J-1)*KX
          IF(INBLK .EQ. 0) THEN
            J = J - 1
            I = KX
          ELSE
            I = INBLK
          ENDIF    
        ENDIF
C          
C       Look at each posibile element attached to this node
        CALL ADDNNP(I,J,K,KX,KY,KZ,NY,NLAY,IPRPS,NOBLK,MAT,
     +             NUMMAT,NUM,TOTAL,IM,PHI,IEOF)
C       Next node
        GOTO 10             
      ENDIF
      RETURN
      END
      SUBROUTINE CCALNP(IOPHDA,CARTES,NX,NY,NZ,XFRAC,YFRAC,ZFRAC,
     +                   LENREF,NODMAX,CORD,IONEP,IERR)
C
C
      REAL XFRAC(LENREF),YFRAC(LENREF),ZFRAC(LENREF)
      REAL CORD(3,NODMAX)
      LOGICAL CARTES
C
      KX = NX + 1
      KY = NY + 1
      KZ = NZ + 1
C
      IERR = 0
      IF(KX*KY*KZ .GT. NODMAX) THEN 
        IERR = 10
        CALL ERRNP(IONEP,IERR,' ')
        GOTO 900
      ENDIF  
C      
      READ(IOPHDA,REC=4,ERR=750)RINNER
      READ(IOPHDA,REC=6,ERR=750)XFRAC
      READ(IOPHDA,REC=7,ERR=750)YFRAC
      READ(IOPHDA,REC=8,ERR=750)ZFRAC
      IN = 0
C     Assign the coordinates to the nodes
C
      ZCOORD = 0
      DO 70 IZ = 1,KZ
        YCOORD = 0                                                      
        DO 60 IY = 1,KY                                                 
          XCOORD = 0                                                    
          DO 50 IX = 1,KX                                               
C           Node number
            IN = IN + 1
C           Calculate X and Y if Polar
            IF (.NOT.CARTES) THEN
C             Calculate coordinates
              XCO = (YCOORD + RINNER) * SIN(XCOORD)
              YCO = (YCOORD + RINNER) * COS(XCOORD)
            ELSE
              XCO = XCOORD
              YCO = YCOORD
            ENDIF
C           Write out node
            CORD(1,IN) =  XCO
            CORD(2,IN) =  YCO
            CORD(3,IN) =  ZCOORD
C           Next X value
            XCOORD = XFRAC(IX)                                          
            IF (XCOORD .LT. 10E-10) XCOORD = 0                          
  50      CONTINUE
C         Next Y value
          YCOORD = YFRAC(IY)                                          
          IF (YCOORD.LT.10E-10) YCOORD = 0                              
  60    CONTINUE
C       Next Y value
        ZCOORD = ZFRAC(IZ)
        IF (ZCOORD.LT.10E-10) ZCOORD = 0
  70  CONTINUE
C
C
      GOTO 900
 750  CONTINUE
      IERR = 6
      CALL ERRNP(IONEP,IERR,' ')
 900  CONTINUE
C 
      RETURN
      END      
      SUBROUTINE ERRNP(IONEP,IERR,LINE)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Error message routine.
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
      CHARACTER*56 STRNG(3)
      CHARACTER*(*) LINE
C
      STRNG(1)='                                                       '
      STRNG(2)='                                                       '
      STRNG(3)='                                                       '
C      
      GOTO(10,20,30,40,50,60,70,80,90,100,110,120,130,140,150)IERR
C      
C      
 10   CONTINUE
      STRNG(1) = ' Error Opening NEP file'
      NL = 1
      GOTO 900
C
 20   CONTINUE
      STRNG(1) = ' Error Opening PATGEO file'
      NL = 1
      GOTO 900
C
 30   CONTINUE
      STRNG(1) = ' Error Opening PHI file'
      NL = 1
      GOTO 900
C
 40   CONTINUE
      STRNG(1) = ' Error Opening PHIDA file'
      NL = 1
      GOTO 900
C
 50   CONTINUE
      STRNG(1) = ' Error Reading PHI file'
      NL = 1
      GOTO 900
C
 60   CONTINUE
      STRNG(1) = ' Error Reading PHIDA file'
      NL = 1
      GOTO 900
C
 70   CONTINUE
      STRNG(1) = ' Error Reading PATGEO file'
      NL = 1
      GOTO 900
C      
 80   CONTINUE
      STRNG(1) = ' Number of Blocks exceeds maximum permitted'
      STRNG(2) = ' Increase NBMAX'
      NL = 1
      GOTO 900
C      
 90   CONTINUE
      STRNG(1) = ' Number of Links exceeds maximum permitted'
      STRNG(2) = ' Increase NLMAX'
      NL = 1
      GOTO 900
C      
 100  CONTINUE
      STRNG(1) = ' Number of Nodes exceeds maximum permitted'
      STRNG(2) = ' Increase NODMAX'
      NL = 2
      GOTO 900
C      
 110  CONTINUE
      STRNG(1) = ' Error reading XYZ file'
      NL = 1
      GOTO 900
C      
 120  CONTINUE
      STRNG(1) = ' Number of Elements exceeds maximum permitted'
      STRNG(2) = ' Increase IELMAX'
      NL = 2
      GOTO 900
C      
 130  CONTINUE
      STRNG(1) = ' Error reading PHIDA file'
      NL = 1
      GOTO 900
C      
 140  CONTINUE
      STRNG(1) = ' XYZ file does not match PHI file'
      NL = 1
      GOTO 900
C      
 150  CONTINUE
      STRNG(1) = ' Number of nodal results for the block exceeds NRMAX'
      STRNG(2) = ' Increase the value of NRMAX'
      NL = 2
      GOTO 900
C      
C      
 900  CONTINUE
      WRITE(*,*)'**** Error Creating NEP file ****'
      IF(IERR .GT. 1)WRITE(IONEP,*)'**** Error Creating NEP file ****'
      DO 990 IL = 1,NL
        WRITE(*,*) STRNG(IL)
        IF(IERR .GT. 1)WRITE(IONEP,*)STRNG(IL)
 990  CONTINUE       
      WRITE(*,*)'**** Translation will STOP   ****'
      IF(IERR .GT. 1)WRITE(IONEP,*)'**** Translation will STOP   ****'
      RETURN
      END           
      SUBROUTINE PTYPNP(XDIR,LENX,PDIR,LENP,LBYTE,LENREF)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Find the PHI file type from the config file
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
      LOGICAL XDIR,PDIR
      PDIR = .FALSE.
      XDIR = .FALSE.
      LENX = 5
      LENP = 5
      LENREF = 256
      LBYTE = 4
      RETURN
      END
      SUBROUTINE NAVGNP(IPRPS,PHI,IJKBLK,IB,NOBLK,NMAP,NBLOCK,
     +                 LINK,NLINK,NRMAX,VAL,MATER,NODE,NRV,IONEP,IERR)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Calculates nodally averaged results for a 
C                            block.
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
C     IPRPS - Material numbers array
C     PHI   - Results values - cell centre
C     IJKBLK - Block limits
C     IB    - Block number
C     NOBLK - Ignore material value when blocking
C     NMAP  - Node mapping array
C     NBLOCK - Number of blocks
C     LINK   - Link definitions
C     NLINK  - Number of links
C     NRMAX  - Maximim number of results possible
C
C
C  Output Parameters
C     VAL   - Values for the block
C     MATER - Material for which the value is valid
C     NODE  - Node the result is valid for
C     NRV    - Number of results values for the block
C
C***********************************************************************
      EXTERNAL ADDNNP,INBLNP,MIDSNP 
C
C
      INTEGER MAT(8),NUM(8),IPRPS(*),IJKBLK(6,*),NODE(NRMAX)
      INTEGER MATER(NRMAX),NMAP(*),LINK(11,NLINK)
      REAL TOTAL(8),VAL(NRMAX),PHI(*)
      LOGICAL NOBLK
C
      IERR = 0
      IP = 0
      IN = INOFF(IB,IJKBLK)
      IEOF = IEOFF(IB,IJKBLK)
C     Find the block limits
      KX = IJKBLK(2,IB) - IJKBLK(1,IB) + 2
      KY = IJKBLK(4,IB) - IJKBLK(3,IB) + 2
      KZ = IJKBLK(6,IB) - IJKBLK(5,IB) + 2
      KLAY = KX * KY
      NX = KX - 1
      NY = KY - 1
      NLAY = NX * NY
C     For each node in the block
      DO 90 IZ = 1,KZ
        DO 80 IY = 1,KY
          DO 70 IX = 1,KX
C           Find the node number
            IN = IN + 1
            NUMMAT = 0
C           Find the surrounding elements any one can be missing                             
            CALL ADDNNP(IX,IY,IZ,KX,KY,KZ,NY,NLAY,IPRPS,NOBLK,MAT,
     +          NUMMAT,NUM,TOTAL,IM,PHI,IEOF)
C
            NN = IN
C           Look for other nodes
            IF(IABS(NMAP(IN)) .NE. IN ) THEN
              CALL INBLNP(IN,NMAP,IJKBLK,IPRPS,NOBLK,MAT,
     +             NUMMAT,NUM,TOTAL,IM,PHI,NBLOCK)
              IF(IABS(NMAP(IN)) .LT. IN) NN = -IN
            ENDIF 
            IF(NMAP(IN) .LT. 0) 
     +       CALL MIDSNP(IB,IX,IY,IZ,IPRPS,NOBLK,MAT,NUMMAT,NUM,
     +        TOTAL,IM,PHI,NBLOCK,LINK,NLINK,IJKBLK)         
C           Calculate nodal average for each material
            IF(NUMMAT .NE. 0) THEN
              DO 30 IM = 1,NUMMAT
                IP = IP + 1
                IF(IP .GT. NRMAX) THEN
                  IERR = 15
                  CALL ERRNP(IONEP,IERR,' ')
                  GOTO 900
                ENDIF  
                VAL(IP) = TOTAL(IM) / NUM(IM)
                NODE(IP) = NN
                MATER(IP) = MAT(IM)  
 30           CONTINUE 
            ENDIF                
C
 70       CONTINUE
 80     CONTINUE
 90   CONTINUE
      NRV = IP
 900  CONTINUE
C
      RETURN
      END
      SUBROUTINE LINKNP(LINK,NLINK,IJKBLK,NMAP)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Converts links to coincident node numbers.
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
      INTEGER LINK(11,NLINK),IJKBLK(6,*),NMAP(*)
      INTEGER IDA(3),KDA(3),IDB(3),KDB(3)
C
C     For each link
      DO 100 IL = 1,NLINK,2
C       Get body number
        IBA = LINK(1,IL)
        IBB = LINK(2,IL) 
        IBOA = INOFF(IBA,IJKBLK)       
        IBOB = INOFF(IBB,IJKBLK)   
C       Set start and finish values
        IS1A = LINK(4,IL)
        IF1A = LINK(5,IL)
        IS2A = LINK(7,IL)
        IF2A = LINK(8,IL)
        IS3A = LINK(10,IL)
        IF3A = LINK(11,IL)
        IS1B = LINK(4,IL+1)
        IF1B = LINK(5,IL+1)
        IS2B = LINK(7,IL+1)
        IF2B = LINK(8,IL+1)
        IS3B = LINK(10,IL+1)
        IF3B = LINK(11,IL+1)
C             
C       Find step  for each direction
C       Direction1
        ND1A = IF1A - IS1A + 1                   
        ND1B = IF1B - IS1B + 1    
        IF(ND1A .GT. ND1B) THEN
          ND1 = ND1A
          ND1SA = (ND1A - 1) / (ND1B - 1)
          ND1SB = 1
          ND1ST = ND1SA
          LINK(1,IL) = -IABS(LINK(1,IL))
        ELSEIF(ND1B .GT. ND1A) THEN
          ND1 = ND1B
          ND1SB = (ND1B - 1) / (ND1A - 1)
          ND1SA = 1
          ND1ST = ND1SB
          LINK(1,IL) = -IABS(LINK(1,IL))
        ELSE
          ND1 = ND1A
          ND1SA = 1
          ND1SB = 1
          ND1ST = 1
        ENDIF
C       Direction2
        ND2A = IF2A - IS2A + 1                   
        ND2B = IF2B - IS2B + 1    
        IF(ND2A .GT. ND2B) THEN
          ND2 = ND2A
          ND2SA = (ND2A - 1) / (ND2B - 1)
          ND2SB = 1
          ND2ST = ND2SA
          LINK(1,IL) = -IABS(LINK(1,IL))
        ELSEIF(ND2B .GT. ND2A) THEN
          ND2 = ND2B
          ND2SB = (ND2B - 1) / (ND2A - 1)
          ND2SA = 1
          ND2ST = ND2SB
          LINK(1,IL) = -IABS(LINK(1,IL))
        ELSE
          ND2 = ND2A
          ND2SA = 1
          ND2SB = 1
          ND2ST = 1
        ENDIF
C       Direction3
        ND3A = IF3A - IS3A + 1                   
        ND3B = IF3B - IS3B + 1    
        IF(ND3A .GT. ND3B) THEN
          ND3 = ND3A
          ND3SA = (ND3A - 1) / (ND3B - 1)
          ND3SB = 1
          ND3ST = ND3SA
          LINK(1,IL) = -IABS(LINK(1,IL))
        ELSEIF(ND3B .GT. ND3A) THEN
          ND3 = ND3B
          ND3SB = (ND3B - 1) / (ND3A - 1)
          ND3SA = 1
          ND3ST = ND3SB
          LINK(1,IL) = -IABS(LINK(1,IL))
        ELSE
          ND3 = ND3A
          ND3SA = 1
          ND3SB = 1
          ND3ST = 1
        ENDIF
C       
C       Convert direction to step value
        DO 10 ID = 1,3
          IDA(ID) = LINK(3*ID,IL)
          IF(IDA(ID) .EQ. 1) THEN
            KDA(ID) = 1
          ELSEIF(IDA(ID) .EQ. 2) THEN
            KDA(ID) = IJKBLK(2,IBA) - IJKBLK(1,IBA) + 2  
          ELSE
            KDA(ID) = (IJKBLK(2,IBA) - IJKBLK(1,IBA) + 2 ) *
     +                (IJKBLK(4,IBA) - IJKBLK(3,IBA) + 2 )
          ENDIF
          IDB(ID) = LINK(3*ID,IL+1)
          IF(IDB(ID) .EQ. 1) THEN
            KDB(ID) = 1
          ELSEIF(IDB(ID) .EQ. 2) THEN
            KDB(ID) = IJKBLK(2,IBB) - IJKBLK(1,IBB) + 2  
          ELSE
            KDB(ID) = (IJKBLK(2,IBB) - IJKBLK(1,IBB) + 2 ) *
     +                (IJKBLK(4,IBB) - IJKBLK(3,IBB) + 2 )
          ENDIF
 10     CONTINUE
C
C       Generate node list
        DO 50 ID1 = 1,ND1,ND1ST      
          DO 40 ID2 = 1,ND2,ND2ST      
            DO 30 ID3 = 1,ND3,ND3ST  
              ID = (ID1-1)/ND1ST + 1 
              IID1A = (ID-1)*ND1SA + IS1A - IJKBLK(IDA(1)*2-1,IBA) + 1
              IID1B = (ID-1)*ND1SB + IS1B - IJKBLK(IDB(1)*2-1,IBB) + 1
              ID = (ID2-1)/ND2ST + 1 
              IID2A = (ID-1)*ND2SA + IS2A - IJKBLK(IDA(2)*2-1,IBA) + 1
              IID2B = (ID-1)*ND2SB + IS2B - IJKBLK(IDB(2)*2-1,IBB) + 1
              ID = (ID3-1)/ND3ST + 1 
              IID3A = (ID-1)*ND3SA + IS3A - IJKBLK(IDA(3)*2-1,IBA) + 1
              IID3B = (ID-1)*ND3SB + IS3B - IJKBLK(IDB(3)*2-1,IBB) + 1
              INA = 1 + (IID1A-1)*KDA(1) + (IID2A-1)*KDA(2) + 
     +                      (IID3A-1)*KDA(3) + IBOA
              INB = 1 + (IID1B-1)*KDB(1) + (IID2B-1)*KDB(2) + 
     +                      (IID3B-1)*KDB(3) + IBOB
              CALL LNODNP(INA,INB,NMAP)
C             Negate the intermediate nodes
              IF(ND1SA .GT. 1) THEN
                DO 20 IDD = ID1+1,ID1+ND1SA-1
                  II = (IDD-1) + IS1A - IJKBLK(IDA(1)*2-1,IBA) + 1
                  INA = 1 + (II-1)*KDA(1) + (IID2A-1)*KDA(2) + 
     +                      (IID3A-1)*KDA(3) + IBOA
                  NMAP(INA) = -IABS(NMAP(INA))
 20             CONTINUE         
              ENDIF        
              IF(ND2SA .GT. 1) THEN
                DO 21 IDD = ID2+1,ID2+ND2SA-1
                  II = (IDD-1) + IS2A - IJKBLK(IDA(2)*2-1,IBA) + 1
                  INA = 1 + (IID1A-1)*KDA(1) + (II-1)*KDA(2) + 
     +                      (IID3A-1)*KDA(3) + IBOA
                  NMAP(INA) = -IABS(NMAP(INA))
 21             CONTINUE  
              ENDIF               
              IF(ND3SA .GT. 1) THEN
                DO 22 IDD = ID3+1,ID3+ND3SA-1
                  II = (IDD-1) + IS3A - IJKBLK(IDA(3)*2-1,IBA) + 1
                  INA = 1 + (IID1A-1)*KDA(1) + (II-1)*KDA(2) + 
     +                      (IID3A-1)*KDA(3) + IBOA
                  NMAP(INA) = -IABS(NMAP(INA))
 22             CONTINUE  
              ENDIF               
              IF(ND1SB .GT. 1) THEN
                DO 23 IDD = ID1+1,ID1+ND1SB-1
                  II = (IDD-1) + IS1B - IJKBLK(IDB(1)*2-1,IBB) + 1
                  INB = 1 + (II-1)*KDB(1) + (IID2B-1)*KDB(2) + 
     +                      (IID3B-1)*KDB(3) + IBOB
                  NMAP(INB) = -IABS(NMAP(INB))
 23             CONTINUE         
              ENDIF        
              IF(ND2SB .GT. 1) THEN
                DO 24 IDD = ID2+1,ID2+ND2SB-1
                  II = (IDD-1) + IS2B - IJKBLK(IDB(2)*2-1,IBB) + 1
                  INB = 1 + (IID1B-1)*KDB(1) + (II-1)*KDB(2) + 
     +                      (IID3B-1)*KDB(3) + IBOB
                  NMAP(INB) = -IABS(NMAP(INB))
 24             CONTINUE  
              ENDIF               
              IF(ND3SB .GT. 1) THEN
                DO 25 IDD = ID3+1,ID3+ND3SB-1
                  II = (IDD-1) + IS3B - IJKBLK(IDB(3)*2-1,IBB) + 1
                  INB = 1 + (IID1B-1)*KDB(1) + (II-1)*KDB(2) + 
     +                      (IID3B-1)*KDB(3) + IBOB
                  NMAP(INB) = -IABS(NMAP(INB))
 25             CONTINUE  
              ENDIF               
 30         CONTINUE
 40       CONTINUE
 50     CONTINUE     
 100  CONTINUE
C
      RETURN
      END
      SUBROUTINE RNODNP(IONEP,IOGRD,DIRECT,IJKBLK,NBLOCK,LENREC,
     +                  NODMAX,NX,NY,NZ,CBIG,COORD,IERR )
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Reads the coordinates from the GRID file and 
C                           sorts then.
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
C     IONEP  - NEP file unit number
C     IOGRD  - XYZ file unit number
C     DIRECT - XYZ file type
C     IJKBLK - Block definitions
C     NBLOCK - Number of blocks
C     LENREC - Record Length
C     NODMAX - Maximum number of nodes permitted
C
C  Output Parameters
C     NX     - Number of cells in I
C     NY     - Number of cells in J
C     NZ     - Number of cells in K
C     COORD  - Nodal coordinates array
C     IERR   - Error flag
C
C***********************************************************************
      EXTERNAL ERRNP
C
      LOGICAL DIRECT
      REAL COORD(3,NODMAX),CBIG(3,NODMAX)
      INTEGER IJKBLK(6,NBLOCK)
C
      IF(DIRECT) THEN
C       Read the grid coordinates
        READ(IOGRD,REC=1)KX,KY,KZ
      ELSE
C       Read the grid coordinates
        READ(IOGRD,'(3I5)')KX,KY,KZ
      ENDIF
C     Does the grid file match
      IF(KX .NE. (NX+1) .OR. KY .NE. (NY+1) .OR. KZ .NE. (NZ+1)) THEN
        IERR = 14 
        CALL ERRNP(IONEP,IERR,' ')
        GOTO 999
      ENDIF  
      IF(KX*KY*KZ .GT. NODMAX) THEN
        IERR = 10 
        CALL ERRNP(IONEP,IERR,' ')
        GOTO 999
      ENDIF  
C
      IR = 1
      NRLAY = KX*KY / LENREC
      NREM = KX*KY - (NRLAY * LENREC)
      IF(NREM .EQ. 0) THEN
        NREM = LENREC
      ELSE
        NRLAY = NRLAY + 1
      ENDIF  
C
      DO 50 K = 1,KZ
C       For each direction
        DO 40 ID = 1,3
          INF = (K-1)*KX*KY
          IF(DIRECT) THEN
            DO 30 IP = 1,NRLAY
              IR = IR + 1
              INS = INF + 1
              INF = INS + LENREC - 1
              IF(IP .EQ. NRLAY)INF = INS + NREM - 1
              READ(IOGRD,REC=IR)(CBIG(ID,IN),IN=INS,INF)
 30         CONTINUE
          ELSE
            INS = (K-1)*KX*KY + 1
            INF = K*KX*KY
            READ(IOGRD,'(5(E13.6:))')(CBIG(ID,IN),IN=INS,INF)
          ENDIF  
 40     CONTINUE
 50   CONTINUE
C
      IN = 0
      DO 100 IB = 1,NBLOCK
        KXB = IJKBLK(2,IB) - IJKBLK(1,IB) + 2
        KYB = IJKBLK(4,IB) - IJKBLK(3,IB) + 2
        KZB = IJKBLK(6,IB) - IJKBLK(5,IB) + 2
        DO 90 K = 1,KZB
          DO 80 J = 1,KYB
            DO 70 I = 1,KXB
              IN = IN + 1
              II = IJKBLK(1,IB) + I - 1
              JJ = IJKBLK(3,IB) + J - 1
              KK = IJKBLK(5,IB) + K - 1
              INBIG = JJ + (II-1)*KY + (KK-1)*KX*KY
              COORD(1,IN) = CBIG(1,INBIG) 
              COORD(2,IN) = CBIG(2,INBIG) 
              COORD(3,IN) = CBIG(3,INBIG) 
 70         CONTINUE
 80       CONTINUE
 90     CONTINUE
 100  CONTINUE             
C
      GOTO 999
 999  CONTINUE 
      RETURN
      END     
      SUBROUTINE VELONP(LOCU,LOCV,LOCW,IONEP,IOPHDA,LENREF,IJKBLK,NX,
     +    NY,NZ,NEBIG,IP,NUMRES,IWORK,WORK,CCC,CRT,NBLOCK,NRMAX,NRESV,
     +    NUMNOD,NAME,NMAP,LINK,NLINK,IERR,
     +	NUMELE,TEMP,CDA_ID,NDA_ID,IDSIZE)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Write out velocity values.
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
C     Write out velocities
C
      INTEGER IJKBLK(6,NBLOCK),IWORK(*),NRESV(NBLOCK),NMAP(*),
     + LINK(NLINK)
      REAL XFRAC(256),WORK(*)
      LOGICAL NOBLK,CCC,CRT
      CHARACTER*4 NAME(50)
c XXX
	INTEGER NUMELE,CDA_ID(IDSIZE), NDA_ID(IDSIZE)
	REAL TEMP(*)

c XXX
C
      IERR = 0            
C      
C     Are there any velocities
      IF(LOCU.GT.0 .OR. LOCV.GT.0 .OR. LOCW.GT.0) THEN 
        CALL RVELNP(IONEP,IOPHDA,LENREF,IJKBLK,NBLOCK,NX,NY,NZ,
     +   NUMRES,LOCU,LOCV,LOCW,XFRAC,CCC,CRT,WORK(3*NEBIG+1),WORK,
     +   WORK(NEBIG+1),WORK(2*NEBIG+1),IERR)
        IF(IERR .GT. 0) GOTO 900
        NOBLK = .FALSE.
C       Offset pointers
        IOFF = 1
        DO 200 IB = 1,NBLOCK
C         Scalar velocities        
          CALL VELSNP(IB,IJKBLK,IWORK,NOBLK,CCC,WORK,WORK(NEBIG+1),
     +     WORK(2*NEBIG+1),NX,NY,NZ,WORK(3*NEBIG+IOFF),
     +     WORK(4*NEBIG+IOFF),WORK(5*NEBIG+IOFF),IWORK(NEBIG+IOFF),
     +     NRESV(IB))
          IOFF = IOFF + NRESV(IB)
 200    CONTINUE         
        IF(LOCU .GT. 0) THEN
          IS = 1
          WRITE(IONEP,*)NAME(LOCU),' ELEMC'
          DO 210 IB = 1,NBLOCK
            WRITE(IONEP,*)NRESV(IB)
            IF = IS + NRESV(IB) - 1
c            WRITE(IONEP,'(3(I8,1X,E13.6,1X))')
c     +      (IWORK(NEBIG+I),WORK(3*NEBIG+I),I=IS,IF)
c XXX
		  DO 81 I=IS,IF
	        NE = IWORK(NEBIG+I)
		    TEMP(NE) = WORK(3*NEBIG+I)
 81		  CONTINUE
c XXX
            IS = IS + NRESV(IB)
 210      CONTINUE
	    CALL WRITE_DATA(CDA_ID,LOCU,NAME(LOCU),NUMELE,TEMP)         
        ENDIF
        IF(LOCV .GT. 0) THEN
          IS = 1
          WRITE(IONEP,*)NAME(LOCV),' ELEMC'
          DO 220 IB = 1,NBLOCK
            WRITE(IONEP,*)NRESV(IB)
            IF = IS + NRESV(IB) - 1
c           WRITE(IONEP,'(3(I8,1X,E13.6,1X))')
c     +     (IWORK(NEBIG+I),WORK(4*NEBIG+I),I=IS,IF)
c XXX
		  DO 82 I=IS,IF
	        NE = IWORK(NEBIG+I)
		    TEMP(NE) = WORK(4*NEBIG+I)
 82		  CONTINUE
c XXX
            IS = IS + NRESV(IB)

 220      CONTINUE         
	    CALL WRITE_DATA(CDA_ID,LOCV,NAME(LOCV),NUMELE,TEMP)         
        ENDIF
        IF(LOCW .GT. 0) THEN
          IS = 1
          WRITE(IONEP,*)NAME(LOCW),' ELEMC'
          DO 230 IB = 1,NBLOCK
            WRITE(IONEP,*)NRESV(IB)
            IF = IS + NRESV(IB) - 1
c            WRITE(IONEP,'(3(I8,1X,E13.6,1X))')
c     +     (IWORK(NEBIG+I),WORK(5*NEBIG+I),I=IS,IF)
c XXX
		  DO 83 I=IS,IF
	        NE = IWORK(NEBIG+I)
		    TEMP(NE) = WORK(5*NEBIG+I)
 83		  CONTINUE
c XXX
            IS = IS + NRESV(IB)
 230      CONTINUE
		CALL WRITE_DATA(CDA_ID,LOCW,NAME(LOCW),NUMELE,TEMP)                
        ENDIF
C          
C       Offset pointers
        IOFF = 1
        DO 300 IB = 1,NBLOCK
C         Nodal averaged velocities          
          CALL VAVGNP(IWORK,IJKBLK,IB,NOBLK,NMAP,NBLOCK,LINK,
     +     NLINK,WORK,WORK(NEBIG+1),WORK(2*NEBIG+1),CCC,NRMAX,
     +     WORK(3*NEBIG+IOFF),WORK(3*NEBIG+IOFF+NRMAX),
     +     WORK(3*NEBIG+IOFF+2*NRMAX),IWORK(NEBIG+IOFF),
     +     IWORK(NEBIG+NRMAX+IOFF),NRESV(IB),IONEP,IERR)
          IF(IERR .GT. 0) GOTO 900
          IOFF = IOFF + NRESV(IB)
 300    CONTINUE         
        IF(LOCU .GT. 0) THEN
          IS = 1
          WRITE(IONEP,*)NAME(LOCU),' NODAL'
          DO 310 IB = 1,NBLOCK
            WRITE(IONEP,*)NRESV(IB)
            IF = IS + NRESV(IB) - 1
c           WRITE(IONEP,'(3(I7,1X,I3,1X,E13.6,1X))')
c     +      (IWORK(NEBIG+NRMAX+I),
c     +      IWORK(NEBIG+I),WORK(3*NEBIG+I),I=IS,IF)
c XXX
		  DO 84 I=IS,IF
	        NN = IWORK(NEBIG+NRMAX+I)
		    TEMP(NN) = WORK(3*NEBIG+I)
 84		  CONTINUE
c XXX
            IS = IS + NRESV(IB)
 310      CONTINUE
		CALL WRITE_DATA(NDA_ID,LOCU-1,NAME(LOCU),NUMNOD,TEMP)
        ENDIF
        IF(LOCV .GT. 0) THEN
          IS = 1
          WRITE(IONEP,*)NAME(LOCV),' NODAL'
          DO 320 IB = 1,NBLOCK
            WRITE(IONEP,*)NRESV(IB)
            IF = IS + NRESV(IB) - 1
c            WRITE(IONEP,'(3(I7,1X,I3,1X,E13.6,1X))')
c     +      (IWORK(NEBIG+NRMAX+I),
c     +      IWORK(NEBIG+I),WORK(3*NEBIG+I+NRMAX),I=IS,IF)
c XXX
		  DO 85 I=IS,IF
	        NN = IWORK(NEBIG+NRMAX+I)
		    TEMP(NN) = WORK(3*NEBIG+I+NRMAX)
 85		  CONTINUE
c XXX
            IS = IS + NRESV(IB)
 320      CONTINUE         
		CALL WRITE_DATA(NDA_ID,LOCV-1,NAME(LOCV),NUMNOD,TEMP)
        ENDIF
        IF(LOCW .GT. 0) THEN
          IS = 1
          WRITE(IONEP,*)NAME(LOCW),' NODAL'
          DO 330 IB = 1,NBLOCK
            WRITE(IONEP,*)NRESV(IB)
            IF = IS + NRESV(IB) - 1
c            WRITE(IONEP,'(3(I7,1X,I3,1X,E13.6,1X))')
c     +      (IWORK(NEBIG+NRMAX+I),IWORK(NEBIG+I),
c     +      WORK(3*NEBIG+I+2*NRMAX),I=IS,IF)
c XXX
		  DO 86 I=IS,IF
	        NN = IWORK(NEBIG+NRMAX+I)
		    TEMP(NN) = WORK(3*NEBIG+I+2*NRMAX)
 86		  CONTINUE
c XXX
            IS = IS + NRESV(IB)
 330      CONTINUE         
		CALL WRITE_DATA(NDA_ID,LOCW-1,NAME(LOCW),NUMNOD,TEMP)
        ENDIF
      ENDIF
C
 900  CONTINUE
      RETURN
      END            
      SUBROUTINE MIDSNP(IBR,IXREF,IYREF,IZREF,IPRPS,NOBLK,MAT,NUMMAT,
     +  NUM,TOTAL,IM,PHI,NBLOCK,LINK,NLINK,IJKBLK)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Averages across many to one links.
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
      REAL TOTAL(8),PHI(*)
      INTEGER IPRPS(*),MAT(*),NUM(*),LINK(11,NLINK),IJKBLK(6,*),IPREF(3)
      INTEGER ND(3),LD(3)	                                         
      LOGICAL NOBLK,XIN,YIN,ZIN
C
C      
      DO 100 IL = 1,NLINK,2
C       Many to 1 link has -ve 1st value
        IF(LINK(1,IL) .LT. 0 .AND. 
     +    (LINK(1,IL) .EQ. -IBR .OR. LINK(2,IL) .EQ. IBR) ) THEN
C         Extract link limits
C         This node is on IBR other body is IBL
          IF(LINK(1,IL) .EQ. -IBR) THEN
            IBL = LINK(2,IL)
            ID1R = LINK(3,IL)
            IS1R = LINK(4,IL) - IJKBLK(ID1R*2-1,IBR) + 1          
            IF1R = LINK(5,IL) - IJKBLK(ID1R*2-1,IBR) + 1            
            ID2R = LINK(6,IL)
            IS2R = LINK(7,IL) - IJKBLK(ID2R*2-1,IBR) + 1            
            IF2R = LINK(8,IL) - IJKBLK(ID2R*2-1,IBR) + 1            
            ID3R = LINK(9,IL)
            IS3R = LINK(10,IL) - IJKBLK(ID3R*2-1,IBR) + 1            
            IF3R = LINK(11,IL) - IJKBLK(ID3R*2-1,IBR) + 1            
            ID1L = LINK(3,IL+1)
            IS1L = LINK(4,IL+1) - IJKBLK(ID1L*2-1,IBL) + 1            
            IF1L = LINK(5,IL+1) - IJKBLK(ID1L*2-1,IBL) + 1            
            ID2L = LINK(6,IL+1)
            IS2L = LINK(7,IL+1) - IJKBLK(ID2L*2-1,IBL) + 1            
            IF2L = LINK(8,IL+1) - IJKBLK(ID2L*2-1,IBL) + 1            
            ID3L = LINK(9,IL+1)
            IS3L = LINK(10,IL+1) - IJKBLK(ID3L*2-1,IBL) + 1            
            IF3L = LINK(11,IL+1) - IJKBLK(ID3L*2-1,IBL) + 1
          ELSE              
            IBL = IABS(LINK(1,IL))
            ID1R = LINK(3,IL+1)
            IS1R = LINK(4,IL+1) - IJKBLK(ID1R*2-1,IBR) + 1            
            IF1R = LINK(5,IL+1) - IJKBLK(ID1R*2-1,IBR) + 1            
            ID2R = LINK(6,IL+1)
            IS2R = LINK(7,IL+1) - IJKBLK(ID2R*2-1,IBR) + 1            
            IF2R = LINK(8,IL+1) - IJKBLK(ID2R*2-1,IBR) + 1            
            ID3R = LINK(9,IL+1)
            IS3R = LINK(10,IL+1) - IJKBLK(ID3R*2-1,IBR) + 1            
            IF3R = LINK(11,IL+1) - IJKBLK(ID3R*2-1,IBR) + 1            
            ID1L = LINK(3,IL)
            IS1L = LINK(4,IL) - IJKBLK(ID1L*2-1,IBL) + 1            
            IF1L = LINK(5,IL) - IJKBLK(ID1L*2-1,IBL) + 1            
            ID2L = LINK(6,IL)
            IS2L = LINK(7,IL) - IJKBLK(ID2L*2-1,IBL) + 1            
            IF2L = LINK(8,IL) - IJKBLK(ID2L*2-1,IBL) + 1            
            ID3L = LINK(9,IL)
            IS3L = LINK(10,IL) - IJKBLK(ID3L*2-1,IBL) + 1            
            IF3L = LINK(11,IL) - IJKBLK(ID3L*2-1,IBL) + 1
          ENDIF 
C
C         Is reference node covered by link limits on ref body
          IF(ID1R .EQ. 1)  XIN = IS1R .LE. IXREF .AND. IF1R .GE. IXREF
          IF(ID2R .EQ. 1)  XIN = IS2R .LE. IXREF .AND. IF2R .GE. IXREF
          IF(ID3R .EQ. 1)  XIN = IS3R .LE. IXREF .AND. IF3R .GE. IXREF
          IF(ID1R .EQ. 2)  YIN = IS1R .LE. IYREF .AND. IF1R .GE. IYREF
          IF(ID2R .EQ. 2)  YIN = IS2R .LE. IYREF .AND. IF2R .GE. IYREF
          IF(ID3R .EQ. 2)  YIN = IS3R .LE. IYREF .AND. IF3R .GE. IYREF
          IF(ID1R .EQ. 3)  ZIN = IS1R .LE. IZREF .AND. IF1R .GE. IZREF
          IF(ID2R .EQ. 3)  ZIN = IS2R .LE. IZREF .AND. IF2R .GE. IZREF
          IF(ID3R .EQ. 3)  ZIN = IS3R .LE. IZREF .AND. IF3R .GE. IZREF
C
C         This link includes the reference node so resolve
          IF(XIN .AND. YIN .AND. ZIN) THEN
C
            IEOFL = IEOFF(IBL,IJKBLK)
            IPREF(1) = IXREF
            IPREF(2) = IYREF
            IPREF(3) = IZREF
            MULT = 1
C           Convert direction to step value
            IF(ID1L .EQ. 1) THEN
              ND(1) = 1
              LD(1) = IJKBLK(2,IBL) - IJKBLK(1,IBL) + 1
            ELSEIF(ID1L .EQ. 2) THEN
              ND(1) = IJKBLK(2,IBL) - IJKBLK(1,IBL) + 1  
              LD(1) = IJKBLK(4,IBL) - IJKBLK(3,IBL) + 1
            ELSE
              ND(1) = (IJKBLK(2,IBL) - IJKBLK(1,IBL) + 1 ) *
     +                (IJKBLK(4,IBL) - IJKBLK(3,IBL) + 1 )
              LD(1) = IJKBLK(6,IBL) - IJKBLK(5,IBL) + 1
            ENDIF
            IF(ID2L .EQ. 1) THEN
              ND(2) = 1
              LD(2) = IJKBLK(2,IBL) - IJKBLK(1,IBL) + 1
            ELSEIF(ID2L .EQ. 2) THEN
              ND(2) = IJKBLK(2,IBL) - IJKBLK(1,IBL) + 1  
              LD(2) = IJKBLK(4,IBL) - IJKBLK(3,IBL) + 1
            ELSE
              ND(2) = (IJKBLK(2,IBL) - IJKBLK(1,IBL) + 1 ) *
     +                (IJKBLK(4,IBL) - IJKBLK(3,IBL) + 1 )
              LD(2) = IJKBLK(6,IBL) - IJKBLK(5,IBL) + 1
            ENDIF
            IF(ID3L .EQ. 1) THEN
              ND(3) = 1
              LD(3) = IJKBLK(2,IBL) - IJKBLK(1,IBL) + 1
            ELSEIF(ID3L .EQ. 2) THEN
              ND(3) = IJKBLK(2,IBL) - IJKBLK(1,IBL) + 1  
              LD(3) = IJKBLK(4,IBL) - IJKBLK(3,IBL) + 1
            ELSE
              ND(3) = (IJKBLK(2,IBL) - IJKBLK(1,IBL) + 1 ) *
     +                (IJKBLK(4,IBL) - IJKBLK(3,IBL) + 1 )
              LD(3) = IJKBLK(6,IBL) - IJKBLK(5,IBL) + 1
            ENDIF
C           Many to one in first direction            
            IF(IF1L-IS1L .LT. IF1R-IS1R ) THEN
C             Find step size and number of steps to ref node            
              ND1ST = (IF1R - IS1R) / (IF1L - IS1L)
              IPD1 = (IPREF(ID1R) - IS1R) / ND1ST
              IPD1OF = IS1L + IPD1
c              IF(IPD1*ND1ST .NE. IS1R) THEN
              IF(IPD1*ND1ST .NE. (IPREF(ID1R) -IS1R)) THEN
                ND1 = 1
                MULT = MULT * 2
              ELSE
                ND1 = 2
              ENDIF
            ELSE
              ND1 = 2    
              IPD1OF = IS1L            
            ENDIF
C           Many to one in second direction            
            IF(IF2L-IS2L .LT. IF2R-IS2R ) THEN
C             Find step size and number of steps to ref node            
              ND2ST = (IF2R - IS2R) / (IF2L - IS2L)
              IPD2 = (IPREF(ID2R) - IS2R) / ND2ST
              IPD2OF = IS2L + IPD2
c              IF(IPD2*ND2ST .NE. IS2R) THEN
              IF(IPD2*ND2ST .NE. (IPREF(ID2R) -IS2R)) THEN
                ND2 = 1
                MULT = MULT * 2
              ELSE
                ND2 = 2
              ENDIF
            ELSE
              ND2 = 2    
              IPD2OF = IS2L            
            ENDIF
C           Many to one in third direction       
            IF(IF3L-IS3L .LT. IF3R-IS3R ) THEN
C             Find step size and number of steps to ref node            
              ND3ST = (IF3R - IS3R) / (IF3L - IS3L)
              IPD3 = (IPREF(ID3R) - IS3R) / ND3ST
              IPD3OF = IS3L + IPD3
c              IF(IPD3*ND3ST .NE. IS3R) THEN
              IF(IPD3*ND3ST .NE. (IPREF(ID3R) -IS3R)) THEN
                ND3 = 1
                MULT = MULT * 2
              ELSE
                ND3 = 2
              ENDIF
            ELSE
              ND3 = 2    
              IPD3OF = IS3L            
            ENDIF
C
C           Add the contribution from each node
            DO 30 ID1 = 1,ND1
              DO 20 ID2 = 1,ND2
                DO 10 ID3 = 1,ND3
                  ID1OFF = IPD1OF - ID1 
                  ID2OFF = IPD2OF - ID2 
                  ID3OFF = IPD3OF - ID3
                  IF(ID1OFF.GT.-1 .AND. ID2OFF.GT.-1 .AND. ID3OFF.GT.-1
     +               .AND. ID1OFF.LT.LD(1) .AND. ID2OFF.LT.LD(2) .AND.
     +               ID3OFF.LT.LD(3) ) THEN
C     +               .AND. ID1OFF.LT.ND(1) .AND. ID2OFF.LT.ND(2) .AND.
C     +               ID3OFF.LT.ND(3) ) THEN
                    IE = IEOFL+ID1OFF*ND(1)+ID2OFF*ND(2)+ID3OFF*ND(3)+1 
C                   Is this a valid cell                  
                    IF(IPRPS(IE) .LT. 100 .OR. 
     +               (IPRPS(IE).LT.198 .AND. NOBLK)) THEN
C                     Find the material number
                      CALL FNDMAT(IPRPS(IE),MAT,NUMMAT,NUM,TOTAL,IM)
C                     Add this value to the total                    
                      TOTAL(IM) = TOTAL(IM) + (MULT*PHI(IE))
                      NUM(IM)   = NUM(IM) + MULT
                    ENDIF
                  ENDIF
 10             CONTINUE    
 20           CONTINUE    
 30         CONTINUE 
          ENDIF
        ENDIF
 100  CONTINUE            
C                                               
      RETURN
      END  
      SUBROUTINE RPATNP(IONEP,IOPAT,NBMAX,NODMAX,NLMAX,NX,NY,NZ,IJKBLK,
     +                   NMAP,NBLOCK,NUMNOD,LINK,ILK,IERR) 
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Reads fill the NMAP arraythe PATGEO file
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
C     IONEP  - NEP file unit number
C     IOPAT  - PATGEO file unit number
C     NBMAX  - Maximum number of blocks
C     NODMAX - Maximum number of nodes
C     NLMAX  - Maximum number of links
C     NX     - Number of cells in I for whole model grid      	
C     NY     - Number of cells in J for whole model grid      
C     NZ     - Number of cells in K for whole model grid      
C
C  Output Parameters
C     IJKBLK - Block Limits
C     NMAP   - Nodal Mapping array
C     NBLOCK - Number of blocks found
C     NUMNOD - Number of nodes found
C     LINK   - Link definitions
C     ILK    - Number of links found
C     IERR   - Error flag
C
C***********************************************************************
       EXTERNAL ERRNP
C
       CHARACTER*80 LINE
       CHARACTER*8 SNUM
       CHARACTER*1 DIR1,DIR2,DIR3
       INTEGER IJKBLK(6,NBMAX),LINK(11,NLMAX),NMAP(NODMAX)
C
       IERR = 0
       ILK = 0
       IBMAX = 0
C
       IF(IOPAT  .EQ. 0) GOTO 200
 10    CONTINUE    
       READ(IOPAT,'(A80)',END=200,ERR=800)LINE       
 20    CONTINUE    
       IF(LINE(1:6) .EQ. ' * MBD') THEN
C        Block definition
         READ(LINE(7:14),'(I8)')IB  
         IF(IB .GT. IBMAX)THEN
           IBMAX = IB   
           IF(IBMAX .GT. NBMAX) THEN
             IERR = 8
             CALL ERRNP(IONEP,IERR,' ')
             GOTO 999
           ENDIF
         ENDIF        
         READ(IOPAT,'(16X,I6,I6,2X,I6,I6)',ERR=800)(IJKBLK(I,IB),I=3,6)
         READ(IOPAT,'(/,/,16X,I6,I6)',ERR=800)(IJKBLK(I,IB),I=1,2)
       ELSEIF(LINE(1:6) .EQ. ' * MBL') THEN
C        Link definition
         IN = 0
C        Find the decimal point
         DO 110 IC = 7,80
           IF(LINE(IC:IC) .EQ. '.' ) THEN
             GOTO 120
           ELSEIF(LINE(IC:IC) .EQ. '-') THEN
             ILK = ILK - 1
             GOTO 10  
           ELSEIF(LINE(IC:IC) .EQ. '0' .OR. LINE(IC:IC) .EQ. '1' .OR.
     +      LINE(IC:IC) .EQ. '2' .OR. LINE(IC:IC) .EQ. '3' .OR.
     +      LINE(IC:IC) .EQ. '4' .OR. LINE(IC:IC) .EQ. '5' .OR.
     +      LINE(IC:IC) .EQ. '6' .OR. LINE(IC:IC) .EQ. '7' .OR.
     +      LINE(IC:IC) .EQ. '8' .OR. LINE(IC:IC) .EQ. '9') THEN
             IN = IN + 1
             SNUM(IN:IN) = LINE(IC:IC)
           ELSE
             GOTO 10
           ENDIF           
 110     CONTINUE
 120     CONTINUE
         IF(IN .LT. 8) THEN
           DO 130 I = IN+1,8
             SNUM(I:I) = ' '
 130       CONTINUE
         ENDIF
         ILK = ILK + 1
         IF(ILK .GT. NLMAX) THEN
           IERR = 9
           CALL ERRNP(IONEP,IERR,' ')
           GOTO 999
         ENDIF
         READ(SNUM,'(I8)')LINK(1,ILK) 
         READ(LINE(IC+1:IC+8),'(I8)')LINK(2,ILK)
C        Find the block values
         READ(IOPAT,'(7X,A1,I6,2(1X,A1,I6,I6))',ERR=800)DIR1,LINK(4,ILK)
     +    ,DIR2,LINK(7,ILK),LINK(8,ILK),DIR3,LINK(10,ILK),LINK(11,ILK)  
C        
         LINK(5,ILK) = LINK(4,ILK)
         LINK(8,ILK) = LINK(8,ILK) + 1
         LINK(11,ILK) = LINK(11,ILK) + 1
         IF(DIR1 .EQ. 'X') THEN
           LINK(3,ILK) = 1
         ELSEIF(DIR1 .EQ. 'Y') THEN
           LINK(3,ILK) = 2
         ELSEIF(DIR1 .EQ. 'Z') THEN
           LINK(3,ILK) = 3
         ENDIF 
         IF(DIR2 .EQ. 'X') THEN
           LINK(6,ILK) = 1
         ELSEIF(DIR2 .EQ. 'Y') THEN
           LINK(6,ILK) = 2
         ELSEIF(DIR2 .EQ. 'Z') THEN
           LINK(6,ILK) = 3
         ENDIF 
         IF(DIR3 .EQ. 'X') THEN
           LINK(9,ILK) = 1
         ELSEIF(DIR3 .EQ. 'Y') THEN
           LINK(9,ILK) = 2
         ELSEIF(DIR3 .EQ. 'Z') THEN
           LINK(9,ILK) = 3
         ENDIF 
         READ(IOPAT,'(A80)',END=200,ERR=800)LINE
         IF(LINE(2:2) .EQ. '*') GOTO 20
         READ(IOPAT,'(/,16X,I6,I6)',ERR=800)(LINK(I,ILK),I=4,5)
         LINK(5,ILK) = LINK(5,ILK) + 1           
       ENDIF 
       GOTO 10
C
 200   CONTINUE
C
       NBLOCK = IBMAX
C       
C      No blocks found get information from PHIDA
       IF(IBMAX .EQ. 0) THEN
         IJKBLK(1,1) = 1
         IJKBLK(2,1) = NX
         IJKBLK(3,1) = 1
         IJKBLK(4,1) = NY
         IJKBLK(5,1) = 1
         IJKBLK(6,1) = NZ
         NBLOCK = 1
       ENDIF   
C              
C      Fill the  NMAP array
C
       IN = 0
       DO 300 IB = 1,NBLOCK
         DO 290 IZ = IJKBLK(5,IB),IJKBLK(6,IB)+1
           DO 280 IY = IJKBLK(3,IB),IJKBLK(4,IB)+1
             DO 270 IX = IJKBLK(1,IB),IJKBLK(2,IB)+1
               IN = IN + 1
               IF(IN .GT. NODMAX) THEN
                 IERR = 10
                 CALL ERRNP(IONEP,IERR,' ')
                 GOTO 999
               ENDIF
               NMAP(IN) = IN
 270         CONTINUE
 280       CONTINUE
 290     CONTINUE
 300   CONTINUE    
       NUMNOD = IN
C
C      Modify the NMAP array to allow for the links
       IF (NBLOCK .GT. 1)CALL LINKNP(LINK,ILK,IJKBLK,NMAP)
C
       GOTO 999
 800   CONTINUE
       IERR=7
       CALL ERRNP(IONEP,IERR,' ')              
 999   CONTINUE
C         
       RETURN
       END  
      SUBROUTINE FNDVNP(IP,NAME,NUMRES,CCC,CRT,LOCU,LOCV,LOCW)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Finds the velocity values that should be used.
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
      LOGICAL CRT,CCC
      CHARACTER*4 UC(2),VC(2),WC(2),U(2),V(2),W(2),
     +            UCRT(2),VCRT(2),WCRT(2),NAME(50)
      DATA UC,VC,WC/'UC1 ','UC2 ','VC1 ','VC2 ','WC1 ','WC2 '/
      DATA U,V,W/'U1  ','U2  ','V1  ','V2  ','W1  ','W2  '/
      DATA UCRT,VCRT,WCRT/'UCRT','UCT2','VCRT','VCT2','WCRT','WCT2'/
C
      CCC = .FALSE.    
      CRT = .FALSE.    
C 
      DO 10 IR = 1,NUMRES
C       Set cell centre velocity flag      
        IF(NAME(IR) .EQ. UC(IP) .OR. NAME(IR) .EQ. VC(IP) .OR. 
     +     NAME(IR) .EQ. WC(IP)) CCC = .TRUE.
        IF(NAME(IR) .EQ. UCRT(IP) .OR. NAME(IR) .EQ. VCRT(IP) .OR. 
     +     NAME(IR) .EQ. WCRT(IP)) CRT = .TRUE.
 10   CONTINUE    
C       Look for velocities 
      LOCU = 0
      LOCV = 0
      LOCW = 0
C      
      DO 100 IR = 1,NUMRES
        IF(NAME(IR).EQ.U(IP) .AND. LOCU.EQ.0 .AND. .NOT.CCC) LOCU = IR
        IF(NAME(IR).EQ.V(IP) .AND. LOCV.EQ.0 .AND. .NOT.CCC) LOCV = IR
        IF(NAME(IR).EQ.W(IP) .AND. LOCW.EQ.0 .AND. .NOT.CCC) LOCW = IR
        IF(NAME(IR).EQ.UCRT(IP)) LOCU = IR
        IF(NAME(IR).EQ.VCRT(IP)) LOCV = IR
        IF(NAME(IR).EQ.WCRT(IP)) LOCW = IR
        IF(NAME(IR).EQ.UC(IP) .AND. LOCU.EQ.0) LOCU = IR
        IF(NAME(IR).EQ.VC(IP) .AND. LOCV.EQ.0) LOCV = IR
        IF(NAME(IR).EQ.WC(IP) .AND. LOCW.EQ.0) LOCW = IR
 100  CONTINUE    
C
      RETURN
      END
      SUBROUTINE PRPSNP(IONEP,IOPHDA,LENREC,LOG,NAME,IJKBLK,
     +                  NBLOCK,NX,NY,NZ,PBIG,NUMRES,IPRPS,PHI,IERR)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine:  Reads material information from the PHI file
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
C      IONEP  - NEP file unit number
C      IOPHDA - PHIDA file unit number
C      LENREC - PHIDA file record length
C      NUMELE - Number of elements
C      LOG    - Flags for results in file
C      NAME   - Names of results stored
C      IJKBLK - Block limits
C      NBLOCK - Number of blocks
C      NX     - Number of cells in I
C      NY     - Number of cells in J
C      NZ     - Number of cells in K
C      NUMRES - Number of results found
C
C  Output Parameters
C      IPRPS  - PRPS number array
C      PHI    - PRPS real values
C      IERR   - Error flag
C
C***********************************************************************
      EXTERNAL RDVLNP
C
      LOGICAL LOG(50)
      CHARACTER*4 NAME(50)
      INTEGER IPRPS(NX*NY*NZ),IJKBLK(6,NBLOCK)
      REAL PHI(2*NX*NY*NZ),PBIG(NX*NY*NZ)
C
      NUMELE =NX*NY*NZ
      LPRPS = 0
      LVPOR = 0
C     Compress LOG and NAME
      NUMRES = 0
      DO 5 I = 1,50
        IF(LOG(I)) THEN
          NUMRES = NUMRES + 1
          LOG(NUMRES) = LOG(I)
          NAME(NUMRES) = NAME(I)
        ENDIF
 5    CONTINUE     
C
C     Find the PRPS and VPOR values if stored
      DO 10 I = 1,NUMRES
        IF(LOG(I) .AND. NAME(I).EQ.'PRPS') THEN
          LPRPS = I
        ELSEIF(LOG(I) .AND. NAME(I).EQ.'VPOR') THEN
          LVPOR = I
        ENDIF  
 10   CONTINUE
C
C     Neither VPOR or PRPS found so set all values = 0
C
      IF(LVPOR .EQ. 0 .AND. LPRPS .EQ. 0) THEN
        DO 30 I = 1,NUMELE
          IPRPS(I) = 0
          PHI(I) = 0.0
 30     CONTINUE
      ELSE
C       Read the PRPS values        
        IF(LPRPS .GT. 0) THEN
          CALL RDVLNP(IONEP,IOPHDA,LPRPS,NUMRES,LENREC,IJKBLK,NBLOCK,
     +                NX,NY,NZ,PBIG,PHI,IERR)
          IF(IERR .GT. 0) GOTO 900
C              
          DO 40 I = 1,NUMELE
            IPRPS(I) = NINT(PHI(I))
 40       CONTINUE
        ENDIF
C       Read the VPOR values        
        IF(LVPOR .GT. 0) THEN
          IN = NUMELE + 1
          CALL RDVLNP(IONEP,IOPHDA,LVPOR,NUMRES,LENREC,IJKBLK,NBLOCK,
     +                NX,NY,NZ,PBIG,PHI(IN),IERR)
          IF(IERR .GT. 0) GOTO 900
C                              
          DO 90 I = 1,NUMELE
            IF(PHI(NUMELE+I) .LT. 1.0E-10) THEN
              IPRPS(I) = 198 
              PHI(I) = 198.0
            ELSEIF(LPRPS .EQ. 0)THEN
              IPRPS(I) = 0
              PHI(I) = 0.0
            ENDIF
 90       CONTINUE
        ENDIF
      ENDIF  
C
 900  CONTINUE
      RETURN
      END     
      SUBROUTINE RVELNP(IONEP,IOPHDA,LENREC,IJKBLK,NBLOCK,NX,NY,NZ,
     + NUMRES,LOCU,LOCV,LOCW,XFRAC,CCC,CRT,PBIG,UVEL,VVEL,WVEL,IERR)
C***********************************************************************
C  Author: CYC
C  Creation Date: 18th July 1994
C  Amendment History
C     Nature      Date      Author
C
C  Function of Subroutine: 
C     Read the velocity values
C     If UC/VC/WC values are present set CCC flag and average as scalar
C     If UCRT/VCRT/WCRT values are present use these not U1/V1/W1
C     Set missing values to 0
C
C
C  Examples of use
C
C  Input Parameters 
C
C  Output Parameters
C
C***********************************************************************
C
      INTEGER IJKBLK(6,NBLOCK)
      REAL XFRAC(NX),UVEL(NX*NY*NZ),VVEL(NX*NY*NZ)
      REAL WVEL(NX*NY*NZ),PBIG(NX*NY*NZ)
      LOGICAL CCC,CRT,CARTES,LOG1,BFC,LOG2
C
      IERR = 0
      NUMELE = NX*NY*NZ
C     Read the velocity values
      IF(LOCU .GT. 0) THEN
        CALL RDVLNP(IONEP,IOPHDA,LOCU,NUMRES,LENREC,IJKBLK,NBLOCK,
     +            NX,NY,NZ,PBIG,UVEL,IERR) 
      ELSE
C       Zero the values
        DO 110 IE = 1,NUMELE
          UVEL(IE) = 0.0
 110    CONTINUE
      ENDIF                
      IF(LOCV .GT. 0) THEN
        CALL RDVLNP(IONEP,IOPHDA,LOCV,NUMRES,LENREC,IJKBLK,NBLOCK,
     +            NX,NY,NZ,PBIG,VVEL,IERR) 
      ELSE
C       Zero the values
        DO 120 IE = 1,NUMELE
          VVEL(IE) = 0.0
 120    CONTINUE
      ENDIF                
      IF(LOCW .GT. 0) THEN
        CALL RDVLNP(IONEP,IOPHDA,LOCW,NUMRES,LENREC,IJKBLK,NBLOCK,
     +            NX,NY,NZ,PBIG,WVEL,IERR) 
      ELSE
C       Zero the values
        DO 130 IE = 1,NUMELE
          WVEL(IE) = 0.0
 130    CONTINUE
      ENDIF                
C      
C     If no cartesian components on a polar grid correct
C     the U1 and V1 values based on angle
      ie = 0
C
      IF(.NOT. CRT) THEN
C       Read the logicals      
        READ(IOPHDA,REC=2)CARTES,LOG1,BFC,LOG2
        IF(.NOT.CARTES .AND. .NOT.BFC) THEN
          READ(IOPHDA,REC=6,ERR=750)XFRAC
          IF(CCC) THEN
C           Model must be a single block
            DO 160 K = NZ,1,-1
              DO 150 J = NY,1,-1
                DO 140 I = NX,1,-1
                  IE = I + (J-1)*NX + (K-1)*NX*NY
                  U1 = UVEL(IE)
                  V1 = VVEL(IE)
                  IF(I.EQ.1) THEN
                    ALAST = 0.0
                  ELSE
                    ALAST = XFRAC(I-1)
                  ENDIF    
                  ANG = ALAST + (XFRAC(I)-ALAST)/2
                  UVEL(IE) = U1*COS(ANG) + V1*SIN(ANG)
                  VVEL(IE) = V1*COS(ANG) - U1*SIN(ANG)
 140            CONTINUE
 150          CONTINUE
 160        CONTINUE
          ELSE
C           Model must be a single block
            DO 200 K = NZ,1,-1
              DO 190 J = NY,1,-1
                DO 180 I = NX,1,-1
                  IE = I + (J-1)*NX + (K-1)*NX*NY
                  U1 = UVEL(IE)
                  ANGY = XFRAC(I)
                  IF(I .EQ. NX) THEN
                    U1 = UVEL(IE-1)
                    ANGY = XFRAC(I-1)
                  ENDIF  
                  IF(I.EQ.1) THEN
                    ALAST = 0.0
                  ELSE
                    ALAST = XFRAC(I-1)
                  ENDIF    
                  V1 = VVEL(IE)
                  IF(J .EQ. NY) V1 = VVEL(IE-NX)
                  ANGX = ALAST + (XFRAC(I)-ALAST)/2
                  UVEL(IE) = U1*COS(ANGX) + V1*SIN(ANGY)
                  VVEL(IE) = V1*COS(ANGY) - U1*SIN(ANGX)
 180            CONTINUE
 190          CONTINUE
 200        CONTINUE
          ENDIF
        ENDIF
      ENDIF   
C
      GOTO 900
 750  CONTINUE
      IERR = 6
      CALL ERRNP(IONEP,IERR,' ')
 900  CONTINUE
      RETURN
      END                  
      
