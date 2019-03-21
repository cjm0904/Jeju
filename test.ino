#include <SoftwareSerial.h>
#define CNUM "C001"

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
  String data = "";
  data += CNUM;
  if(mySerial.available()){
    for(i=0; i<sizeof(arr)/sizeof(int);i++){
      arr[i] = mySerial.read();
      data += arr[i];
//      Serial.print(arr[i]);
      if(i == 16){
        if(arr[16] != 10){
          i=0;
          arr[17] = {0,}; 
        }
        else{
          Serial.print(data);
        }
      }
    }
  }
  arr[17] = {0,};
  delay(3000);
}
