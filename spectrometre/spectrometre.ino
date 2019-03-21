#include <Stepper.h>
#include <SoftwareSerial.h>

const int stepsPerRevolution = 200; 
int stepCount = 0;

boolean LancerMesure = LOW;
int sensorHome ;
int pinSensorHome;



String cmd="";
String dataToSend = "";
double MesurePhotodetecteur=0.0f;


int inByte ;
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);


void initialisation (){
  sensorHome = digitalRead (pinSensorHome);
  
  while (sensorHome != LOW) {
    // Mouvement du moteur
    Serial.println("clockwise");
    myStepper.step(0-stepsPerRevolution);

    // Qd la commande est fini je remets à 0 les sorties 8,9, 10 et 11
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    
    sensorHome = digitalRead (pinSensorHome);
}
}



void setup() {
  //Moteur pas à pas
  myStepper.setSpeed(60);

  //Moniteur série
  Serial.begin(9600);

  //Bluetooth
  Serial1.begin(9600);

  //  Initialisation du capteur photoélectrique
  pinSensorHome = 28;
  pinMode(pinSensorHome, INPUT);

  // Initialisation Moteur
  initialisation(); 
}

void loop() {

  while(Serial1.available()>0){
    cmd+=(char)Serial1.read();
  }
 
 if(cmd!=""){
   Serial.print("Command received : ");
   Serial.println(cmd);

   if(cmd=="acq"){
    dataToSend = "";

    //passage de spectres
    /*
      stepCount = 0; 

      
      while (stepCount != 350){
        // Automatic one step at time CW:
        myStepper.step(1);
        stepCount++;
        Serial.println(String(stepCount));
        
        delay(200);
        // Quand la commande est fini je remet à 0 les sorties 8,9, 10 et 11
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
    
  
      }*/
    
    stepCount = 0; 
    while (stepCount != 600){
      // Automatic one step at time CW:
      myStepper.step(1);
      stepCount++;
      Serial.println(String(stepCount));
      Serial.println(String(analogRead (A0)));
      dataToSend = String(analogRead (A0)) + '\n';
      Serial1.print(dataToSend);
      
      delay(200);
      // Quand la commande est fini je remet à 0 les sorties 8,9, 10 et 11
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
  

    }

    // Remise à 0 du compteur et de la mesure
    stepCount = 0;

   Serial.println("Sent");
   Serial1.print("endOfTransmission");
   
   initialisation();
   Serial.println("Reinit OK");
   
   }
   else if(cmd=="calib"){
      Serial.println("Calib\n");
      //TODO
   }
   else if(cmd.indexOf("left") >= 0){
      Serial.println("Left\n" + cmd.substring(cmd.indexOf("-")) );

      myStepper.step(0 - cmd.substring(cmd.indexOf("-")+1).toInt());
      delay(200);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
   }
   else if(cmd.indexOf("right") >= 0){
      Serial.println("Right\n");
      
      myStepper.step(cmd.substring(cmd.indexOf("-")+1).toInt());
      delay(200);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
   }
   else if(cmd=="raz"){
      initialisation();
   }
   else{
       Serial.println("Not a command\n");
   }
   cmd="";
 }
 delay(100);
}
