      BLOCK DATA PRBLCK
C
C BLOCK DATA SUBROUTINE FOR POSTCONV
C
      COMMON/SAMMF/NSDEF(0:12,255),IF255(6,8),IFADJ7(255)
      COMMON/PSTLAB/SCALAB(30),VECLAB(3),SRFLAB(25),FORLAB(6)
      CHARACTER*4 SCALAB,VECLAB,SRFLAB,FORLAB
      DATA SCALAB/'F1  ','F2  ','F3  ',
     1            'F4  ','F5  ','F6  ',
     2            'SU  ','SV  ','SW  ',
     3            'P   ','TE  ','ED  ',
     4            'VIS ','T   ','DENS',
     5            'LAMV','CP  ','COND',
     6            'STRE','MACH','CONC',
     7            'VMAG','PSTA','PTOT',
     8            'TTOT','TWOL','FMU ',
     9            'ENTH','VOID','USER'/
      DATA VECLAB/'U   ','V   ','W   '/
      DATA SRFLAB/'SFXY','SFX ','SFY ','SFZ ',
     1            'SFTO','SFTX','SFTY','SFTZ',
     2            'YPLU','DIST','TEMP','HTRA','MTRA','HFLU','MFLU',
     3            'HCOE','MCOE','TIRA','TRAD','SIRA','SRAD','    ',
     4            '    ','    ','    '/
      DATA FORLAB/'FX  ','FY  ','FZ  ','FTX ','FTY ','FTZ '/
C SAMM TYPE DEFINITIONS
C    NSDEF(0) = NUMBER OF EXTRA NODES ON EDGES
C    NSDEF(1)-NSDEF(NSDEF(0)) - EDGE NUMBERS OF THE EXTRA NODES
C    NSDEF(NSDEF(0)+1) ONWARD  - FACE NUMBERS OF EXTRA NODES
C    I=TYPE 1,255 = EXTRA NODES ON EDGES
C    NSDEF(1-12)=EDGE/FACE THE NODE LIES ON
      DATA (NSDEF(J,  1),J=0,12)/ 3, 1, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,  2),J=0,12)/ 3, 2, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,  3),J=0,12)/ 4, 2, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,  4),J=0,12)/ 3, 3, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,  5),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,  6),J=0,12)/ 4, 3, 1, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,  7),J=0,12)/ 5, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,  8),J=0,12)/ 3, 4, 3, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,  9),J=0,12)/ 4, 8, 5, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 10),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 11),J=0,12)/ 5, 2, 3, 8, 5, 6, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 12),J=0,12)/ 4, 4, 2, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 13),J=0,12)/ 5, 1, 2, 7, 8, 5, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 14),J=0,12)/ 5, 4, 1, 6, 7, 8, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 15),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 16),J=0,12)/ 3,12, 9, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 17),J=0,12)/ 4, 9, 1, 4,12, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 18),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 19),J=0,12)/ 5, 9, 6, 2, 4,12, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 20),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 21),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 22),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 23),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 24),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 25),J=0,12)/ 5, 8,12, 9, 1, 3, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 26),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 27),J=0,12)/ 6, 9, 6, 2, 3, 8,12, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 28),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 29),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 30),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 31),J=0,12)/ 5, 9, 6, 7, 8,12, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 32),J=0,12)/ 3, 9,10, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 33),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 34),J=0,12)/ 4,10, 2, 1, 9, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 35),J=0,12)/ 5, 5, 9,10, 2, 4, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 36),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 37),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 38),J=0,12)/ 5,10, 7, 3, 1, 9, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 39),J=0,12)/ 6,10, 7, 3, 4, 5, 9, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 40),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 41),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 42),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 43),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 44),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 45),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 46),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 47),J=0,12)/ 5,10, 7, 8, 5, 9, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 48),J=0,12)/ 4, 6, 5,12,10, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 49),J=0,12)/ 5, 6, 1, 4,12,10, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 50),J=0,12)/ 5, 1, 5,12,10, 2, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 51),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 52),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 53),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 54),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 55),J=0,12)/ 5, 3, 4,12,10, 7, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 56),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 57),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 58),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 59),J=0,12)/ 5, 8,12,10, 2, 3, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 60),J=0,12)/ 8, 4, 8,12, 5, 2, 7,10, 6, 0, 0, 0, 0/
      DATA (NSDEF(J, 61),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 62),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 63),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 64),J=0,12)/ 3,10,11, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 65),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 66),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 67),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 68),J=0,12)/ 4,11, 3, 2,10, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 69),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 70),J=0,12)/ 5, 6,10,11, 3, 1, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 71),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 72),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 73),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 74),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 75),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 76),J=0,12)/ 5,11, 8, 4, 2,10, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 77),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 78),J=0,12)/ 6,11, 8, 4, 1, 6,10, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 79),J=0,12)/ 5,11, 8, 5, 6,10, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 80),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 81),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 82),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 83),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 84),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 85),J=0,12)/ 8, 2, 3, 4, 1,11,12,13,10, 0, 0, 0, 0/
      DATA (NSDEF(J, 86),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 87),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 88),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 89),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 90),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 91),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 92),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 93),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 94),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 95),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 96),J=0,12)/ 4, 7, 6, 9,11, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 97),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 98),J=0,12)/ 5, 7, 2, 1, 9,11, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J, 99),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,100),J=0,12)/ 5, 2, 6, 9,11, 3, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,101),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,102),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,103),J=0,12)/ 5, 5, 9,11, 3, 4, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,104),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,105),J=0,12)/ 8, 1, 6, 9, 5, 3, 7,11, 8, 0, 0, 0, 0/
      DATA (NSDEF(J,106),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,107),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,108),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,109),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,110),J=0,12)/ 5, 4, 1, 9,11, 8, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,111),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,112),J=0,12)/ 5,12,11, 7, 6, 5, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,113),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,114),J=0,12)/ 6, 1, 5,12,11, 7, 2, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,115),J=0,12)/ 5, 7, 2, 4,12,11, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,116),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,117),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,118),J=0,12)/ 5,12,11, 3, 1, 5, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,119),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,120),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,121),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,122),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,123),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,124),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,125),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,126),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,127),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,128),J=0,12)/ 3,11,12, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,129),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,130),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,131),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,132),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,133),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,134),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,135),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,136),J=0,12)/ 4,12, 4, 3,11, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,137),J=0,12)/ 5,12, 5, 1, 3,11, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,138),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,139),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,140),J=0,12)/ 5, 7,11,12, 4, 2, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,141),J=0,12)/ 6,12, 5, 1, 2, 7,11, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,142),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,143),J=0,12)/ 5,12, 5, 6, 7,11, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,144),J=0,12)/ 4,11, 9, 5, 8, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,145),J=0,12)/ 5, 4, 8,11, 9, 1, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,146),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,147),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,148),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,149),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,150),J=0,12)/ 8, 6, 9, 5, 1, 7,11, 8, 3, 0, 0, 0, 0/
      DATA (NSDEF(J,151),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,152),J=0,12)/ 5, 5, 4, 3,11, 9, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,153),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,154),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,155),J=0,12)/ 5, 2, 3,11, 9, 6, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,156),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,157),J=0,12)/ 5, 7,11, 9, 1, 2, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,158),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,159),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,160),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,161),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,162),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,163),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,164),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,165),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,166),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,167),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,168),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,169),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,170),J=0,12)/ 8, 1, 2, 3, 4,10,11,12,13, 0, 0, 0, 0/
      DATA (NSDEF(J,171),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,172),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,173),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,174),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,175),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,176),J=0,12)/ 5,11,10, 6, 5, 8, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,177),J=0,12)/ 6, 4, 8,11,10, 6, 1, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,178),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,179),J=0,12)/ 5,11,10, 2, 4, 8, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,180),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,181),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,182),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,183),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,184),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,185),J=0,12)/ 5, 6, 1, 3,11,10, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,186),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,187),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,188),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,189),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,190),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,191),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,192),J=0,12)/ 4, 8, 7,10,12, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,193),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,194),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,195),J=0,12)/ 8, 8,12, 5, 4, 7,10, 6, 2, 0, 0, 0, 0/
      DATA (NSDEF(J,196),J=0,12)/ 5, 8, 3, 2,10,12, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,197),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,198),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,199),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,200),J=0,12)/ 5, 3, 7,10,12, 4, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,201),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,202),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,203),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,204),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,205),J=0,12)/ 5, 1, 2,10,12, 5, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,206),J=0,12)/ 5, 6,10,12, 4, 1, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,207),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,208),J=0,12)/ 5,10, 9, 5, 8, 7, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,209),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,210),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,211),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,212),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,213),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,214),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,215),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,216),J=0,12)/ 6, 3, 7,10, 9, 5, 4, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,217),J=0,12)/ 5,10, 9, 1, 3, 7, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,218),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,219),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,220),J=0,12)/ 5, 5, 4, 2,10, 9, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,221),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,222),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,223),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,224),J=0,12)/ 5, 9,12, 8, 7, 6, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,225),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,226),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,227),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,228),J=0,12)/ 6, 2, 6, 9,12, 8, 3, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,229),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,230),J=0,12)/ 5, 8, 3, 1, 9,12, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,231),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,232),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,233),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,234),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,235),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,236),J=0,12)/ 5, 9,12, 4, 2, 6, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,237),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,238),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,239),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,240),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,241),J=0,12)/ 5, 4, 8, 7, 6, 1, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,242),J=0,12)/ 5, 1, 5, 8, 7, 2, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,243),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,244),J=0,12)/ 5, 2, 6, 5, 8, 3, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,245),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,246),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,247),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,248),J=0,12)/ 5, 3, 7, 6, 5, 4, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,249),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,250),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,251),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,252),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,253),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,254),J=0,12)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (NSDEF(J,255),J=0,12)/12, 1, 1, 2, 3, 3, 4, 9, 9,10,11,11,12/
      DATA IF255/1,2,3,4,5,6,   7,8,9,10,11,12,  2,1,7,8,0,0,
     1           4,5,11,10,0,0, 3,2,8,9,0,0,     5,6,12,11,0,0,
     2           4,3,9,10,0,0,  6,1,7,12,0,0/ 
C MODIFIED JUNE 8 1995 TO HAVE MISSING FACE FOR SAMM 5 ELEMENTS
C SAMM 5 ELEMENTS ARE CELLS WITH 5 MISSING VERTICES IN THE MAIN DEFINITION
C NU CHANGES 2310
      DATA (IFADJ7(J),J=  1, 10)/ 1, 1, 1, 1, 0, 1, 1, 1, 1, 0/
      DATA (IFADJ7(J),J= 11, 20)/ 1, 1, 1, 1, 0, 2, 3, 0, 3, 0/
      DATA (IFADJ7(J),J= 21, 30)/ 0, 0, 0, 0, 5, 0, 1, 0, 0, 0/
      DATA (IFADJ7(J),J= 31, 40)/ 1, 2, 0, 3, 3, 0, 0, 6, 1, 0/
      DATA (IFADJ7(J),J= 41, 50)/ 0, 0, 0, 0, 0, 0, 1, 2, 3, 3/
      DATA (IFADJ7(J),J= 51, 60)/ 0, 0, 0, 0, 3, 0, 0, 0, 3, 0/
      DATA (IFADJ7(J),J= 61, 70)/ 0, 0, 0, 2, 0, 0, 0, 4, 0, 6/
      DATA (IFADJ7(J),J= 71, 80)/ 0, 0, 0, 0, 0, 4, 0, 1, 1, 0/
      DATA (IFADJ7(J),J= 81, 90)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (IFADJ7(J),J= 91,100)/ 0, 0, 0, 0, 0, 2, 0, 6, 0, 6/
      DATA (IFADJ7(J),J=101,110)/ 0, 0, 6, 0, 0, 0, 0, 0, 0, 6/
      DATA (IFADJ7(J),J=111,120)/ 0, 2, 0, 2, 3, 0, 0, 6, 0, 0/
      DATA (IFADJ7(J),J=121,130)/ 0, 0, 0, 0, 0, 0, 0, 2, 0, 0/
      DATA (IFADJ7(J),J=131,140)/ 0, 0, 0, 0, 0, 4, 5, 0, 0, 4/
      DATA (IFADJ7(J),J=141,150)/ 1, 0, 1, 2, 5, 0, 0, 0, 0, 0/
      DATA (IFADJ7(J),J=151,160)/ 0, 5, 0, 0, 5, 0, 5, 0, 0, 0/
      DATA (IFADJ7(J),J=161,170)/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0/
      DATA (IFADJ7(J),J=171,180)/ 0, 0, 0, 0, 0, 2, 2, 0, 3, 0/
      DATA (IFADJ7(J),J=181,190)/ 0, 0, 0, 0, 5, 0, 0, 0, 0, 0/
      DATA (IFADJ7(J),J=191,200)/ 0, 2, 0, 0, 0, 4, 0, 0, 0, 4/
      DATA (IFADJ7(J),J=201,210)/ 0, 0, 0, 0, 4, 4, 0, 2, 0, 0/
      DATA (IFADJ7(J),J=211,220)/ 0, 0, 0, 0, 0, 2, 5, 0, 0, 4/
      DATA (IFADJ7(J),J=221,230)/ 0, 0, 0, 2, 0, 0, 0, 2, 0, 6/
      DATA (IFADJ7(J),J=231,240)/ 0, 0, 0, 0, 0, 4, 0, 0, 0, 0/
      DATA (IFADJ7(J),J=241,250)/ 2, 2, 0, 2, 0, 0, 0, 2, 0, 0/
      DATA (IFADJ7(J),J=251,255)/ 0, 0, 0, 0, 5/
      DATA IF255/1,2,3,4,5,6,   7,8,9,10,11,12,  2,1,7,8,0,0,
     1           4,5,11,10,0,0, 3,2,8,9,0,0,     5,6,12,11,0,0,
     2           4,3,9,10,0,0,  6,1,7,12,0,0/ 
      END
C
C SUBROUTINES ARE:
C   STRTRM, UPPER, PATELM, PATWAL, PATBND, BINRD, BINRDP,
C   LDDATA, LDPDAT, VECCLR, RESUME, INIT1, LOAD9, TRLOAD,
C   CELGET, SRFGET, STRMAP, CELOUT
C
C FUNCTIONS ARE:
C   NUMREC, NUMRDP
C
      SUBROUTINE STRTRM(INPUT,OUTPUT,LOUT)
C
C TRIM STRING OF EXCESS BLANKS
C
      CHARACTER*(*)INPUT,OUTPUT
      LIN=LEN(INPUT)
      DO 10 I=LIN,1,-1
        LOUT=I
        IF (INPUT(LOUT:LOUT).NE.' ')GOTO 11
10    CONTINUE
C NO NONBLANK CHARACTERS FOUND
      LOUT=0
      OUTPUT=' '
      RETURN
C
11    OUTPUT(1:LOUT)=INPUT(1:LOUT)
      RETURN
      END
C
      SUBROUTINE UPPER(FROM)
C
C TRANSLATE LOWERCASE TO UPPERCASE
C
      CHARACTER*(*) FROM
      DO 100 I=1,LEN(FROM)
        IF(ICHAR(FROM(I:I)).GT.96) THEN
          FROM(I:I)=CHAR(ICHAR(FROM(I:I))-32)
        ENDIF
100   CONTINUE
      RETURN
      END
C
      SUBROUTINE PATELM(FMTOUT)
C
C CONVERT TO ELEMENT DATA
C
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
      LOGICAL FMTOUT
C
      CALL STRTRM(CASE,CASE,LCASE)
      FNAME=CASE(1:LCASE)//'.ele'
      IF (FMTOUT) THEN
        OPEN(14,FILE=FNAME,STATUS='UNKNOWN',FORM='FORMATTED')
        WRITE(14,1000)TITLE
1000    FORMAT(A)
        WRITE(14,1001)13
1001    FORMAT(I5)
        WRITE(CITER,1002)ITERS
1002    FORMAT(I6)
        SUB1='DATA FROM ITERATION NO. '//CITER
        SUB2=' '
        WRITE(14,1000)SUB1
        WRITE(14,1000)SUB2
        DO 10 I=1,MAXE
          WRITE(14,1004)I,8,(VECTOR(J,I),J=1,13)
1004      FORMAT(2I8,3(/,5E13.7))
10      CONTINUE
      ELSE
        OPEN(14,FILE=FNAME,STATUS='UNKNOWN',FORM='UNFORMATTED')
        READ(TITLE,1005)ISUB
1005    FORMAT(80A1)
        WRITE(14)ISUB,13
        WRITE(CITER,1002)ITERS
        SUB1='DATA FROM ITERATION NO. '//CITER
        SUB2=' '
        READ(SUB1,1005)ISUB
        WRITE(14)ISUB
        READ(SUB2,1005)ISUB
        WRITE(14)ISUB
        DO 20 I=1,MAXE
          WRITE(14)I,8,(VECTOR(J,I),J=1,13)
20      CONTINUE
        WRITE(14)0,8,(0.,J=1,13)
      ENDIF
      CLOSE(14)
      WRITE(6,1006)FNAME(1:LCASE+4)
1006  FORMAT(' ELEMENT RESULTS FILE CALLED ',A,' WRITTEN',/,
     1       ' DATA(PER COLUMN) IS:',/,
     2       ' U, V, W, PRESSURE, TURB ENERGY',/,
     3       ' DISSIP, VISCOSITY, TEMP, DENSITY, LAM VISCOSITY',/,
     4       ' SPEC HEAT, CONDUCTIVITY, CONCENTRATION')
      RETURN
      END
C
      SUBROUTINE PATWAL(FMTOUT)
C
C CONVERT WALL DATA
C
      COMMON/PDATA/VECTOR(14,1)
      COMMON/CASEN/CASE,TITLE
      COMMON/KEY/MAXN,MAXE,ICSYS,MAXSYS,ITYPE,PFIRST,IVERS,NUMW,
     1           NI,NJ,NK,NF,MAXB,IBFILL,NOVICE,KEYS(10),LW,LR,
     2           LECHO,LNUT,MXTB,KEYS2(6),MAXCP,KEYS3(17)
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
      LOGICAL FMTOUT
C
      CALL STRTRM(CASE,CASE,LCASE)
      FNAME=CASE(1:LCASE)//'.wal'
      IF (FMTOUT) THEN
        OPEN(14,FILE=FNAME,STATUS='UNKNOWN',FORM='FORMATTED')
        WRITE(14,1000)TITLE
1000    FORMAT(A)
        WRITE(14,1001)14
1001    FORMAT(I5)
        WRITE(CITER,1002)ITERS
1002    FORMAT(I6)
        SUB1='DATA FROM ITERATION NO. '//CITER
        SUB2=' '
        WRITE(14,1000)SUB1
        WRITE(14,1000)SUB2
        DO 10 I=ICSH1,MAXE
          WRITE(14,1004)I,8,(VECTOR(J,I),J=1,14)
1004      FORMAT(2I8,3(/,5E13.7))
10      CONTINUE
      ELSE
        OPEN(14,FILE=FNAME,STATUS='UNKNOWN',FORM='UNFORMATTED')
        READ(TITLE,1005)ISUB
1005    FORMAT(80A1)
        WRITE(14)ISUB,14
        WRITE(CITER,1002)ITERS
        SUB1='DATA FROM ITERATION NO. '//CITER
        SUB2=' '
        READ(SUB1,1005)ISUB
        WRITE(14)ISUB
        READ(SUB2,1005)ISUB
        WRITE(14)ISUB
        DO 20 I=ICSH1,MAXE
          WRITE(14)I,8,(VECTOR(J,I),J=1,14)
20      CONTINUE
        WRITE(14)0,8,(0.,J=1,14)
      ENDIF
      CLOSE(14)
      WRITE(6,1006)FNAME(1:LCASE+4)
1006  FORMAT(' WALL DATA RESULTS FILE CALLED ',A,' WRITTEN',/,
     1       ' DATA(PER COLUMN) IS:',/,
     2       ' SFX, SFY, SFZ, YPLUS, DIST,',/,
     3       ' TEMP, HTRAN, MTRAN, HFLUX, MFLUX',/,
     4       ' TRAD, TIRA, SRAD, SIRA')
      RETURN
      END
C
c*jns******************************************************

      SUBROUTINE PATBND(FMTOUT)
C
C CONVERT TO BOUNDARY DATA
C
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
      LOGICAL FMTOUT
C
      CALL STRTRM(CASE,CASE,LCASE)
      FNAME=CASE(1:LCASE)//'.bnd'
      IF (FMTOUT) THEN
        OPEN(14,FILE=FNAME,STATUS='UNKNOWN',FORM='FORMATTED')
        WRITE(14,1000)TITLE
1000    FORMAT(A)
        WRITE(14,1001)13
1001    FORMAT(I5)
        WRITE(CITER,1002)ITERS
1002    FORMAT(I6)
        SUB1='DATA FROM ITERATION NO. '//CITER
        SUB2=' '
        WRITE(14,1000)SUB1
        WRITE(14,1000)SUB2
        DO 10 I=ICSH1,MAXE
          WRITE(14,1004)I,8,(VECTOR(J,I),J=1,13)
1004      FORMAT(2I8,3(/,5E13.7))
10      CONTINUE
      ELSE
        OPEN(14,FILE=FNAME,STATUS='UNKNOWN',FORM='UNFORMATTED')
        READ(TITLE,1005)ISUB
1005    FORMAT(80A1)
        WRITE(14)ISUB,13
        WRITE(CITER,1002)ITERS
        SUB1='DATA FROM ITERATION NO. '//CITER
        SUB2=' '
        READ(SUB1,1005)ISUB
        WRITE(14)ISUB
        READ(SUB2,1005)ISUB
        WRITE(14)ISUB
        DO 20 I=ICSH1,MAXE
          WRITE(14)I,8,(VECTOR(J,I),J=1,13)
20      CONTINUE
        WRITE(14)0,8,(0.,J=1,13)
      ENDIF
      CLOSE(14)
      WRITE(6,1006)FNAME(1:LCASE+4)
1006  FORMAT(' BOUNDARY RESULTS FILE CALLED ',A,' WRITTEN',/,
     1       ' DATA(PER COLUMN) IS:',/,
     2       ' U, V, W, PRESSURE, TURB ENERGY',/,
     3       ' DISSIP, VISCOSITY, TEMP, DENSITY, LAM VISCOSITY',/,
     4       ' SPEC HEAT, CONDUCTIVITY, CONCENTRATION')
      RETURN
      END
c*jns*******************************
C
      SUBROUTINE BINRD(IREC,V,NV1,NV2)
C
C READ DATA FROM BLOCKED BINARY DIRECT ACCESS FILE (9 OR 29)
C
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
      COMMON/SCRT2/ARR(2048)
      LOGICAL LRSKIP
      DIMENSION V(NV2)
C
C BEGIN NEW ARRAY
C
      IREC=IREC+1
      NWORD=NV2-NV1+1
      K1=NV1
      NUMR=(NWORD-1)/2048+1
      IFIN=IREC+NUMR-1
      DO 100 J=IREC,IFIN
        READ(LPOST,REC=J)ARR
        K2=K1+2048-1
        K2=MIN0(K2,NV2)
        IF (LRSKIP) THEN
          IF (K2.LE.0) GO TO 60
          DO 50 K=K1,K2
            K3=K-K1+1
            IF (K.GT.0) V(K)=ARR(K3)
50        CONTINUE
60        K1=K2+1
        ELSE
          DO 70 K=K1,K2
            K3=K-K1+1
            V(K)=ARR(K3)
70        CONTINUE
          K1=K2+1
        ENDIF
100   CONTINUE
      IREC=IFIN
      RETURN
      END
C
      SUBROUTINE BINRDP(IREC,V,NV1,NV2)
C
C READS DOUBLE PRECISION DATA FROM BLOCKED BINARY DIRECT ACCESS FILE (9 OR 29)
C
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
      COMMON/SCRT2/ARR(1024)
      LOGICAL LRSKIP
      DOUBLE PRECISION ARR
      DIMENSION V(NV2)
C
C BEGIN NEW ARRAY
C
      IREC=IREC+1
      NWORD=NV2-NV1+1
      K1=NV1
      NUMR=(NWORD-1)/1024+1
      IFIN=IREC+NUMR-1
      DO 100 J=IREC,IFIN
        READ(LPOST,REC=J)ARR
        K2=K1+1024-1
        K2=MIN0(K2,NV2)
        IF (LRSKIP) THEN
          IF (K2.LE.0) GO TO 60
          DO 50 K=K1,K2
            K3=K-K1+1
            IF (K.GT.0) V(K)=ARR(K3)
50        CONTINUE
60        K1=K2+1
        ELSE
          DO 70 K=K1,K2
            K3=K-K1+1
            V(K)=ARR(K3)
70        CONTINUE
          K1=K2+1
        ENDIF
100   CONTINUE
      IREC=IFIN
      RETURN
      END
C
      SUBROUTINE LDDATA(NREC,DATA,IPOS,IMAX,N1,N2,LMAP,NCMAP)
C
C LOADS A GIVEN RECORD INTO DATA
C
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
      COMMON/SCRT2/IARR(2048),DUM(1)
      DIMENSION DATA(IMAX,1),NCMAP(1)
      LOGICAL LMAP
C
      NRECO=NREC
      CALL BINRD(NRECO,DUM,N1,N2)
C CELL DATA
      IF (LMAP) THEN
        DO 110 I=1,N2
          NPT=NCMAP(I)
          IF (NPT.GT.0) DATA(IPOS,NPT)=DUM(I)
110     CONTINUE
        IF (N1.LT.0) THEN
          CALL BINRD(NREC,DUM,1,-N1)
          DO 115 I=1,-N1
            NPT=NCMAP(ICSH1+I-1)
            DATA(IPOS,NPT)=DUM(I)
115       CONTINUE
        ENDIF
C ALL VERTEX DATA
      ELSE
        DO 120 I=1,N2
          DATA(IPOS,I)=DUM(I)
120     CONTINUE
      ENDIF
      RETURN
      END
C
      SUBROUTINE LDPDAT(NREC,DATA,IPOS,IMAX,N1,N2,LMAP,NCMAP)
C
C LOADS A GIVEN DOUBLE PRECISION RECORD INTO DATA
C
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
      COMMON/SCRT2/IARR(2048),DUM(1)
      DIMENSION DATA(IMAX,1),NCMAP(1)
      LOGICAL LMAP
C
      NRECO=NREC
      CALL BINRDP(NRECO,DUM,N1,N2)
C CELL DATA
      IF (LMAP) THEN
        DO 110 I=1,N2
          NPT=NCMAP(I)
          DATA(IPOS,NPT)=DUM(I)
110     CONTINUE
        IF (N1.LT.0) THEN
          CALL BINRDP(NREC,DUM,1,-N1)
          DO 115 I=1,-N1
            NPT=NCMAP(ICSH1+I-1)
            DATA(IPOS,NPT)=DUM(I)
115       CONTINUE
        ENDIF
C ALL VERTEX DATA
      ELSE
        DO 120 I=1,N2
          DATA(IPOS,I)=DUM(I)
120     CONTINUE
      ENDIF
      RETURN
      END
C
      SUBROUTINE VECCLR(VECTOR,I1,NDIM,MAXE)
C
C ZEROS OUT POST DATA STORAGE
C
      DIMENSION VECTOR(NDIM,MAXE)
      DO 100 I=1,MAXE
100   VECTOR(I1,I)=0.
      RETURN
      END
C
      SUBROUTINE VECNST(VECTOR,IRS,IDIM,VAL,N1,MAXE,ICSH1)
C
C LOADS A MATERIALLY VARYING POST VALUE
C
      COMMON/CELLS/NE(9,1)
      COMMON/CTABLE/ICTAB(10,1)
      DIMENSION VECTOR(IDIM,1),VAL(99)
C
      DO 100 I=N1,MAXE
        IF (NE(1,I).LT.1) GO TO 100
        ICTID=NE(9,I)
        IF (ICTAB(1,ICTID).LE.2) THEN
          IMAT=ICTAB(4,ICTID)
          VECTOR(IRS,I)=VAL(IMAT)
        ELSEIF (I.GE.ICSH1) THEN
          ICTID=NE(9,NE(7,I))
          IMAT=ICTAB(4,ICTID)
          VECTOR(IRS,I)=VAL(IMAT)
        ENDIF
100   CONTINUE
      RETURN
      END
C
      SUBROUTINE RESUME(LF,IERROR,NCPDIM,NCYDIM,ISCYC,ICOUP)
C
C RESTORE PREPROCESSING INFORMATION
C
      COMMON/ARSIZE/MAXCEL,MAXVRT,MAXNCP,MAXTAB,MAXNBU,MAXREG,MAXPST
      COMMON/KEY/MAXN,MAXE,ICSYS,MAXSYS,ITYPE,PFIRST,IVERS,NUMW,
     1           NI,NJ,NK,NF,MAXB,IBFILL,NOVICE,MAXR,NLINE,MAXCY,
     2           LSCTYP,RMSIZE,INPECH,ISVER,MAXS,MXBL,ISTOP,LW,LR,
     3           LECHO,LNUT,MXTB,LTURBI,LTURBP,SETADD,NSENS,NPART,
     4           SCALE8,MAXCP,LOC180,MVER,MAXSCL,ISTYPE,MXSTB,
     5           NUMBCP,IOPTBC,LTURBF,LTURBT,MAXCRS,MXSAM,NUMBF,
     6           PBTOL,RCPATCH,MXLCOM,KEYSD(2)
      DIMENSION KEYS(18),KEYS2(25),LKEYS2(25)
      LOGICAL LKEYS2,EOF
      EQUIVALENCE (KEYS(1),NUMW),(KEYS2(1),MXTB),(KEYS2,LKEYS2)
      COMMON/NEWKEY/NKEYS(60)
      COMMON/NODES/C(3,1)
      COMMON/CELLS/NE(9,1)
      COMMON/SAMMC/NESAMM(13,1)
      COMMON/SAMML/LSAMM,NLSAMM(1)
      COMMON/CTABLE/ICTAB(10,1)
      COMMON/NTABLE/CTNAME(1)
      COMMON/BOUND/IBDS(6,0:1)
      COMMON/REGTYP/IRTYPE(-1:1)
      COMMON/PRINTS/NPRSRF(3),NPROBS
      COMMON/BCPCOM/IBCTAB(3,1)
      COMMON/PROPS/LMDEF(99)
      COMMON/BOLIST/NBND(6)
      COMMON/SCRT1/IARR(900),RARR(900),IARR2(2048)
      COMMON/CASEN/CASE,TITLE
      COMMON/SCADEF/SCALR(15,50)
      COMMON/SCAMAT/SCALRM(5,99,50)
      COMMON/SCACNT/SCALRC(10,50)
      COMMON/SCAPOR/SCPOR(3,1000)
      COMMON/SCAREG/SCALRB(400,0:1)
      DIMENSION ICOUP(NCPDIM,1),ISCYC(NCYDIM,1)
      DIMENSION ISCAL(15,50),ISCALM(5,99,50),ISCALC(10,50),
     1          LSCAL(15,50),LSCALM(5,99,50),LSCALC(10,50),
     2          ISCPOR(3,1000)
      EQUIVALENCE (SCALR,ISCAL,LSCAL),(SCALRM,ISCALM,LSCALM),
     1            (SCALRC,ISCALC,LSCALC),(SCPOR,ISCPOR)
      DIMENSION IS(100),ITITLE(3)
      COMMON/POLYDF/PPOLY(55,3,99),SPOLY(55,3,50),
     1              RPPOLY(15,3,99),RSPOLY(15,3,50)
      DIMENSION ISCALB(400,0:1)
      EQUIVALENCE (SCALRB,ISCALB)
      DIMENSION ISCAL1(15,50),ISCAL2(15,99,50),
     1          LSCAL1(15,50),LSCAL2(15,99,50)
      EQUIVALENCE (SCALR1,ISCAL1,LSCAL1),(SCALR2,ISCAL2,LSCAL2)
      LOGICAL LS(100),LOGKEY(100),BADSIZ
      LOGICAL LSAMM
      CHARACTER CASE*70,FNAME*80,TITLE*52,CTNAME*4,ITITLE*80
      EQUIVALENCE (NKEYS,LOGKEY,RKEYS)
      IBAD=0
C
C INITIALIZATION
C
      CALL INIT1
C
      BADSIZ=.FALSE.
C
      CALL STRTRM(CASE,CASE,LCASE)
      FNAME=CASE(1:LCASE)//'16'
      IBAD=1
      OPEN(LF,FILE=FNAME,FORM='UNFORMATTED',
     1     ACCESS='SEQUENTIAL',STATUS='UNKNOWN',ERR=900)
      REWIND LF
      IBAD=2
      READ(LF,END=900,ERR=900)MAXN,MAXE,(IS(I),I=1,3),JVERS,KEYS
      IF (JVERS.LT.2300) THEN
        WRITE(LW,1003)
1003    FORMAT(' **ERROR - CANNOT RESUME FROM PROSTAR VERSIONS',
     1         ' EARLIER THAN 2.300')
        STOP
      ENDIF
      IF (MAXN.GT.MAXVRT) THEN
        WRITE(LW,1004)MAXN,MAXVRT
1004    FORMAT(' **ERROR-RESUME FILE HAS MAX VERTEX OF ',I7,/,
     1         '   THIS VERSION WILL ACCEPT A MAXIMUM OF ',I7,
     2         ' VERTICES',/,'   RESUME UNSUCCESSFUL')
        BADSIZ=.TRUE.
      ENDIF
      IF (MAXE.GT.MAXCEL) THEN
        WRITE(LW,1005)MAXE,MAXCEL
1005    FORMAT(' **ERROR-RESUME FILE HAS MAX CELL OF ',I7,/,
     1         '   THIS VERSION WILL ACCEPT A MAXIMUM OF ',I7,
     2         ' CELLS',/,'   RESUME UNSUCCESSFUL')
        BADSIZ=.TRUE.
      ENDIF
      IF (MAXB.GT.MAXNBU) THEN
        WRITE(LW,1006)MAXB,MAXNBU
1006    FORMAT(' **ERROR-RESUME FILE HAS MAX BOUNDARY OF ',I7,/,
     1         '   THIS VERSION WILL ACCEPT A MAXIMUM OF ',I7,
     2      ' BOUNDARIES',/,'   RESUME UNSUCCESSFUL')
        BADSIZ=.TRUE.
      ENDIF
      IF (MAXR.GT.MAXREG) THEN
        WRITE(LW,1007)MAXR,MAXREG
1007    FORMAT(' **ERROR-RESUME FILE HAS MAX REGION OF ',I7,/,
     1         '   THIS VERSION WILL ACCEPT A MAXIMUM OF ',I7,
     2         ' REGIONS',/,'   RESUME UNSUCCESSFUL')
        BADSIZ=.TRUE.
      ENDIF
      IF (BADSIZ) THEN
        CALL INIT1
        IERROR=1
        RETURN
      ENDIF
C TITLE
      IBAD=3
      READ(LF,END=900,ERR=900)ITITLE
      WRITE(LW,1000)ITITLE(1)
1000  FORMAT(' TITLE OF DATA ON FILE16=',/,1X,A)
C
C MAX NUMBER OF CELLS IN CYCLIC AND COUPLED SETS
C
      IBAD=4
      READ(LF,END=900,ERR=900)NCYDMF,NCPDMF
C
C SKIP UNUSED DATA
C
      IBAD=4
      DO 5 I=1,12
        READ(LF,END=900,ERR=900)
5     CONTINUE

C
C CELL DATA
C
      IF (MAXE.GT.0) THEN
        NUMREC=(MAXE-1)/100+1
        NST=0
        IBAD=5
        DO 20 I=1,NUMREC
          READ(LF,END=900,ERR=900)IARR
          IST=NST+1
          NST=IST+99
          IF (NST.GT.MAXE) NST=MAXE
          DO 10 J=IST,NST
            DO 10 K=1,9
              INDEX=9*(J-IST)+K
10            NE(K,J)=IARR(INDEX)
20      CONTINUE
      ENDIF
C
C VERTEX DATA
C
      IF (MAXN.GT.0) THEN
        NUMREC=(MAXN-1)/300+1
        NST=0
        IBAD=6
        DO 40 I=1,NUMREC
          READ(LF,END=900,ERR=900)RARR
          IST=NST+1
          NST=IST+299
          IF (NST.GT.MAXN) NST=MAXN
          DO 30 J=IST,NST
            DO 30 K=1,3
              INDEX=3*(J-IST)+K
30            C(K,J)=RARR(INDEX)
40        CONTINUE
      ENDIF
C
C BOUNDARY DEFINITION DATA
C
      IF (MAXB.GT.0) THEN
        NPB=6
        IBAD=7
        NDIV=900/NPB
        NUMREC=(MAXB-1)/NDIV+1
        NST=0
        DO 60 I=1,NUMREC
          READ(LF,END=900,ERR=900)IARR
          IST=NST+1
          NST=IST+(NDIV)-1
          IF (NST.GT.MAXB) NST=MAXB
          DO 50 J=IST,NST
            DO 50 K=1,NPB
              INDEX=NPB*(J-IST)+K
50            IBDS(K,J)=IARR(INDEX)
60      CONTINUE
      ENDIF
C
C REGION DATA
C
      IF (MAXR.GT.0) THEN
        NPREG=55
        IBAD=8
        NDIV=900/NPREG
        NUMREC=MAXR/NDIV+1
        DO 80 I=1,NUMREC
          READ(LF,END=900,ERR=900)
80      CONTINUE
          NUMREC=MAXR/900+1
          NST=-1
          IBAD=9
          DO 85 I=1,NUMREC
            IST=NST+1
            NST=IST+899
            IF (NST.GT.MAXR) NST=MAXR
            READ(LF,END=900,ERR=900)(IRTYPE(J),J=IST,NST)
85        CONTINUE
      ENDIF
C
C CONTROLS AND PRINT DATA
C
      IBAD=10
      READ(LF,END=900,ERR=900)
      IBAD=11
      READ(LF,END=900,ERR=900)(LS(I),I=1,29),(IS(I),I=1,99),LS(30),
     1  NPRSRF,NPROBS
      IF (NPROBS.LE.0) GO TO 130
      IBAD=12
      MAXOBF=10
      NUMREC=(MAXOBF-1)/900+1
      DO 120 I=1,NUMREC
        READ(LF,END=900,ERR=900)
120   CONTINUE
C
C PROPERTY DATA
C
130   IBAD=13
      READ(LF,END=900,ERR=900)IS(1),LMDEF
      IBAD=14
       DO 137 IM=1,99
        IF (LMDEF(IM).NE.0) THEN
          READ(LF,END=900,ERR=900)
          READ(LF,END=900,ERR=900)
        ENDIF
137   CONTINUE
      IBAD=15
      READ(LF,END=900,ERR=900) ROTA
      IBAD=16
C
C TWO LAYER
C
      READ(LF,END=900,ERR=900)
      IBAD=17
C
C SKIP BOUNDARY LIST ORDER FOR OLD VERSIONS OF PROSTAR
C
      IF(JVERS.LE.2330) THEN
        READ(LF,END=900,ERR=900)NBND
        NBNDMX=MAX0(NBND(1),NBND(2),NBND(3),NBND(4),NBND(5),NBND(6))
        IBAD=18
        IF (NBNDMX.GT.0) THEN
        NUMREC=(NBNDMX-1)/150+1
        DO 150 I=1,NUMREC
          READ(LF,END=900,ERR=900)
150     CONTINUE
      ENDIF
      ENDIF
C
C NEWKEY DATA
C
      IBAD=19
      READ(LF,ERR=900,END=900)NKEYS
      READ(LF,ERR=900,END=900)LSWTCH,RSWTCH
C
C CYCLIC DATA
C
      MAXCY=KEYS(11)
      IF (MAXCY.GT.0) THEN
        IBAD=20
        NN1=2048/NCYDMF
        NN2=NCYDMF
        NUMREC=(MAXCY-1)/NN1+1
        KKMAX=NCYDIM
        NST=0
        DO 170 I=1,NUMREC
          READ(LF,END=900,ERR=900)IARR2
170     CONTINUE
      ENDIF
C
C DATA BASE SETS
C
      IBAD=21
      READ(LF,END=900,ERR=900)
C
C CELL DATA
C
      IF (MAXE.GT.0) THEN
      IBAD=22
        NUMREC=(MAXE-1)/900+1
        DO 190 I=1,NUMREC
          READ(LF,END=900,ERR=900)
190       CONTINUE
      ENDIF
C
C VERTEX DATA
C
      IF (MAXN.GT.0) THEN
      IBAD=23
        NUMREC=(MAXN-1)/900+1
        DO 210 I=1,NUMREC
          READ(LF,END=900,ERR=900)
210     CONTINUE
      ENDIF
C
C BOUNDARY DATA
C
      IF (MAXB.GT.0) THEN
      IBAD=24
        NUMREC=(MAXB-1)/900+1
        DO 230 I=1,NUMREC
          READ(LF,END=900,ERR=900)
230     CONTINUE
      ENDIF
C
C SPLINE DATA
C
      IF (MAXS.GT.0) THEN
      IBAD=25
        NUMREC=(MAXS-1)/900+1
        DO 232 I=1,NUMREC
          READ(LF,END=900,ERR=900)
232     CONTINUE
      ENDIF
C
C BLOCK DATA
C
      IF (MXBL.GT.0) THEN
        IBAD=26
        NUMREC=(MXBL-1)/900+1
        DO 234 I=1,NUMREC
          READ(LF,END=900,ERR=900)
234       CONTINUE
      ENDIF
C
C SPLINE DATA
C
      NPS=102
      IF (MAXS.GT.0) THEN
        IBAD=27
        NUMREC=(MAXS-1)/8+1
        DO 250 I=1,NUMREC
          READ(LF,END=900,ERR=900)
250     CONTINUE
      ENDIF
C
C BLOCK DATA
C
      IF (MXBL.GT.0) THEN
        IBAD=28
        NUMREC=(MXBL-1)/31+1
        DO 270 I=1,NUMREC
          READ(LF,END=900,ERR=900)
270     CONTINUE
      ENDIF
C
C MORE KEYS AND CELL TABLES
C

      IBAD=29
      READ(LF,END=900,ERR=900)KEYS2
C
C SAMM CELLS
C
      IF (MXSAM.GT.0) LSAMM=.TRUE.
      IF (JVERS.GE.2310.AND.MXSAM.GT.0) THEN
        IBAD=30
        NUMREC=(MXSAM-1)/2048+1
        NST=0
        ICSAMM=0
        DO 271 I=1,NUMREC
          READ(LF,END=900,ERR=900)IARR2
          IST=NST+1
          NST=IST+2047
          IF (NST.GT.MXSAM) NST=MXSAM
          DO 272 J=IST,NST
            JARR=J-IST+1
            ICSAMM=ICSAMM+1
            NLSAMM(IARR2(JARR))=ICSAMM
272       CONTINUE
271     CONTINUE
        NUMREC=(MXSAM-1)/150+1
        NST=0
        DO 273 I=1,NUMREC
          READ(LF,END=900,ERR=900)IARR2
          IST=NST+1
          NST=IST+149
          IF (NST.GT.MXSAM) NST=MXSAM
          DO 274 J=IST,NST
            DO 274 K=1,13
              INDEX=13*(J-IST)+K
274           NESAMM(K,J)=IARR2(INDEX)
273     CONTINUE
      ENDIF
C
C
      IF (MVER.LT.0.OR.MVER.GT.1) MVER=0
      IF (MVER.EQ.1) THEN
        IF (MAXN.GT.MAXVRT/2) THEN
          WRITE(LW,1016)MAXN,MAXVRT/2
1016      FORMAT(' **ERROR-RESUME FILE HAS MAX VERTEX OF ',I7,/,
     1           '   THIS VERSION WILL ACCEPT A MAXIMUM OF ',I7,
     2         ' VERTICES IN ANSYS MODE',/,'   RESUME UNSUCCESSFUL')
          CALL INIT1
          IERROR=1
          RETURN
        ELSE
          DO 275 I=MAXVRT/2+1,MAXVRT
275         C(1,I)=0.
          MAXVRT=MAXVRT/2
        ENDIF
      ENDIF
C
      IF (MXTB.GT.MAXTAB) THEN
        WRITE(LW,1012)'CELL',MXTB,MAXTAB
1012    FORMAT(' **ERROR-RESUME FILE HAS MAX ',A,' TABLE INDEX OF ',
     1         I7,/,'   THIS VERSION WILL ACCEPT A MAXIMUM OF ',I7,
     2         ' INDICES',/,'   RESUME UNSUCCESSFUL')
        GO TO 900
      ENDIF
        IF (MXSTB.GT.MAXTAB) THEN
          WRITE(LW,1012)'SPLINE',MXSTB,MAXTAB
          GO TO 900
        ENDIF
      IF (MAXCP.GT.MAXNCP) THEN
        WRITE(LW,1015)MAXCP,MAXNCP
1015    FORMAT(' **ERROR-RESUME FILE HAS MAX COUPLED SET OF ',I7,/,
     2         '   THIS VERSION WILL ACCEPT A MAXIMUM OF ',I7,
     3         '   COUPLED SETS',/,'   RESUME UNSUCCESSFUL')
        GO TO 900
      ENDIF
C
C ICTABLE ENTRIES
C
      IF (MXTB.GT.0) THEN
        IBAD=31
        NUMREC=(MXTB-1)/90+1
        NST=0
        DO 285 I=1,NUMREC
          READ(LF,END=900,ERR=900)IARR
          IST=NST+1
          NST=IST+89
          IF (NST.GT.MXTB) NST=MXTB
          DO 280 J=IST,NST
              INDEX=10*(J-IST)+1
               IF (IARR(INDEX).EQ.7) THEN
                  ICTAB(1,J) = 4
               ELSE
                  ICTAB(1,J) = IARR(INDEX)
               ENDIF
               DO 280 K=2,10
                  INDEX=10*(J-IST)+K
                  ICTAB(K,J)=IARR(INDEX)
 280           CONTINUE
 285     CONTINUE
        READ(LF,END=900,ERR=900)(CTNAME(J),J=1,MXTB)
        DO 290 I=1,MXTB
          IF (ICTAB(3,I).LT.0) ICTAB(3,I)=0
          IF (ICTAB(4,I).LE.0) ICTAB(4,I)=1
290     CONTINUE
      ENDIF
C
C SPLINE TABLES
C
      IF (MXSTB.GT.0) THEN
      IBAD=32
        NUMREC=(MXSTB-1)/180+1
        DO 300 I=1,NUMREC
          READ(LF,END=900,ERR=900)
300     CONTINUE
      ENDIF
C
C PROBE DATA
C
      IF (NSENS.GT.0) THEN
        IBAD=33
        NUMREC=(NSENS-1)/450+1
        DO 320 I=1,NUMREC
          READ(LF,END=900,ERR=900)
320     CONTINUE
      ENDIF
C
C PARTICLE DATA
C
      IF (NPART.GT.0) THEN
        IBAD=34
        NUMREC=(NPART-1)/100+1
        DO 340 I=1,NUMREC
          READ(LF,END=900,ERR=900)
340     CONTINUE
      ENDIF
C
C POROSITY DATA
C
      IBAD=35
      READ(LF,END=900,ERR=900)
C
C COUPLED SET DATA
C
      IF (MAXCP.GT.0) THEN
        IBAD=36
        NN1=2048/NCPDMF
        NN2=NCPDMF
        NUMREC=(MAXCP-1)/NN1+1
        NST=0
        KKMAX=NCPDIM
        DO 360 I=1,NUMREC
          READ(LF,END=900,ERR=900)IARR2
          IST=NST+1
          NST=IST+NN1-1
          IF (NST.GT.MAXCP) NST=MAXCP
          DO 355 J=IST,NST
            DO 350 K=1,NN2
              INDEX=NN2*(J-IST)+K
              IF(IARR2(INDEX).LT.1) GO TO 355
              NCF=IARR2(INDEX)
              IF(K.GT.NCPDIM) THEN
                IF(K.GT.KKMAX) KKMAX=K
                GO TO 350
              ENDIF
              ICOUP(K,J)=NCF
350         CONTINUE
355       CONTINUE
360     CONTINUE
      ENDIF
C
C ANSYS NODAL ROTATIONS
C
      IF (MVER.EQ.1) THEN
        IBAD=37
        NUMREC=(MAXN-1)/300+1
        NST=0
        DO 380 I=1,NUMREC
          READ(LF,END=900,ERR=900)RARR
          IST=NST+1
          NST=IST+299
          DO 370 J=IST,NST
            JJ=J+MAXVRT
            DO 370 K=1,3
              INDEX=3*(J-IST)+K
370           C(K,JJ)=RARR(INDEX)
380     CONTINUE
      ENDIF
C
C SCALAR MATERIAL PROPERTIES
C
      IF (MAXSCL.GT.0) THEN
        IBAD=38
        READ(LF,END=900,ERR=900)SCALR
        READ(LF,END=900,ERR=900)SCNAME
          DO 385 NSC=1,MAXSCL
            IF (ISCAL(1,NSC).NE.0) THEN
C RESTRUCTURE CODE - WRO - 1.NOV.1996
              READ(LF,END=900,ERR=900)
     1          ((SCALRM(J,IM,NSC),J=1,5),IM=1,99),
     2          (SCALRC(K,NSC),K=1,10)
C
C           POLYNOMIAL REPRESENTATIONS
C
              READ(LF,END=900,ERR=900)
     1          (RSPOLY(J,1,NSC),J=1,15),(SPOLY(J,1,NSC),J=1,55),
     2          (RSPOLY(J,2,NSC),J=1,15),(SPOLY(J,2,NSC),J=1,55),
     3          (RSPOLY(J,3,NSC),J=1,15),(SPOLY(J,3,NSC),J=1,55)
            ENDIF
385       CONTINUE
          NUMREC=4
          NST=0
          DO 395 I=1,NUMREC
            READ(LF,END=900,ERR=900)RARR
            IST=NST+1
            NST=IST+299
            IF (NST.GT.1000) NST=1000
              DO 394 J=IST,NST
                DO 394 K=1,3
                  INDEX=3*(J-IST)+K
394               SCPOR(K,J)=RARR(INDEX)
395       CONTINUE
C
C SCALAR BOUNDARY CONDITIONS
C
          IF (MAXR.GT.0) THEN
            IRST=0
            DO 400 I=IRST,MAXR
              READ(LF,END=900,ERR=900)(SCALRB(J,I),J=1,400)
              IF (JVERS.LT.2400) THEN
C  ACCOUNT FOR THE FACT THAT DEPOSITION OPTION WAS ELIMINATED AND
C  IDENTIFIER OF DIFFUSION OPTION CHANGED FROM 6 TO 5
                DO 396 K=1,50
                  INDEX=8*(K-1)+1
                  IF (ISCALB(INDEX,I).EQ.6) THEN
                    ISCALB(INDEX,I)=5
                    IF (IRTYPE(I).EQ.9) THEN
                      ISCALB(INDEX+4,I)=5
                    ENDIF
                  ELSEIF (ISCALB(INDEX,I).EQ.5) THEN
                    WRITE(LW,9050)K,I
9050  FORMAT(' **WARNING - SCALAR NUMBER ',I3,' OF REGION ',I7,
     1       ' HAS BOUNDARY CONDITION',/,
     2       '             SET AS ''DEPOSITION'' - NOT VALID IN THIS',
     3       ' VERSION OF PROSTAR')
                  ENDIF
396             CONTINUE
              ENDIF
400         CONTINUE
          ENDIF
      ENDIF
C
C LIQUID PHASE COMPONENT INFORMATION
C
      IF (JVERS.GE.2312.AND.MXLCOM.GT.0) THEN
        IBAD=39
        READ(LF,END=900,ERR=900)COMP
        READ(LF,END=900,ERR=900)LCNAME
      ENDIF
C
C BOUNDARY COUPLED SET DATA
C
      IF (NUMBCP.GT.0) THEN
        IBAD=40
        NUMREC=(NUMBCP-1)/300+1
        NST=0
        DO 171 I=1,NUMREC
          READ(LF,END=900,ERR=900)IARR
          IST=NST+1
          NST=IST+299
          IF (NST.GT.NUMBCP) NST=NUMBCP
          DO 161 J=IST,NST
            DO 161 K=1,3
              INDEX=3*(J-IST)+K
161           IBCTAB(K,J)=IARR(INDEX)
171       CONTINUE
      ENDIF
C
C IGNORE THE REST OF FILE 16 
C
      GO TO 450
C
C ALL CURRENT PLOT DATA INCLUDING CURRENT SURFACE
C NOTE: 7*NCELL+NFACE CHANGED TO NUMBF*NCELL+NFACE IN VERSION 2310
C
      IBAD=41
      READ(LF,END=900,ERR=900)CMIN,CMAX,CENT,VDIR,SCENT,VSCALE,
     1           PDIST,SECPR,CENTWR,ROT,VSIR,VMASTR,VOFF,NCPLOT,
     2           LSTPLT
C
C      IBAD=42
C      IF (NCPLOT.GT.0) THEN
C        NUMREC=(NCPLOT-1)/2048+1
C        NST=0
C        DO 420 I=1,NUMREC
C          READ(LF,END=900,ERR=900)IARR2
C          IST=NST+1
C          NST=IST+2047
C          IF (NST.GT.NCPLOT) NST=NCPLOT
C          DO 410 J=IST,NST
C            INDEX=J-IST+1
C410         ICLIS(J)=IARR2(INDEX)
C420     CONTINUE
C        IF (LSTPLT.EQ.2) THEN
C          REWIND 43
C430       READ(LF,END=450)IARR2
C          IF (JVERS.LT.2310) THEN
C            DO 435 I=1,2048
C              IF (IARR2(I).EQ.-1) GO TO 436
C              NCELL=IABS(IARR2(I))/7
C              NFACE=MOD(IABS(IARR2(I)),7)
C              IF (IARR2(I).GT.0) THEN
C                IARR2(I)=NUMBF*NCELL+NFACE
C              ELSE
C                IARR2(I)=NUMBF*NCELL+NFACE
C                IARR2(I)=-IARR2(I)
C              ENDIF
C435         CONTINUE
C          ENDIF
C436       WRITE(43)IARR2
C          DO 440 I=1,2048
C            IF (IARR2(I).EQ.-1) GO TO 450
C440       CONTINUE
C          GO TO 430
C        ENDIF
C      ENDIF
C
C
450   REWIND LF
499   WRITE(LW,1001)LF
1001  FORMAT(' ALL DATA RESTORED FROM FILE',I3)
      CLOSE(LF)
      RETURN
900   WRITE(LW,1011)IBAD,LF
1011  FORMAT(' **ERROR ',I2,' WHILE READING RESUME FILE',I3,/,
     1       '   NOT ALL DATA RESTORED')
      IERROR=1
      CLOSE(LF)
      RETURN
      END
C
      SUBROUTINE INIT1
C
C INITIALIZE ANY REQUIRED VARIABLES BEFORE RESUMING
C
      COMMON/ARSIZE/MAXCEL,MAXVRT,MAXNCP,MAXTAB,MAXNBU,MAXREG,MAXPST
      COMMON/KEY/MAXN,MAXE,ICSYS,MAXSYS,ITYPE,PFIRST,IVERS,NUMW,
     1           NI,NJ,NK,NF,MAXB,IBFILL,NOVICE,MAXR,NLINE,MAXCY,
     2           LSCTYP,RMSIZE,INPECH,ISVER,MAXS,MXBL,ISTOP,LW,LR,
     3           LECHO,LNUT,MXTB,LTURBI,LTURBP,SETADD,NSENS,NPART,
     4           SCALE8,MAXCP,LOC180,MVER,MAXSCL,ISTYPE,MXSTB,
     5           NUMBCP,IOPTBC,LTURBF,LTURBT,MAXCRS,PBTOL,KEYS2(6)
      COMMON/NEWKEY/NCTOT,NCYTOT,STEADY,MOVGR,IBDY,DT,MAXCOR,RESOC,
     1           RESNOC,IWR1,IWR2,IPRI,IPST,LSIMP,MOVWAL(6),LVECT,
     2           LRADI,NCOTOT,LSOLAR,LCPUTI,MAXPOR,MAXMAT,URFDPW,
     3           URFRAD,LCONJ,LSWTCH(100),RSWTCH(100),MAXLOC,NRFRQ,
     4           ITOP(4),SWBL,NBEAM,COURNT,LSTREM,LMACH,LMFRAM,
     5           URFBNP,URFBNU,IUPDAT,ITBEG,IGPF0,IGPF1,URFLUX,
     6           URFSOL,LRNG,LFLINJ,IRGAS,IRENTH,NRCOMP,ICSOL,
     7           SOLANG,SOLINT,NCYINT,NCYARB
      COMMON/NODES/C(3,1)
      COMMON/CELLS/NE(9,1)
      COMMON/SAMML/LSAMM,NLSAMM(1)
      COMMON/BOUND/IBDS(6,0:1)
      COMMON/REGTYPE/IRTYPE(-1:1)
      COMMON/CTABLE/ICTAB(10,1)
      COMMON/NTABLE/CTNAME(1)
      COMMON/PRINTS/NPRSRF(3),NPROBS
      COMMON/PROPS/LMDEF(99)
      COMMON/BOLIST/NBND(6)
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
C
C VARIABLE TYPES
C
      CHARACTER CTNAME*4,LPREP*12,ITITLE*80,CHNUM*3,SUBTI1*80,SUBTI2*80
      LOGICAL INPECH,LSCTYP,PFIRST
      LOGICAL STEADY,MOVGR,IBDY,LSIMP,MOVWAL,LVECT,LRADI,LSOLAR
      LOGICAL LRSKIP,LDSURF,LSRF,LCPUTI,LSWTCH,LCONJ,LCNPST,LDPPRS
      LOGICAL WPOST,LDCELL,LDVERT,LSORT,LUNMOD,LOPER
      LOGICAL LTURBI,LTURBP,SETADD
      LOGICAL LPCORD,LOC180,LSTREM,LMACH,LMFRAM,LTURBF,LTURBT
      LOGICAL LSTAR,SPCYC,LSAMM
C
      RVER=FLOAT(IVERS)/1000.
      WRITE(CHNUM,1000)RVER
1000  FORMAT(F3.1)
      LPREP='PROSTAR '//CHNUM
      ITITLE=' '
      SUBTI1=' '
      SUBTI2=' '
      ISVER=2
      RMSIZE=0.
      NLINE=20
      MAXN=0
      MAXE=0
      MAXS=0
      MXBL=0
      NUMW=0
      NI=3
      NJ=3
      NK=3
      NF=0
      MAXB=0
      MAXR=0
      MAXCY=0
      MAXCP=0
      NUMBCP=0
      IOPTBC=0
      IBFILL=4
      LTURBI=.FALSE.
      LTURBP=.TRUE.
      LTURBF=.FALSE.
      LTURBT=.FALSE.
      SETADD=.FALSE.
      ICSYS=1
      MAXSYS=4
      ITYPE=1
      ISTYPE=1
      INPECH=.FALSE.
      LOC180=.FALSE.
      IF (MVER.EQ.1) THEN
        MVER=0
        MAXVRT=MAXVRT*2
      ENDIF
      MAXSCL=0
      DO 20 I=1,MAXVRT
20      C(1,I)=1.E30
      DO 41 I=1,MAXTAB
        CTNAME(I)='    '
        DO 39 J=1,10
39        ICTAB(J,I)=0
41    CONTINUE
      DO 50 I=1,MAXCEL
        DO 45 J=1,9
45        NE(J,I)=0
        NLSAMM(I)=0
50    CONTINUE
      LSAMM=.FALSE.
      MXSTB=1
      LSCTYP=.FALSE.
      NPROBS=0
      I=13
      DO 135 IM=1,99
        PREPP(1,I)=0.
        PREPP(2,I)=0.
        TREFM(I)=0.
        LMDEF(IM)=0
135   CONTINUE
      LMDEF(1)=1
      MAXPOR=0
      MAXMAT=1
      DO 140 I=1,6
        MOVWAL(I)=.FALSE.
140     NBND(I)=0
      NCTOT=0
      NCYTOT=0
      NCOTOT=0
      STEADY=.TRUE.
      MOVGR=.FALSE.
      IBDY=.FALSE.
      LSIMP=.TRUE.
      LVECT=.FALSE.
      LRADI=.FALSE.
      LSOLAR=.FALSE.
      LCONJ=.FALSE.
      LCPUTI=.FALSE.
      DO 145 I=1,100
        RSWTCH(I)=0.
145     LSWTCH(I)=.FALSE.
      MAXLOC=0
      NRFRQ=10000
      ITOP(1)=1
      ITOP(2)=1
      ITOP(3)=1
      ITOP(4)=1
      DT=.005
      MAXCOR=10
      RESOC=.25
      RESNOC=.15
      URFDPW=.5
      URFRAD=.3
      NBEAM=100
      COURNT=.35
      LSTREM=.FALSE.
      LMACH=.FALSE.
      LMFRAM=.FALSE.
      URFBNP=.8
      URFBNU=.8
      IUPDAT=1
      ITBEG=1
      IGPREF=0
      ITGRP=0
      IWR1=0
      IWR2=0
      IPRI=5000
      IPST=5000
      ITERU=0
      TIMEU=-1.
      ITERS=0
      TIMES=0
      DO 155 I=1,200
        NDATA(I,1)=0
        NDATA(I,2)=0
        NDATA(I,3)=0
        NDATAP(I,1)=0
        NDATAP(I,2)=0
        NDATAP(I,3)=0
        LSRF(I)=.FALSE.
155   CONTINUE
      SPCYC=.FALSE.
      DO 156 I=1,100
        LSTAR(I)=.FALSE.
156   CONTINUE
      WPOST=.FALSE.
      LDVERT=.FALSE.
      LDCELL=.FALSE.
      LSORT=.FALSE.
      LUNMOD=.FALSE.
      LOPER=.FALSE.
      LRSKIP=.TRUE.
      LDSURF=.FALSE.
      ISRTAB=0
      ICSH1=0
      NSITEM=0
      NBCUT=0
      NBCYC=0
      PREF1=0.
      IPSYS=1
      LCNPST=.FALSE.
      LDPPRS=.TRUE.
      LPCORD=.FALSE.
      NSENS=0
      NPART=0
      SWBL=.9
      SCALE8=-1.
C
      RETURN
      END
C
      SUBROUTINE LOAD9(LREC,IERROR)
      COMMON/ARSIZE/MAXCEL,MAXVRT,MAXPST,MAXCUT,MAXDAR,MAXFAC,MAXBRK,
     1              MAXSID,MAXBND,MAXNBU,MAXB1D,MAXREG,MAXNCY,MAXRST,
     2              MAXOBS,MAXOBF,MXIJK,MAXSPL,MAXBLK,MAXTAB,MAXINF,
     3              MAXSC1,MAXSC2,MAXPRB,MAXKT,MAXNCP,MAXPS2,MAXPR2,
     4              MAXVR2,MXSTOR,MAXDRP,MAXPR,MAXBCP,MAXEVE,MAXCEV,
     5              MAXEAT,MAXEDE,MAXREF,MAXRGS,MAXASI
C
C LOAD GIVEN ITERATION FROM THE RESTART DATA FILE
C
      COMMON/KEY/MAXN,MAXE,ICSYS,MAXSYS,ITYPE,PFIRST,IVERS,NUMW,
     1           NI,NJ,NK,NF,MAXB,IBFILL,NOVICE,KEYS(10),LW,LR,
     2           LECHO,LNUT,MXTB,LTURBI,LTURBP,SETADD,NSENS,NPART,
     4           SCALE8,KEYS2(18)
      COMMON/NEWKEY/NCTOT,NCYTOT,ITIMFL,MOVGR,IBDY,DT,MAXCOR,RESOC,
     1           URFPCR,IWR1,IWR2,IPRI,IPST,LSIMP,MOVWAL(6),LVECT,
     2           LRADI,NCOTOT,LSOLAR,LCPUTI,MAXPOR,MAXMAT,URFDPW,
     3           URFRAD,LCONJ,MAXLOC,NRFRQ,
     4           ITOP(4),SWBL,NBEAM,COURNT,LSTREM,NKEY(20)
      COMMON/CSWTCH/LSWTCH(200),RSWTCH(200)
      COMMON/CELLS/NE(9,1)
      COMMON/CASEN/CASE,TITLE
      COMMON/LOCDEF/CS0(3,99),ICSTYP(99),CSVEC(3,3,99),CAUXP(99)
      COMMON/POSTD/ITERU,TIMEU,ITERS,TIMES,VMING,VMAXG,VMINL,VMAXL,
     1             SMING,SMAXG,SMINL,SMAXL,NDATA(200,3),LPOST,
     2             NCELL,NBC,NBW,NBS,NBB,NNODE,WPOST,LDCELL,LDVERT,
     3             LSORT,LUNMOD,IPSYS,LOPER,LRSKIP,TREF,LDSURF,
     4             ISRTAB,ICSH1,NSITEM,LSRF(200),NBCUT,NBCYC,PREF,
     5             PREPP(2,99),NTCELL,LCNPST,LDPPRS,
     6             LPCORD,NUMCON,NSC,TREFM(99),LVERS,LDFLUX,
     7             IREFRM,IRESYS,OMEREF,ADDREL,PGRAV(7,99),PSPIN(7,99),
     8             PMW(99),PMWS(50,99),SCALE9,LPTYP,LADJST,LPINTP,TIMEP,
     9             ITERP,LRELA,LMVGR,NDATAP(200,3),IRETW,NWSET,SPCYC,
     *             LSTAR(100),NBNCST,NBNCEN,NBSI,NSCL,NREG,
     1             MRECMM,NBP23,NCLPS,NWPRSM,NPRSM,NCLP,NCLPF,ITYPEN,
     2             IPATCH
      COMMON/PROPS/IDEN(99),CDEN(5,99),VISCO(3,99),CP(2,99),
     1  CONDUC(2,99),ITURB(99),CTURB(5,99),CINIT(12,99),PRFPRP(99),
     2  IPREF(99),ACCEL(7,99),ROTA(7,99),COKE(10,99),ISOPT(20,99),
     3  PRLAM(99),TDATUM(99),TSTAG(99),LMDEF(99),MATCUR
      LOGICAL LSTAR,SPCYC
      LOGICAL IPRSG,ITEG,IEDG,ITEMG,IVISG,IDENG,IVILG,ICPOG,ICDOG,IAS
      COMMON/POSTC/DENM(99),VISM(99),CPHM(99),CONDM(99)
      COMMON /CMDPND/ NDROP,NDROP9,NDREC,NDRP1,NDRP2,NDRPIC
      COMMON/EVCURR/LEVENT,NUMEVE,MXEVE,NEVENT,NCACT,NCDACT,NCFLUI,
     1              NATTAC,NDETAC,LGRID,NECON,NDCON,TEVENT,MXCEV,
     2              MXEAT,MXEDE,IDTYPE,IDT1,IDT2,NGRID,IRGRID,NRGRID,
     3              NRTOT,IEVREC,NUMCND,NUMACT,IEVCHK,TEOFF,TEFACT,
     4              IFLGEV,RPM,THETAI,YINIT,ISTATE,YTDC,
     5              CONRL,CRAD,ITEV,TINITP,DELTAT
      COMMON/EVLDIN/LOADEV,TELOAD,NELOAD,LGCFLG,NGRL,IRGRL,NRGRL,
     1              LWDONE,LAFTGC,NEVNEW,DONTEX,LATTAC,LSCDEF,LNWSET,
     2              LDOCO,LDOUP,LDOGR,LDONW,LDEAC,LACTI,LATCH,MXTOLD,
     3              IEPREP,IEPOST,ITST1,ITST2,DTIT,NLCON,LENCON(100),
     4              LREFIN,MXNOLD,MXEOLD,IRCND,IRASI,IRPSM,LINGC,IMSTAR
      LOGICAL LAFTGC,LWDONE,DONTEX,LATTAC,LSCDEF,LNWSET
      LOGICAL LDOCO,LDOUP,LDOGR,LDONW,LDEAC,LACTI,LATCH,LREFIN,
     1        IMSTAR
      COMMON/SCRT1/IDUM1(1),ZZDUM1(3)
      COMMON/SCRT2/IARR(2048)
      DIMENSION CINPS(20),RINPS(20),INPS(20),RINP(20),INP(20)
      DIMENSION ARR(2048),LARR(2048),IROTA(7,99)
      LOGICAL LARR,WPOST,PFIRST,HRAD,LSRF,LMVGRD,LRAMP,LCNPST,LDPPRS
      LOGICAL LRELA
      LOGICAL NOVICE,COMPG,LSTRE,LMACH,STEADY,WSURF,LVOID,LCOUP
      CHARACTER CINPS*4
      CHARACTER CASE*70,FNAME*80,TITLE*52
      EQUIVALENCE (IARR,ARR,LARR),(ROTA,IROTA)
      EQUIVALENCE (LSTAR(4),IPRSG), (LSTAR(5),ITEG),  (LSTAR(6),IEDG),
     *            (LSTAR(7),ITEMG), (LSTAR(8),IVISG), (LSTAR(9),IDENG),
     *            (LSTAR(10),IVILG),(LSTAR(11),ICPOG),(LSTAR(12),ICDOG),
     *            (LSTAR(24),IAS)
C
C FIRST OPEN THIS UNIT CORRECTLY
C
      DO 1 I=1,200
        NDATA(I,1)=0
        NDATA(I,2)=0
        NDATA(I,3)=0
        LSRF(I)=.FALSE.
1     CONTINUE
      DO 5 I=1,99
        PMW(I)=0.
        DO 2 J=1,7
          PGRAV(J,I)=0.
          PSPIN(J,I)=0.
2       CONTINUE
        DO 3 J=1,50
3         PMWS(J,I)=0.
5     CONTINUE
      NDROP9=0
      NDREC=0
C
      CALL STRTRM(CASE,CASE,LCASE)
      FNAME=CASE(1:LCASE)//'09'
      OPEN(LPOST,FILE=FNAME,STATUS='OLD',FORM='UNFORMATTED',
     1     RECL=LREC,ACCESS='DIRECT',ERR=900)
      MAXREC=1
      READ(LPOST,REC=MAXREC,ERR=900)IARR
C
C FOUND AN ITERATION
C
      ITER=IARR(1)
      TIME=ARR(2)
      NCELL=IARR(3)
      NBC=IARR(4)
      NBW=IARR(5)
      NBS=IARR(6)
      NBB=IARR(7)
      NNODE=IARR(8)
      WPOST=LARR(9)
      WRITE(TITLE,1003)(IARR(J),J=10,22)
1003  FORMAT(13A4)
      ISKIP=IARR(23)
      NBCYC=IARR(24)
      NBCUT=IARR(25)
      IPATCH=0
      LVERS=IARR(26)
      IF (IABS(LVERS).GT.10000) THEN
        IPATCH=MOD(IABS(LVERS),10)
        LVERS=LVERS/10
      ENDIF
      IF (LVERS.LT.0.AND.IABS(LVERS).GT.1800.AND.IABS(LVERS).LT.3000)
     1       THEN
        WRITE(LW,1007)LPOST
1007    FORMAT(' **FILE',I3,' IS PROBABLY A TRANSIENT DATA FILE',/,
     1         '   USE THE TRLOAD COMMAND FOR THIS FILE')
        IERR=1
        RETURN
      ENDIF
C POST FILE GT.2151
      IF (LVERS.GT.2151) THEN
C NUMBER OF RECORDS FOR TWO LAYER
        IRETW=IARR(27)
C NUMBER OF RECORDS FOR TWO PHASE FLOW
        IRTPL=IARR(32)
        STEADY=LARR(55)
        WSURF=LARR(102)
        NDR=IARR(125)
        NDROP9=NDR
        NSOL=IARR(127)
        NWSET=IARR(128)
        NBPT=IARR(129)
        NBCUTO=IARR(137)
      ELSE
        IRETW=0
        IRTPL=0
        STEADY=.FALSE.
        WSURF=.FALSE.
        NDR=0
        NDROP9=0
        NSOL=0
        NWSET=0
        NBPT=0
        NBCUTO=0
      ENDIF
C POST FILE GT 2.011
      IF (LVERS.GT.2011) THEN
        TREFM(1)=ARR(27)
        IFVER=IARR(28)
        NTREC=IARR(29)
        HRAD=LARR(30)
        NPATCH=IARR(31)
        NTPCH=IARR(32)
        NUMCON=IARR(33)
        NSITEM=IARR(34)
        IF (NSITEM.GT.0) THEN
          LSRF(1)=LARR(35)
          LSRF(2)=LARR(36)
          LSRF(3)=LARR(37)
          LSRF(6)=LARR(38)
          LSRF(7)=LARR(38)
          LSRF(8)=LARR(39)
          LSRF(9)=LARR(40)
          LSRF(10)=LARR(41)
          LSRF(4)=LARR(42)
          LSRF(5)=LARR(43)
          LSRF(58)=LARR(44)
          LSRF(59)=LARR(45)
          LSRF(60)=LARR(46)
          LSRF(61)=LARR(47)
        ENDIF
C POST FILE LE 2.011
      ELSE
        TREFM(1)=0.
        IFVER=0
        NTREC=0
        HRAD=.FALSE.
        NPATCH=0
        NTPCH=0
        NUMCON=0
        NSITEM=0
      ENDIF
C POST FILE GE 2013
      IF (LVERS.GE.2013) THEN
        LSTEP=IARR(51)
        LRAMP=LARR(52)
        ITERSO=IARR(53)
        ITERSN=IARR(54)
        PREPP(2,1)=ARR(100)
        PREPP(1,1)=0.
        LMVGRD=LARR(101)
C POST FILE LT 2013
      ELSE
        LSTEP=0
        LRAMP=.FALSE.
        ITERSO=0
        ITERSN=0
        PREPP(1,1)=0.
        PREPP(2,1)=0.
        LMVGRD=.FALSE.
      ENDIF
C POST FILE GE 2014
      IF (LVERS.GE.2014) THEN
        NTCELL=IARR(99)
        NUMMAT=IARR(200)
        NSMAT=IARR(201)
        PREF=0.
        DO 30 I=1,NUMMAT
          I2=I+NUMMAT
          I3=I+2*NUMMAT
          PREPP(1,I)=ARR(201+I)
          PREPP(2,I)=ARR(201+I2)
          TREFM(I)=ARR(201+I3)
30      CONTINUE
        DO 35 ISOL=1,NSOL
          IMAT=IARR(201+6*NUMMAT+NSMAT+ISOL)
          I=IMAT+NSMAT+1
          TREFM(-IMAT)=ARR(201+6*NUMMAT+I)
35      CONTINUE
C POST FILE LT 2014
      ELSE
        NTCELL=NCELL
        NUMMAT=1
        NSMAT=0
      ENDIF
      IF (LVERS.GE.2016) THEN
        LDPPRS=.TRUE.
        COMPG=LARR(56)
      ELSE
        LDPPRS=.FALSE.
        COMPG=.FALSE.
      ENDIF
      IF (LVERS.GE.2100) THEN
        NBC012=IARR(57)
        MREC29=IARR(83)
        MREC39=IARR(84)
        SCALE9=ARR(85)
        IF (SCALE9.NE.SCALE8) THEN
          WRITE(LW,1011)SCALE9,SCALE8
1011      FORMAT(' **NOTE - THE GEOMETRY SCALE FACTOR FROM FILE 9 IS ',
     1    G13.6,/,' AND IS NOT EQUAL TO THE FACTOR SAVED ON FILE 16 ',
     2    ' WHICH IS ',G13.6)
        ENDIF
      ELSE
        SCALE9=1.
      ENDIF
C POST FILE GT 2112
      IF (LVERS.GT.2112.AND.LVERS.LT.2268) THEN
        LSTRE=LARR(123)
      ELSE
        LSTRE=.FALSE.
      ENDIF
      IF (LVERS.GT.2141.AND.LVERS.LT.2268) THEN
        LMACH=LARR(124)
      ELSE
        LMACH=.FALSE.
      ENDIF
      IF (NTCELL.GT.NCELL) THEN
        LCNPST=.TRUE.
      ELSE
        LCNPST=.FALSE.
      ENDIF
C POST FILE GT 2114
      IF (LVERS.GT.2114.AND.NSITEM.GT.0) THEN
        I2=0
        DO 40 I=1,200
40        LSRF(I)=.FALSE.
        IF (LVERS.GT.2151) THEN
C FX,Y,Z,YPL,NDIST,(HTRAN,TEMP),MCOEF(50),MTRAN(50),HFLUX,MFLUX(50),
C (THRMINCI,RADIOS), (SOLARTHRMINCI,SOLARRADIOS)
          DO 45 I=1,109
            I2=I2+1
            LSRF(I2)=LARR(I+1236)
            IF (I.EQ.6.OR.I.EQ.108.OR.I.EQ.109) THEN
              I2=I2+1
              LSRF(I2)=LARR(I+1236)
            ELSEIF (I.GT.6.AND.I.LT.57.AND.LVERS.GT.2269) THEN
              LSRF(I+106)=LARR(I+1236)
            ENDIF
45        CONTINUE
        ELSE
          DO 50 I=1,107
            I2=I2+1
            LSRF(I2)=LARR(I+1236)
            IF (I.EQ.6) THEN
              I2=I2+1
              LSRF(I2)=LARR(I+1236)
            ENDIF
50        CONTINUE
        ENDIF
      ENDIF
      IF (LVERS.LT.2003.OR.LVERS.GT.4000) ISKIP=0
      ITERU=ITER
      ITERS=ITER
      TIMEU=TIME
      TIMES=TIME
      IF (LVERS.GE.2100) WRITE(LW,1012)SCALE9
1012  FORMAT(' ALL GEOMETRY ON FILE 9 SCALED BY ',G13.6)
C
C LOGICAL FLAGS FOR WRITING VARIABLES IN FILE9 (NEW IN VERSION 2264)
C 100 LOGICALS FROM 1801-1900
C
C     TEMPLATE:
C
C LSTAR
C     1-BOUNDARIES   6-ED    11-CP    16-GVLO1 21-FLUXI  26-TWPHL
C     2-6 FLUXES     7-T     12-COND  17-RSMX1 22-FLUXJ  27-MOVGR
C     3-3 VELOCITIES 8-TVIS  13-DENDP 18-PP    23-FB
C     4-PRESSURE     9-DEN   14-DENO  19-T4PAT 24-FBSI
C     5-TE          10-LVIS  15-SNRI1 20-HRPAT 25-TWOLYR
C
      DO 51 I=1,100
        IF (LVERS.GT.2263) THEN
          LSTAR(I)=LARR(1800+I)
        ELSE
          LSTAR(I)=.TRUE.
        ENDIF
51    CONTINUE
      IF (LVERS.GT.2263) THEN
        NBNCST=IARR(1901)
        NBNCEN=IARR(1902)
        NSCL  =IARR(1904)
        NREG  =IARR(1905)
        NBSI  =IARR(1906)
        SPCYC =LARR(1907)
      ELSE
        NBNCST=0
        NBNCEN=0
        NSCL  =0
        NREG  =0
        NBSI  =0
        SPCYC =.FALSE.
      ENDIF
C ENTHALPY FLAG 0=STATIC,1=TOTAL,2=ROTHALPY,3=MIX
C VOID FRACTION FLAG
      IF (LVERS.GT.2267) THEN
        ITYPEN=IARR(1915)
        LVOID=LARR(1346)
        LCOUP=LARR(1349)
        MSPIN=IARR(1350)
      ELSE
        ITYPEN=0
        LVOID=.FALSE.
        LCOUP=.FALSE.
        MSPIN=0
      ENDIF
C NU CHANGES 2268 ONLY IF EVENTS
      IF (LVERS.GT.2268.AND.LMVGRD) THEN
        MRECMM=IARR(1908)
        NBP23 =IARR(1909)
        NCLPS =IARR(1910)
        NWPRSM=IARR(1911)
        NPRSM =IARR(1912)
        NCLP  =IARR(1913)
        NCLPF =IARR(1914)
      ELSE
        MRECMM=0
        NBP23 =0
        NCLPS =0
        NWPRSM=0
        NPRSM =0
        NCLP  =0
        NCLPF =0
      ENDIF
C
C SET UP ARRAY TO DEFINE WHERE EACH VARIABLE BEGINS
C FIRST IS BOUNDARY MAP ARRAY
C (F1,F2,F3,(F4,F5,F6),U,V,W,P,TE,ED,VIS,T,DENS,LAMV,CP,COND,ENTHALPY,
C  VOID FRACTION,SC1-50)
C (WALL ITEMS - FOX,FOY,FOZ,HFILM,YPLUS,...)
C
      IF (LVERS.LT.2264) THEN
        IF (LVERS.GT.2151) THEN
          NDATA(1,3)=MAXREC
          MAXREC=MAXREC+NUMREC(1,NBC)
        ENDIF
        NDATA(1,1)=MAXREC
        IF (LVERS.GT.2114) THEN
          NDATA(2,1)=NDATA(1,1)+NUMREC(1,NCELL)
          NDATA(3,1)=NDATA(2,1)+NUMREC(1,NCELL)
          NDATA(4,1)=NDATA(3,1)+NUMREC(1,NCELL)
          NDATA(5,1)=NDATA(4,1)+NUMREC(1,NCELL)
          NDATA(6,1)=NDATA(5,1)+NUMREC(1,NCELL)
          NDATA(7,1)=NDATA(6,1)+NUMREC(1,NCELL)
        ELSE
          NDATA(2,1)=NDATA(1,1)+NUMREC(-NBW,NCELL)
          NDATA(3,1)=NDATA(2,1)+NUMREC(-NBS,NCELL)
          NDATA(4,1)=0
          NDATA(5,1)=0
          NDATA(6,1)=0
          NDATA(7,1)=NDATA(3,1)+NUMREC(-NBB,NCELL)
        ENDIF
        NDATA(8,1)=NDATA(7,1)+NUMREC(-NBC,NCELL)
        NDATA(9,1)=NDATA(8,1)+NUMREC(-NBC,NCELL)
        NDATA(10,1)=NDATA(9,1)+NUMREC(-NBC,NCELL)
        IF (LDPPRS) THEN
          NDATA(11,1)=NDATA(10,1)+NUMRDP(-NBC,NCELL)
        ELSE
          NDATA(11,1)=NDATA(10,1)+NUMREC(-NBC,NCELL)
        ENDIF
        NDATA(12,1)=NDATA(11,1)+NUMREC(-NBC,NCELL)
        NDATA(13,1)=NDATA(12,1)+NUMREC(-NBC,NCELL)
        IF (LVERS.LT.2152) THEN
          NDATA(14,1)=NDATA(13,1)+NUMREC(-NBC,NTCELL)
          NDATA(15,1)=NDATA(14,1)+NUMREC(-NBC,NTCELL)
          NDATA(16,1)=0
          NDATA(17,1)=0
          NDATA(18,1)=0
          MMM=NDATA(15,1)
          NCADD=NCELL
        ELSE
          NDATA(14,1)=NDATA(13,1)+NUMREC(-NBC,NCELL)
          NDATA(15,1)=NDATA(14,1)+NUMREC(-NBC,NTCELL)
          NDATA(16,1)=NDATA(15,1)+NUMREC(-NBC,NCELL)
          NDATA(17,1)=NDATA(16,1)+NUMREC(-NBC,NCELL)
          NDATA(18,1)=NDATA(17,1)+NUMREC(-NBC,NTCELL)
          MMM=NDATA(18,1)
          NCADD=NTCELL
        ENDIF
      ELSE
C
C VERSION 2264 ONWARDS
C
        NDATA(1,3)=MAXREC
        MAXREC=MAXREC+NUMREC(1,NBC)
C 6 FLUXES
        NDATA(1,1)=MAXREC
        NDATA(2,1)=NDATA(1,1)+NUMREC(1,NCELL)
        NDATA(3,1)=NDATA(2,1)+NUMREC(1,NCELL)
        NDATA(4,1)=NDATA(3,1)+NUMREC(1,NCELL)
        NDATA(5,1)=NDATA(4,1)+NUMREC(1,NCELL)
        NDATA(6,1)=NDATA(5,1)+NUMREC(1,NCELL)
C 3 VELOCITIES
        NDATA(7,1)=NDATA(6,1)+NUMREC(1,NCELL)
        NDATA(8,1)=NDATA(7,1)+NUMREC(-NBC,NCELL)
        NDATA(9,1)=NDATA(8,1)+NUMREC(-NBC,NCELL)
C PRESSURE
        NDATA(10,1)=NDATA(9,1)+NUMREC(-NBC,NCELL)
        NTOTR=NDATA(10,1)
        NCADD=NCELL
C TE,ED,TURB VIS
        IF (IVISG) THEN
          IF (LDPPRS) THEN
            NDATA(11,1)=NDATA(10,1)+NUMRDP(-NBC,NCELL)
          ELSE
            NDATA(11,1)=NDATA(10,1)+NUMREC(-NBC,NCELL)
          ENDIF
          NDATA(12,1)=NDATA(11,1)+NUMREC(-NBC,NCELL)
          NDATA(13,1)=NDATA(12,1)+NUMREC(-NBC,NCELL)
          NTOTR=NDATA(13,1)
        ELSE
          NDATA(11,1)=-1
          NDATA(12,1)=-1
          NDATA(13,1)=-1
          NTOTR=NDATA(10,1)+NUMRDP(-NBC,NCELL)-NUMREC(-NBC,NCELL)
        ENDIF
C TEMP
        IF (ITEMG) THEN
          NTOTR=NTOTR+NUMREC(-NBC,NCADD)
          NDATA(14,1)=NTOTR
          NCADD=NTCELL
        ELSE
          NDATA(14,1)=-1
        ENDIF
C DENS
        IF (IDENG) THEN
          NTOTR=NTOTR+NUMREC(-NBC,NCADD)
          NDATA(15,1)=NTOTR
          NCADD=NCELL
        ELSE
          NDATA(15,1)=-1
        ENDIF
C
C NU CHANGES - SREENADH CHANGES HERE SINCE SOLID DENSITIES FOR CONJ HEAT
C              TRANSFER ARE NOT WRITTEN
C LAM VIS
        IF (IVILG) THEN
          NTOTR=NTOTR+NUMREC(-NBC,NCADD)
          NDATA(16,1)=NTOTR
          NCADD=NCELL
        ELSE
          NDATA(16,1)=-1
        ENDIF
C CP
        IF (ICPOG) THEN
          NTOTR=NTOTR+NUMREC(-NBC,NCADD)
          NDATA(17,1)=NTOTR
          NCADD=NTCELL
        ELSE
          NDATA(17,1)=-1
        ENDIF
C COND
        IF (ICDOG) THEN
          NTOTR=NTOTR+NUMREC(-NBC,NCADD)
          NDATA(18,1)=NTOTR
C NU FIX PATCH B - SREENADH 2/15/96 Bug ADAib00398
C         NCADD=NCELL
          NCADD=NTCELL
        ELSE
          NDATA(18,1)=-1
        ENDIF
        MMM=NTOTR
      ENDIF
      NDATA(19,1)=0
      NDATA(20,1)=0
      IF (NUMCON.GT.0) THEN
        DO 60 I=1,NUMCON
          NDATA(20+I,1)=MMM+NUMREC(-NBC,NCADD)
          NCADD=NCELL
          MMM=NDATA(20+I,1)
60      CONTINUE
      ENDIF
C GET 2ND SET OF MATERIAL PROPERTIES (ACCELERATIONS AND SPIN)
      IF (LVERS.GT.2151) THEN
        IREC=MMM+NUMREC(-NBC,NCADD)+2*NUMREC(1,11859)+NUMREC(1,5900)+1
        READ(LPOST,REC=IREC)ARR
        DO 61 IMAT=1,NUMMAT
C GX,GY,GZ,XG,YG,ZG,DENS
          PGRAV(1,IMAT)=ARR(IMAT)
          PGRAV(2,IMAT)=ARR(NUMMAT+IMAT)
          PGRAV(3,IMAT)=ARR(2*NUMMAT+IMAT)
          PGRAV(4,IMAT)=ARR(3*NUMMAT+IMAT)
          PGRAV(5,IMAT)=ARR(4*NUMMAT+IMAT)
          PGRAV(6,IMAT)=ARR(5*NUMMAT+IMAT)
          PGRAV(7,IMAT)=ARR(6*NUMMAT+IMAT)
61      CONTINUE
C OMEGA,X1,Y1,Z1,X2,Y2,Z2
        IF (.NOT.LCOUP) THEN
          DO 62 IMAT=1,NUMMAT
            PSPIN(1,IMAT)=ARR(7*NUMMAT+IMAT)*9.5492966
            PSPIN(2,IMAT)=ARR(8*NUMMAT+IMAT)/SCALE9
            PSPIN(3,IMAT)=ARR(9*NUMMAT+IMAT)/SCALE9
            PSPIN(4,IMAT)=ARR(10*NUMMAT+IMAT)/SCALE9
            PSPIN(5,IMAT)=ARR(11*NUMMAT+IMAT)/SCALE9
            PSPIN(6,IMAT)=ARR(12*NUMMAT+IMAT)/SCALE9
            PSPIN(7,IMAT)=ARR(13*NUMMAT+IMAT)/SCALE9
62        CONTINUE
        ELSE
          J=7*NUMMAT
          DO 63 IMAT=1,MSPIN
            PSPIN(1,IMAT)=ARR(J+IMAT)*9.5492966
            PSPIN(2,IMAT)=ARR(J+MSPIN+IMAT)/SCALE9
            PSPIN(3,IMAT)=ARR(J+2*MSPIN+IMAT)/SCALE9
            PSPIN(4,IMAT)=ARR(J+3*MSPIN+IMAT)/SCALE9
            PSPIN(5,IMAT)=ARR(J+4*MSPIN+IMAT)/SCALE9
            PSPIN(6,IMAT)=ARR(J+5*MSPIN+IMAT)/SCALE9
            PSPIN(7,IMAT)=ARR(J+6*MSPIN+IMAT)/SCALE9
63        CONTINUE
        ENDIF
C MOLECULAR WEIGHTS
        IREC=IREC+1
        READ(LPOST,REC=IREC)ARR
        
C---- FLUID MATERIAL POINTER ARRAY
        NFLU=IARR(1)
        DO 66 IFLU=1,NFLU
          IMAT=IARR(1+IFLU)
          DO 66 ISC=0,NUMCON
C           ISCP=IMAT+NFLU*ISC
            ISCP=IMAT+NUMMAT*ISC
C---- BACKGROUND (ISC=0) AND SPECIES MOLECULAR WT.
            IF (ISC.EQ.0) THEN
              PMW(IMAT)=ARR(1+NFLU+ISCP)
            ELSE
              PMWS(ISC,IMAT)=ARR(1+NFLU+ISCP)
            ENDIF
66      CONTINUE
C READ ISKIP RECORD NUMBER 5 DENM, LAMVISM, CPHM, CONDM - VERSION 2268 + NU
C IF NOT SOLVING FOR ANY OTHER VARIABLE, THEN =0.
        NMAT=MAX(NUMMAT,NSMAT)
        IF (LVERS.GT.2267) THEN
          IREC=IREC+1
          READ(LPOST,REC=IREC)ARR
          DO 67 IMAT=1,NMAT
            DENM(IMAT)=ARR(IMAT)
            VISM(IMAT)=ARR(NMAT+IMAT)
            CPHM(IMAT)=ARR(2*NMAT+IMAT)
            CONDM(IMAT)=ARR(3*NMAT+IMAT)
C           TREFM(IMAT)=ARR(4*NMAT+IMAT)
67        CONTINUE
        ENDIF
      ELSE
C MAKE A DUMMY PSPIN FROM THE ROTA ARRAY
        DO 68 IMAT=1,NMAT
          ICCUR=IROTA(2,IMAT)
          PSPIN(1,IMAT)=ROTA(1,IMAT)
          PSPIN(2,IMAT)=CS0(1,ICCUR)
          PSPIN(3,IMAT)=CS0(2,ICCUR)
          PSPIN(4,IMAT)=CS0(3,ICCUR)
          PSPIN(5,IMAT)=0.
          PSPIN(6,IMAT)=0.
          PSPIN(7,IMAT)=1.
68      CONTINUE
      ENDIF
      MMM=MMM+NUMREC(-NBC,NCADD)+ISKIP
      MAXREC=MMM
C
C GET TWO LAYER DATA LOCATION
C
      IF (IRETW.GT.0) THEN
        NDATA(71,1)=MAXREC-IRTPL-IRETW
        IF (LVERS.GT.2267.AND.ITEMG)
     1      NDATA(71,1)=NDATA(71,1)-NUMREC(-NBC,NTCELL)
      ENDIF
C
C GET DROPLET DATA LOCATION (ADD NUMBER OF COMPONENTS)
C
      IF (NDR.GT.0) THEN
        NDREC=MAXREC-IRTPL
        IF (LVERS.GT.2267) THEN
          IF (ITEMG) NDREC=NDREC-NUMREC(-NBC,NTCELL)
          IF (LVOID) NDATA(20,1)=NDREC+(NDR-1)/146+1
        ENDIF
      ENDIF
C ENTHALPY DATA
      IF (ITEMG) NDATA(19,1)=MAXREC-NUMREC(-NBC,NTCELL)
C
C MOVING GRID DATA (ADD RECORDS FOR RESTART AND ASI)
C
      IF (LMVGRD) THEN
        IF (LVERS.LT.2115) THEN
          MMM=MMM+NUMREC(-NBW,NCELL)+NUMREC(-NBS,NCELL)+
     1            NUMREC(-NBB,NCELL)
          IF (NBCYC.GT.0) MMM=MMM+2*NUMREC(1,NBCYC)
          IF (NBCUT.GT.0) MMM=MMM+2*NUMREC(1,NBCUT)
C VERTEX COORDINATES DATA
          NDATA(1,2)=MMM
          NDATA(2,2)=NDATA(1,2)+NUMREC(1,NNODE)
          NDATA(3,2)=NDATA(2,2)+NUMREC(1,NNODE)
          MMM=NDATA(3,2)+NUMREC(1,NNODE)
          MAXREC=MMM
        ELSE
          MMM1=MMM+3*NUMREC(-NBC,NCELL)
          IF (LVERS.GT.2268) THEN
            MMM=MRECMM
          ELSE
            MMM=MMM1
            MRECMM=MMM1
          ENDIF
C VERTEX COORDINATES DATA
          NDATA(1,2)=MMM
          NDATA(2,2)=NDATA(1,2)+NUMREC(1,NNODE)
          NDATA(3,2)=NDATA(2,2)+NUMREC(1,NNODE)
          MMM=NDATA(3,2)+NUMREC(1,NNODE)
          IF (LVERS.GT.2152) THEN
            MMM=MMM+6*NUMREC(1,NCELL)+NUMREC(-NBC,NCELL)+
     1          2*NUMREC(1,NBPT)
            IF (NBCUTO.GT.0) MMM=MMM+2*NUMREC(1,2*NBCUTO)
            IF (NBCYC.GT.0) MMM=MMM+3*NUMREC(1,2*NBCYC)
C           MMM=MMM+1
          ENDIF
          MAXREC=MMM
        ENDIF
      ELSE
        NDATA(1,2)=0
        NDATA(2,2)=0
        NDATA(3,2)=0
      ENDIF
C NU CHANGES 2268 ARBITRARY MATCHES
C                 PRISM CELLS
C                 CONDITIONAL EVENTS
      IRASI=0
      IRPSM=0
      IRCND=0
      IF (LMVGRD.AND.LVERS.GE.2268) THEN
C NBPT SHOULD BE REPLACED BY NBP23
        MAXREC=MAXREC-2*NUMREC(1,NBPT)+2*NUMREC(1,NBP23)
C ASI DATA: IFASI,LSI,ICSRI,SBSI,CXSI
        IF (IAS) THEN
          IRASI=MAXREC+NUMREC(1,NBPT)
          MAXREC=MAXREC+NUMREC(1,NBPT)+2*NUMREC(1,2*NBSI)+
     &           NUMREC(1,3*NBSI)+NUMREC(1,6*NBSI)
        ENDIF
C OLD VOLUMES
        MAXREC=MAXREC+NUMREC(1,NCELL)
C
C PRISM COLLAPSE DATA
C
        IF (NWPRSM.EQ.1) THEN
          IF (LVERS.GE.2500) THEN
            IF (NPRSM.GT.0) THEN
              IRPSM=MAXREC
              MAXREC=MAXREC+(9*NPRSM-1)/2043+1
            ENDIF
          ELSE
C NCOLPS,ICOLTY AND ICOLPS
            MAXREC=MAXREC+2*NUMREC(1,NCLP)
            MAXREC=MAXREC+NUMREC(1,NCLPS)
C PRISM CELL DEFINITIONS
            IF (NPRSM.GT.0) THEN
              IRPSM=MAXREC
              MAXREC=MAXREC+(9*NPRSM-1)/2043+1
            ENDIF
          ENDIF
        ENDIF
      ENDIF
C 1 RECORD FOR CONDITIONAL EVENTS
      IF (LMVGRD) THEN
        IRCND=MAXREC
        MAXREC=MAXREC+1
      ENDIF
      NDATA(4,2)=0
      NDATA(5,2)=0
      NDATA(6,2)=0
      IF (WPOST) THEN
        NDATA(7,2)=MMM
        NDATA(8,2)=NDATA(7,2)+NUMREC(1,NNODE)
        NDATA(9,2)=NDATA(8,2)+NUMREC(1,NNODE)
        NDATA(10,2)=NDATA(9,2)+NUMREC(1,NNODE)
        NDATA(11,2)=NDATA(10,2)+NUMREC(1,NNODE)
        NDATA(12,2)=NDATA(11,2)+NUMREC(1,NNODE)
        NDATA(13,2)=NDATA(12,2)+NUMREC(1,NNODE)
        NDATA(14,2)=NDATA(13,2)+NUMREC(1,NNODE)
        NDATA(15,2)=NDATA(14,2)+NUMREC(1,NNODE)
        IF (LVERS.LT.2150) THEN
          NDATA(16,2)=0
          NDATA(17,2)=0
          NDATA(18,2)=0
          MMM=NDATA(15,2)
        ELSE
          NDATA(16,2)=NDATA(15,2)+NUMREC(1,NNODE)
          NDATA(17,2)=NDATA(16,2)+NUMREC(1,NNODE)
          NDATA(18,2)=NDATA(17,2)+NUMREC(1,NNODE)
          MMM=NDATA(18,2)
        ENDIF
        IF (NUMCON.GT.0) THEN
          DO 70 I=1,NUMCON
            MMM=MMM+NUMREC(1,NNODE)
            NDATA(20+I,2)=MMM
70        CONTINUE
        ENDIF
        IF (LSTRE) THEN
          MMM=MMM+NUMREC(1,NNODE)
          NDATA(19,2)=MMM
        ENDIF
        IF (LMACH) THEN
          MMM=MMM+NUMREC(1,NNODE)
          NDATA(20,2)=MMM
        ENDIF
        MMM=MMM+NUMREC(1,NNODE)
        MAXREC=MMM
      ENDIF
      IF (NDATA(1,2).NE.0) WPOST=.TRUE.
C
C WALL DATA
C
      IF (NSITEM.GT.0) THEN
        IF (LVERS.LT.2152) THEN
          NDATA(1,3)=MMM
          MMM=MMM+NUMREC(1,NBC)
        ENDIF
        IF (LVERS.GT.2114) THEN
          DO 90 I=1,162
            IF (LSRF(I)) THEN
              NDATA(1+I,3)=MMM
              MMM=MMM+NUMREC(1,NBC)
            ENDIF
90        CONTINUE
        ELSE
          DO 100 I=1,3
            IF (LSRF(I)) THEN
              NDATA(1+I,3)=MMM
              MMM=MMM+NUMREC(1,NBC)
            ENDIF
100       CONTINUE
          DO 110 I=7,10
            IF (LSRF(I)) THEN
              NDATA(1+I,3)=MMM
              MMM=MMM+NUMREC(1,NBC)
            ENDIF
110       CONTINUE
          DO 120 I=4,5
            IF (LSRF(I)) THEN
              NDATA(1+I,3)=MMM
              MMM=MMM+NUMREC(1,NBC)
            ENDIF
120       CONTINUE
          DO 130 I=58,61
            IF (LSRF(I)) THEN
              NDATA(1+I,3)=MMM
              MMM=MMM+NUMREC(1,NBC)
            ENDIF
130       CONTINUE
          IF (LSRF(6)) THEN
            NDATA(7,3)=MMM
            MMM=MMM+NUMREC(1,NBC)
          ENDIF
        ENDIF
        MAXREC=MMM
      ENDIF
      MAXREC=MAXREC+1
      READ(LPOST,REC=MAXREC,ERR=900)IARR
      IF (IARR(1).EQ.-1) THEN
        WRITE(LW,1017)TITLE,ITERS,TIMES,NCELL,NBC,NNODE
1017    FORMAT(/,' DATA ON FILE 9 FROM RUN TITLED:',/,1X,A,/
     1           ' ITERATION NO.     = ',I7,/,
     2           ' TIME              = ',G12.6,/,
     3           ' NO. OF CELLS      = ',I7,/,
     4           ' NO. OF BOUNDARIES = ',I7,/,
     5           ' MAX. VERTEX NO.   = ',I7,/)
        RETURN
      ENDIF
C
900   WRITE(LW,1005)LPOST
1005  FORMAT(' **POST FILE ',I2,' DOES NOT SEEM TO BE COMPLETE',/,
     1       '   SOME OR ALL THE DATA MAY BE CORRUPTED OR MISSING',/,
     2       '   PLEASE USE CAUTION WHEN LOOKING AT RESULTS')
      IERROR=1
      RETURN
      END
      FUNCTION NUMREC(N1,N2)
      NUMREC=(N2-N1)/2048+1
      RETURN
      END
      FUNCTION NUMRDP(N1,N2)
      NUMRDP=(N2-N1)/1024+1
      RETURN
      END
C
      SUBROUTINE TRLOAD(LREC,IERROR,trtype,trstep,trtime)
C
C LOAD AN ITERATION FROM THE TRANSIENT DATA FILE
C
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
      COMMON/POSTC/DENM(99),VISM(99),CPHM(99),CONDM(99)
      LOGICAL LSTAR,SPCYC,LCOUP
      LOGICAL IPRSG,ITEG,IEDG,ITEMG,IVISG,IDENG,IVILG,ICPOG,ICDOG,IAS
C
      EQUIVALENCE (LSTAR(4),IPRSG), (LSTAR(5),ITEG),  (LSTAR(6),IEDG),
     *            (LSTAR(7),ITEMG), (LSTAR(8),IVISG), (LSTAR(9),IDENG),
     *            (LSTAR(10),IVILG),(LSTAR(11),ICPOG),(LSTAR(12),ICDOG),
     *            (LSTAR(24),IAS)
      COMMON /CMDPND/ NDROP,NDROP9,NDREC
      COMMON/SCRT2/IARR(2048)
      COMMON/CASEN/CASE,TITLE
      DIMENSION ARR(2048),LARR(2048)
      LOGICAL LARR,WPOST,LSRF
      LOGICAL LCT(200),LVT(200),LMVGRD,LCNPST,LDPPRS
      CHARACTER ANS*1,CASE*70,FNAME*80,TITLE*52
      EQUIVALENCE (IARR,ARR,LARR)
chj
      character trtype*1 
      integer trstep
      double precision trtime

C
C PROMPT FOR ITERATION AND TIME
C
chj12    WRITE(LW,1900)
chj1900  FORMAT(
chj     1' DO YOU WANT TO LOAD THE FIRST TIME STEP, THE LAST TIME STEP,',/,
chj     2' A SPECIFIC TIME STEP, OR A SPECIFIC TIME INSTANT?',/,
chj     3' ENTER F, L, S, I')
chj      READ(LR,1906)ANS
chj1906  FORMAT(A)
      ans = trtype
chj
      CALL UPPER(ANS)
      IF (ANS.EQ.' ') ANS='F'
      IF (ANS.EQ.'F') THEN
        ITERU=-3
        TIMEU=-1.0
      ELSEIF (ANS.EQ.'L') THEN
        ITERU=0
        TIMEU=-1.0
      ELSEIF (ANS.EQ.'S') THEN
chj        WRITE(LW,1910)
chj1910    FORMAT(' ENTER THE TIME STEP')
chj        READ(LR,*)ITERU
         iteru=trstep
chj
        TIMEU=-1.0
      ELSEIF (ANS.EQ.'I') THEN
chj        WRITE(LW,1930)
chj1930    FORMAT(' ENTER THE TIME INSTANT')
chj        READ(LR,*)TIMEU
         timeu=real(trtime)
        ITERU=-1
      ELSE
chj        GO TO 12
         go to 900
      ENDIF
C
C FIRST OPEN THIS UNIT CORRECTLY
C
      DO 1 I=1,200
        NDATA(I,1)=0
        NDATA(I,2)=0
        NDATA(I,3)=0
        LCT(I)=.FALSE.
        LVT(I)=.FALSE.
        LSRF(I)=.FALSE.
1     CONTINUE
      DO 5 I=1,99
        PMW(I)=0.
        DO 2 J=1,7
          PGRAV(J,I)=0.
          PSPIN(J,I)=0.
2       CONTINUE
        DO 3 J=1,50
3         PMWS(J,I)=0.
5     CONTINUE
      NDROP9=0
      NDREC=0
      SCALE9=1.
C
      CALL STRTRM(CASE,CASE,LCASE)
      FNAME=CASE(1:LCASE)//'29'
      OPEN(LPOST,FILE=FNAME,STATUS='OLD',FORM='UNFORMATTED',
     1     RECL=LREC,ACCESS='DIRECT',ERR=900)
C
      MAXREC=0
10    MAXREC=MAXREC+1
      LDPPRS=.FALSE.
      READ(LPOST,REC=MAXREC,ERR=900)IARR
C
C FOUND END OF FILE
C
      IF (IARR(1).EQ.-1) THEN
        IF (ITERU.EQ.0) THEN
            WRITE(LW,1001)ITER,TIME
1001        FORMAT(' LAST TIME STEP FOUND IS =',I6,' TIME=',G12.6)
            WRITE(LW,1008)
1008        FORMAT(' PRESSURE REFERENCES PER MATERIAL NUMBER ADDED TO',
     1             ' FIELD DATA ARE:')
            WRITE(LW,1009)(J,PREPP(1,J),J=1,NUMMAT)
1009        FORMAT(5(I3,1X,G12.5))
            WRITE(LW,1010)
            WRITE(LW,1009)(J,PREPP(2,J),J=1,NUMMAT)
1010        FORMAT(' LOADED PRESSURES (Pa) PER MATERIAL NO. ARE',
     1             ' RELATIVE TO:')
            WRITE(LW,1011)
1011        FORMAT(' LOADED TEMPERATURES (K) PER MATERIAL NO. ARE',
     1             ' RELATIVE TO:')
            NMAT=MAX(NUMMAT,NSMAT)
            WRITE(LW,1009)(J,TREFM(J),J=1,NMAT)
          ITERS=-ITER
          TIMES=TIME
        ELSE
          WRITE(LW,1002)
1002      FORMAT(' **USER REQUESTED TIME STEP OR TIME NOT FOUND')
        ENDIF
        RETURN
      ENDIF
C
C FOUND AN ITERATION
C
      ITER=IARR(1)
      TIME=ARR(2)
      NCELL=IARR(3)
      NBC=IARR(4)
      NBW=IARR(5)
      NBS=IARR(6)
      NBB=IARR(7)
      NNODE=IARR(8)
      WRITE(TITLE,1003)(IARR(J),J=9,21)
1003  FORMAT(13A4)
      NBCYC=IARR(22)
      NBCUT=IARR(23)
      NUMCON=IARR(25)
      LVERS=IARR(26)
      IF (LVERS.GT.0) THEN
        WRITE(LW,1007)LPOST
1007    FORMAT(' **FILE',I3,' IS PROBABLY A RESTART FILE',/,
     1         '   USE THE LOAD PROCEDURE TO READ THIS FILE')
        IERROR=1
        RETURN
      ENDIF
      LVERS=IABS(LVERS)
      SCALE9=ARR(85)
      IF (SCALE9.NE.SCALE8) THEN
        WRITE(LW,1012)LPOST,SCALE9,SCALE8
1012    FORMAT(' **NOTE - THE GEOMETRY SCALE FACTOR FROM FILE',I3,
     1  ' IS ',G13.6,/,' AND IS NOT EQUAL TO THE FACTOR SAVED ON',
     2  ' FILE 16  WHICH IS ',G13.6)
      ENDIF
      NSOL=IARR(24)
      NDROP9=IARR(27)
      NDROP9=IARR(28)
      NCITEM=IARR(29)
      NVITEM=IARR(30)
      IF (NVITEM.GT.0) THEN
        WPOST=.TRUE.
      ELSE
        WPOST=.FALSE.
      ENDIF
      NSITEM=IARR(31)
      LMVGRD=LARR(32)
      LCT(1)=LARR(74)
      LCT(2)=LARR(74)
      LCT(3)=LARR(74)
      LCT(4)=LARR(74)
      LCT(5)=LARR(74)
      LCT(6)=LARR(74)
C U,V,W,P,TE,ED,VS,T,DENS
      DO 20 I=1,9
        LCT(I+6)=LARR(I+32)
20    CONTINUE
C SCALARS 1-3
      DO 25 I=1,3
        LCT(I+20)=LARR(I+41)
25    CONTINUE
C LAMVIS,CP,COND
      LCT(16)=LARR(91)
      LCT(17)=LARR(92)
      LCT(18)=LARR(93)
C ENTHALPY, VOID FRACTION, FLUX
      LCT(19)=LARR(40)
      LCT(20)=LARR(1346)
C FX,FY,FZ
      LSRF(1)=LARR(57)
      LSRF(2)=LARR(58)
      LSRF(3)=LARR(59)
C HTR,TEMP,MTRAN1-3
      LSRF(6)=LARR(60)
      LSRF(7)=LARR(60)
      LSRF(8)=LARR(61)
      LSRF(9)=LARR(62)
      LSRF(10)=LARR(63)
      LSRF(113)=LSRF(8)
      LSRF(114)=LSRF(9)
      LSRF(115)=LSRF(10)
C YPLUS,YNORM
      LSRF(4)=LARR(64)
      LSRF(5)=LARR(65)
C HFLUX,MFLUX1-3
      LSRF(58)=LARR(66)
      LSRF(59)=LARR(67)
      LSRF(60)=LARR(68)
      LSRF(61)=LARR(69)
C HRAD
      LSRF(109)=LARR(72)
      LSRF(110)=LARR(72)
C HSOL
      LSRF(111)=LARR(73)
      LSRF(112)=LARR(73)
C SCALARS 4,50
      DO 30 I=4,50
        LCT(I+20)=LARR(I+97)
30    CONTINUE
      DO 31 I=1,100
        LSTAR(I)=LARR(I+1800)
31    CONTINUE
      ITYPEN=IARR(1915)
      LCOUP=LARR(1916)
      MSPIN=IARR(1917)
C MOVING MESH DATA
      IF (LMVGRD) THEN
        NWPRSM=IARR(1911)
        NPRSM =IARR(1912)
        NBSI  =IARR(1913)
        IAS   =LARR(1914)
      ELSE
        NWPRSM=0
        NPRSM =0
        NBSI  =0
        IAS   =.FALSE.
      ENDIF
C SCALAR WALL DATA - MTRANS,MFLUX,MCOEF
      DO 40 I=4,50
        LSRF(I+7)=LARR(I+997)
        LSRF(I+58)=LARR(I+1044)
        LSRF(I+112)=LSRF(I+7)
40    CONTINUE
      PREF=0.
C POST FILE GE 2014
      NTCELL=IARR(99)
      NUMMAT=IARR(200)
      NSMAT=IARR(201)
      DO 50 I=1,NUMMAT
        I2=I+NUMMAT
        I3=I+2*NUMMAT
        PREPP(1,I)=ARR(201+I)
        PREPP(2,I)=ARR(201+I2)
        TREFM(I)=ARR(201+I3)
50    CONTINUE
      DO 55 ISOL=1,NSOL
        IMAT=IARR(201+6*NUMMAT+NSMAT+ISOL)
        I=IMAT+NSMAT+1
        TREFM(-IMAT)=ARR(201+6*NUMMAT+I)
55    CONTINUE
      NMAT=MAX(NUMMAT,NSMAT)
      IF (NTCELL.GT.NCELL) THEN
        LCNPST=.TRUE.
      ELSE
        LCNPST=.FALSE.
      ENDIF
      IF (.NOT.(ITERU.EQ.-2.AND.ITER.LE.IABS(ITERS))) THEN
        WRITE(LW,1006)TITLE
1006    FORMAT(' DATA FILE TITLE:',/,1X,A)
        WRITE(LW,1004)ITER,TIME
1004    FORMAT(' FOUND TIME STEP NO.',I6,' TIME=',G12.6)
      ENDIF
C
C SET UP ARRAY TO DEFINE WHERE EACH VARIABLE BEGINS
C (X,Y,Z)
C (U,V,W,P,TE,ED,VIS,T,DENS,LAMV,CP,COND,SCN AT CELLS)
C (FOX,FOY,FOZ,HFILM,YPLUS,...AT WALLS)
C
        NDATA(1,3)=MAXREC
        MAXREC=MAXREC+NUMREC(1,NBC)
C GET 2ND SET OF MATERIAL PROPERTIES (ACCELERATIONS AND SPIN)
        IREC=MAXREC+1
        READ(LPOST,REC=IREC)ARR
        DO 65 IMAT=1,NUMMAT
C GX,GY,GZ,XG,YG,ZG,DENS
          PGRAV(1,IMAT)=ARR(IMAT)
          PGRAV(2,IMAT)=ARR(NUMMAT+IMAT)
          PGRAV(3,IMAT)=ARR(2*NUMMAT+IMAT)
          PGRAV(4,IMAT)=ARR(3*NUMMAT+IMAT)
          PGRAV(5,IMAT)=ARR(4*NUMMAT+IMAT)
          PGRAV(6,IMAT)=ARR(5*NUMMAT+IMAT)
          PGRAV(7,IMAT)=ARR(6*NUMMAT+IMAT)
65      CONTINUE
C OMEGA,X1,Y1,Z1,X2,Y2,Z2
        IF (.NOT.LCOUP) THEN
          DO 62 IMAT=1,NUMMAT
            PSPIN(1,IMAT)=ARR(7*NUMMAT+IMAT)*9.5492966
            PSPIN(2,IMAT)=ARR(8*NUMMAT+IMAT)/SCALE9
            PSPIN(3,IMAT)=ARR(9*NUMMAT+IMAT)/SCALE9
            PSPIN(4,IMAT)=ARR(10*NUMMAT+IMAT)/SCALE9
            PSPIN(5,IMAT)=ARR(11*NUMMAT+IMAT)/SCALE9
            PSPIN(6,IMAT)=ARR(12*NUMMAT+IMAT)/SCALE9
            PSPIN(7,IMAT)=ARR(13*NUMMAT+IMAT)/SCALE9
62        CONTINUE
        ELSE
          J=7*NUMMAT
          DO 63 IMAT=1,MSPIN
            PSPIN(1,IMAT)=ARR(J+IMAT)*9.5492966
            PSPIN(2,IMAT)=ARR(J+MSPIN+IMAT)/SCALE9
            PSPIN(3,IMAT)=ARR(J+2*MSPIN+IMAT)/SCALE9
            PSPIN(4,IMAT)=ARR(J+3*MSPIN+IMAT)/SCALE9
            PSPIN(5,IMAT)=ARR(J+4*MSPIN+IMAT)/SCALE9
            PSPIN(6,IMAT)=ARR(J+5*MSPIN+IMAT)/SCALE9
            PSPIN(7,IMAT)=ARR(J+6*MSPIN+IMAT)/SCALE9
63        CONTINUE
        ENDIF
C MOLECULAR WEIGHTS
        IREC=IREC+1
        MAXREC=IREC
        READ(LPOST,REC=IREC)ARR
C FLUID MATERIAL POINTER ARRAY
        NFLU=IARR(1)
        DO 66 IFLU=1,NFLU
          IMAT=IARR(1+NFLU)
          DO 66 ISC=0,NUMCON
            ISCP=IMAT+NFLU*ISC
C BACKGROUND (ISC=0) AND SPECIES MOLECULAR WT.
            IF (ISC.EQ.0) THEN
              PMW(IMAT)=ARR(1+NFLU+ISCP)
            ELSE
              PMWS(ISC,IMAT)=ARR(1+NFLU+ISCP)
            ENDIF
66      CONTINUE
C - ADDITIONAL CONSTANT VALUES
        MAXREC=MAXREC+1
        READ(LPOST,REC=MAXREC)ARR
        DO 67 IMAT=1,NMAT
          DENM(IMAT)=ARR(IMAT)
          VISM(IMAT)=ARR(NMAT+IMAT)
          CPHM(IMAT)=ARR(2*NMAT+IMAT)
          CONDM(IMAT)=ARR(3*NMAT+IMAT)
67      CONTINUE
      IF (LMVGRD) THEN
        NDATA(1,2)=MAXREC
        MAXREC=MAXREC+NUMREC(1,NNODE)
        NDATA(2,2)=MAXREC
        MAXREC=MAXREC+NUMREC(1,NNODE)
        NDATA(3,2)=MAXREC
        MAXREC=MAXREC+NUMREC(1,NNODE)
      ELSE
        NDATA(1,2)=0
        NDATA(2,2)=0
        NDATA(3,2)=0
      ENDIF
      NDATA(4,2)=0
      NDATA(5,2)=0
      NDATA(6,2)=0
      DO 90 I=1,6
        IF (LCT(I)) THEN
          NDATA(I,1)=MAXREC
          MAXREC=MAXREC+NUMREC(1,NCELL)
        ENDIF
90    CONTINUE
      DO 100 I=7,70
        IF (LCT(I)) THEN
          NDATA(I,1)=MAXREC
          IF (I.EQ.14.OR.I.EQ.17.OR.I.EQ.18.OR.I.EQ.19) THEN
            MAXREC=MAXREC+NUMREC(-NBC,NTCELL)
          ELSEIF (I.EQ.20) THEN
            MAXREC=MAXREC+NUMREC(1,NCELL)
          ELSE
            MAXREC=MAXREC+NUMREC(-NBC,NCELL)
          ENDIF
        ELSE
          NDATA(I,1)=0
        ENDIF
100   CONTINUE
      DO 110 I=7,70
        IF (LVT(I)) THEN
          NDATA(I,2)=MAXREC
          MAXREC=MAXREC+NUMREC(1,NNODE)
        ELSE
          NDATA(I,2)=0
        ENDIF
110   CONTINUE
      IF (NSITEM.GT.0) THEN
        DO 120 I=1,162
          IF (LSRF(I)) THEN
            NDATA(I+1,3)=MAXREC
            MAXREC=MAXREC+NUMREC(1,NBC)
          ELSE
            NDATA(I+1,3)=0
          ENDIF
120     CONTINUE
      ELSE
        DO 140 I=1,162
          NDATA(I+1,3)=0
140     CONTINUE
      ENDIF
C DROP LOCATION
      IF (NDROP9.GT.0) THEN
        NDREC=MAXREC
        MAXREC=MAXREC+(NDROP9-1)/146+1
      ENDIF
C ARBITRARY MATCHES
C PRISM CELLS
C CONDITIONAL EVENTS 
      IRASI=0
      IRPSM=0
      IRCND=0
      IF (IAS) THEN
        IRASI=MAXREC
        MAXREC=MAXREC+NUMREC(1,2*NBSI)
      ENDIF
      IF (NWPRSM.EQ.1) THEN
        IF (NPRSM.GT.0) THEN
          IRPSM=MAXREC
          MAXREC=MAXREC+(9*NPRSM-1)/2043+1
        ENDIF
      ENDIF
C 1 RECORD FOR CONDITIONAL EVENTS
      IF (LMVGRD) THEN
        IRCND=MAXREC
        MAXREC=MAXREC+1
      ENDIF
C OK
      IF (ITER.EQ.ITERU.OR.ABS(TIME-TIMEU)/TIME.LT.1.E-5.OR.
     1    (ITERU.EQ.-3.OR.(ITERU.EQ.-2.AND.ITER.GT.IABS(ITERS)))) THEN
        ITERS=-ITER
        TIMES=TIME
        RETURN
      ENDIF
      GO TO 10
C FAIL
900   WRITE(LW,1005)LPOST
1005  FORMAT(' **ERROR OPENING OR READING UNIT NO.',I3)
      IERROR=1
      RETURN
      END
C
      SUBROUTINE CELGET(ITEM,NCPDIM,ICOUP,pans,tans)
C
C GET SPECIFIC CELL DATA SET
C
      COMMON/ARSIZE/MAXCEL,MAXVRT,MAXNCP,MAXTAB,MAXNBU,MAXREG,MAXPST
      COMMON/KEY/MAXN,MAXE,ICSYS,MAXSYS,ITYPE,PFIRST,IVERS,NUMW,
     1           NI,NJ,NK,NF,MAXB,IBFILL,NOVICE,KEYS(10),LW,LR,
     2           LECHO,LNUT,KEYS2(7),MAXCP,KEYS3(17)
      COMMON/CELLS/NE(9,1)
      COMMON/CTABLE/ICTAB(10,1)
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
      COMMON/PDATA/VECTOR(14,1)
      COMMON/POSTC/DENM(99),VISM(99),CPHM(99),CONDM(99)
      COMMON/SCRT2/IARR(2048),IDUM2(1)
      COMMON/SCRT3/NCMAP(1)
      DIMENSION DTYPE(15),ICOUP(NCPDIM,1)
      LOGICAL ADRELP,ADRELT,LDPPRS
      LOGICAL LSTAR,SPCYC
      CHARACTER ANS*1,DTYPE*4,ITEM*4
      character pans*1, tans*1
      DATA DTYPE/'FLUX',' U  ',' V  ',' W  ',' P  ',' TE ',' ED ',
     1           'VIS ',' T  ','DENS','LAMV',' CP ','COND','ENTH',
     2           'CONC'/
C
C MAKE THE PROSTAR TO STAR CELL MAP
C
      CALL STRMAP(NCMAP,IDUM2,MAXE,.TRUE.,MAXCP,NCPDIM,ICOUP)
C
C ROW NUMBER OF CELL DATA VECTOR
      NROW=14
C INITIALIZE VECTOR
      DO 10 I=1,NCELL
        DO 10 J=1,NROW
10        VECTOR(J,I)=0.
C
C STORE FLUXES (1-6)
C
      IF (ITEM.EQ.'FLUX') THEN
        DO 5 I=1,6
          NREC=NDATA(I,1)
          IF (NREC.GT.0) THEN
            CALL VECCLR(VECTOR,I,14,MAXE)
            CALL LDDATA(NREC,VECTOR,I,14,1,NCELL,.TRUE.,NCMAP)
          ELSE
            WRITE(LW,1006)
1006        FORMAT(' **ERROR - ALL 6 FLUXES ARE NOT ON FILE')
c           INPS(1) = -1
            LDFLUX=0
            RETURN
          ENDIF
5       CONTINUE
        DO 20 I=1,MAXE
          IF (NE(1,I).GT.0) THEN
            ICTYP=ICTAB(1,NE(9,I))
            IF (ICTYP.EQ.1) THEN
              VECTOR(1,I)=-VECTOR(1,I)
              VECTOR(3,I)=-VECTOR(3,I)
              VECTOR(5,I)=-VECTOR(5,I)
              DO 15 IR=1,6
                SMAG=VECTOR(IR,I)
                IF (SMAG.GT.SMAXG) SMAXG=SMAG
                IF (SMAG.LT.SMING) SMING=SMAG
15            CONTINUE
            ENDIF
          ENDIF
20      CONTINUE
      ELSEIF (ITEM.EQ.'SCAL') THEN
C
C CREATE "BOUNDARY" SHELL LAYERS
C
        CALL WALSHL(NCMAP,NCPDIM,ICOUP)
C OR
C STORE SCALAR DATA: U,V,W,P,TE,ED,VIS,T,DENS,LAMV,CP,COND
C OR
C STORE CONCENTRATIONS (UP TO 14 AT A TIME)
C
C U, V, W
C
        DO 40 I=1,3
          NREC=NDATA(I+6,1)
          IF (NREC.GT.0) THEN
            CALL VECCLR(VECTOR,I,NROW,MAXE)
            CALL LDDATA(NREC,VECTOR,I,NROW,-NBC,NCELL,.TRUE.,NCMAP)
          ELSE
            WRITE(LW,1005)DTYPE(I+1)
1005        FORMAT(' **WARNING - CELL VARIABLE ',A,' NOT ON DATA FILE')
          ENDIF
40      CONTINUE
C
C PRESSURE (P) IS CONVERTED FROM DOUBLE PRECISION 
C
        NREC=NDATA(10,1)
        IF (NREC.GT.0) THEN
chj50        WRITE(LW,1015)
chj1015      FORMAT(' DO YOU WANT ABSOLUTE OR RELATIVE PRESSURE?',
chj     *           ' (ENTER A OR R)')
chj          READ(LR,1016)ANS
chj1016      FORMAT(A)
           ans=pans
          CALL UPPER(ANS)
          IF (ANS.EQ.' ') ANS='R'
          IF (ANS.EQ.'R') THEN
            ADRELP=.FALSE.
          ELSEIF (ANS.EQ.'A') THEN
            ADRELP=.TRUE.
chj          ELSE
chj            GO TO 50
          ENDIF
C
          CALL VECCLR(VECTOR,4,NROW,MAXE)
          IF (LDPPRS) THEN
            CALL LDPDAT(NREC,VECTOR,4,NROW,-NBC,NCELL,.TRUE.,NCMAP)
          ELSE
            CALL LDDATA(NREC,VECTOR,4,NROW,-NBC,NCELL,.TRUE.,NCMAP)
          ENDIF
          DO 100 I=1,MAXE
            IF (NE(1,I).LE.0) GO TO 100
            ICTID=NE(9,I)
            IF (ICTAB(1,ICTID).EQ.1) THEN
              VECTOR(4,I)=VECTOR(4,I)+PREPP(1,ICTAB(4,ICTID))
            ELSEIF (I.GE.ICSH1) THEN
              ICTID=NE(9,NE(7,I))
              VECTOR(4,I)=VECTOR(4,I)+PREPP(1,ICTAB(4,ICTID))
            ENDIF
100       CONTINUE
          IF (ADRELP) THEN
            DO 110 I=1,MAXE
              IF (NE(1,I).LE.0) GO TO 110
              ICTID=NE(9,I)
              IF (ICTAB(1,ICTID).EQ.1) THEN
                VECTOR(4,I)=VECTOR(4,I)+PREPP(2,ICTAB(4,ICTID))
              ELSEIF (I.GE.ICSH1) THEN
                ICTID=NE(9,NE(7,I))
                VECTOR(4,I)=VECTOR(4,I)+PREPP(2,ICTAB(4,ICTID))
              ENDIF
110         CONTINUE
          ENDIF
        ELSE
          WRITE(LW,1005)DTYPE(5)
        ENDIF
C
C TE, ED, VIS 
C
        DO 120 I=5,7
          NREC=NDATA(I+6,1)
          IF (NREC.GT.0) THEN
            CALL VECCLR(VECTOR,I,NROW,MAXE)
            CALL LDDATA(NREC,VECTOR,I,NROW,-NBC,NCELL,.TRUE.,NCMAP)
          ELSE
            WRITE(LW,1005)DTYPE(I+1)
          ENDIF
120     CONTINUE
C
C IF T, STORE FROM NBC TO NTCELL INSTEAD OF NBC TO NCELL 
C
        NREC=NDATA(14,1)
        CALL VECCLR(VECTOR,8,NROW,MAXE)
chj125     WRITE(LW,1025)
chj1025    FORMAT(' DO YOU WANT ABSOLUTE OR RELATIVE T? (ENTER A OR R)')
chj        READ(LR,1016)ANS
        ans=tans
        CALL UPPER(ANS)
        IF (ANS.EQ.' ') ANS='R'
        IF (ANS.EQ.'R') THEN
          ADRELT=.FALSE.
        ELSEIF (ANS.EQ.'A') THEN
          ADRELT=.TRUE.
chj        ELSE
chj          GO TO 125
        ENDIF
        IF (NREC.GT.0) THEN
C
          CALL LDDATA(NREC,VECTOR,8,NROW,-NBC,NTCELL,.TRUE.,NCMAP)
          IF (ADRELT) THEN
            DO 130 I=1,MAXE
              IF (NE(1,I).LE.0) GO TO 130
              ICTID=NE(9,I)
              IF (ICTAB(1,ICTID).LE.2) THEN
                VECTOR(8,I)=VECTOR(8,I)+TREFM(ICTAB(4,ICTID))
              ELSEIF (I.GE.ICSH1) THEN
                ICTID=NE(9,NE(7,I))
                VECTOR(8,I)=VECTOR(8,I)+TREFM(ICTAB(4,ICTID))
              ENDIF
130         CONTINUE
          ENDIF
        ELSE
          IF (ADRELT) THEN
            DO 131 I=1,MAXE
              IF (NE(1,I).LT.1) GO TO 131
              ICTID=NE(9,I)
              IF (ICTAB(1,ICTID).LE.2) THEN
                VECTOR(8,I)=TREFM(ICTAB(4,ICTID))
              ELSEIF (I.GE.ICSH1) THEN
                ICTID=NE(9,NE(7,I))
                VECTOR(8,I)=TREFM(ICTAB(4,ICTID))
              ENDIF
131         CONTINUE
          ENDIF
        ENDIF
C
C DENS, LAMV: USE NCELL
C CP, COND: USE NTCELL
C
        DO 140 I=9,12
          NREC=NDATA(I+6,1)
          IF (NREC.GT.0) THEN
            CALL VECCLR(VECTOR,I,NROW,MAXE)
            IF (I.LT.11) THEN
              CALL LDDATA(NREC,VECTOR,I,NROW,-NBC,NCELL,.TRUE.,NCMAP)
            ELSE
              CALL LDDATA(NREC,VECTOR,I,NROW,-NBC,NTCELL,.TRUE.,NCMAP)
            ENDIF
          ELSE
            CALL VECCLR(VECTOR,I,NROW,MAXE)
            IF (I.EQ.9) THEN
              CALL VECNST(VECTOR,I,NROW,DENM,1,MAXE,ICSH1)
            ELSEIF (I.EQ.10) THEN
              CALL VECNST(VECTOR,I,NROW,VISM,1,MAXE,ICSH1)
            ELSEIF (I.EQ.11) THEN
              CALL VECNST(VECTOR,I,NROW,CPHM,1,MAXE,ICSH1)
            ELSEIF (I.EQ.12) THEN
              CALL VECNST(VECTOR,I,NROW,CONDM,1,MAXE,ICSH1)
            ENDIF
          ENDIF
140     CONTINUE
C
C CONCENTRATIONS
C
      ELSEIF (ITEM.EQ.'CONC') THEN
        I=13
        IF (NUMCON.GT.0) THEN
          WRITE(LW,1035) NUMCON
1035      FORMAT(' CHOOSE THE CONCENTRATION:',
     1           ' (AN INTEGER BETWEEN 1 AND ',I3,' )')
          READ(LR,*)NSC
C
          IF (NSC.LT.1) NSC=1
          IF (NSC.GT.NUMCON) NSC=NUMCON
          NREC=NDATA(20+NSC,1)
          IF (NREC.GT.0) THEN
            CALL VECCLR(VECTOR,I,NROW,MAXE)
            CALL LDDATA(NREC,VECTOR,I,NROW,-NBC,NCELL,.TRUE.,NCMAP)
          ELSE
            WRITE(LW,1005)DTYPE(I+1)
          ENDIF
        ELSE
          WRITE(LW,1005)DTYPE(I+1)
        ENDIF
      ENDIF
C
      RETURN
      END
      SUBROUTINE WALSHL(NCMAP,NCPDIM,ICOUP)
      COMMON/ARSIZE/MAXCEL,MAXVRT,MAXNCP,MAXTAB,MAXNBU,MAXREG,MAXPST
C
C CREATE WALL SHELLS
C
      COMMON/KEY/MAXN,MAXE,ICSYS,MAXSYS,ITYPE,PFIRST,IVERS,NUMW,
     1           NI,NJ,NK,NF,MAXB,IBFILL,NOVICE,KEYS(10),LW,LR,
     2           LECHO,LNUT,MXTB,KEYS2(6),MAXCP,KEYS3(17)
      COMMON/NEWKEY/NCTOT,NCYTOT,ITIMFL,MOVGR,IBDY,DT,MAXCOR,RESOC,
     1           URFPCR,IWR1,IWR2,IPRI,IPST,LSIMP,MOVWAL(6),LVECT,
     2           LRADI,NCOTOT,LSOLAR,LCPUTI,MAXPOR,MAXMAT,URFDPW,
     3           URFRAD,LCONJ,LSWTCH(100),RSWTCH(100),MAXLOC,NRFRQ,
     4           ITOPT(4),SWBL,NBEAM,COURNT,LSTREM,LMACH,LMFRAM,
     5           URFBNP,URFBNU,IUPDAT,ITBEG,IGPREF,ITGRP,URFLUX,
     6           NKEY(11)
      COMMON/CELLS/NE(9,1)
      COMMON/NODES/C(3,1)
      COMMON/CTABLE/ICTAB(10,1)
      COMMON/NTABLE/CTNAME(1)
      DIMENSION ICOUP(NCPDIM,1)
      COMMON/POSTD/ITERU,TIMEU,ITERS,TIMES,VMING,VMAXG,VMINL,VMAXL,
     1             SMING,SMAXG,SMINL,SMAXL,NDATA(200,3),LPOST,
     2             NCELL,NBC,NBW,NBS,NBB,NNODE,WPOST,LDCELL,LDVERT,
     3             LSORT,LUNMOD,IPSYS,LOPER,LRSKIP,TREF,LDSURF,
     4             ISRTAB,ICSH1,NSITEM,LSRF(200),NBCUT,NBCYC,PREF,
     5             PREPP(2,99),NTCELL,LCNPST,LDPPRS,
     6             LPCORD,NUMCON,NSC,TREFM(99),LVERS,LDFLUX,
     7             IREFRM,IRESYS,OMEREF,ADDREL,PGRAV(7,99),PSPIN(7,99),
     8             PMW(99),PMWS(50,99),SCALE9,LPTYP,LADJST,LPINTP,TIMEP,
     9             ITERP,LRELA,LMVGR,NDATAP(200,3),IRETW,NWSET,SPCYC,
     *             LSTAR(100),NBNCST,NBNCEN,NBSI,NSCL,NREG,
     1             MRECMM,NBP23,NCLPS,NWPRSM,NPRSM,NCLP,NCLPF,ITYPEN
      LOGICAL LSTAR,SPCYC
      COMMON/POSTC/DENM(99),VISM(99),CPHM(99),CONDM(99)
      COMMON/PLTKEY/NPEN,ICSCAL,VMNPEN,VMXPEN,PENINC,IPLVEC(3),IPLSCL,
     1              VECSCL,JPLTYP,VRANGE,SNORM(3),PMESH,MESHPN,THIN,
     2              MNUM,UGRDON,NPSX,NPSY,BLACKB,IPLSYS,IPLSRF,VISRF,
     3              NMTIC(3),LTRNSL,LPREVE
      COMMON/PSTLAB/SCALAB(30),VECLAB(3),SRFLAB(25),FORLAB(6)
      CHARACTER*4 SCALAB,VECLAB,SRFLAB,FORLAB
      COMMON/PDATA/VECTOR(14,1)
      COMMON/SCRT1/ISLIST(1)
      COMMON/SCRT2/ARR(2048),IDUM2(1)
      DIMENSION NCMAP(MAXCEL)
      DIMENSION IPLUSE(3),NVEL(3),IFACE(4,6)
      LOGICAL WPOST,LDCELL,LDVERT,LSORT,LUNMOD,LOPER,LSRF,LDSURF
      LOGICAL LRELA
      LOGICAL LSWTCH,LMREF
      LOGICAL NOVICE,LOADAL,OPLOAD,ADDREL,LDPPRS
      LOGICAL*1 LCP(8,1)
      LOGICAL ADRELP,ADRELT
      CHARACTER*4 ITEM,CTNAME
      EQUIVALENCE (IDUM2,LCP)
      DATA IFACE/1,2,3,4, 5,6,7,8, 2,1,5,6, 3,4,8,7, 1,4,8,5,
     1           2,3,7,6/
C
51    IF (ISRTAB.GT.0) GO TO 100
      MAXEO=MAXE
      NREC=NDATA(1,3)
      CALL LDDATA(NREC,ISLIST,1,1,1,NBC,.FALSE.,NCMAP)
      ISRTAB=MXTB+4
      IF (ISRTAB.GT.MAXTAB) THEN
        WRITE(LW,1004)MAXTAB
1004    FORMAT(' **WARNING - THE NUMBER OF CELL TABLE ENTRIES IS',
     1         ' COMPLETELY FILLED.',/,'   INCREASE PARAMETER',
     2         ' MAXTAB TO AT LEAST',I7,' AND RECOMPILE PROSTAR.')
        ISRTAB=0
        RETURN
      ENDIF
      ICTAB(1,ISRTAB-3)=4
      ICTAB(2,ISRTAB-3)=6
      ICTAB(3,ISRTAB-3)=0
      CTNAME(ISRTAB-3)='SYMM'
      ICTAB(1,ISRTAB-2)=4
      ICTAB(2,ISRTAB-2)=7
      ICTAB(3,ISRTAB-2)=0
      CTNAME(ISRTAB-2)='WALL'
      ICTAB(1,ISRTAB-1)=4
      ICTAB(2,ISRTAB-1)=8
      ICTAB(3,ISRTAB-1)=0
      CTNAME(ISRTAB-1)='NONW'
      ICTAB(1,ISRTAB)=4
      ICTAB(2,ISRTAB)=9
      ICTAB(3,ISRTAB)=0
      CTNAME(ISRTAB)='PART'
C
C MAKE THE PROSTAR TO STAR CELL MAP
C
      CALL STRMAP(NCMAP,IDUM2,MAXE,.TRUE.,MAXCP,NCPDIM,ICOUP)
C
      I2=0
      IPART=0
      IC=MAXE
      DO 59 IP=1,MAXE
        DO 59 J=1,8
59        LCP(J,IP)=.FALSE.
      DO 60 IP=1,MAXCP
        DO 55 J=1,NCPDIM
          IVAL=ICOUP(J,IP)
          IF (IVAL.LT.1) GO TO 60
          NF=MOD(IVAL,10)
          ICA=IVAL/10
          LCP(NF,ICA)=.TRUE.
55        CONTINUE
60      CONTINUE
      DO 70 I=1,NBC
        IPOS=I
        IF (ISLIST(IPOS).LT.1) GO TO 70
        I2=I2+1
        IC=IC+1
        NCMAP(IC)=MAXE+IPOS
        IF (IC.GT.MAXCEL) THEN
          WRITE(LW,1003)MAXE+NBC
1003      FORMAT(' **WARNING - THE NUMBER OF CURRENT CELLS PLUS WALL',
     1           ' BOUNDARIES EXCEEDS ALLOWABLE STORAGE.',/,
     2           '   INCREASE PARAMETER MAXCEL TO AT LEAST',I7,
     3           ' AND RECOMPILE PROSTAR.')
          RETURN
        ENDIF
        ICELL=ISLIST(IPOS)/7
        ICA=NCMAP(ICELL)
        NFACE=MOD(ISLIST(IPOS),7)
        NE(1,IC)=NE(IFACE(1,NFACE),ICA)
        NE(2,IC)=NE(IFACE(2,NFACE),ICA)
        NE(3,IC)=NE(IFACE(3,NFACE),ICA)
        NE(4,IC)=NE(IFACE(4,NFACE),ICA)
        NE(7,IC)=ICA
        NE(8,IC)=NFACE
        IF (MOD(NFACE,2).EQ.1) THEN
          NND=NE(2,IC)
          NE(2,IC)=NE(1,IC)
          NE(1,IC)=NND
          NND=NE(4,IC)
          NE(4,IC)=NE(3,IC)
          NE(3,IC)=NND
        ENDIF
        NE(9,IC)=ISRTAB-2
C CHECK IF PARTIAL BOUNDARY
        IF (LCP(NFACE,ICA)) THEN
          IPART=IPART+1
          NE(9,IC)=ISRTAB
        ENDIF
70    CONTINUE
      MXTB=ISRTAB
      ICSH1=MAXE+1
      MAXE=MAXE+I2
      NUMSHL=I2
      IPART1=IPART
      WRITE(LW,1015)NUMSHL-IPART,ISRTAB-2
1015  FORMAT(1X,I7,' SHELLS CREATED FOR WALL BOUNDARIES WITH',
     1      ' TABLE ENTRY ICTID=',I7)
C
C STORE NON-WALL BOUNDARIES ALSO
C
      ICUTOF=7*(NCELL+1)
      NCYSH=0
      DO 90 I=1,NBC
        IPOS=I
        IF (ISLIST(IPOS).GT.-1) GO TO 90
        I2=I2+1
        IC=IC+1
        IF (IC.GT.MAXCEL) THEN
          WRITE(LW,1003)MAXE+NBC
          RETURN
        ENDIF
C CHECK IF SYMMETRY OR CYCLIC BOUNDARY FACES
        IF (ISLIST(IPOS).LT.-ICUTOF) THEN
          ISL=IABS(ISLIST(IPOS)+ICUTOF)
          ISRT=ISRTAB-3
          NCYSH=NCYSH+1
        ELSE
          ISL=IABS(ISLIST(IPOS))
          ISRT=ISRTAB-1
        ENDIF
        ICELL=ISL/7
        NCMAP(IC)=MAXE+IPOS
        ICA=NCMAP(ICELL)
        NFACE=MOD(ISL,7)
        NE(1,IC)=NE(IFACE(1,NFACE),ICA)
        NE(2,IC)=NE(IFACE(2,NFACE),ICA)
        NE(3,IC)=NE(IFACE(3,NFACE),ICA)
        NE(4,IC)=NE(IFACE(4,NFACE),ICA)
        NE(7,IC)=ICA
        NE(8,IC)=NFACE
        IF (MOD(NFACE,2).EQ.1) THEN
          NND=NE(2,IC)
          NE(2,IC)=NE(1,IC)
          NE(1,IC)=NND
          NND=NE(4,IC)
          NE(4,IC)=NE(3,IC)
          NE(3,IC)=NND
        ENDIF
        NE(9,IC)=ISRT
C CHECK IF PARTIAL BOUNDARY
        IF (LCP(NFACE,ICA)) THEN
          IPART=IPART+1
          NE(9,IC)=ISRTAB
        ENDIF
90    CONTINUE
      MXTB=ISRTAB
      NUMSHL=I2-NUMSHL
C REVERSE THE BOUNDARY PART OF THE NCMAP
      I2=MAXEO
      DO 110 I=1,NBC
        IF (ISLIST(I).LT.1) THEN
          NCMAP(MAXEO+I)=0
        ELSE
          I2=I2+1
          NCMAP(MAXEO+I)=I2
        ENDIF
110   CONTINUE
      DO 120 I=1,NBC
        IF (ISLIST(I).LT.0) THEN
          I2=I2+1
          NCMAP(MAXEO+I)=I2
        ENDIF
120   CONTINUE
C REVERSE THE MAP
      DO 121 I=1,NBC/2
        II=MAXEO+NBC-I+1
        ID=NCMAP(MAXEO+I)
        NCMAP(MAXEO+I)=NCMAP(II)
        NCMAP(II)=ID
121   CONTINUE

      MAXE=MAXE+NUMSHL
      IPART2=IPART-IPART1
      IF(NUMSHL-IPART2-NCYSH.GT.0)WRITE(LW,1016)
     *                            NUMSHL-IPART2-NCYSH,ISRTAB-1
1016  FORMAT(1X,I7,' SHELLS CREATED FOR NON-WALL BOUNDARIES WITH',
     1      ' TABLE ENTRY ICTID=',I7)
      IF (NCYSH.GT.0) WRITE(LW,1116)NCYSH,ISRTAB-3
1116  FORMAT(1X,I7,' SHELLS CREATED FOR CYCLIC OR SYMMETRY',
     1      ' FACES WITH TABLE ENTRY ICTID=',I7)
      IF (IPART.GT.0) WRITE(LW,1017)IPART,ISRTAB
1017  FORMAT(1X,I7,' SHELLS CREATED FOR PARTIAL BOUNDARIES WITH',
     1      ' TABLE ENTRY ICTID=',I7)
99    WRITE(LW,1018)MAXE
1018  FORMAT(' THE MAXIMUM CELL NUMBER IS NOW ',I7)
100   RETURN
      END
C
      SUBROUTINE SRFGET(IERROR,NCMAP,NCPDIM,ICOUP)
C
C GET SPECIFIC WALL DATA SET
C
      COMMON/ARSIZE/MAXCEL,MAXVRT,MAXNCP,MAXTAB,MAXNBU,MAXREG,MAXPST
      COMMON/KEY/MAXN,MAXE,ICSYS,MAXSYS,ITYPE,PFIRST,IVERS,NUMW,
     1           NI,NJ,NK,NF,MAXB,IBFILL,NOVICE,KEYS(10),LW,LR,
     2           LECHO,LNUT,MXTB,KEYS2(6),MAXCP,KEYS3(17)
      COMMON/CELLS/NE(9,1)
      COMMON/CTABLE/ICTAB(10,1)
      COMMON/NTABLE/CTNAME(1)
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
      COMMON/PLTKEY/NPEN,ICSCAL,VMNPEN,VMXPEN,PENINC,IPLVEC(3),IPLSCL,
     1              VECSCL,JPLTYP,VRANGE,SNORM(3),PMESH,MESHPN,THIN,
     2              MNUM,UGRDON,NPSX,NPSY,BLACKB,IPLSYS,IPLSRF,VISRF,
     3              NMTIC(3),LTRNSL,LPREVE
      COMMON/PDATA/VECTOR(14,1)
      COMMON/SCRT1/ISLIST(1)
      COMMON/SCRT2/ARR(2048),IDUM(1)
      DIMENSION NCMAP(MAXCEL),IFACE(4,6),DTYPE(14),ICOUP(NCPDIM,1)
      LOGICAL LSRF
      CHARACTER CTNAME*4,DTYPE*4
      DATA IFACE/1,2,3,4, 5,6,7,8, 2,1,5,6, 3,4,8,7, 1,4,8,5,
     1           2,3,7,6/
      DATA DTYPE/'SFX ','SFY ','SFZ ','YPLU','DIST','TEMP','HTRA',
     1           'MTRA','HFLU','MFLU','TIRA','TRAD','SIRA','SRAD'/
C
      IF (NDATA(1,3).EQ.0) THEN
        WRITE(LW,1000)
1000    FORMAT(' **ERROR - THIS POST FILE DOES NOT HAVE ANY WALL DATA')
        IERROR=1
        RETURN
      ENDIF
C
C ROW NUMBER OF WALL DATA VECTOR
      NROW=14
C
C IF NOT ALREADY DONE, CREATE WALL SHELLS; ELSE STORE WALL DATA
C
40    IF (ISRTAB.GT.0) GO TO 100
      NREC=NDATA(1,3)
      CALL LDDATA(NREC,ISLIST,1,1,1,NBC,.FALSE.,NCMAP)
      ISRTAB=MXTB+2
      IF (ISRTAB.GT.MAXTAB) THEN
        WRITE(LW,1002)MAXTAB
1002    FORMAT(' **WARNING - THE NUMBER OF CELL TABLE ENTRIES IS',
     1         ' COMPLETELY FILLED.',/,'   INCREASE PARAMETER',
     2         ' MAXTAB TO AT LEAST',I7,' AND RECOMPILE.')
        ISRTAB=0
        IERROR=1
        RETURN
      ENDIF
      ICTAB(1,ISRTAB-1)=4
      ICTAB(2,ISRTAB-1)=7
      ICTAB(3,ISRTAB-1)=0
      CTNAME(ISRTAB-1)='WALL'
      ICTAB(1,ISRTAB)=4
      ICTAB(2,ISRTAB)=8
      ICTAB(3,ISRTAB)=0
      CTNAME(ISRTAB)='NONW'
C
C MAKE THE PROSTAR TO STAR CELL MAP
C
      CALL STRMAP(NCMAP,IDUM,MAXE,.TRUE.,MAXCP,NCPDIM,ICOUP)
C
      I2=0
      IC=MAXE
      DO 70 I=1,NBC
        IPOS=I
        IF (ISLIST(IPOS).LT.1) THEN
          GO TO 70
        ELSE
          I2=I2+1
          IC=IC+1
        ENDIF
        IF (IC.GT.MAXCEL) THEN
          WRITE(LW,1003)MAXE+NBC
1003      FORMAT(' **WARNING - THE NUMBER OF CURRENT CELLS PLUS WALL',
     1           ' ITEMS EXCEEDS ALLOWABLE STORAGE.',/,'   INCREASE',
     2           ' PARAMETER MAXCEL TO AT LEAST',I7,' AND RECOMPILE.')
          IERROR=1
          RETURN
        ENDIF
        ICELL=ISLIST(IPOS)/7
        ICA=NCMAP(ICELL)
        NFACE=MOD(ISLIST(IPOS),7)
        NE(1,IC)=NE(IFACE(1,NFACE),ICA)
        NE(2,IC)=NE(IFACE(2,NFACE),ICA)
        NE(3,IC)=NE(IFACE(3,NFACE),ICA)
        NE(4,IC)=NE(IFACE(4,NFACE),ICA)
        IF (MOD(NFACE,2).EQ.1) THEN
          NND=NE(2,IC)
          NE(2,IC)=NE(1,IC)
          NE(1,IC)=NND
          NND=NE(4,IC)
          NE(4,IC)=NE(3,IC)
          NE(3,IC)=NND
        ENDIF
        NE(9,IC)=ISRTAB-1
70    CONTINUE
80    MXTB=ISRTAB-1
      ICSH1=MAXE+1
      MAXE=MAXE+I2
      NUMSHL=I2
      WRITE(LW,1015)NUMSHL,ISRTAB-1,MAXE
1015  FORMAT(1X,I7,' WALL SHELLS CREATED FOR WALL DATA WITH',
     1      ' TABLE ENTRY ICTID=',I7,/,
     2      ' THE MAXIMUM CELL NUMBER IS NOW ',I7)
C
C STORE NON-WALL BOUNDARIES ALSO
C
      DO 90 I=1,NBC
        IPOS=I
        IF (ISLIST(IPOS).GT.-1) THEN
          GO TO 90
        ELSE
          I2=I2+1
          IC=IC+1
        ENDIF
        IF (IC.GT.MAXCEL) THEN
          WRITE(LW,1003)MAXE+NBC
          IERROR=1
          RETURN
        ENDIF
        ISL=IABS(ISLIST(IPOS))
        ICELL=ISL/7
        ICA=NCMAP(ICELL)
        NFACE=MOD(ISL,7)
        NE(1,IC)=NE(IFACE(1,NFACE),ICA)
        NE(2,IC)=NE(IFACE(2,NFACE),ICA)
        NE(3,IC)=NE(IFACE(3,NFACE),ICA)
        NE(4,IC)=NE(IFACE(4,NFACE),ICA)
        IF (MOD(NFACE,2).EQ.1) THEN
          NND=NE(2,IC)
          NE(2,IC)=NE(1,IC)
          NE(1,IC)=NND
          NND=NE(4,IC)
          NE(4,IC)=NE(3,IC)
          NE(3,IC)=NND
        ENDIF
        NE(9,IC)=ISRTAB
90    CONTINUE
      MXTB=ISRTAB
      NUMSHL=I2-NUMSHL
      MAXE=MAXE+NUMSHL
      WRITE(LW,1016)NUMSHL,ISRTAB,MAXE
1016  FORMAT(1X,I7,' SHELLS CREATED FOR NON-WALL DATA WITH',
     1      ' TABLE ENTRY ICTID=',I7,/,
     2      ' THE MAXIMUM CELL NUMBER IS NOW ',I7)
C INITIALIZE VECTOR
      DO 95 I=ICSH1,MAXE
        DO 95 J=1,NROW
95        VECTOR(J,I)=0. 
C
C WALL SCALAR ITEMS: SFX,SFY,SFZ,YPLUS,DIST,TEMP,HTRAN,MTRAN(NSC),
C                    HFLUX,MFLUX(NSC),TRAD,TIRA,SRAD,SIRA
C
C SFX,SFY,SFZ,YPLUS,DIST,TEMP,HTRAN
C
100   DO 120 I=1,7
        NREC=NDATA(1,3)
        CALL LDDATA(NREC,ISLIST,1,1,1,NBC,.FALSE.,NCMAP)
        I2=0
        DO 110 II=1,NBC
          IF (ISLIST(II).LT.1) THEN
            NCMAP(II)=0
          ELSE
            I2=I2+1
            NCMAP(II)=I2
          ENDIF
110     CONTINUE
C
        NREC=NDATA(I+1,3)
        IF (NREC.GT.0) THEN
          CALL LDDATA(NREC,VECTOR(1,ICSH1),I,NROW,1,NBC,.TRUE.,NCMAP)
        ELSE
          WRITE(LW,1005)DTYPE(I)
1005      FORMAT(' **WARNING - WALL VARIABLE ',A,' NOT ON DATA FILE')
        ENDIF
120   CONTINUE
C
C SCALAR MASS TRANSFER, MTRAN(NSC), I=8
C
        NREC=NDATA(1,3)
        CALL LDDATA(NREC,ISLIST,1,1,1,NBC,.FALSE.,NCMAP)
        I2=0
        DO 130 II=1,NBC
          IF (ISLIST(II).LT.1) THEN
            NCMAP(II)=0
          ELSE
            I2=I2+1
            NCMAP(II)=I2
          ENDIF
130     CONTINUE
C
      I=8
      IF (NUMCON.GT.0) THEN
        WRITE(LW,1035) NUMCON
1035    FORMAT(' CHOOSE THE MASS TRANSFER:',
     1         ' (AN INTEGER BETWEEN 1 AND ',I3,' )')
        READ(LR,*)NSCSTO
C
        IF (NSCSTO.LT.1) NSCSTO=1
        IF (NSCSTO.GT.NUMCON) NSCSTO=NUMCON
        NPT=8+NSCSTO
        NREC=NDATA(NPT,3)
        IF (NREC.GT.0) THEN
          CALL LDDATA(NREC,VECTOR(1,ICSH1),I,NROW,1,NBC,.TRUE.,NCMAP)
        ELSE
          WRITE(LW,1005)DTYPE(I)
        ENDIF
      ELSE
        WRITE(LW,1005)DTYPE(I)
      ENDIF
C
C HEAT FLUX, I=9
C
        NREC=NDATA(1,3)
        CALL LDDATA(NREC,ISLIST,1,1,1,NBC,.FALSE.,NCMAP)
        I2=0
        DO 140 II=1,NBC
          IF (ISLIST(II).LT.1) THEN
            NCMAP(II)=0
          ELSE
            I2=I2+1
            NCMAP(II)=I2
          ENDIF
140     CONTINUE
C
      I=9
      NREC=NDATA(59,3)
      IF (NREC.GT.0) THEN
        CALL LDDATA(NREC,VECTOR(1,ICSH1),I,NROW,1,NBC,.TRUE.,NCMAP)
      ELSE
        WRITE(LW,1005)DTYPE(I)
      ENDIF
C
C SCALAR MASS FLUX, I=10
C
        NREC=NDATA(1,3)
        CALL LDDATA(NREC,ISLIST,1,1,1,NBC,.FALSE.,NCMAP)
        I2=0
        DO 150 II=1,NBC
          IF (ISLIST(II).LT.1) THEN
            NCMAP(II)=0
          ELSE
            I2=I2+1
            NCMAP(II)=I2
          ENDIF
150     CONTINUE
C
      I=10
      IF (NUMCON.GT.0) THEN
        WRITE(LW,1045) NUMCON
1045    FORMAT(' CHOOSE THE MASS FLUX:',
     1         ' (AN INTEGER BETWEEN 1 AND ',I3,' )')
        READ(LR,*)NSCSTO
C
        IF (NSCSTO.LT.1) NSCSTO=1
        IF (NSCSTO.GT.NUMCON) NSCSTO=NUMCON
        NPT=59+NSCSTO
        NREC=NDATA(NPT,3)
        IF (NREC.GT.0) THEN
          CALL LDDATA(NREC,VECTOR(1,ICSH1),I,NROW,1,NBC,.TRUE.,NCMAP)
        ELSE
          WRITE(LW,1005)DTYPE(I)
        ENDIF
      ELSE
        WRITE(LW,1005)DTYPE(I)
      ENDIF
C
C THERMAL RADIOSITY, TRAD; THERMAL INCIDENT RADIATION, TIRA
C SOLAR RADIOSITY, SRAD; SOLAR INCIDENT RADIATION, SIRA
C
      DO 200 I=11,14
        NREC=NDATA(1,3)
        CALL LDDATA(NREC,ISLIST,1,1,1,NBC,.FALSE.,NCMAP)
        I2=0
        DO 160 II=1,NBC
          IF (ISLIST(II).LT.1) THEN
            NCMAP(II)=0
          ELSE
            I2=I2+1
            NCMAP(II)=I2
          ENDIF
160     CONTINUE
C SPECIFICALLY FOR THEST 4 SCALARS
        DO 170 II=1,NBC
          IF (ISLIST(II).LT.0) THEN
            I2=I2+1
            NCMAP(II)=I2
          ENDIF
170     CONTINUE
C PLUS 1 TO ACCOUNT FOR SKIPPING HCOEFFICIENT
        NREC=NDATA(98+I+1,3)
        IF (NREC.GT.0) THEN
          CALL LDDATA(NREC,VECTOR(1,ICSH1),I,NROW,1,NBC,.TRUE.,NCMAP)
        ELSE
          WRITE(LW,1005)DTYPE(I)
        ENDIF
200   CONTINUE
C
      RETURN
      END
C
      SUBROUTINE STRMAP(NCMAP,IDUM,MAXE,LIGDEL,MAXCP,NCPDIM,ICOUP)
C
C MAKE THE PROSTAR TO STAR CELL MAP: NCMAP(STAR #)=PRO #
C
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
      COMMON/CELLS/NE(9,1)
      COMMON/CTABLE/ICTAB(10,1)
      DIMENSION ICOUP(NCPDIM,1)
      DIMENSION NCMAP(1),IDUM(1)
      LOGICAL LCNPST,LIGDEL
      NPT=0
      MPT=MAXE+1
      DO 10 I=1,MAXE
        IF (NE(1,I).LE.0.AND.LIGDEL) GO TO 10
        ICTID=NE(9,I)
        IF (ICTAB(1,ICTID).EQ.1) THEN
          NPT=NPT+1
          NCMAP(NPT)=I
        ELSEIF (ICTAB(1,ICTID).EQ.2.AND.LCNPST) THEN
          MPT=MPT-1
          NCMAP(MPT)=I
        ENDIF
10    CONTINUE
      NTFLU=NPT
      IF (MPT.LE.MAXE) THEN
        KPT=0
        DO 30 I=MAXE,MPT,-1
          KPT=KPT+1
          IDUM(KPT)=NCMAP(I)
30      CONTINUE
        NTSOL=MAXE+1-MPT
        DO 40 I=1,NTSOL
          NCMAP(NPT+I)=IDUM(I)
40      CONTINUE
      ENDIF
C
C IF COUPLED CELLS PUT THE MASTERS AT THE END OF EACH LIST
C (NCMAP(STAR #)=PRO #    -    IDUM(PRO #)=STAR#)
      IF (LVERS.GT.2115.OR.MAXCP.EQ.0) RETURN
C FIRST DO FLUIDS
      DO 60 I=1,MAXE
60      IDUM(I)=0
      DO 70 I=1,NTFLU
70      IDUM(NCMAP(I))=I
      NTMX=MAXE
C MOVE MASTER CELLS TO END OF LIST
      DO 80 I=1,MAXCP
        NCP=ICOUP(1,I)
        IF (NCP.EQ.0) GO TO 80
        NSTAR=IDUM(NCP)
        IF (NSTAR.EQ.0) GO TO 80
        NTMX=NTMX+1
        IDUM(NCP)=NTMX
        NCMAP(NSTAR)=0
80    CONTINUE
C COMPRESS OUT MASTER CELLS
      ITMX=0
      DO 90 I=1,NTFLU
        IF (NCMAP(I).EQ.0) GO TO 90
        ITMX=ITMX+1
        NCMAP(ITMX)=NCMAP(I)
90    CONTINUE
C COMBINE LISTS
      DO 100 I=1,MAXE
        IF (IDUM(I).GT.NTFLU) THEN
          ITMX=ITMX+1
          NCMAP(ITMX)=I
        ENDIF
100   CONTINUE
C
C NOW SAME THING WITH SOLIDS
C
      IF (.NOT.LCNPST) RETURN
      DO 160 I=1,MAXE
160     IDUM(I)=0
      DO 170 II=1,NTSOL
        I=II+NTFLU
170     IDUM(NCMAP(I))=I
      NTMX=MAXE
C MOVE MASTER CELLS TO END OF LIST
      DO 180 I=1,MAXCP
        NCP=ICOUP(1,I)
        IF (NCP.EQ.0) GO TO 180
        NSTAR=IDUM(NCP)
        IF (NSTAR.EQ.0) GO TO 180
        NTMX=NTMX+1
        IDUM(NCP)=NTMX
        NCMAP(NSTAR)=0
180   CONTINUE
C COMPRESS OUT MASTER CELLS
      ITMX=NTFLU
      DO 190 II=1,NTSOL
        I=II+NTFLU
        IF (NCMAP(I).EQ.0) GO TO 190
        ITMX=ITMX+1
        NCMAP(ITMX)=NCMAP(I)
190   CONTINUE
C COMBINE LISTS
      DO 200 I=1,MAXE
        IF (IDUM(I).GT.MAXE) THEN
          ITMX=ITMX+1
          NCMAP(ITMX)=I
        ENDIF
200   CONTINUE
      RETURN
      END
      SUBROUTINE BNCNST(VECTOR,IRS,IDIM,VAL,N1,MAXE)
C
C LOADS A MATERIALLY VARYING POST VALUE ON BOUNDARIES
C
      COMMON/CELLS/NE(9,1)
      COMMON/CTABLE/ICTAB(10,1)
      DIMENSION VECTOR(IDIM,1),VAL(99)
C
      DO 100 I=N1,MAXE
        IF (NE(1,I).LT.1) GO TO 100
        ICTID=NE(9,NE(7,I))
        IF (ICTAB(1,ICTID).GT.2) GO TO 100
        IMAT=ICTAB(4,ICTID)
        VECTOR(IRS,I)=VAL(IMAT)
100   CONTINUE
      RETURN
      END
      SUBROUTINE CTAG(NODE,ITYPE)
C
C GIVEN NODAL DEFINITION RETURNS 1-BRICK,2-PRISM,3-TETRAHEDRON,4-PYRAMID
C
      DIMENSION NODE(8)
      IF (NODE(7).NE.NODE(8)) THEN
        ITYPE=1
      ELSEIF (NODE(3).EQ.NODE(4)) THEN
        IF (NODE(5).EQ.NODE(8)) THEN
          ITYPE=3
        ELSE
          ITYPE=2
        ENDIF
      ELSE
        ITYPE=4
      ENDIF
      RETURN
      END
      SUBROUTINE BINRDV(NVREC,V,NDIM,NV1,NV2,LMAP,NCMAP,MAXC)
C
C LOADS ALL VELOCITY COMPONENTS
C
      COMMON/POSTD/ITERU,TIMEU,ITERS,TIMES,VMING,VMAXG,VMINL,VMAXL,
     1             SMING,SMAXG,SMINL,SMAXL,NDATA(200,3),LPOST,
     2             NCELL,NBC,NBW,NBS,NBB,NNODE,WPOST,LDCELL,LDVERT,
     3             LSORT,LUNMOD,IPSYS,LOPER,LRSKIP,TREF,LDSURF,
     4             ISRTAB,ICSH1,NSITEM,LSRF(200),NBCUT,NBCYC,PREF,
     5             PREPP(2,99),NTCELL,LCNPST,LDPPRS,
     6             LPCORD,NUMCON,NSC,TREFM(99),LVERS,LDFLUX,
     7             IREFRM,IRESYS,OMEREF,ADDREL,PGRAV(7,99),PSPIN(7,99),
     8             PMW(99),PMWS(50,99),SCALE9,LPTYP,LADJST,LPINTP,TIMEP,
     9             ITERP,LRELA,LMVGR,NDATAP(200,3),IRETW,NWSET,SPCYC,
     *             LSTAR(100),NBNCST,NBNCEN,NBSI,NSCL,NREG,
     1             MRECMM,NBP23,NCLPS,NWPRSM,NPRSM,NCLP,NCLPF,ITYPEN
      LOGICAL LSTAR,SPCYC
      COMMON/SCRT2/ARR(2048)
      DIMENSION NCMAP(1),NVREC(3)
      LOGICAL LMAP
      LOGICAL WPOST,LDCELL,LDVERT,LSORT,LUNMOD,LOPER,LRSKIP
      LOGICAL LRELA
      DIMENSION V(NDIM,1)
      LF=IABS(LPOST)
      DO 10 J=1,NDIM
      DO 10 K=1,NV2
10      V(J,K)=0.
C
C BEGIN NEW ARRAY
C
      MAXC=0
      DO 200 J=1,NDIM
        IF(NVREC(J).EQ.0) GO TO 200
        IREC=NVREC(J)+1
        NWORD=NV2-NV1+1
        K1=NV1
        NUMR=(NWORD-1)/2048+1
        IFIN=IREC+NUMR-1
        DO 100 JJ=IREC,IFIN
          READ(LF,REC=JJ)ARR
          K2=K1+2048-1
          K2=MIN0(K2,NV2)
          IF (LRSKIP) THEN
            IF (K2.LT.1) GO TO 60
            DO 50 K=K1,K2
              K3=K-K1+1
              IF (K.GT.0) THEN
                IF (LMAP) THEN
                  NPT=NCMAP(K)
                ELSE
                  NPT=K
                ENDIF
                MAXC=MAX(MAXC,NPT)
                IF (NPT.GT.0) V(J,NPT)=ARR(K3)
              ENDIF
50          CONTINUE
60          K1=K2+1
          ELSE
            DO 70 K=K1,K2
              K3=K-K1+1
              IF (LMAP) THEN
                NPT=NCMAP(K)
              ELSE
                NPT=K
              ENDIF
              MAXC=MAX(MAXC,NPT)
              IF (NPT.GT.0) V(J,NPT)=ARR(K3)
70          CONTINUE
            K1=K2+1
          ENDIF
100     CONTINUE
200   CONTINUE
      RETURN
      END
      SUBROUTINE CELOUT(NC1, NC2, LELEM, LBOUND, FMTOUT, IERR)
      COMMON/KEY/MAXN,MAXE,ICSYS,MAXSYS,ITYPE,PFIRST,IVERS,NUMW,
     1           NI,NJ,NK,NF,MAXB,IBFILL,NOVICE,KEYS(10),LW,LR,
     2           LECHO,LNUT,MXTB,LTURBI,LTURBP,SETADD,NSENS,NPART,
     4           SCALE8,KEYS2(18)
      COMMON/CELLS/NE(9,1)
      COMMON/POSTD/ITERU,TIMEU,ITERS,TIMES,VMING,VMAXG,VMINL,VMAXL,
     1             SMING,SMAXG,SMINL,SMAXL,NDATA(200,3),LPOST,
     2             NCELL,NBC,NBW,NBS,NBB,NNODE,WPOST,LDCELL,LDVERT,
     3             LSORT,LUNMOD,IPSYS,LOPER,LRSKIP,TREF,LDSURF,
     4             ISRTAB,ICSH1,NSITEM,LSRF(200),NBCUT,NBCYC,PREF,
     5             PREPP(2,99),NTCELL,LCNPST,LDPPRS,
     6             LPCORD,NUMCON,NSC,TREFM(99),LVERS,LDFLUX,
     7             IREFRM,IRESYS,OMEREF,ADDREL,PGRAV(7,99),PSPIN(7,99),
     8             PMW(99),PMWS(50,99),SCALE9,LPTYP,LADJST,LPINTP,TIMEP,
     9             ITERP,LRELA,LMVGR,NDATAP(200,3),IRETW,NWSET,SPCYC,
     *             LSTAR(100),NBNCST,NBNCEN,NBSI,NSCL,NREG,
     1             MRECMM,NBP23,NCLPS,NWPRSM,NPRSM,NCLP,NCLPF,ITYPEN
      COMMON/CASEN/CASE,TITLE
      DIMENSION NDUM(9)
      LOGICAL LELEM, LBOUND, FMTOUT
      CHARACTER CASE*70,FNAME*80,TITLE*52
C
C WRITE CELL (ELEMENT) OR BOUNDARY SHELL DEFINITIONS
C
      IERR=0
      CALL STRTRM(CASE,CASE,LCASE)
      IF (LELEM) THEN
        LF=14
        FNAME=CASE(1:LCASE)//'.14'
      ELSEIF (LBOUND) THEN
        LF=23
        FNAME=CASE(1:LCASE)//'.23'
      ELSE
        RETURN
      ENDIF
      IF (FMTOUT) THEN
        OPEN(LF,FILE=FNAME,STATUS='UNKNOWN',FORM='FORMATTED')
      ELSE
        OPEN(LF,FILE=FNAME,STATUS='UNKNOWN',FORM='UNFORMATTED')
      ENDIF
      NC=0
      DO 720 I=NC1,NC2
        IF (NE(1,I).LT.1) GO TO 720
        DO 705 J=1,8
705       NDUM(J)=0
        ITYPE=NE(9,I)
        DO 710 J=1,8
          NDUM(J)=NE(J,I)
710     CONTINUE
        NDUM(9)=NE(9,I)
        IF (FMTOUT) THEN
          WRITE(LF,1222,ERR=900)I,NDUM
1222      FORMAT(I7,6X,9I7)
        ELSE
          WRITE(LF,ERR=900)I,NDUM
        ENDIF
        NC=NC+1
720   CONTINUE
      CLOSE(LF)
      IF (LELEM) THEN
        WRITE(LW,1201)NC,'CELL',FNAME(1:LCASE+4)
1201    FORMAT(1X,I8,1X,A,' DEFINITIONS WRITTEN TO FILE ',A)
      ELSE
        WRITE(LW,1201)NC,'BOUNDARY SHELL',FNAME(1:LCASE+4)
      ENDIF
      RETURN
C
900   IERR=1
      IF (LELEM) THEN
        WRITE(LW,1111) 'CELL'
1111    FORMAT(' **ERROR WRITING ',A,' DEFINITIONS')
      ELSE
        WRITE(LW,1111) 'BOUNDARY SHELL'
      ENDIF
      RETURN
      END
