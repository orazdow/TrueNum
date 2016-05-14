#ifndef TrueNum_h
#define TrueNum_h

#include <arduino.h>
#include "Client.h"

class TrueNum{

public:

TrueNum(const char* user,const char* pwd,const char* numSpace,const char* ID);
TrueNum(const char* user,const char* pwd,const char* numSpace);
void setID(const char* ID);

void getQuery(Client& inclient);
void makeCall(Client& inClient);
void makeCall(const char* in, Client& inClient);
void setVal(const char* intoken, float invalue);
boolean getReturnBool();
float getVal(const char* intoken);
private:

#ifdef __AVR__
static const uint8_t num = 4; //makes big mem diff... 
#else
static const uint8_t num = 8; //makes big mem diff... 
#endif

static const uint8_t len = 70;
char temps[num][len];

struct Node
  {
    Node(): set(false){}     
    char* token;
    float value;
    boolean set;
  };  
Node nodes[num];

const char* user;
const char* pwd; 
const char* numSpace; 
static int delayTime; //change from static?
const char* baseUrl  = "pub.truenumbers.com";
const char* ID;
char statusreq[len];
boolean rtnbool;

#ifndef __AVR__
   const char* urlleadin = "GET /Numberflow/API?auth=";
   const char* nsparam = "&ns=";
   const char* queryParams = "&sto=1&string=notags&cmd=dashboard-search&qry=parameter:"; 
   const char* callParams = "&sto=1&cmd=send-tspeak&tspeak="; 
   const char* uniParams = "&sto=1&cmd=enter-unibox&ubox=status-report+of+";
   const char* uniParams2 = "+=+%22note%22//Tokens:+";
#endif

void callUniBox(Client& inclient);
void callUrl(char* statement, Client& inclient);
uint8_t getCondition(char* in);
void getConditionalStmt(char* in);
uint8_t checkSpecialNum(char* in);
void doSpecialNum(char* in, Client& inClient);
char* getToken(char* in);
float getTarget(char* in);
void replaceToken(char* in, float val);
void replaceChar(char* in, char inchar, const char* outstr);
uint8_t valueStart(char* in);

};

#endif
