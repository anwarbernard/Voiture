#include <SoftwareSerial.h>

SoftwareSerial bluetoothSerial(13, 12); // RX, TX (Connectez RX du HC-06 à 10 et TX à 11)

int16_t accelerometer_data[2] = {0};
String receivedString = "";            // Chaîne pour stocker les données reçues

int ENA=9;
int IN1=4;
int IN2=5;

int ENB=10;
int IN3=6;
int IN4=7;

void setup() {
  Serial.begin(38400);
  bluetoothSerial.begin(38400); // Initialisez le port série pour la communication Bluetooth

  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);

  analogWrite(ENA,255);
  analogWrite(ENB,255);

}

void loop() {
  // Vérifier s'il y a des données disponibles sur Bluetooth
  while (bluetoothSerial.available()) {
    char receivedChar = bluetoothSerial.read();

    // Si une virgule ou un saut de ligne est reçu, cela signifie la fin d'une valeur
    if (receivedChar == ',' || receivedChar == '\n') {
      // Convertir la chaîne reçue en entier
      int receivedValue = receivedString.toInt();

      // Mettre à jour les valeurs du tableau d'accéléromètre à partir des données reçues
      for (int i = 0; i < 2; i++) {
        if (accelerometer_data[i] == 0) {
          accelerometer_data[i] = receivedValue;
          break;
        }
      }

      // Réinitialisez la chaîne pour la prochaine valeur
      receivedString = "";

      // Si le tableau est complètement construit, afficher chaque valeur
      if (accelerometer_data[0] != 0 && accelerometer_data[1] != 0 ) {
        Serial.print("Tableau d'accéléromètre complet : ");
        Serial.print(accelerometer_data[0]);
        Serial.print(", ");
        Serial.println(accelerometer_data[1]);
       
        if(accelerometer_data[0]<-10000 && accelerometer_data[1]<-5000){ //avancer
          digitalWrite(IN1,HIGH);
          digitalWrite(IN2,LOW);
          digitalWrite(IN3,LOW);
          digitalWrite(IN4,HIGH);
        }
        else if(accelerometer_data[0]>10000 && accelerometer_data[1]>1000){//reculer
          digitalWrite(IN1,LOW);
          digitalWrite(IN2,HIGH);
          digitalWrite(IN3,HIGH);
          digitalWrite(IN4,LOW);
        }
       
        else if(accelerometer_data[0]>1000 && accelerometer_data[1]<10000){//tourner a gauche
          digitalWrite(IN1,HIGH);
          digitalWrite(IN2,LOW);
          digitalWrite(IN3,HIGH);
          digitalWrite(IN4,LOW);
        }
        else if(accelerometer_data[0]<-5000 && accelerometer_data[1]>5000){//tourner a droite
          digitalWrite(IN1,LOW);
          digitalWrite(IN2,HIGH);
          digitalWrite(IN3,LOW);
          digitalWrite(IN4,HIGH);
        }
        else{ //static
          digitalWrite(IN1,LOW);
          digitalWrite(IN2,LOW);
          digitalWrite(IN3,LOW);
          digitalWrite(IN4,LOW);
        }
       
        // Réinitialisez le tableau pour la prochaine série de valeurs
        memset(accelerometer_data, 0, sizeof(accelerometer_data));
      }
    } else {
      // Ajoutez le caractère à la chaîne en cours de réception
      receivedString += receivedChar;
    }
  }
}