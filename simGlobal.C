#include<string>
#include<iostream>
#include<sstream>
#include<map>
#include "simBase.h"
#include "simProp.h"
#include "simObj.h"
#include "simParse.h"
#include "simGlobal.h"
using namespace std;

map<string,simObj*> objLst;
int initial_seed;

int simRead_(char* fileName, int seed) {
  initial_seed = seed;
  cout << "Reading test knob file "<<fileName<<" with seed " <<seed <<"\n";
  return(simRead(fileName));
}


void createProp(string objName, string propName, string propVal) {
  int seed = initial_seed;
  char ch;
  if(objLst.find(objName) == objLst.end())
    objLst[objName] = new simObj;
  
  for (std::string::iterator it=propName.begin(); it!=propName.end(); ++it) {
    ch = *it;
    seed = seed + ch;
  };
  for (std::string::iterator it=objName.begin(); it!=objName.end(); ++it) {
    ch = *it;
    seed = seed + ch;
  };
#ifdef DEBUG
  printf("SIM_DBG: Creating obj %s, prop %s with val %s with seed %d\n",objName.c_str(),propName.c_str(),propVal.c_str(),seed);
#endif
  objLst[objName]->createProp(propName,propVal,seed);
}


std::string eraseBlanks(std::string s) {
  s.erase( remove( s.begin(), s.end(), ' ' ), s.end() );
  return s;
}

void objPropExpand(string objName,string propName,string propVal,
    int isObj, std::size_t startPos, std::size_t endPos, string rangeStr) {
  std::string str,newStr,pchar;
  char ch;
  int rangeStart=0;
  int hex=0;
  int min,val;
  char digStr[32];

  for (std::string::iterator it=rangeStr.begin(); it!=rangeStr.end(); ++it) {
    ch = *it;
    switch(ch) {
      case 'x':
      case 'X': if(pchar=="0") hex = 1;
                  break;
      case ')':
      case ',': std::sscanf(str.c_str(), hex ?"%x":"%d", &val);
                if(rangeStart==0) {
                  min = val;
                }
                for(int i=min;i<=val;i++) {
                  sprintf(digStr,"%0d",i);
                  if(isObj==1) {
                    newStr = objName;
                    cout << "GANGAM objName " << newStr << " rangeStr " << rangeStr << " idx " << digStr << " start " << startPos << endl;
                    newStr.replace(startPos,endPos-startPos+1,digStr);
                    cout << "GANGAM objName " << newStr << " rangeStr " << rangeStr << " idx " << digStr << endl;
                    createObjProp_(newStr,propName,propVal);
                  } else {
                    newStr = propName;
                    cout << "GANGAM propName " << newStr << " rangeStr " << rangeStr << " idx " << digStr << " start " << startPos << endl;
                    newStr.replace(startPos,endPos-startPos+1,digStr);
                    cout << "GANGAM propName " << newStr << " rangeStr " << rangeStr << " idx " << digStr << endl;
                    createObjProp_(objName,newStr,propVal);
                  }
                }
                str="";
                hex=0;
                rangeStart=0;
                break;
      case '.': if(pchar==".") {
                  std::sscanf(str.c_str(), hex ?"%x":"%d", &min);
                  str = "";
                  rangeStart = 1;
                  hex = 0;
                }
                break;
      case ' ':
      case '(': break;
      default : str = str + *it;
                break;
    }
    pchar = ch;
  }
}
//till %d exists either in objName or propName call the expand function recursively.
void createObjProp_(string objName, string propName, string propVal) {
  std::size_t startPos,endPos;
  std::string rangeStr;

  startPos = objName.find("%d");
  if(startPos != std::string::npos) {
    endPos = objName.find(")");
    rangeStr = objName.substr(startPos+2,endPos);
    //cout << "GANGAM objName " << objName << " rangeStr" << rangeStr << endl;
    //objName.replace(startPos,endPos-startPos+1," ");
    //cout << "GANGAM objName " << objName << " rangeStr" << rangeStr << endl;
    objPropExpand(objName,propName,propVal,1,startPos,endPos,rangeStr);
    return;
  }

  startPos = propName.find("%d");
  if(startPos != std::string::npos) {
    endPos = propName.find(")");
    rangeStr = propName.substr(startPos+2,endPos);
    //cout << "GANGAM propName " << propName << " rangeStr" << rangeStr << endl;
    //propName.replace(startPos,endPos-startPos+1," ");
    //cout << "GANGAM propName " << propName << " rangeStr" << rangeStr << endl;
    objPropExpand(objName,propName,propVal,0,startPos,endPos,rangeStr);
    return;
  }
  createProp(objName,propName,propVal);
}

void createObjPropPre(string objName, string propName, string propVal) {
  createObjProp_(eraseBlanks(objName),eraseBlanks(propName),eraseBlanks(propVal));
}


void createObjProp(char* objName, char* propName, char* propVal) {
#ifdef DEBUG
  printf("SIM_DBG: Creating obj %s, prop %s with val %s\n",objName,propName,propVal);
#endif
  createObjPropPre(objName,propName,propVal);

}

int getVal(std::string objName, std::string propName, int defVal) {
   int rtnVal;
   simProp* simPropPtr;
   //#ifdef OBJ_CREATE_RUNTIME
   //initObj(objName,propName);
   //#endif
   map<string,simObj*>::iterator it;
#ifdef DEBUG
   printf("getVal : objName %s, propName %s, defVal %0d \n",objName.c_str(),propName.c_str(),defVal); 
#endif
   it = objLst.find(objName);
   if(it != objLst.end()) {
     if(objLst[objName]->propLst.find(propName) != objLst[objName]->propLst.end()) {
       simPropPtr = objLst[objName]->propLst[propName];
       if((simPropPtr->simValQ.size()==0)&&(simPropPtr->uniq==1)) {
         if(simPropPtr->cyclic==0) {
           cout << "\nSim Error : ObjName: "<<objName<< ", propName "<<propName<<" :All the range values for this uniq property are used \n";
           return -1;
         } else {
           simPropPtr->processPropVal();
         }
       }
       rtnVal = simPropPtr->getVal();
       if(simPropPtr->print) {
         printf("getVal() :: %s.%s = %0d , 0x%0x \n",objName.c_str(),propName.c_str(),rtnVal,rtnVal);
       }
       return rtnVal;
     } else {
#ifdef DEBUG
       cout << "SIM_DBG : getVal() :: Returning defualt value as PROP "<<propName<<" does not exist under OBJ "<< objName << endl; 
#endif
       return defVal;
     }
   } else {
#ifdef DEBUG
     cout << "SIM_DBG : getVal() :: Returning defualt value as OBJ "<<objName<<" does not exist\n";
#endif
     return defVal;
   }
}

int getVal_(std::string objName, std::string propName, int idx, int defVal) {
  int rtnVal;
  char extPropName[256]; //assuming propName not more than 256 characters.
  string str;
  sprintf(extPropName, "%s[%0d]",propName.c_str(),idx);
  rtnVal = getVal(objName,extPropName,-1);
  if(rtnVal == -1) 
    rtnVal = getVal(objName,propName,defVal);
  return rtnVal;
}

int getList(std::string objName, std::string propName, int* list) {
  int rtnVal;
   simProp* simPropPtr;
   //#ifdef OBJ_CREATE_RUNTIME
   //initObj(objName,propName);
   //#endif
   map<string,simObj*>::iterator it;
   it = objLst.find(objName);
   if(it != objLst.end()) {
     if(objLst[objName]->propLst.find(propName) != objLst[objName]->propLst.end()) {
       simPropPtr = objLst[objName]->propLst[propName];
       rtnVal = simPropPtr->getList(list);
       if(simPropPtr->print) {
         printf("getList() %s.%s list[%0d] = ",objName.c_str(),propName.c_str(),rtnVal);
         for(int i=0;i<rtnVal;i++) 
           printf("%0d ",list[i]);
         printf("\n");
       }
       return rtnVal;
     } else {
       return 0;
     }
   } else {
     return 0;
   }

}


int getList_(std::string objName, std::string propName, int idx, int list[]) {
  int rtnVal;
  char extPropName[256];
  sprintf(extPropName, "%s[%0d]",propName.c_str(),idx);
  rtnVal = getList(objName,extPropName,list);
  if(rtnVal == 0) 
    rtnVal = getList(objName,propName,list);
  return rtnVal;
}


std::string getString(std::string objName, std::string propName, std::string defVal) {
   std::string rtnVal;
   simProp* simPropPtr;
   //#ifdef OBJ_CREATE_RUNTIME
   //initObj(objName,propName);
   //#endif
   map<string,simObj*>::iterator it;
   it = objLst.find(objName);
   if(it != objLst.end()) {
     if(objLst[objName]->propLst.find(propName) != objLst[objName]->propLst.end()) {
       simPropPtr = objLst[objName]->propLst[propName];
       rtnVal = simPropPtr->getString();
       if(simPropPtr->print) {
         cout << "getString() :: "<<objName<<"."<<propName<<" = "<<rtnVal<<endl;
       }
       return rtnVal;
     } else {
#ifdef DEBUG
       cout << "SIM_DBG : getString() :: Returning defualt value as PROP "<<propName<<" does not exist under OBJ "<< objName << endl; 
#endif
       return defVal;
     }
   } else {
#ifdef DEBUG
     cout << "SIM_DBG : getString() :: Returning defualt value as OBJ "<<objName<<" does not exist\n";
#endif
     return defVal;
   }
}

std::string getString_(std::string objName, std::string propName, int idx, std::string defVal) {
  std::string rtnVal;
  char extPropName[256];
  sprintf(extPropName, "%s[%0d]",propName.c_str(),idx);
  rtnVal = getString(objName,extPropName,"null");
  if(rtnVal == "null") 
    rtnVal = getString(objName,propName,defVal);
  return rtnVal;
}

int   cDpi_getVal    (char* objName, char* propName, int defVal) {
  std::string obj(objName);
  std::string prop(propName);
  return(getVal(obj,prop,defVal));
}
int   cDpi_getVal_   (char* objName, char* propName, int idx, int defVal) {
  std::string obj(objName);
  std::string prop(propName);
  return(getVal_(obj,prop,idx,defVal));
}
int   cDpi_getList   (char* objName, char* propName, int* list) {
  std::string obj(objName);
  std::string prop(propName);
  int rtnVal = (getList(obj,prop,list));
  printf("GANGAM cDpi_getList rtnVal = %0d\n",rtnVal);
  return(rtnVal);
}
int   cDpi_getList_  (char* objName, char* propName, int idx, int list[]) {
  std::string obj(objName);
  std::string prop(propName);
  return(getList_(obj,prop,idx,list));
}
char* cDpi_getString (char* objName, char* propName, char* defVal) {
  std::string obj(objName);
  std::string prop(propName);
  std::string def(defVal);
  std::string rtn = getString(obj,prop,def);
  return(const_cast<char *>(rtn.c_str()));
}
char* cDpi_getString_(char* objName, char* propName, int idx, char* defVal) {
  std::string obj(objName);
  std::string prop(propName);
  std::string def(defVal);
  std::string rtn = getString_(obj,prop,idx,def);
  //return(rtn.c_str());
  return(const_cast<char *>(rtn.c_str()));
}
















