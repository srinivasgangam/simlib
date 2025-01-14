#include <stdio.h>
#include "simdpi.h"
#include "assert.h"

int dpi_simRead_(char* fileName,int seed) {
  return(simRead_(fileName,seed));
}
int dpi_getVal(char* objName, char* propName, int defVal) {
  return(cDpi_getVal(objName,propName,defVal));
}
int dpi_getVal_(char* objName, char* propName, int idx, int defVal) {
  return(cDpi_getVal_(objName,propName,idx,defVal));
}
//int dpi_getList(char* objName, char* propName, const svOpenArrayHandle list) {
int dpi_getList(char* objName, char* propName, int* list) {
  return(cDpi_getList(objName,propName,list));
  //int size;
  //int lst[512];
  //int i;

  //svBitVecVal  sv_val;

  //int * arrptr  = svGetArrayPtr(list);
  //int arrnelems = svHigh(list,1) - svLow(list,1) + 1;
  //int arrsize   = svSizeOfArray(list);

  //assert(arrnelems >= size);
  //assert(arrptr != NULL);
  //assert(arrsize == arrnelems * sizeof( int ) );
  //assert(arrsize <= sizeof(lst));

  //for( i=0; i <size; i++){
  //  printf("lst[%0d] = %0d \n",i,lst[i]);
  //}


  ////printf("Array Left %d, Array Right %d \n\n", svLeft(list,1), svRight(list, 1) ); 
  ////for (i= svRight(list,1); i <= svLeft(list,1); i++) { 
  ////  printf("C: %d %d \n", i, *(int*)svGetArrElemPtr1(dyn_arr, i) ); 
  ////} 
  //for( i=0; i <size; i++){
  //  sv_val = lst[i];
  //  svPutBitArrElem1VecVal(list, &sv_val,i);
  //}

  //printf("GANGAM: dpi_getList_ %0d \n",size);
  //return size;
}
int dpi_getList_(char* objName, char* propName, int idx, int* list) {
  return (cDpi_getList_(objName,propName,idx,list));
}
char* dpi_getString(char* objName, char* propName, char* defVal) {
  return(cDpi_getString(objName,propName,defVal));
}
char* dpi_getString_(char* objName, char* propName, int idx, char* defVal) {
  return(cDpi_getString_(objName,propName,idx,defVal));
}
