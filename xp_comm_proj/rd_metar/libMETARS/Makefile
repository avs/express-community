# include standard variable defs
include ../../../include/xp_defs.mk

SRCDIR=$(XP_ROOT)/xp_comm_proj/metars/libMETARS

LLIB=$(XP_LIB_DIR)/libmetars.a
DLLIB=$(XP_LIB_DIR)/libmetars

GFCINCDIR=$(XP_ROOT)/Utility/Modules/GFC
PROJ_INCLUDES=-I$(GFCINCDIR)

# Please add files in alphabetical order
LIBOFILES= \
    $(LLIB)(METARS.o) \
    $(LLIB)(Dcdmetar.o) \
    $(LLIB)(ANTOI.o) \
    $(LLIB)(CHARCMP.o) \
    $(LLIB)(DCDMTRMK.o) \
    $(LLIB)(FRACPART.o) \
    $(LLIB)(PRTDMETR.o) \
    $(LLIB)(STSPACK2.o) \
    $(LLIB)(STSPACK3.o) \
    $(LLIB)(METARSccbridge.o) \
    $(LLIB)(METARSStation.o) \
    $(LLIB)(METARSStationChoosen.o) \
    $(EMPTY)

default: $(LLIB) $(DLLIBNAME)

include $(XP_MACHRULE_PATH)

clean::
	rm -f $(LLIB) $(DLLIBNAME)
