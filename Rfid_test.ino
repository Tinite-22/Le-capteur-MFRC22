#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN); // Création de l'objet

void setup() {
  Serial.begin(9600);
  SPI.begin();      // Initialisation du bus SPI
  rfid.PCD_Init();  // Initialisation du module RC522
  Serial.println("Approchez votre badge...");
}

void loop() {
  // Vérifie si une nouvelle carte est présente
  if (!rfid.PICC_IsNewCardPresent()) return;

  // Vérifie si la carte a pu être lue
  if (!rfid.PICC_ReadCardSerial()) return;

  // Affiche l'UID sur le moniteur série
  Serial.print("UID de la carte :");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  rfid.PICC_HaltA(); // Arrête la lecture
}