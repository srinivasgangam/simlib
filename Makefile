######################################################################
# Author : Srinivas Gangam
######################################################################

CC_FILES+=\
	 $(wildcard *.C)

C_FILES+=\
	 $(wildcard *.c)

 INCPATH += \
	  -I ./

EXE?=./obj
OBJ_DIR?= ./obj
TARGET_DIR?=./obj


DLL:=lib.so

include ../../Makerules.c

