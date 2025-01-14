
CC_FILES+=\
	 simGlobal.C

C_FILES+=\
	 lex.yy.c \

# search path for header files
INCPATH+=\
	-I./

DLL:=libdpi.so
TARGET_DIR?=./obj
OBJ_DIR?=./obj

all :
	lex sim_parse.l
	mkdir -p obj
	g++ simGlobal.C lex.yy.c test.c  -o run -I ./ #-DDEBUG=1
	./run

#include Makerules.c

run : 
	./run
