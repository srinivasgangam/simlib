%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simParse.h"
#define YY_NO_UNPUT
#ifdef __cplusplus
extern "C" {
#endif
  void setCurrentObj (char* name);
  void setCurrentProp(char* name);
  void setCurrentVal (char* name);
  void processProp(char* objName, char* propName, char* propVal);
  void createObjProp(char* objName, char* propName, char* propVal);
#ifdef __cplusplus
}
#endif


%}
%s OBJ
%s PROP
%s EQ
%s VAL
%s INCLUDE

obj     \@[_a-zA-Z0-9]+
objSb   \@[^\n\t ]*\[[^\]\n]*\]
objNows \@[^\n\t ]+

propExpand      [_0-9a-zA-Z][^\=\/\n\t]*
prop            [_0-9a-zA-Z][^\=\/\n\t\%#@]*


eq      [\t ]*\=[\t ]*
comment [\t ]*\/\/.*
val     \[?[_0-9a-zA-Z]*\-?".."?\;?\,?[_0-9a-zA-Z]*\]?[_0-9a-zA-Z]?
include \#include

step            [+-]?(0[xX])*[0-9a-fA-F]+(h)*
valSb           \[((\/)?[^\n\/]+)+\][A-Za-z0-9\|\-\:]*([\t ]*{step})*
valRb           \(((\/)?[^\n\/]+)+\)([\t ]*{step})*
valDq           \"((\/)?[^\n\/]+)+\"
valNows         \{*[^\n\t\{\<\[\(\"\' ]+[^\n\t ]*
    

%%
<INITIAL>{objSb}      {setCurrentObj(&yytext[1]);}
<INITIAL>{objNows}    {setCurrentObj(&yytext[1]);}
<INITIAL>{obj}        {setCurrentObj(&yytext[1]);}
<INITIAL>{prop}       {BEGIN EQ; setCurrentProp(yytext);}
<INITIAL>{propExpand} {BEGIN EQ; setCurrentProp(yytext);}
<INITIAL>{comment}           ;

<EQ>{eq}              {BEGIN VAL;}
<VAL>{valNows}        {BEGIN INITIAL; setCurrentVal(yytext);}
<VAL>{valSb}          {BEGIN INITIAL; setCurrentVal(yytext);}
<VAL>{valRb}          {BEGIN INITIAL; setCurrentVal(yytext);}
<VAL>{valDq}          {BEGIN INITIAL; setCurrentVal(yytext);}
<VAL>{obj}            {BEGIN INITIAL; setCurrentObj(&yytext[1]);}
<VAL>{comment}        {printf("Error: No value for the property VAL %s\n",yytext);}

%%

//<PROP>{prop}          {BEGIN VAL; setCurrentProp(&yytext[1]);}
//<PROP>{comment}           ;

//prop    \+[_0-9a-zA-Z.%\(\)\[\]]*\[?\]?*
//propExpand      \+[_0-9a-zA-Z][^\/\n\t]*
extern void svProcessProp(char* objName, char* propName, char* propVal);

char objName [64];
char propName[64];
char propVal [128];
int   objSeen = 0;
int   propSeen = 0;
//<HEAT>{wrongHeat}    {BEGIN INITIAL; printf("Error: Wrong option %s: heat value <on/off> should be used\n",yytext);}
void setCurrentVal(char* name) {
 if((objSeen==0) || (propSeen==0)) {
    printf("Error : No prior object/property is seen but the propert value %s is sent\n",name);
 } else {
   propSeen = 0;
   strcpy(propVal, name);
   //printf("Value is ||%s|| \n", propVal);
   processProp(objName,propName,propVal);
 }
}
void setCurrentProp(char* name) {
 if(objSeen == 0) {
    printf("Error : Object is not set but propert value is seen for propert %s\n",name);
 } else if(propSeen == 1) {
    printf("Error : Value is not sent for the property %s, but next propert %s seen\n", propName,name);
 } else {
   propSeen = 1;
   strcpy(propName, name);
   //printf("PROP is ||%s|| \n", propName);
 }
}
void setCurrentObj(char* name) {
 if(propSeen == 1) {
    printf("Error : Value is not sent for the property %s but next object seen\n", propName);
 } else {
   objSeen = 1;
   strcpy(objName, name);
   //printf("OBJ is ||%s|| \n", objName);
 }
}
void processProp(char* objName, char* propName, char* propVal) {
   //svProcessProp(objName,propName,propVal);
   createObjProp(objName,propName,propVal);
}

int yywrap() {
  return 1;
}

int simRead(char *fileName)
{
  FILE * scFptr;
  scFptr = fopen(fileName,"r");
  if (scFptr==NULL) {
    printf("ERROR!!! failed to open file %s\n", fileName);
    return 0;
  }
  printf("Processing SimFile %s \n", fileName);
  yyin=scFptr;
        yylex();
  printf("Done Processing SimFile %s \n", fileName);
        return 1;
}


