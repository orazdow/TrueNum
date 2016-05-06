#ifndef TrueNum_h
#define TrueNum_h

#include <arduino.h>
#include "Client.h"

class TrueNum{

public:

TrueNum(String user, String pwd, String numSpace);
TrueNum(const char* user,const char* pwd,const char* numSpace,const char* ID);
void getConditionalStmt(char* in);

void getQuery(Client& inclient);
void makeCall(Client& inClient);
void callUniBox(Client& inclient);

void setVal(const char* intoken, float invalue);
void callUrl(char* statement, Client& inclient);
boolean getReturnBool();

float getVal(const char* intoken);
char* getToken(char* in);
float getTarget(char* in);
void replaceToken(char* in, float val);
void replaceChar(char* in, char inchar, const char* outstr);
uint8_t valueStart(char* in);
uint8_t getCondition(char* in);
uint8_t checkSpecialNum(char* in);
void doSpecialNum(char* in, Client& inClient);

static const uint8_t num = 4; //makes big mem diff... 
static const uint8_t len = 70;
char temps[num][len];

const char* user;
const char* pwd; 
const char* numSpace; 
static int delayTime; //change from static?
const char* baseUrl  = "pub.truenumbers.com";
const char* ID;
char statusreq[len];
boolean rtnbool;
struct Node
  {
    Node(): set(false){}     
    char* token;
    float value;
    boolean set;
  };
  
Node nodes[num];


};

#endif
