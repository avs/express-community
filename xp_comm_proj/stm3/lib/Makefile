# 
# Makefile Multi-plateform (IRIX - LINUX - Solaris)
#

# We compiled this on Solaris8 (s8), SGI (sg6) and Linux
# Change flags for whatever you need


include ../../../include/xp_defs.mk
#OBJECTS_DIR = ./s8
#OBJECTS_DIR = ./linux
#OBJECTS_DIR = ./sg6
OBJECTS_DIR = .

#CXXFLAGS = -Dsparc -Dsol2 -Dsol7 -Dsol8 -mt -KPIC -xs -xO4 -dalign -xarch=v8
#CXXFLAGS = -fPIC -march=pentium -mcpu=pentiumpro -O3
#CXXFLAGS = -fPIC -march=pentium -mcpu=pentiumpro -g
#CXXFLAGS = -64 -mips4 -g -OPT:Olimit_opt=on $(FLAGS)
CXXFLAGS = $(CXX_OPTS) $(MACH_CFLAGS)

VPATH = $(OBJECTS_DIR)

.SUFFIXES: .cxx .o

.cxx.o:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c -o $@ $<

#----------
# User Code
#----------

PROG= libstm3.a

all: $(PROG)

OBJECTS=read_pdb.o read_Cgamess.o read_chem_3d.o read_coord.o read_cube.o read_dcar.o read_dmol_xyz.o read_mopac_esp.o read_gamess.o \
	read_mm3.o read_mol2.o read_mol.o read_plain_coord.o read_mopac_xyz.o read_shelx.o \
	read_kino.o read_dcd.o \
        atom_properties.o name_to_z.o res_to_color.o

$(PROG): ${OBJECTS}
	ar cr ${PROG} ${OBJECTS}
	ranlib ${PROG}

#------------------------------- Dependencies --------------------------------

read_pdb.o:	stm3.h ../base/mol_type.h
read_Cgamess.o:	stm3.h
read_chem_3d.o:	stm3.h
read_coord.o:	stm3.h
read_cube.o:	stm3.h ../base/mol_type.h
name_to_z.o:	stm3.h
read_dcar.o:	stm3.h
read_dmol_xyz.o:  stm3.h
read_mopac_esp.o: stm3.h
read_gamess:    stm3.h
read_mm3.o:     stm3.h
read_mol2.o:    stm3.h
read_mol.o:     stm3.h
read_plain_coord.o: stm3.h
read_mopac_xyz.o:   stm3.h
read_kino.o:    stm3.h
read_dcd.o:     stm3.h
read_shelx.o:	stm3.h ../base/mol_type.h

#------------------------------------------------------------------------------
clean:
	-rm -fr $(VPATH)/*.o $(VPATH)/*.a $(PROG) $(OBJECTS_DIR)/$(PROG)


#------------------------------------------------------------------------------
