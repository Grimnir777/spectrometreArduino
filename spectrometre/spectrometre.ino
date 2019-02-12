#include <Stepper.h>
#include <SoftwareSerial.h>

const int stepsPerRevolution = 200; 
int stepCount = 0;

String cmd="";
String dataToSend = "";
float sensor_val=0.0f;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);
SoftwareSerial hc06(7,8);

void setup() {
  Serial.begin(115200);
  hc06.begin(9600);
}

void loop() {
  while(hc06.available()>0){
    cmd+=(char)hc06.read();
  }
 
 if(cmd!=""){
   Serial.print("Command received : ");
   Serial.println(cmd);
   
   if(cmd=="acq"){
    dataToSend = "";
    for (int i=0; i <= stepsPerRevolution; i++){
      if(i%2==0){
        sensor_val = 0.0f;
      }
      else{
        sensor_val = 5.0f;
      }
      dataToSend = String(i) + ',' + String(sensor_val) +';';
      hc06.print(dataToSend);
     /* myStepper.step(1);
      stepCount++;*/
      delay(100);
   }
    hc06.print("\n");
/*
    //retour à 0 manuel pour l'instant
    for (int i=0; i <= stepsPerRevolution; i++){
      myStepper.step(-1);
      stepCount--;
      delay(100);
    }
    digitalWrite(8, LOW); 
    digitalWrite(9, LOW);  
    digitalWrite(10, LOW);  
    digitalWrite(11, LOW); 
   */ 
   }
   else if(cmd=="test"){
      Serial.println("testOK\n");
      hc06.print("testOK\n");
   }
   else{
       Serial.println("Not a command");
   }
   cmd="";
 }
 delay(100);
}
