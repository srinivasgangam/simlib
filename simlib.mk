######################################################################
# Author : Srinivas Gangam
######################################################################

CC_FILES+=\
	 simGlobal.C #$(wildcard *.C)

C_FILES+=\
	 lex.yy.c simdpi.c #$(wildcard *.c)

 INCPATH += \
	  -I ./

EXE?=./obj
OBJ_DIR?= ./obj
TARGET_DIR?=./obj

DLL:=libdpi.so
#LIB:=libdpi.a 

include Makerules.c

