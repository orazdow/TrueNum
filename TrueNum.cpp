#include "TrueNum.h"

int TrueNum::delayTime = 8000;

TrueNum::TrueNum(const char* user,const char* pwd,const char* numSpace,const char* ID){
    this->user = user;
    this->pwd = pwd;
    this->numSpace = numSpace;
    this->ID = ID;
}

TrueNum::TrueNum(const char* user,const char* pwd,const char* numSpace){
    this->user = user;
    this->pwd = pwd;
    this->numSpace = numSpace;
}

void TrueNum::setID(const char* ID){
    this->ID = ID;
}

boolean TrueNum::getReturnBool(){
  return rtnbool;
}

void TrueNum::getQuery(Client& inclient){
 int a = -1; int x = 0; boolean spec = false;
  if (inclient.connect(baseUrl, 80)){   
   
   #ifdef __AVR__
    inclient.print(F("GET "));  
    inclient.print(F("/Numberflow/API?auth="));
    inclient.print(user); 
    inclient.print(F(":")); 
    inclient.print(pwd); 
    inclient.print(F("&ns=")); 
    inclient.print(numSpace); 
    inclient.print(F("&sto=1&string=notags&cmd=dashboard-search&qry=parameter:"));
    inclient.print(ID);  
   #else 
    inclient.print(urlleadin);  
    inclient.print(user); 
    inclient.print(":"); 
    inclient.print(pwd); 
    inclient.print(nsparam); 
    inclient.print(numSpace); 
    inclient.print(queryParams);
    inclient.print(ID); 
   #endif 
    inclient.println(); 
    delay(500); 
  }else {
    Serial.println("connection failed");
  }
    while(inclient.available()) {
    char c = inclient.read(); 
    // Serial.print(c);  
    if(c == ','){
    if(a != -1) { 
     temps[a][x] = '\0';  
     x = 0;  
     spec = false;
     if(checkSpecialNum(temps[a]))
      { 
      doSpecialNum(temps[a], inclient);  
      spec = true; 
      temps[a][0] = 0; //memset(temps[a], 0, len); 
      }
     }     

      if(a < num-1){  
      if(!spec){
      a++;
      }
      }else{
         Serial.println("\nlimit reached"); break;
      }
    }
    
      if(c != '{')
      if(c != '}')
      if(c != '[')
      if(c != ']')    
      if(c != '\"')
      if(c != ',')
      if(c != '\\')
      if(c != '\0')
      if(a != -1)
      if(a < num)
      {   
      temps[a][x] = c; 
      if(x < len-1)
      x++; 
     }

  }  
      //remove json info from first num
      if(strstr(temps[0], "truenumbers:")) 
      memmove(temps[0],temps[0]+12,len-12);

      //handle if first slot is special num
      if( checkSpecialNum(temps[a])){
        doSpecialNum(temps[a], inclient); 
        temps[a][0] = 0; //memset(temps[a], 0, len); 
        }

      //null remaining entries
      for(uint8_t i = a+1; i < num; i++)
       temps[i][0] = 0; //memset(temps[i], 0, len); 

      //print queried nums
      Serial.println(); 
      for(uint8_t i = 0; i < num; i++)
      if(temps[i][0]!= 0)
      Serial.println(temps[i]);
      Serial.println(); 

      //flush serial buffer
      while(inclient.available()) { inclient.read(); }    
}

void TrueNum::callUrl(char* statement, Client& inclient){
    replaceChar(statement, '%', "%25");
    replaceChar(statement, ' ', "+");
 
    #ifdef __AVR__
    Serial.println(F("calling:")); 
    #else
    Serial.println("calling:"); 
    #endif
    Serial.println(statement); 
    
 if (inclient.connect(baseUrl, 80)){ 

  #ifdef __AVR__
    inclient.print(F("GET "));  
    inclient.print(F("/Numberflow/API?auth=")); 
    inclient.print(user); 
    inclient.print(F(":")); 
    inclient.print(pwd); 
    inclient.print(F("&ns=")); 
    inclient.print(numSpace); 
    inclient.print(F("&sto=1&cmd=send-tspeak&tspeak=")); 
    inclient.print(statement);
  #else
    inclient.print(urlleadin);  
    inclient.print(user); 
    inclient.print(":"); 
    inclient.print(pwd); 
    inclient.print(nsparam); 
    inclient.print(numSpace); 
    inclient.print(callParams); 
    inclient.print(statement);
  #endif
    inclient.println();
    delay(500);
  } else {
    Serial.println("connection failed");
  }
  
  while(inclient.available()) {
    char c = inclient.read();
    Serial.print(c);
  }
  
}

void TrueNum::callUniBox(Client& inclient){
   //tokens used, report interval
if (inclient.connect(baseUrl, 80)){ 
  #ifdef __AVR__
    inclient.print(F("GET "));  
    inclient.print(F("/Numberflow/API?auth=")); 
    inclient.print(user); 
    inclient.print(F(":")); 
    inclient.print(pwd); 
    inclient.print(F("&ns=")); 
    inclient.print(numSpace); 
    inclient.print(F("&sto=1&cmd=enter-unibox&ubox=")); 
    inclient.print(F("status-report+of+"));
    inclient.print(ID);
    inclient.print(F("+=+%22note%22//"));
    inclient.print(F("Tokens:+"));
 #else
    inclient.print(urlleadin);  
    inclient.print(user); 
    inclient.print(":"); 
    inclient.print(pwd); 
    inclient.print(nsparam); 
    inclient.print(numSpace);
    inclient.print(uniParams);
    inclient.print(ID);
    inclient.print(uniParams2);
 #endif
  
  for(uint8_t i = 0; i < num; i++){
    if(nodes[i].token != NULL){
    inclient.print(nodes[i].token);
    inclient.print('+');
      }
    }
  #ifdef __AVR__     
    inclient.print(F("+Interval:+"));
  #else
    inclient.print("+Interval:+");
  #endif
    float ds = delayTime / 1000;
    inclient.print(ds);   
    inclient.println(); 
    delay(500); 
    
  } else {
    Serial.println("connection failed");
  } 
    //flush serial buffer
    while(inclient.available()) { inclient.read(); }
}

void TrueNum::makeCall(Client& inClient){
  
  for(uint8_t i = 0; i < num; i++){ 
     if(temps[i][0] != 0){

        if(getCondition(temps[i]) == 0)
        {            
             replaceToken( temps[i], getVal(getToken(temps[i])) );    
        } 
        else{ 
             getConditionalStmt(temps[i]);           
           }
        if(temps[i][0]!= 0)
        {
          callUrl(temps[i], inClient);         
        } 
       }
  }     
        delay(delayTime);
}

void TrueNum::makeCall(const char* in, Client& inClient){

     static char callBuff[len];
     memcpy(callBuff, in, len);

     if(callBuff[0] != 0){

        if(getCondition(callBuff) == 0)
        {            
          replaceToken( callBuff, getVal(getToken(callBuff)) );    
        } 
        else{
            getConditionalStmt(callBuff);         
        }
        if(callBuff[0] != 0)
        {
          callUrl(callBuff, inClient);         
        } 
       }  
        delay(delayTime);       
}

void TrueNum::getConditionalStmt(char* in){

    uint8_t state = getCondition(in);
    float val = getVal(getToken(in));
    float target = getTarget(in);
    boolean check = false;
    //nocond-0, ==-1, >-2, <-3, >=-4, <=-5
     
  if( state == 3){ 
      if( val < target ){  
          replaceToken(in, val); 
          check = true;
      } 
    }
  else if( state == 2){  
      if( val > target ){
          replaceToken(in, val);
          check = true;
      } 
    }
  else if( state == 1){ 
      if( abs(val - target) < 0.001 ){  
          replaceToken(in, val);
          check = true;
      } 
    }
  else if( state == 4){ 
      if( val > target || ( abs(val - target) < 0.001 ) ){ 
          replaceToken(in,val);
          check = true;
      } 
    }
  else if( state == 5){ 
      if( val < target || ( abs(val - target) < 0.001 ) ){ 
          replaceToken(in, val);
          check = true;
      } 
    } 
         if(!check){
         in[0] = 0;
         }else
         {
        char* p = NULL;
        uint8_t sp = 0;
        char cset[] = "<>= .0123456789"; 
        p = strchr(in, '>');
        if(!p)
        p = strchr(in, '<');
        if(!p)
        p = strstr(in, "==");
        if(p){
        sp = strspn(p, cset); 
        memmove(p, p+sp, sp); 
        }
        }
}

uint8_t TrueNum::checkSpecialNum(char* in){
     uint8_t rtn = 0;

      if(strstr(in, "status-report"))
          rtn = 1; 

      else if(strstr(in, "report-interval"))
          rtn = 2; 

      else if(strstr(in, "return-bool"))
          rtn = 3; 
   
  return rtn;
}

void TrueNum::doSpecialNum(char* in, Client& inClient){
  
    uint8_t state = checkSpecialNum(in);
    
    if(state == 1){  
     if( strcmp(in, statusreq) != 0){
     #ifdef __AVR__ 
      Serial.println(F("\nsending status report...\n"));
     #else
     Serial.println("\nsending status report...\n");
     #endif
      callUniBox(inClient);
      memcpy(statusreq, in, len);
      
       }   
     }  
            
    else if(state == 2){ 
       int val = getTarget(in);
       if(val)
       if( val*1000 != delayTime ){
       delayTime = val*1000; 
       Serial.print("\ndelay set to ");
       Serial.print(val);
       Serial.print(" seconds");
       Serial.println();       
       }  
      }

    else if(state == 3){
     uint8_t vs = valueStart(in);
      if(strstr(in+vs, "true")){
        rtnbool = true;
      }
      else if(strstr(in+vs, "false")){
        rtnbool = false;
      }
      else{
     getConditionalStmt(in);
     if(in[0] != 0){
      rtnbool = true;
     }else{rtnbool = false;}
     }
   }
}

uint8_t TrueNum::getCondition(char* in){
 //nocond-0, ==-1, >-2, <-3, >=-4, <=-5
   uint8_t rtn = 0;
   uint8_t s = valueStart(in); 
    if(s > 0 ){
    if( strstr(in+s, "==") ){ rtn = 1; }
    else if( strstr(in+s, ">=") ){ rtn = 4; }
    else if( strstr(in+s, "<=") ){ rtn = 5; }
    else if( strchr(in+s, '>') ){ rtn = 2; }
    else if( strchr(in+s, '<') ){ rtn = 3; }
    }
   return rtn;
}

void TrueNum::setVal(const char* intoken, float invalue){
 
  boolean found = false;

//if token in array, update value
for(uint8_t i = 0; i < num; i++){
if( strcmp(nodes[i].token,intoken) == 0){ 
  nodes[i].value = invalue; found = true;
  }
}
//else, add to array, update value
if(!found){ 
  uint8_t i = 0;
  while(nodes[i].set)
  {
    i++;
  }
  if (i < num)
  {
  nodes[i].token = (char*)intoken;
  nodes[i].value = invalue;
  nodes[i].set = true;
  } else{ //error }
    }
  }  
}

float TrueNum::getVal(const char* intoken){
// returns value of token
  for(uint8_t i = 0; i < num; i++){
   if(strcmp(nodes[i].token, intoken) == 0)
      return nodes[i].value;}
}

char* TrueNum::getToken(char* in){   
    static char tkbuff[20];
    memset(tkbuff, 0, 20); 
    char* p = strchr(in, '$');
    if(p){
    char* p2 = strchr(p, ' ');
    if(p2){
    uint8_t a = (p2-p);
    if(a < 20){
    memcpy(tkbuff, p, a);
    tkbuff[a] = '\0'; }
    } }
   return tkbuff;
}

float TrueNum::getTarget(char* in){
     uint8_t i;
     char buff[12];
     uint8_t a = 0;
     int s = valueStart(in)-1; 
     for(i = s; in[i]!='\0'; i++){ 
     if(in[i] == ' ' )
     if(in[i+1] >= 46 && in[i+1] <= 57)
     break;
     } 
     for(i; in[i]!='\0'; i++){ 
        if( a < 12 ){
          buff[a] = in[i];  a++;}
        }
  return atof(buff);
}

uint8_t TrueNum::valueStart(char* in){
   uint8_t s = 0;
   char* p =  strstr(in, " is ");
   char* p2 =  strstr(in, " = ");
   if(p)
   s = (p - in)+4;
   if(p2)
   s = (p2 - in)+3;
   return s;
}

void TrueNum::replaceToken(char* in, float val){ 
  char* p = strchr(strchr(in, '$'), ' ');
  if(p){
  String valstr = String(val);  
  uint8_t slen = strlen(in);
  uint8_t sublen = valstr.length();
  uint8_t aftok = p-in;
  uint8_t dollar = strchr(in, '$')-in;
  uint8_t tklen = aftok - dollar;

  if(slen+(sublen-tklen) < len){
    memmove(in+aftok+(sublen-tklen), in+aftok, slen-aftok);
    memcpy(in+dollar, valstr.c_str(), sublen);
    in[slen+(sublen-tklen)] = '\0'; 
    }   
  }
}

void TrueNum::replaceChar(char* in, char inchar, const char* outstr){
char* p = strchr(in, inchar);
uint8_t slen = strlen(in);
uint8_t sublen = strlen(outstr);
while (p != NULL){
    if (sublen > 1){
    memmove(p + (sublen-1), p, slen);
    memcpy(p, outstr, sublen);
    p = strchr(p+(sublen-1), inchar);
    }
    else{
    in[p - in] = outstr[0];
    p = strchr(p, inchar);
    }    
 }  
}
