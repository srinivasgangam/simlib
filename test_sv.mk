
CC_FILES+=\
	 $(wildcard *.C)

C_FILES+=\
	 lex.yy.c \

# search path for header files
INCPATH+=\
	-I/dept/asicapps/cadence/incisive/13.10.005/linux/tools.lnx86/inca/include/ \

vpath %c \
  /dept/asicapps/cadence/incisive/13.10.005/linux/tools.lnx86/inca/include/ \

ifdef DLL
LIB:=libdpi.so
endif
ifdef LIB
LIB:=libdpi.a
endif

TARGET_DIR?=./obj
OBJ_DIR?=./obj

all :
	make -f simlib.mk
	irun -f simPkg.vf test.sv -sv_lib obj/libdpi.so +define+DEBUG
	#flex -Ptb_yy sim_parse.l
	#lex sim_parse.l
	#mkdir -p obj
	#gcc -m32 -MMD -Wall  -g -m32  -fPIC -export-dynamic -I/dept/asicapps/cadence/incisive/13.10.005/linux/tools.lnx86/inca/include/ -c lex.yy.c -o obj/lex.yy.o
	#gcc -m32 -MMD -Wall  -g -m32  -fPIC -export-dynamic -I/dept/asicapps/cadence/incisive/13.10.005/linux/tools.lnx86/inca/include/ -c simGlobal.C -o obj/simGlobal.o
	#/bin/rm -f obj/libdpi.so && g++ -m32  -o obj/libdpi.so -shared -Wall  -g -m32  -fPIC -export-dynamic ./obj/lex.yy.o ./obj/simGlobal.o
	#vcs -sverilog ./obj/libdpi.so -f simPkg.vf test.sv
	
	

#include Makerules.c

run : 
	irun -f simPkg.vf simTest.sv -sv_lib obj/libdpi.so +define+DEBUG
