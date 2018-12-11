#include <SoftwareSerial.h>

SoftwareSerial mySerial(3,2);

void setup()
{
  Serial.begin(9600);
  while (!Serial){
  ;
  }
  
  Serial.println("Goodnight moon!");
  mySerial.begin(9600);
}

void loop(){
  int arr[17];
  int i=0;
  
  if(mySerial.available()){
    for(i=0; i<17;i++){
      arr[i] = mySerial.read();
      Serial.print(arr[i]);
    }
    
    Serial.println("");
  }
  
  int arr[17] = {0,};
  
}
