const byte TRIGGER_PIN = 4;
const byte ECHO_PIN = 5;

const byte LED_VERTE = 3;
const byte LED_ROUGE = 2;

const unsigned long MEASURE_INTERVAL = 100;
int min_distance = 2;

void setup() {
   Serial.begin(9600);
  
   pinMode(ECHO_PIN, INPUT);
   pinMode(TRIGGER_PIN, OUTPUT);
   pinMode(LED_VERTE, OUTPUT);
   pinMode(LED_ROUGE, OUTPUT);
   
   digitalWrite(TRIGGER_PIN, LOW);
   
   digitalWrite(LED_VERTE, HIGH);
   digitalWrite(LED_ROUGE, LOW);
}

unsigned long previousMeasureMillis = 0;

void loop() {
  unsigned long currentMillis = millis();

  if(currentMillis - previousMeasureMillis >= MEASURE_INTERVAL){
    previousMeasureMillis = millis();
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
  
    long duree = pulseIn(ECHO_PIN, HIGH);
    int distance = duree*340/(2*10000);  
  
    if(distance <= min_distance){
      digitalWrite(LED_VERTE,LOW);
      digitalWrite(LED_ROUGE,HIGH);
    }else{
      digitalWrite(LED_VERTE,HIGH);
      digitalWrite(LED_ROUGE,LOW);    
    }
    
    /*if(distance <= 0){  
      Serial.println("Hors de portee");
    }else{
      Serial.print(distance);
      Serial.print(" cm ");
      Serial.print(duree);
      Serial.println(" ms");
    }*/
  }
}
