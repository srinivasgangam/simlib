/* This header can be read by both C and C++ compilers */
#ifndef CONNECT_H
#define CONNECT_H

#ifdef __cplusplus
extern "C" {
#endif

  extern void createObjProp(char* objName, char* propName, char* propVal);
  extern int  simRead(char* fileName);

 // extern int simRead_(char* fileName,int seed);
 // extern int getVal(char* objName, char* propName, int defVal);
 // extern int getVal_(char* objName, char* propName, int idx, int defVal);
 // extern int getList(char* objName, char* propName, int list[]);
 // extern int getList_(char* objName, char* propName, int idx, int list[]);
 // extern char* getString(char* objName, char* propName, char* defVal);
 // extern char* getString_(char* objName, char* propName, int idx, char* defVal);
#ifdef __cplusplus
}
#endif

#endif


