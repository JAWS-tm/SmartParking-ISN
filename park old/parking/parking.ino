#include <Servo.h>;

// Constantes Capteur
const byte TRIGGER_PIN = 4;
const byte ECHO_PIN = 5;

const byte LED_VERTE = 3;
const byte LED_ROUGE = 2;

const unsigned long MEASURE_INTERVAL = 100;
const byte MIN_DISTANCE = 2;

// Constantes Barrière
Servo barriereEntrance;
const int SERVO_PIN = 10;
const byte MIN_POS_SERVO = 15;

const int BUTTON_PIN = 8;
const int LED_VERTE_SERVO = 12;
const int LED_ROUGE_SERVO = 13;

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
  barriereEntrance.attach(SERVO_PIN);

  pinMode(BUTTON_PIN, INPUT);

  pinMode(LED_VERTE_SERVO, OUTPUT);
  pinMode(LED_ROUGE_SERVO, OUTPUT);

  barriereEntrance.write(MIN_POS_SERVO);
  digitalWrite(LED_ROUGE_SERVO, HIGH);
}

void loop() {
  capteurLoop();
  barriereLoop();
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
  static int pos = MIN_POS_SERVO;
  
  unsigned long currentMillis = millis();
  
  switch (openingState) {
    case START:
      digitalWrite(LED_VERTE_SERVO, HIGH);
      digitalWrite(LED_ROUGE_SERVO, LOW);

      previousMillisBarriere = millis();
      openingState = OPENING;
      break;
    case OPENING:
      if (currentMillis - previousMillisBarriere >= STARTING_DELAY) {
        if (pos < 70 + MIN_POS_SERVO && currentMillis - previousMillisMouvBarriere >= MOVEMENT_INTERVAL) {
          pos += 1;
          barriereEntrance.write(pos);
          previousMillisMouvBarriere = millis();
        } else if (pos == 70 + MIN_POS_SERVO) {
          previousMillisBarriere = millis();
          openingState = CLOSING;
        }
      }
      break;
    case CLOSING:
      if (currentMillis - previousMillisBarriere >= OPEN_INTERVAL) {
        if (pos == 70 + MIN_POS_SERVO) {
          digitalWrite(LED_VERTE_SERVO, LOW);
          digitalWrite(LED_ROUGE_SERVO, HIGH);
        }
        if (pos > MIN_POS_SERVO && currentMillis - previousMillisMouvBarriere >= MOVEMENT_INTERVAL) {
          pos -= 1;
          barriereEntrance.write(pos);
          previousMillisMouvBarriere = millis();
        } else if (pos == MIN_POS_SERVO) {
          previousMillisBarriere = millis();
          openingState = WAITING;
        }
      }
      break;
    case WAITING:
      if (digitalRead(BUTTON_PIN)) {
        openingState = START;
      }
      break;
  }
}
