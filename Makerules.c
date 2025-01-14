######################################################################
# Generic makerules to compile C/C++ code to an executable or a library
# User makefile provides: CFLAGS, OBJ_DIR, TARGET_DIR, EXE, LIB
# GCC, G++, AR, LIBLINKS
######################################################################

# declare (default) target first
all: comp

# [speedup] define an explict rule to make this makefile, or make will search for an implicit rule
Makerules.c : ;

# default settings
GCC?=gcc
G++?=g++
AR?=ar
CFLAGS ?= -Wall ${LOCAL_CFLAGS}
ifneq ($(strip ${ALLOW_WARN}),1)
#CFLAGS += -Werror
endif

# if not set, or set to null, upgrade to 0
ifeq ($(strip ${use_minusO}),)
  use_minusO=0
endif

ifneq ($(strip ${use_minusO}),0)
  ifeq ($(strip ${use_minusO}),3)
    CFLAGS += -O3
  else
    ifeq ($(strip ${use_minusO}),2)
      CFLAGS += -O2
    else
      ifeq ($(strip ${use_minusO}),1)
        CFLAGS += -O1
      else
        $(error Unrecognized value for $${use_minusO} = '${use_minusO}')
      endif
    endif
  endif
  fixCFLAGS = $(strip $(subst -g,,${CFLAGS}))
else
  ifneq ($(strip ${STRIP_DEBUG}),1)
  CFLAGS += -g
  endif
  fixCFLAGS = ${CFLAGS}
endif



ifdef REL_ID
  OBJ_DIR?=./$(shell /bin/${REL_ID}/uname)
else
  OBJ_DIR?=./$(shell /bin/uname)
endif
UNAME?=$(shell uname)

ifdef CC_FILES # if have C++ code, always use g++
  LD:=${G++}
else
  LD:=${GCC}
endif

ifeq (${USE_64BIT_SIM}, 1)
  GCC += -m64
  G++ += -m64
  CFLAGS += -m64 
  LD += -m64 
else
  GCC += -m32
  G++ += -m32
  CFLAGS += -m32 
  LD += -m32 
endif


ifdef EXE
  TARGET:=${TARGET_DIR}/${EXE}
endif
ifdef LIB
  TARGET:=${TARGET_DIR}/${LIB}
endif
ifdef DLL
  TARGET:=${TARGET_DIR}/${DLL}
  LD_FLAGS+=-shared
  CFLAGS+=-fPIC
#CFLAGS+=-fPIC -export-dynamic
endif
ifndef TARGET
  $(error Couldn't find target EXE or LIB or DLL!)
endif

#OS_VER=$(shell grep "Red Hat Enterprise Linux" /etc/redhat-release | wc -l | sed 's/^ *//g')
#ifneq (${OS_VER},1)
#  $(error "Attempt to compile on a server with the wrong Linux version -- Expect "Red Hat Enterprise Linux" version! " ******************** )
#endif

### begin new dependency methodology
# don't include dependencies when make target 'clean'
ifneq (${MAKECMDGOALS},clean)

DEPFLAGS += -MMD
 # line 1: converts relative to absolute paths (.d's for .C's in directory make was launched from)
 #         fixes known gcc deficiency where generated .d's have relative paths for launch dir files
 # line 2-3: foreach file in .P, create a null rule (file :) line
 #         fixes problem where .d refers to a file which is later deleted. Null rule will prevent error
 # line 4: .d is unused, .P is only needed file
 FIXDEPEND = \
 @sed -e "s% \([a-zA-Z0-9_][a-zA-Z0-9_/.]*\)% $$(pwd)\/\1%g" < ${OBJ_DIR}/$*.d > ${OBJ_DIR}/$*.P; \
 sed -e 's/\#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//'                \
 -e '/^$$/ d' -e 's/$$/ :/' < ${OBJ_DIR}/$*.d >> ${OBJ_DIR}/$*.P; \
 rm -f ${OBJ_DIR}/$*.d

 DEPFILES = $(C_FILES:%.c=%.P) $(CC_FILES:%.C=%.P) $(CCC_FILES:%.cc=%.P)
 -include $(addprefix ${OBJ_DIR}/, ${DEPFILES})

 # [speedup] define an explicit rule to make dependency files, or make will search for an implicit one
 %.P: ;

endif
### end new dependency methodology

### begin include path speedup

# cacheVar evaluates a variable just once, after that it returns a cached copy of the result
# (note: this differs from :=, in that := depends on the parse order of the file - this does not)
cacheVar = $(if ${_cached_$1},${_cached_$1},$(eval _cached_$1=${$1})${_cached_$1})

# filterDUPS removes entries occurring more than once, preserving the first occurrance.  Order is unaffected
filterDUPS = $(if $1,$(strip $(word 1,$1) $(call $0,$(filter-out $(word 1,$1),$1))))

# fixPATH reduces VERILIB and LOCAL_DV_DIR to those variable names in paths
fixPATH = $(subst ${LOCAL_DV_DIR},$$$$LOCAL_DV_DIR,$(subst ${VERILIB},$$$$VERILIB,$1))

ifdef LONGINC
  shortINCPATH = ${INCPATH}
else
  shortINCPATH = $(call cacheVar,fixINCPATH)

  # define a function to filter INCPATH
  #   note: done as a function so it can be called indirectly, otherwise every call to cacheVar
  #         would have to expand INCPATH just to evaluate the function and returned the cached value
  fixINCPATH = $(call fixPATH,$(call filterDUPS,$(subst -I ,-I,${INCPATH})))
endif

### end include path speedup


C_OBJ_FILES = $(addprefix ${OBJ_DIR}/, $(subst .c,.o,${C_FILES}))
CC_OBJ_FILES += $(addprefix ${OBJ_DIR}/, $(subst .C,.o,${CC_FILES}))
CCC_OBJ_FILES += $(addprefix ${OBJ_DIR}/, $(subst .cc,.o,${CCC_FILES}))
C_SORTED_OBJ_FILES = $(sort ${C_OBJ_FILES} ${CC_OBJ_FILES} ${CCC_OBJ_FILES})


ifdef EXE
${TARGET_DIR}/${EXE} : ${C_SORTED_OBJ_FILES}
	@-mkdir -p ${OBJ_DIR} ${TARGET_DIR}
	@echo "MAKE: Linking to generate executable" $@
	${LD} -o $@ ${CFLAGS} ${C_SORTED_OBJ_FILES} ${LIBLINKS}
endif

ifdef LIB
${TARGET_DIR}/${LIB} : ${C_SORTED_OBJ_FILES}
	@-mkdir -p ${OBJ_DIR} ${TARGET_DIR}
	@echo "MAKE: Linking to generate static library" $@
	/bin/rm -f $@ && ${AR} -r $@ ${C_SORTED_OBJ_FILES}
endif

ifdef DLL
${TARGET_DIR}/${DLL} : ${C_SORTED_OBJ_FILES}
	@-mkdir -p ${OBJ_DIR} ${TARGET_DIR}
	@echo "MAKE: Linking to generate dynamic library" $@
	/bin/rm -f $@ && ${LD} -o $@ ${LD_FLAGS} ${CFLAGS} ${C_SORTED_OBJ_FILES} ${LIBLINKS}
endif

$(sort ${C_OBJ_FILES}): ${OBJ_DIR}/%.o : %.c
	@echo "MAKE: Compiling ${<}"
	${GCC} ${DEPFLAGS} ${fixCFLAGS} ${shortINCPATH} -c $< -o $@ 
	${FIXDEPEND}

$(sort ${CC_OBJ_FILES}): ${OBJ_DIR}/%.o : %.C
	@echo "MAKE: Compiling ${<}"
	${G++} ${DEPFLAGS} ${fixCFLAGS} ${shortINCPATH} -c $< -o $@ 
	${FIXDEPEND}

$(sort ${CCC_OBJ_FILES}): ${OBJ_DIR}/%.o : %.cc
	@echo "MAKE: Compiling ${<}"
	${G++} ${DEPFLAGS} ${fixCFLAGS} ${shortINCPATH} -c $< -o $@ 
	${FIXDEPEND}


# order-only prereqs only supported on RHE40 and beyond
OOPREREQ = $(shell perl -e 'my $$var = $$ENV{LINUX_DIS}; $$var =~ m/rhe(\d+)/; print (($$1 && $$1 > 30) ? "1" : "0");')

ifeq (${OOPREREQ},1)
  # create OBJ_DIR if it doesn't exist using order-only prerequisite (next 2 rules)
  ${C_SORTED_OBJ_FILES} : | ${OBJ_DIR}

  ${OBJ_DIR} :
	@mkdir -p ${OBJ_DIR}
else
  $(warning Running on a machine with LINUX_DIS='${LINUX_DIS}' < rhe40, disabling auto-creation of ${OBJ_DIR})
endif


comp : ${TARGET}

clean:
	@-\rm -rf ${TARGET} ${OBJ_DIR}/* 
	@-\echo "---------"
	@-\echo "- Clean File: ${TARGET}"
	@-\echo "- Clean Dir:  ${OBJ_DIR}"
	@-\echo "---------"

help:
	@echo Default targets: all(comp), clean, help

.PHONY: all comp clean help

list_cc:
	@-\echo CC_FILES := $(CC_FILES)

list_c:
	@-\echo C_FILES := $(C_FILES)

list_inc:
	@-\echo INCPATH := ${shortINCPATH}
