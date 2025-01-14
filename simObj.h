

#ifndef SIM_OBJ_H_
#define SIM_OBJ_H_

#include<iostream>
#include<string>
#include<map>
#include<list>
#include "simBase.h"
#include "simProp.h"

using namespace std;

class simObj {
  public:
    map<std::string,simProp*> propLst;
    std::string objName;

    virtual void createProp(string propName, string propVal, int seed) {
      int end_done=0;
      string newPropVal="";
      simType_t typ = {0,0,0,0};
      char ch;
      for (std::string::iterator it=propVal.begin(); it!=propVal.end(); ++it) {
        ch = *it;
        switch(ch) {
          case '[': break;
          case ']': end_done=1; break;
          default : if(end_done==1) {
                      if     (ch=='s') typ.sequential = 1;
                      else if(ch=='u') typ.uniq       = 1;
                      else if(ch=='o') typ.cyclic     = 1;
                      else if(ch=='p') typ.print      = 1;
                      else if(ch=='c') typ.constant   = 1;
                      else cout << "SIM Error : unknown type for property " << propName <<  ", propVal " << propVal << "\n";
                    } else 
                    newPropVal = newPropVal + *it;
                    break;
        }
      }
      if(typ.sequential == 1) {            
        propLst[propName] = new simPropSeq(seed);
#ifdef DEBUG
        printf("createProp : creating simPropSeq prop %s, val %s\n",propName.c_str(),newPropVal.c_str());
#endif
      } else if (typ.constant == 1) {
        propLst[propName] = new simPropConst(seed);
#ifdef DEBUG
        printf("createProp : creating simPropConst prop %s, val %s\n",propName.c_str(),newPropVal.c_str());
#endif
      } else {
        propLst[propName] = new simProp(seed);
#ifdef DEBUG
        printf("createProp : creating simProp prop %s, val %s\n",propName.c_str(),newPropVal.c_str());
#endif
      }
      propLst[propName]->initPropVal(newPropVal,typ);

    }
};



#endif
