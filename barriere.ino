#include <Servo.h>;

Servo monServoMoteur;
int minPos = 15;
int pos = minPos;

const int pinButton = 8;
const int pinLedVerte = 12;
const int pinLedRouge = 13;

boolean opening = false;

void setup() {
  monServoMoteur.attach(9);
  
  
  pinMode(pinButton, INPUT);
  
  pinMode(pinLedVerte, OUTPUT);
  pinMode(pinLedRouge, OUTPUT);
  
  monServoMoteur.write(minPos);
  digitalWrite(pinLedRouge, HIGH);
}

void loop() {  
  if(opening && pos == minPos){
    digitalWrite(pinLedVerte, HIGH);
    digitalWrite(pinLedRouge, LOW);
    delay(1000);
    
    for (pos = minPos; pos < 70 + minPos; pos += 1){      
      monServoMoteur.write(pos);
      delay(20);
    }
    
    delay(5000);
    
    digitalWrite(pinLedVerte, LOW);
    digitalWrite(pinLedRouge, HIGH);
    
    for (pos = 70 + minPos; pos > minPos; pos -= 1){
      monServoMoteur.write(pos);
      delay(20);
    }    
    
    opening = false;
  }else{
    if(digitalRead(pinButton)){
      canPush = false;
    }
  }
}
