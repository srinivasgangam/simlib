
#ifndef SIM_GLOBAL_H
#define SIM_GLOBAL_H

#include<string>
#include<iostream>
#include<sstream>
#include<map>
#include<simObj.h>
using namespace std;


#ifdef __cplusplus
extern "C" {
#endif

void createObjProp_(string objName, string propName, string propVal);
void createProp(string objName, string propName, string propVal);
std::string eraseBlanks(std::string s); 
void objPropExpand(string objName,string propName,string propVal, int isObj, std::size_t startPos, std::size_t endPos, string rangeStr); 
void createObjPropPre(string objName, string propName, string propVal) ;
void createObjProp(char* objName, char* propName, char* propVal) ;

int getVal  (std::string objName, std::string propName, int defVal) ;
int getVal_ (std::string objName, std::string propName, int idx, int defVal) ;
int getList (std::string objName, std::string propName, int list[]) ;
int getList_(std::string objName, std::string propName, int idx, int list[]) ;
std::string getString (std::string objName, std::string propName, std::string defVal) ;
std::string getString_(std::string objName, std::string propName, int idx, std::string defVal) ;


extern int simRead_(char* fileName, int seed);
extern int   cDpi_getVal    (char* objName, char* propName, int defVal) ;
extern int   cDpi_getVal_   (char* objName, char* propName, int idx, int defVal) ;
extern int   cDpi_getList   (char* objName, char* propName, int list[]) ;
extern int   cDpi_getList_  (char* objName, char* propName, int idx, int list[]) ;
extern char* cDpi_getString (char* objName, char* propName, char* defVal) ;
extern char* cDpi_getString_(char* objName, char* propName, int idx, char* defVal) ;

#ifdef __cplusplus
}
#endif







#endif









