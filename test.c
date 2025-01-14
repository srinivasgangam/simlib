



#include<iostream>
#include "simdpi.h"
using namespace std;

#define repeat(M) for(int i=0;i<M;i++)

int main() {
  char* filename;
int array[16];
string str;
  filename = "test.sim";
  
  simRead_(filename, rand());


 printf("obj1.value1 %0d \n",cDpi_getVal("OBJ1","value1",123));
 for(int i=0;i<10;i++) 
 printf("OBJ1-value1  = %0d\n",cDpi_getVal("OBJ1","value1",111));


 printf("OBJ1-value10 = %0d\n",cDpi_getVal("OBJ1","value10",456));
 printf("OBJ2-value1  = %0d\n",cDpi_getVal("OBJ2","value1",4561));
 printf("OBJ3-value1  = %0d\n",cDpi_getVal("OBJ3","value1",4561));


 for(int i=0;i<10;i++) {
 printf("OBJ1-value4  = %0d\n",cDpi_getVal("OBJ1","value4",111));
 }
 printf("OBJ1-value5 = ");
 repeat(5)
 printf(" %0d ",cDpi_getVal("OBJ1","value5",111));
 printf("\n");

 printf("OBJ1-value7 = ");
 repeat(20)
 printf(" %0d ",cDpi_getVal("OBJ1","value7",112));
 printf("\n");

 printf("OBJ1-value8 = ");
 repeat(20)
 printf(" %0d ",cDpi_getVal("OBJ1","value8",113));
 printf("\n");

 printf("OBJ1-valueSeq = ");
 repeat(40)
 printf(" %0d ",cDpi_getVal("OBJ1","valueSeq",113));
 printf("\n");
 
 printf("OBJ1-valueSeqU = ");
 repeat(14)
 printf(" %0d ",cDpi_getVal("OBJ1","valueSeqU",113));
 printf("\n");

 cout << "OBJ1-string1 = " << cDpi_getString("OBJ1","string1","nothing");
 cout << "\n";
 cout << "OBJ1-string2 = " << cDpi_getString("OBJ1","string2","nothing");
 cout << "\n";
 cout << "OBJ1-string2 = " << cDpi_getString("OBJ1","string2","nothing");
 cout << "\n";
 cout << "OBJ1-value8  = " << cDpi_getString("OBJ1","value8" ,"nothing");
 cout << "\n";

 cDpi_getList("OBJ1","list",array);

 printf("OBJ1-wtVal = ");
 repeat(30)
 printf(" %0d ",cDpi_getVal("OBJ1","wtVal",113));
 printf("\n");
 printf("OBJ1-wtVal1 = ");
 repeat(30)
 printf(" %0d ",cDpi_getVal("OBJ1","wtVal1",113));
 printf("\n");
 printf("OBJ1-wtValSeq = ");
 repeat(30)
 printf(" %0d ",cDpi_getVal("OBJ1","wtValSeq",113));
 printf("\n");
 printf("OBJ1-wtVal1Seq = ");
 repeat(30)
 printf(" %0d ",cDpi_getVal("OBJ1","wtVal1Seq",113));
 printf("\n");
 printf("OBJ1-wtVal1SeqU = ");
 repeat(30)
 printf(" %0d ",cDpi_getVal("OBJ1","wtVal1SeqU",113));
 printf("\n");
 //str= "ffffffff";
 //printf("atoi.-10 %d",str.atohex());
 repeat(5)
 printf("OBJ2 new_val_arr[0] = %0d ",cDpi_getVal_("OBJ2","new_val_arr",0,113));
 printf("\n");
 repeat(5)
 printf("OBJ2 new_val_arr[1] = %0d ",cDpi_getVal_("OBJ2","new_val_arr",1,113));
 printf("\n");
 repeat(5)
 printf("OBJ2 new_val_arr[4] = %0d ",cDpi_getVal_("OBJ2","new_val_arr",4,113));
 printf("\n");
 repeat(5)
 printf("OBJ2 new_val_arr[10] = %0d ",cDpi_getVal_("OBJ2","new_val_arr",0xa,113));
 printf("\n");
 printf("OBJ2 uniqVal =");
 repeat(50)
 printf("%x ",cDpi_getVal("OBJ2","uniq_val",113));
 printf("\n");

}

//void createObjProp(char* objName,char* propName, char* propVal) {
//  cout << "creating objProp for " << objName << propName << propVal << endl;
//}

