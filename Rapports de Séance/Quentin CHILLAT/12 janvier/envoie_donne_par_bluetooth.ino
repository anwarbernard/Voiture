#include <Wire.h>
#include <SoftwareSerial.h>

// Adresse I2C de l'accéléromètre GY-521
#define MPU_ADDR 0x68

SoftwareSerial bluetoothSerial(10, 11); // RX, TX pour le module Bluetooth HC-05

int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
char tmp_str[7]; // temporary variable used in convert function

char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}



void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  bluetoothSerial.begin(38400);
}

void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
  
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  accelerometer_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  
  
  // Envoyer les données via Bluetooth
  bluetoothSerial.print("X");
  bluetoothSerial.print(convert_int16_to_str(accelerometer_x));
  Serial.print("X"); Serial.print(convert_int16_to_str(accelerometer_x));
  bluetoothSerial.print(",Y");
  bluetoothSerial.print(convert_int16_to_str(accelerometer_y));
  Serial.print(",Y"); Serial.print(convert_int16_to_str(accelerometer_y));
  bluetoothSerial.print(",Z");
  bluetoothSerial.println(convert_int16_to_str(accelerometer_z));
  Serial.print(",Z");Serial.print(convert_int16_to_str(accelerometer_z));
  Serial.println();


  delay(500); // Ajoutez un délai si nécessaire
}



