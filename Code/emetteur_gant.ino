#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial bluetoothSerial(10, 11); // RX, TX (Connectez RX du HC-05 à 10 et TX à 11)
#define MPU6050_ADDRESS 0x68        // Adresse I2C du MPU6050
int16_t accelerometer_x, accelerometer_y, accelerometer_z;
char tmp_str[7]; // variable temporaire utilisée dans la fonction de conversion

char* convert_int16_to_str(int16_t i) { // convertit int16 en chaîne de caractères. De plus, les chaînes résultantes auront la même longueur dans le moniteur de débogage.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

void setup() {
  Serial.begin(38400);
  Wire.begin();
  Wire.beginTransmission(MPU6050_ADDRESS); // Commence une transmission à l'esclave I2C (carte GY-521)
  Wire.write(0x6B); // Registre PWR_MGMT_1
  Wire.write(0); // réglé à zéro (réveille le MPU-6050)
  Wire.endTransmission(true);
  bluetoothSerial.begin(38400);
}

void loop() {
  // Lire les données de l'accéléromètre (MPU6050)
  Wire.beginTransmission(MPU6050_ADDRESS);
  Wire.write(0x3B); // Adresse du registre de l'accélération X haute
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDRESS, 7 * 2, true);

  accelerometer_x = Wire.read() << 8 | Wire.read();
  accelerometer_y = Wire.read() << 8 | Wire.read();
  accelerometer_z = Wire.read() << 8 | Wire.read();

  // Envoyer les données à HC-06 sous forme de chaînes de caractères
  bluetoothSerial.print(convert_int16_to_str(accelerometer_x));
  bluetoothSerial.print(",");
  bluetoothSerial.println(convert_int16_to_str(accelerometer_y));
  /*bluetoothSerial.print(",");
  bluetoothSerial.println(convert_int16_to_str(accelerometer_z));*/

  // Attendre un court délai pour que les données soient envoyées
  delay(500);
}