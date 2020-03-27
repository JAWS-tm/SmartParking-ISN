#include <Servo.h>;

// Constantes Écran
const unsigned long PRINT_LATENCE = 8;

const byte DIGIT_PIN[3] = {2,3,4}; // {2,3,4,5}; // digit1, digit2, etc...
const byte SEGMENT_PIN[8] = {6,7,8,9,10,11,12,13}; // segA, segB, etc...

const byte SEGMENTS_CHAR_MAPPING[2][8] = {
  {LOW,LOW,HIGH,HIGH,LOW,LOW,LOW,HIGH}, // Lettre P | segA, segB, segC, etc...
  {HIGH,HIGH,HIGH,LOW,HIGH,HIGH,LOW,HIGH} // Caractère = | segA, segB, segC, etc...
};

const byte SEGMENTS_NUMBER_MAPPING[5][8] = {
  {LOW,LOW,LOW,LOW,LOW,LOW,HIGH,HIGH}, // Chiffre 0 | segA, segB, segC, etc...
  {HIGH,LOW,LOW,HIGH,HIGH,HIGH,HIGH,HIGH}, // Chiffre 1 | segA, segB, segC, etc...
  {LOW,LOW,HIGH,LOW,LOW,HIGH,LOW,HIGH}, // Chiffre 2 | segA, segB, segC, etc...  
  {LOW,LOW,LOW,LOW,HIGH,HIGH,LOW,HIGH}, // Chiffre 3 | segA, segB, segC, etc...
  {HIGH,LOW,LOW,HIGH,HIGH,LOW,LOW,HIGH} // Chiffre 4 | segA, segB, segC, etc...
};

// Constantes Capteur
struct arrayPIN {
  byte TRIGGER_PIN;
  byte ECHO_PIN;
  byte LED_VERTE;
  byte LED_ROUGE;
};

const byte SENSOR_NB = 2;

const struct arrayPIN PARKING_SENSOR_PIN[SENSOR_NB] = {
  {51,50,52,53},
  {47,46,45,44}
};

/*const byte TRIGGER_PIN = 51; // PIN pour l'envoi d'impulsions sur le capteur
const byte ECHO_PIN = 50; // PIN de récéption des impulsions sur le capteur

const byte LED_VERTE = 52;
const byte LED_ROUGE = 53;*/

const unsigned long MEASURE_INTERVAL = 1000;
const byte MIN_DISTANCE = 2;

// Constantes Barrière
Servo barriereEntrance;
const int SERVO_PIN = 24;
const byte MIN_POS_SERVO = 15;

const int BUTTON_PIN = 25;
const int LED_VERTE_SERVO = 22;
const int LED_ROUGE_SERVO = 23;

enum {START, OPENING, CLOSING, WAITING};

const unsigned long STARTING_DELAY = 1000;
const unsigned long MOVEMENT_INTERVAL = 20;
const unsigned long OPEN_INTERVAL = 5000;

void setup() {
  Serial.begin(9600);

  // SETUP Écran  
  for(byte i = 0; i < 8; i++){
    pinMode(SEGMENT_PIN[i], OUTPUT);
  }
  
  for(byte i = 0; i < 3; i++){
    pinMode(DIGIT_PIN[i], OUTPUT);
    digitalWrite(DIGIT_PIN[i], LOW);
  }
  
  // SETUP Capteur
  for(byte i = 0; i < SENSOR_NB; i++){
    pinMode(PARKING_SENSOR_PIN[i].TRIGGER_PIN, OUTPUT);
    pinMode(PARKING_SENSOR_PIN[i].ECHO_PIN, INPUT);
    pinMode(PARKING_SENSOR_PIN[i].LED_VERTE, OUTPUT);
    pinMode(PARKING_SENSOR_PIN[i].LED_ROUGE, OUTPUT);
    
    digitalWrite(PARKING_SENSOR_PIN[i].TRIGGER_PIN, LOW);
   
    digitalWrite(PARKING_SENSOR_PIN[i].LED_VERTE, HIGH);
    digitalWrite(PARKING_SENSOR_PIN[i].LED_ROUGE, LOW);
  }
  
  /*pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(LED_VERTE, OUTPUT);
  pinMode(LED_ROUGE, OUTPUT);
  
  digitalWrite(TRIGGER_PIN, LOW);
   
  digitalWrite(LED_VERTE, HIGH);
  digitalWrite(LED_ROUGE, LOW);*/
  
  // SETUP Barrière
  barriereEntrance.attach(SERVO_PIN);

  pinMode(BUTTON_PIN, INPUT);

  pinMode(LED_VERTE_SERVO, OUTPUT);
  pinMode(LED_ROUGE_SERVO, OUTPUT);

  barriereEntrance.write(MIN_POS_SERVO);
  digitalWrite(LED_ROUGE_SERVO, HIGH);
}

byte nbPlacesRestantes = SENSOR_NB;

void loop() {  
  capteurLoop();
  barriereLoop();
  remaingingPlaces(nbPlacesRestantes);
}

void capteurLoop(){
  static unsigned long previousMeasureMillis = 0;
  //static byte actualSensor = 0; // Capteur numéro 1; 0 ici car ils sont dans une liste
  
  unsigned long currentMillis = millis();

  if(currentMillis - previousMeasureMillis >= MEASURE_INTERVAL){ // || actualSensor != 0){    
    previousMeasureMillis = millis();

    nbPlacesRestantes = SENSOR_NB;
      
    /*digitalWrite(PARKING_SENSOR_PIN[actualSensor].TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(PARKING_SENSOR_PIN[actualSensor].TRIGGER_PIN, LOW);
    
    long duree = pulseIn(PARKING_SENSOR_PIN[actualSensor].ECHO_PIN, HIGH);
    int distance = duree*340/(2*10000);  
  
    if(distance <= MIN_DISTANCE){
      digitalWrite(PARKING_SENSOR_PIN[actualSensor].LED_VERTE,LOW);
      digitalWrite(PARKING_SENSOR_PIN[actualSensor].LED_ROUGE,HIGH);
      nbPlacesRestantes -= 1;
    }else{
      digitalWrite(PARKING_SENSOR_PIN[actualSensor].LED_VERTE,HIGH);
      digitalWrite(PARKING_SENSOR_PIN[actualSensor].LED_ROUGE,LOW);
    } 

    actualSensor++;
    if(actualSensor >= SENSOR_NB){
      actualSensor = 0;
    }*/
    for(byte i = 0; i < SENSOR_NB; i++){
      previousMeasureMillis = millis();
      
      digitalWrite(PARKING_SENSOR_PIN[i].TRIGGER_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(PARKING_SENSOR_PIN[i].TRIGGER_PIN, LOW);
    
      long duree = pulseIn(PARKING_SENSOR_PIN[i].ECHO_PIN, HIGH);
      int distance = duree*340/(2*10000);  
    
      if(distance <= MIN_DISTANCE){
        digitalWrite(PARKING_SENSOR_PIN[i].LED_VERTE,LOW);
        digitalWrite(PARKING_SENSOR_PIN[i].LED_ROUGE,HIGH);
        nbPlacesRestantes -= 1;
      }else{
        digitalWrite(PARKING_SENSOR_PIN[i].LED_VERTE,HIGH);
        digitalWrite(PARKING_SENSOR_PIN[i].LED_ROUGE,LOW);    
      }
    }
    
    /*previousMeasureMillis = millis();
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
    }*/
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

void remaingingPlaces(int num){
  static unsigned long previousPrintPlacesMillis = 0;
  static byte printDigitNb = 1;
  
  unsigned long currentMillis = millis();

  if(currentMillis - previousPrintPlacesMillis >= PRINT_LATENCE){
    previousPrintPlacesMillis = millis();
    
    switch (printDigitNb) {
      case 1:
        digitalWrite(DIGIT_PIN[2], LOW);
        digitalWrite(DIGIT_PIN[0], HIGH);

        printDigitNb++;

        for(byte i = 0; i < 8; i++){
          digitalWrite(SEGMENT_PIN[i], SEGMENTS_CHAR_MAPPING[0][i]);
        }
        
        break;
      case 2: // Caractère "="
        digitalWrite(DIGIT_PIN[0], LOW);
        digitalWrite(DIGIT_PIN[1], HIGH);

        printDigitNb++;

        for(byte i = 0; i < 8; i++){
          digitalWrite(SEGMENT_PIN[i], SEGMENTS_CHAR_MAPPING[1][i]);
        }
        
        break;
      case 3: // Espace
        digitalWrite(DIGIT_PIN[1], LOW);
        digitalWrite(DIGIT_PIN[2], HIGH);

        printDigitNb = 1;

        for(byte i = 0; i < 8; i++){
          digitalWrite(SEGMENT_PIN[i], SEGMENTS_NUMBER_MAPPING[num][i]);
        }
    }
  }
}
