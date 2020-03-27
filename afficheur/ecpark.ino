//arduino 4 digits 7 segments + 4DP (dp = digital point)

// int : declaration d'une variable (nbre entier)
// stockage de l'information dans l'arduino

int delai_latence=6;

int digit1 = 2; //Afficheur le plus à gauche - chiffre 1000
int digit2 = 3; //Afficheur centre gauche - chiffre 100
int digit3 = 4; //Afficheur centre droit - chiffre 10
int digit4 = 5; //Afficheur le plus à droite - chiffre 1

int segA = 6;
int segB = 7;
int segC = 8;
int segD = 9;
int segE = 10;
int segF = 11;
int segG = 12;
int dp = 13;//dp = digital point

//void = fonction, contient le programme
//setup : configuration
void setup()
{                
 pinMode(segA, OUTPUT);
 pinMode(segB, OUTPUT);
 pinMode(segC, OUTPUT);
 pinMode(segD, OUTPUT);
 pinMode(segE, OUTPUT);
 pinMode(segF, OUTPUT);
 pinMode(segG, OUTPUT);
 pinMode(dp, OUTPUT);

pinMode(digit1, OUTPUT);
pinMode(digit2, OUTPUT);
pinMode(digit3, OUTPUT);
pinMode(digit4, OUTPUT);

digitalWrite(digit1, LOW);
digitalWrite(digit2, LOW);
digitalWrite(digit3, LOW);
digitalWrite(digit4, LOW);
//on commence par tout éteindre
}

void remaingingPlaces(int num){
  digitalWrite(digit4, LOW);
  digitalWrite(digit1, HIGH);

  // Caractère "P"
  digitalWrite(segA, LOW);
  digitalWrite(segB, LOW);
  digitalWrite(segC, HIGH);
  digitalWrite(segD, HIGH);
  digitalWrite(segE, LOW);
  digitalWrite(segF, LOW);
  digitalWrite(segG, LOW);
  digitalWrite(dp, HIGH);
  
  delay(delai_latence);
  
  digitalWrite(digit1, LOW);
  digitalWrite(digit2, HIGH);
  
  // Caractère "="
  digitalWrite(segA, HIGH); //allumer a
  digitalWrite(segB, HIGH);
  digitalWrite(segC, HIGH);
  digitalWrite(segD, LOW);
  digitalWrite(segE, HIGH);
  digitalWrite(segF, HIGH);
  digitalWrite(segG, LOW); //éteindre g
  digitalWrite(dp, HIGH);
  
  delay(delai_latence);  
  
  digitalWrite(digit2, LOW);
  digitalWrite(digit3, HIGH);

  // Espace
  digitalWrite(segA, HIGH); //allumer a
  digitalWrite(segB, HIGH);
  digitalWrite(segC, HIGH);
  digitalWrite(segD, HIGH);
  digitalWrite(segE, HIGH);
  digitalWrite(segF, HIGH);
  digitalWrite(segG, HIGH); //éteindre g
  digitalWrite(dp, HIGH);
  
  delay(delai_latence);  
  
  digitalWrite(digit3, LOW);
  digitalWrite(digit4, HIGH);

  // Nombre de places
  switch (num) {
    case 0:
      digitalWrite(segA, LOW);
      digitalWrite(segB, LOW);
      digitalWrite(segC, LOW);
      digitalWrite(segD, LOW);
      digitalWrite(segE, LOW);
      digitalWrite(segF, LOW);
      digitalWrite(segG, HIGH);
      digitalWrite(dp, HIGH);
      break;
    case 1:
      digitalWrite(segA, HIGH);
      digitalWrite(segB, LOW);
      digitalWrite(segC, LOW);
      digitalWrite(segD, HIGH);
      digitalWrite(segE, HIGH);
      digitalWrite(segF, HIGH);
      digitalWrite(segG, HIGH);
      digitalWrite(dp, HIGH);
      break;
    case 2:
      digitalWrite(segA, LOW);
      digitalWrite(segB, LOW);
      digitalWrite(segC, HIGH);
      digitalWrite(segD, LOW);
      digitalWrite(segE, LOW);
      digitalWrite(segF, HIGH);
      digitalWrite(segG, LOW);
      digitalWrite(dp, HIGH);
      break;
    case 3:
      digitalWrite(segA, LOW);
      digitalWrite(segB, LOW);
      digitalWrite(segC, LOW);
      digitalWrite(segD, LOW);
      digitalWrite(segE, HIGH);
      digitalWrite(segF, HIGH);
      digitalWrite(segG, LOW);
      digitalWrite(dp, HIGH);
      break;
    case 4:
      digitalWrite(segA, HIGH);
      digitalWrite(segB, LOW);
      digitalWrite(segC, LOW);
      digitalWrite(segD, HIGH);
      digitalWrite(segE, HIGH);
      digitalWrite(segF, LOW);
      digitalWrite(segG, LOW);
      digitalWrite(dp, HIGH);
      break;
  }
  delay(delai_latence);  
}

void loop(){
  remaingingPlaces(4);
// on choisit le chiffre que l'on souhaite afficher

//delai_latence=1+(analogRead(potar)/2);

// pour la compréhension, on a ajouté un potentiomètre nous permettant de faire varier la durée du temps de latence entre l'affichage de chaque chiffre
// dans l'ordre :
 //affichage du chiffre des 1000
 //affichage du chiffre des 100
 //affichage du chiffre des 10
 //affichage du chiffre des 1
// l'afficheur a 4 chiffres est tellement rapide que nous avons l'impression de les avoir affiché en simultanée
// en réalité, ils s'affichent les uns après les autres

}
