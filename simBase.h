#ifndef SIM_BASE_H
#define SIM_BASE_H


#include <iostream>
#include <list>
#include <map>

 typedef struct packed{
   int sequential;
   int uniq;
   int cyclic;
   int constant;
   int print;
} simType_t;


class randClass {
  public: 
  int lfsrSeed;
   randClass(int seed) {
     lfsrSeed = seed;
   }
   int getRandVal(int min, int max) {
      //printf("before lfsrSeed %0d\n",lfsrSeed);
      srand(lfsrSeed);
      this->lfsrSeed = rand();
      //printf("lfsrSeed %0d\n",lfsrSeed);
      return (min+(lfsrSeed%(max-min+1)));
   }
   int getRandVal() {
      srand(lfsrSeed);
      lfsrSeed = rand();
      return (lfsrSeed);
   }
};

class simBase {
  public:
    int cnt;
    simBase() {};
    virtual ~simBase() {};
    virtual int getVal(int cnt_en=0) { return 0; }
    virtual void randomize(randClass* randObj) {}
    virtual std::string getString() {
      return "null";
    }
};


class simSingle : public simBase {
  public:
    int val;
    simSingle(int val, int cnt) {
      this->val = val;
      this->cnt = cnt;
    }
    ~simSingle() {}
    virtual int getVal(int cnt_en=0) {
      if(cnt_en==1) {
        cnt = (cnt==1) ? 1 : cnt-1;
      }
      return val;
    }
};

class simString : public simBase {
  public:
    std::string val;
    simString(std::string val, int cnt) {
      this->val = val;
      this->cnt = cnt;
    }
    virtual std::string getString() {
      return val;
    }
};

class simList : public simBase {
  public:
    std::list<int> valLst;
    int getList(int list[]) {
      int i=0;
      std::list<int>::iterator it;
      for (i=0, it=valLst.begin(); it!=valLst.end(); ++it,i++) {
        list[i] = *it;
      }
      return i;
    }
};

class simRange : public simBase {
  public:
    int min,max,val;
 
    simRange(int val, int min, int max, int cnt) {
      this->val = val;
      this->min = min;
      this->max = max;
      this->cnt = cnt;
    }
    virtual void randomize(randClass* randObj) {
      val = randObj->getRandVal(min,max);
    }
    virtual int getVal(int cnt_en=0) {
      if(cnt_en==1) {
        cnt = (cnt==1) ? max-min+1 : cnt-1;
      }
      return val;
    }
};


class simUniqRange : public simBase {
  public:
    int min,max,val;
    std::map<int,short int> genLst;
    simUniqRange(int val, int min, int max, int cnt) {
      this->val = val;
      this->min = min;
      this->max = max;
      this->cnt = cnt;
    }
    virtual void  randomize(randClass* randObj) {
      bool exists=true;
      int  loops=0;
      while(exists && (cnt>0)) {
        this->val = randObj->getRandVal(min,max);
        if(genLst.find(val) == genLst.end()) {
          genLst[val] = 1;
          break;
        }
        loops++;
        if(loops == 256*(max-min)) { //loop some fixed number of time to get a value which not allocated yet
          printf("Error :: Uniqness Randomization failed\n");
          break;
        }
      }
    }
    virtual int getVal(int cnt_en=0) {
      if(cnt_en == 1) {
        cnt = (cnt==1) ? max-min+1 : cnt-1;
      }
      return val;
    }
};















#endif
