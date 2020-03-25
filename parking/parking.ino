#include <Servo.h>;

// Constantes Capteur
const byte TRIGGER_PIN = 4;
const byte ECHO_PIN = 5;

const byte LED_VERTE = 3;
const byte LED_ROUGE = 2;

const unsigned long MEASURE_INTERVAL = 100;
const int MIN_DISTANCE = 2;

// Constantes Barrière
Servo barriereEntrance;
const int pinServo = 10;
const byte minPos = 15;

const int pinButton = 8;
const int pinLedVerte = 12;
const int pinLedRouge = 13;

enum {START, OPENING, CLOSING, WAITING};

const unsigned long STARTING_DELAY = 1000;
const unsigned long MOVEMENT_INTERVAL = 20;
const unsigned long OPEN_INTERVAL = 5000;

void setup() {
  Serial.begin(9600);
  
  // SETUP Capteur
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(LED_VERTE, OUTPUT);
  pinMode(LED_ROUGE, OUTPUT);
  
  digitalWrite(TRIGGER_PIN, LOW);
   
  digitalWrite(LED_VERTE, HIGH);
  digitalWrite(LED_ROUGE, LOW);
  
  // SETUP Barrière
  barriereEntrance.attach(pinServo);

  pinMode(pinButton, INPUT);

  pinMode(pinLedVerte, OUTPUT);
  pinMode(pinLedRouge, OUTPUT);

  barriereEntrance.write(minPos);
  digitalWrite(pinLedRouge, HIGH);
}

void loop() {
  capteurLoop();
  barriereLoop();
  Serial.println("LOOP");
}

void capteurLoop(){
  static unsigned long previousMeasureMillis = 0;
  
  unsigned long currentMillis = millis();

  if(currentMillis - previousMeasureMillis >= MEASURE_INTERVAL){
    previousMeasureMillis = millis();
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
  
    long duree = pulseIn(ECHO_PIN, HIGH);
    int distance = duree*340/(2*10000);  
  
    if(distance <= MIN_DISTANCE){
      digitalWrite(LED_VERTE,LOW);
      digitalWrite(LED_ROUGE,HIGH);
    }else{
      digitalWrite(LED_VERTE,HIGH);
      digitalWrite(LED_ROUGE,LOW);    
    }
  }
}

void barriereLoop() {
  static unsigned long previousMillisBarriere = 0;
  static unsigned long previousMillisMouvBarriere = 0;
  static byte openingState = WAITING;
  static int pos = minPos;
  
  unsigned long currentMillis = millis();
  
  switch (openingState) {
    case START:
      digitalWrite(pinLedVerte, HIGH);
      digitalWrite(pinLedRouge, LOW);

      previousMillisBarriere = millis();
      openingState = OPENING;
      break;
    case OPENING:
      if (currentMillis - previousMillisBarriere >= STARTING_DELAY) {
        if (pos < 70 + minPos && currentMillis - previousMillisMouvBarriere >= MOVEMENT_INTERVAL) {
          pos += 1;
          barriereEntrance.write(pos);
          previousMillisMouvBarriere = millis();
        } else if (pos == 70 + minPos) {
          previousMillisBarriere = millis();
          openingState = CLOSING;
        }
      }
      break;
    case CLOSING:
      if (currentMillis - previousMillisBarriere >= OPEN_INTERVAL) {
        if (pos == 70 + minPos) {
          digitalWrite(pinLedVerte, LOW);
          digitalWrite(pinLedRouge, HIGH);
        }
        if (pos > minPos && currentMillis - previousMillisMouvBarriere >= MOVEMENT_INTERVAL) {
          pos -= 1;
          barriereEntrance.write(pos);
          previousMillisMouvBarriere = millis();
        } else if (pos == minPos) {
          previousMillisBarriere = millis();
          openingState = WAITING;
        }
      }
      break;
    case WAITING:
      if (digitalRead(pinButton)) {
        openingState = START;
      }
      break;
  }
}
