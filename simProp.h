#ifndef SIM_PROP_H_
#define SIM_PROP_H_

#include<iostream>
#include<list>
#include<string>
#include<ctype.h>
#include "simBase.h"

class simProp {
  public:
    randClass* randObj;
    std::list<simBase*> simValQ;

    uint8_t print;
    uint8_t uniq;
    uint8_t wtEn;
    uint8_t cyclic;

    std::string propVal;

    simProp() {}
    simProp(int seed) {
      randObj = new randClass(seed);
    }
    ~simProp() {
      while(!simValQ.empty())
      {
        delete simValQ.front();
        simValQ.pop_front();
      }
    }

    virtual void createSimSingle(int val, int wt=-1) {
      int cnt = (wt==-1) ? 1 : wt;
      simBase* simVal = new simSingle(val,cnt);
      simValQ.push_back(simVal);
#ifdef DEBUG
      printf("SIM_DBG: createSimSingle with val %0d, wt %0d \n",val,wt);
#endif
    }
    virtual void createSimString(std::string val, int wt=-1) {
      int cnt = (wt==-1) ? 1 : wt;
      simBase* simVal = new simString(val,cnt);
      simValQ.push_back(simVal);
#ifdef DEBUG
      printf("SIM_DBG: createSimString with val %s, wt %0d \n",val.c_str(),wt);
#endif
    }
    virtual void createSimRange(int min, int max, int wt=-1) {
      int cnt = (wt==-1) ? max-min+1 : wt;
      simBase* simVal = new simRange(min,min,max,cnt);
      simValQ.push_back(simVal);
#ifdef DEBUG
      printf("SIM_DBG: createSimRange with min %0d, max %0d, wt %0d \n",min,max,wt);
#endif
    }
    virtual void createSimUniqRange(int min, int max, int wt=-1) {
      int cnt = (wt==-1) ? max-min+1 : wt;
      simBase* simVal = new simUniqRange(min,min,max,cnt);
      simValQ.push_back(simVal);
#ifdef DEBUG
      printf("SIM_DBG: createSimUniqRange with min %0d, max %0d, wt %0d \n",min,max,wt);
#endif
    }
    
    virtual void initPropVal(std::string propVal, simType_t typ) {
      this->propVal = propVal;
      this->print   = typ.print;
      this->uniq    = typ.uniq;
      this->cyclic  = typ.cyclic;
      processPropVal();
    }

    virtual void processPropVal() {
      int val,min;
      int wt=-1;
      int wtStart=0;
      int rangeStart=0;
      std::string cchar,pchar,str;
      int stringStart=0;
      int hex=0;
      char ch;

#ifdef DEBUG
      printf("SIM_DBG: createSimProp for prop with val %s \n",this->propVal.c_str());
#endif
      for (std::string::iterator it=this->propVal.begin(); it!=this->propVal.end(); ++it) {
        ch = *it;
        switch(ch) {
          case '[': 
          case ' ': 
          case ']': break;
          case 'x':
          case 'X': if(pchar=="0") hex = 1;
                    break;
          case '#': wtStart=1;
                    std::sscanf(str.c_str(), hex ?"%x":"%d", &val);
                    str="";
                    break;
          case ';':
          case ',': if(wtStart==1) {
                      std::sscanf(str.c_str(), hex ?"%x":"%d", &wt);
                      wtStart=0;
                      this->wtEn = 1;
                    } else {
                      std::sscanf(str.c_str(), hex ?"%x":"%d", &val);
                      wt = -1;
                    }
                    if(rangeStart==1) {
                      if(this->uniq==1) 
                        createSimUniqRange(min,val,wt);
                      else
                        createSimRange(min,val,wt);
                    } else if (stringStart) {
                      createSimString(str,wt);
                    } else {
                      createSimSingle(val,wt);
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
          default : if(isalpha(ch) && (hex==0)) {
                      stringStart = 1;
                    }
                    str = str + *it;
                    break;
        }
        pchar = ch;
      }

      if(wtStart==1) {
        std::sscanf(str.c_str(), hex ?"%x":"%d", &wt);
        wtStart=0;
        this->wtEn = 1;
      } else {
        std::sscanf(str.c_str(), hex ?"%x":"%d", &val);
        wt = -1;
      }
      if(rangeStart==1) {
        if(this->uniq==1) 
          createSimUniqRange(min,val,wt);
        else
          createSimRange(min,val,wt);
      } else if (stringStart) {
        createSimString(str,wt);
      } else {
        createSimSingle(val,wt);
      }
      str="";
      hex=0;
      rangeStart=0;

    }

    virtual int getVal() {
      std::list<simBase*>::iterator it;
      int rtnVal;
      int seqDone=0;
      
      it = getOneIdx();
      if((uniq==1) && ((*it)->cnt == 1))
        seqDone = 1;
      //printf("GANGAM :: uniq %0d, cnt %0d seqDone %0d\n", uniq, (*it)->cnt,seqDone);
      (*it)->randomize(randObj);
      rtnVal = (*it)->getVal(uniq);
      if(seqDone==1) {
        delete *it;
        simValQ.erase(it);
      }
      return rtnVal;
    }

    virtual std::string getString() {
      std::list<simBase*>::iterator it;
      it = getOneIdx();
      simString* simStrPtr = dynamic_cast<simString*>(*it);
      if(simStrPtr) { //if casting not successfull it will be a NULL
        return simStrPtr->getString();
      } else {
        return propVal;
      }
    }

    virtual std::list<simBase*>::iterator getOneIdx() {
      int sum=0;
      int wt=0;
      std::list<simBase*>::iterator it;
      int size = simValQ.size();
      if(size < 2)
        return(simValQ.begin());
      else if (this->wtEn) {
        for(it=simValQ.begin(); it!=simValQ.end(); ++it)
          sum = sum+((*it)->cnt);
        wt = this->randObj->getRandVal(0,sum-1);
        //printf("GANGAM : sum %0d, wt %0d\n",sum,wt);
        sum = 0;
        for(it=simValQ.begin(); it!=simValQ.end(); ++it) {
          sum = sum+((*it)->cnt);
          if(sum>wt) return it;
        }
      } else {
        wt = this->randObj->getRandVal(0,size-1);
        it = simValQ.begin();
        for(int i=0;i<wt;i++) ++it;
        return it;
      }
      return it;
    }

    virtual int getList(int list[]) {
      std::list<simBase*>::iterator it;
      int i=0;
      for(it=simValQ.begin(); it!=simValQ.end(); ++it,i++) {
        list[i] = (*it)->getVal();
      }
      return(i); //i will be size of the Q.
    }

};


class simPropConst : public simProp {
  public:
    int currVal;
    int firstTimeCalled;

    simPropConst(int seed) {
      firstTimeCalled=0;
      randObj = new randClass(seed);
    }

    virtual int getVal(){
      std::list<simBase*>::iterator it;
      int rtnVal;
      if(firstTimeCalled==1) return currVal;
      
      it = getOneIdx();
      (*it)->randomize(randObj);
      rtnVal = (*it)->getVal();
     
      firstTimeCalled = 1;
      currVal = rtnVal;

      while(!simValQ.empty())
      {
        delete simValQ.front();
        simValQ.pop_front();
      }
      return rtnVal;
    }
};


class simPropSeq : public simProp {
  public:
    int currIdxCnt;
    int firstTimeCalled;

    simPropSeq(int seed) {
      firstTimeCalled=0;
      randObj = new randClass(seed);
    }

    virtual std::list<simBase*>::iterator getOneIdx() {
      return(simValQ.begin());
    }

    virtual void createSimUniqRange(int min, int max, int wt=-1) {
      createSimRange(min,max,wt);
    }

    virtual int getVal() {
      std::list<simBase*>::iterator it;
      int rtnVal;
      int seqDone=0;
      


      it = getOneIdx();
      if(this->firstTimeCalled == 0) {
        currIdxCnt = (*it)->cnt;
        this->firstTimeCalled=1;
     //     printf("init curr cnt %0d simVal cnt %0d \n",currIdxCnt,(*it)->cnt);
      }

      if((*it)->cnt == 1)
        seqDone = 1;

      rtnVal = (*it)->getVal(1);
      simRange* simRangePtr = dynamic_cast<simRange*>(*it);
      if(simRangePtr) { //if casting not successfull it will be a NULL
        simRangePtr->val = (seqDone==1) ? simRangePtr->min : simRangePtr->val+1;
      }
      if(seqDone==1) {
        if(this->uniq) {
          delete *it;
          simValQ.erase(it);
        } else {
       //   printf("curr cnt %0d simVal cnt %0d \n",currIdxCnt,(*it)->cnt);
          (*it)->cnt = currIdxCnt;
          simBase* basePtr = simValQ.front();
          simValQ.pop_front();
          simValQ.push_back(basePtr);
          it=simValQ.begin();
          currIdxCnt=(*it)->cnt;
        }
      }
      return rtnVal;
    }
        
};
  


















#endif
