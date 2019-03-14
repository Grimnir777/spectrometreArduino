#include <Stepper.h>
#include <SoftwareSerial.h>

const int stepsPerRevolution = 200; 
int stepCount = 0;

boolean LancerMesure = LOW;
int sensorHome ;
int pinSensorHome;



String cmd="";
String dataToSend = "";
float MesurePhotodetecteur=0.0f;


int inByte ;
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);


void initialisation (){
  sensorHome = digitalRead (pinSensorHome);
  
  while (sensorHome != LOW) {
    // Mouvement du moteur
    Serial.println("clockwise");
    myStepper.step(10);

    // Qd la commande est fini je remets à 0 les sorties 8,9, 10 et 11
    sensorHome = digitalRead (pinSensorHome);
  }
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
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
    initialisation(); 
    
    stepCount = 0; 
    while (stepCount != 80){
      // Automatic one step at time CW:
      myStepper.step(1);
      stepCount++;
      //Serial.println(stepCount);
      
      dataToSend = String(analogRead (A0)) + '\n';
      Serial1.print(dataToSend);
      
      delay(1000);
      // Quand la commande est fini je remet à 0 les sorties 8,9, 10 et 11
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);

      // Lancer acquisition
      
      Serial.println(MesurePhotodetecteur);
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
   else{
       Serial.println("Not a command\n");
   }
   cmd="";
 }
 delay(100);
}
