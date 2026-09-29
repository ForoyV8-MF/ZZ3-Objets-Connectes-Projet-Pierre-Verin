#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_SHTC3.h"

SHTC3 mySHTC3;

void scanI2C() {
  Serial.println("\n--- Scan I2C ---");
  byte count = 0;
  for (byte address = 1; address < 127; ++address) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("-> Appareil detecte a l'adresse : 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      count++;
    }
  }
  if (count == 0) {
    Serial.println("Aucun appareil I2C detecte !");
  } else {
    Serial.print("Total : ");
    Serial.println(count);
  }
  Serial.println("----------------\n");
}

void setup() {
  Serial.begin(115200);
  time_t start = millis();
  while (!Serial && (millis() - start < 4000));

  Serial.println("=== Demarrage RAK11300 ===");

  // Alimentation du slot capteur WisBlock (3V3_S)
  pinMode(WB_IO2, OUTPUT);
  digitalWrite(WB_IO2, HIGH);
  delay(300);

  // Initialisation I2C standard (routee automatiquement sur 6 et 7)
  Wire.begin();

  // Verification materielle immediate
  scanI2C();

  // Demarrage du capteur
  if (mySHTC3.begin() != SHTC3_Status_Nominal) {
    Serial.println("Echec initialisation SHTC3 !");
  } else {
    Serial.println("SHTC3 detecte et operationnel !");
  }
}

void loop() {
  mySHTC3.update();

  if (mySHTC3.lastStatus == SHTC3_Status_Nominal) {
    Serial.print("Temperature : ");
    Serial.print(mySHTC3.toDegC(), 2);
    Serial.print(" °C | Humidite : ");
    Serial.print(mySHTC3.toPercent(), 2);
    Serial.println(" %");
  } else {
    Serial.println("Erreur de lecture capteur");
  }

  delay(2000);
}