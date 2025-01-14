/* This header can be read by both C and C++ compilers */
#ifndef CONNECT_H
#define CONNECT_H
#include "svdpi.h"

#ifdef __cplusplus
extern "C" {
#endif
  extern int   simRead_(char* fileName,int seed);
  extern int   cDpi_getVal(char* objName, char* propName, int defVal);
  extern int   cDpi_getVal_(char* objName, char* propName, int idx, int defVal);
  extern int   cDpi_getList(char* objName, char* propName, int list[]);
  extern int   cDpi_getList_(char* objName, char* propName, int idx, int list[]);
  extern char* cDpi_getString(char* objName, char* propName, char* defVal);
  extern char* cDpi_getString_(char* objName, char* propName, int idx, char* defVal);
#ifdef __cplusplus
}
#endif

#endif


