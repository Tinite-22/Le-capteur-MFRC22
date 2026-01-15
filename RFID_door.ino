#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define BUZZER_PIN 7

MFRC522 rfid(SS_PIN, RST_PIN);
Servo myServo;  // Crée un objet Servo

// Définissez ici les UID des cartes autorisées (à adapter selon vos cartes)
String authorizedUIDs[] = {
  " 91 C2 6D 06"  // Remplacez par les UIDs de vos cartes
};

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  myServo.attach(6);  // Attache le servomoteur à la broche 6
  myServo.write(180);  // Ferme la porte (position initiale)

  pinMode(BUZZER_PIN, OUTPUT);  // Configure la broche du buzzer en sortie
  Serial.println("Placez votre carte/tag RFID près du lecteur...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    uid.concat(String(rfid.uid.uidByte[i], HEX));
  }
  uid.toUpperCase();

  Serial.print("UID tag : ");
  Serial.println(uid);

  if (isAuthorized(uid)) {
    Serial.println("Accès autorisé");
    myServo.write(90);  // Ouvre la porte
    delay(3000);  // Garde la porte ouverte pendant 3 secondes
    myServo.write(180);  // Ferme la porte
  } else {
    Serial.println("Accès refusé");
    tone(BUZZER_PIN, 1000);  // Émet un bip à 1000 Hz
    delay(500);  // Attente de 1 seconde
    noTone(BUZZER_PIN);  // Arrête le bip
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

bool isAuthorized(String uid) {
  for (String authorizedUID : authorizedUIDs) {
    if (uid.equals(authorizedUID)) {
      return true;
    }
  }
  return false;
}
