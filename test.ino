#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#define CNUM "C002"
SoftwareSerial mySerial(50,51);

int stdCo = 600;
int stdhmd = 600;
unsigned long coTime = 0;
unsigned long hmTime = 0;
unsigned long vtTime = 0;

int hm=400, hx=600; //습도 최소,최대
int cm=700, cx=1200; //이산화탄소 최소 최대
int tm=250, tx=300; //온도 최소,최대

int notiTime = 3000;

int coFlag = false; //0 :  off, 1 : on
int hmFlag = false;
int vtFlag = false;

int rede = 5;
int relayCo = 6;
int relayHm = 7;
int relayVentil = 8;

unsigned long now = millis();

//GW에서 min, max 값 등의 유저지정값 받아오는 함수
void readStd(){
  String myStd = Serial.readString();
  StaticJsonDocument<200> myObject;
  DeserializationError error = deserializeJson(myObject, myStd);
  if(CNUM==myObject["deviceNo"]){
    hm = atoi(myObject["hm"]);
    hx = atoi(myObject["hx"]);
    cm = atoi(myObject["cm"]);
    cx = atoi(myObject["cx"]);
    tm = atoi(myObject["tm"]);
    tx = atoi(myObject["tx"]);
    notiTime = atoi(myObject["notiTime"]);
  }
}

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  mySerial.begin(9600);
  
  pinMode(rede, OUTPUT);
  pinMode(relayVentil, OUTPUT);
  pinMode(relayCo, OUTPUT);
  pinMode(relayHm, OUTPUT);

  digitalWrite(relayVentil, HIGH); //off
  digitalWrite(relayCo, HIGH); //off
  digitalWrite(relayHm, HIGH); //off
}

String airQ(){
  String data = "";
  int i=0, j=0;
  int co=0, temp=0, hum=0;
  int sendData[8] = {0x03, 0x03, 0x00, 0x65, 0x00, 0x03, 0x14, 0x36};
  int re[11];
  digitalWrite(rede, HIGH);
  for(i=0;i<9;i++){
    mySerial.write((byte)sendData[i]);
  }
  digitalWrite(rede, LOW);
  if(mySerial.available()>0){
  for(j=0;j<11;j++){
    re[j]=mySerial.read();
  }
 }
 co = 256*re[3] + re[4];
 temp = 256*re[5] + re[6];
 hum = 256*re[7] + re[8];
 data = String(co) + String(temp) + String(hum); 
  
  return data;
}
    

void loop() {
  // put your main code here, to run repeatedly:
  int arr[17];
  int arr2[17];
  int i=0, j=0;
  String data = "";
  String myData = "";
  String result = "";

  String airq = "";
  int m=0, n=0;
  int sendData[8] = {0x03, 0x03, 0x00, 0x65, 0x00, 0x03, 0x14, 0x36};
  int re[11];

  int first_co2 = 0, first_temp = 0, first_humid = 0;
  int scnd_co2=0, scnd_temp=0, scnd_humid=0;
  int thrd_co2=0, thrd_temp=0, thrd_humid=0;
  int avgCo2=0, avgTemp=0, avgHumid=0;

   if(Serial2.available()){
    for(i=0; i<sizeof(arr)/sizeof(int);i++){
      arr[i] = Serial2.read();//온습도, CO2 받아옴
      data += arr[i];
//      Serial.print(arr[i]);
//    데이터 깨질 경우 복구처리
      if(i == 16){
        if(arr[16] != 10){
          i=0;
          arr[17] = {0,}; 
          }else{
            break;
          }
      }
    }
   }else{
    data = "0000000000000000000000000000000000";
   }

   if(Serial1.available()){
    for(j=0; j<sizeof(arr2)/sizeof(int);j++){
      arr2[j] = Serial1.read();
      myData+=arr2[j];
      if(j==16){
        if(arr2[16]!=10){
          j=0;
          arr2[17] = {0,};
        }else{
          break;
        }
      }
    }
   }else{
    myData = "0000000000000000000000000000000000";
   }

   //airQ
   digitalWrite(rede, HIGH);
   for(m=0;m<9;m++){
    mySerial.write((byte)sendData[m]);
   }
   digitalWrite(rede, LOW);
   if(mySerial.available()>0){
   for(n=0;n<11;n++){
    re[n]=mySerial.read();
   }
  }
  thrd_co2 = 256*re[3] + re[4];
  thrd_temp = 256*re[5] + re[6];
  thrd_humid = 256*re[7] + re[8];
  airq = String(re[3]) + String(re[4]) + String(re[5]) + String(re[6]) + String(re[7]) + String(re[8]); 
  
   result += CNUM;
   result += data + myData;  
//   result += data;
//   result += airQ();
   result += airq;   

   first_co2 = (arr[0]-48)*1000 + (arr[1]-48)*100 + (arr[2]-48)*10 + (arr[3]-48); //단위 ppm
   first_temp = (arr[6]-48)*100 + (arr[7]-48)*10 + (arr[9]-48)*1; // nature temp * 10
   first_humid = (arr[11]-48)*100 + (arr[12]-48)*10 + arr[14] *1; // nature humidty * 10
   
   scnd_co2 = (arr2[0]-48)*1000 + (arr2[1]-48)*100 + (arr2[2]-48)*10 + (arr2[3]-48); //단위 ppm
   scnd_temp = (arr2[6]-48)*100 + (arr2[7]-48)*10 + (arr2[9]-48)*1; // nature temp * 10
   scnd_humid = (arr2[11]-48)*100 + (arr2[12]-48)*10 + arr2[14] *1; // nature humidty * 10

//   thrd_co2 = (arr[0]-48)*1000 + (arr[1]-48)*100 + (arr[2]-48)*10 + (arr[3]-48); //단위 ppm
//   thrd_temp = (arr[6]-48)*100 + (arr[7]-48)*10 + (arr[9]-48)*1; // nature temp * 10
//   thrd_humid = (arr[11]-48)*100 + (arr[12]-48)*10 + arr[14] *1; // nature humidty * 10

   if(first_co2>0 && scnd_co2>0 && thrd_co2>0){
    avgCo2 = (first_co2 + scnd_co2 + thrd_co2)/3;
    avgTemp = (first_temp + scnd_temp + thrd_temp)/3;
    avgHumid = (first_humid + scnd_humid + thrd_humid)/3;

//////////////////////////////////////////////////////////
//릴레이모듈 수정해야하는 부분

   if(avgCo2>0 && avgHumid>0){
    if(avgCo2>cx && avgHumid>hx){
      if(vtFlag == 0){
        vtFlag = 1;
        digitalWrite(relayVentil, LOW);
      }
    }else{
      vtFlag = 0;
      digitalWrite(relayVentil, HIGH);
    }
   }

   if(avgHumid>0){
    if(vtFlag ==0){
      if(avgHumid<hm){
        hmFlag = 1;
        digitalWrite(relayHm, LOW);
      }else if(avgHumid>hx){
        hmFlag = 0;
        digitalWrite(relayHm, HIGH);
      }
    }
   }

   if(avgCo2>0){
    if(vtFlag == 0){
      if(avgCo2<cm){
        coFlag = 1;
        digitalWrite(relayCo, LOW);
      }else if(avgCo2>cx){
        coFlag = 0;
        digitalWrite(relayCo, HIGH);
      }
    }
   }
   
//   if(avgCo2>0 && avgHumid>0){
//    if(avgCo2>cx && avgHumid>hx){
//       if(!vtFlag){
//        vtFlag = true;
//        vtTime = millis();
//        digitalWrite(relayVentil, LOW); //vetilation ON
//       }else{
//        if(now - coTime > 300000){
//          vtFlag = false;
//          digitalWrite(relayVentil, HIGH);  
//        }else{
//         vtFlag = true;
//         digitalWrite(relayVentil, LOW);
//        }
//       }
//    }
//   }
//
//   
//   if(avgCo2>0){
//    if(!vtFlag){
//      if(avgCo2<cm){
//        coFlag = true;
//        coTime = millis();
//        digitalWrite(relayCo, LOW); //Co2 generator on
//        }else{
//          if(now - coTime>300000){
//            coFlag = false;
//            digitalWrite(relayCo, HIGH);
//          }else{
//            coFlag = true;
//            digitalWrite(relayCo, LOW);
//          }
//        }
//      }
//    }
//
//  if(avgHumid>0){
//    if(!vtFlag){
//      if(avgHumid<hm){
//        hmFlag=true;
//        hmTime=millis();
//        digitalWrite(relayHm, LOW);
//      }else{
//        if(now - hmTime>300000){
//          coFlag = false;
//          digitalWrite(relayHm, HIGH);
//        }else{
//          hmFlag = true;
//          digitalWrite(relayHm, LOW);
//        }
//      }
//    }
//  }
//    
//   if(avgCo2>0){
//    Serial.println(avgCo2);
//    if(avgCo2<cm){
//      if(!coFlag){
//        //function of operating co2 generator is needed.
//        coFlag = true;
//        coTime = millis();
//        digitalWrite(relayCo,LOW);
//        }else{
//          if(now-coTime>(300000)){ // 5 min
//            coFlag = false;
//            digitalWrite(relayCo,HIGH);
//            }
//          }
//         }else if(avgCo2>cm && avgCo2>cx){
//          digitalWrite(relayCo, LOW);
//         }
//    }
   
//릴레이모듈 완료
   result += hmFlag;
   result += coFlag;
   result += vtFlag;
   Serial.println(result);
   arr[17] = {0,}; //초기화    
   arr2[17] = {0,};
  }
  delay(notiTime);
   
}
