#include <SoftwareSerial.h>
#define CNUM 1

SoftwareSerial mySerial(2,3);

void setup()
{
  Serial.begin(9600);
  while (!Serial){
  ;
  }
  mySerial.begin(9600);
}
    

void loop(){
  int arr[17];
  int i=0;
  
  if(mySerial.available()){
    for(i=0; i<sizeof(arr)/sizeof(int);i++){
      arr[i] = mySerial.read();
      Serial.print(arr[i]);
//      if(arr[sizof(arr)/sizeof(int) - 1] != 10){
//        i=0;
//      }
//      Serial.println(i);
//      Serial.println(arr[i]);
//      Serial.println("----------------");
      if(i == 16){
        if(arr[16] != 10){
          i=0;
          Serial.print("HI");
          Serial.println(""); 
          arr[17] = {0,};
        }
        else{
          Serial.print("HELLO");
        }
      }
    }
  }
  Serial.println("");
  arr[17] = {0,};
  delay(3000);
}
