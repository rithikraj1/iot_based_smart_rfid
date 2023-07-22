
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPL33G5g3RH8"
#define BLYNK_TEMPLATE_NAME "IoT Smart Lock"


#define BLYNK_FIRMWARE_VERSION  "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 4  // sda pin D2
#define RST_PIN 5 // RST (flash) pin D2

#define relay 2 //Relay Pin D4
#define BUZZER 15 //buzzer pin D8


MFRC522 mfrc522(SS_PIN, RST_PIN);        // Create MFRC522 instance.

SimpleTimer timer;
int button1 = 0;
int button2 = 0;
WidgetTerminal terminal(V2);

void setup()
{
  Serial.begin(115200);
  delay(100);

  BlynkEdgent.begin();

  pinMode(relay, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  SPI.begin();                // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card
  timer.setInterval(3000L, iot_rfid);
}

void loop() {
  timer.run(); // Initiates SimpleTimer
  BlynkEdgent.run();
}
void iot_rfid()
{

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  // Now a card is selected. The UID and SAK is in mfrc522.uid.

  // Dump UID
  Serial.print("Card UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], DEC);
  }
  Serial.println();

  // Dump PICC type
  byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  //    Serial.print("PICC type: ");
  //Serial.println(mfrc522.PICC_GetTypeName(piccType));
  if (        piccType != MFRC522::PICC_TYPE_MIFARE_MINI
              &&        piccType != MFRC522::PICC_TYPE_MIFARE_1K
              &&        piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    //Serial.println("This sample only works with MIFARE Classic cards.");
    return;
  }
  // defining Cards here

  if ( ((mfrc522.uid.uidByte[0] == 138) && (mfrc522.uid.uidByte[1] == 129) && (mfrc522.uid.uidByte[2] == 131) && (mfrc522.uid.uidByte[3] == 14)) && (button1 == 1) )
  {
    Serial.println("Access Granted to Mr. Walter White");
    Blynk.virtualWrite(V2, "Access Granted to Mr. Walter White" );
    digitalWrite(relay, LOW);
    delay(5000);
    digitalWrite(relay, HIGH);
  }
  else if (( (mfrc522.uid.uidByte[0] == 128) && (mfrc522.uid.uidByte[1] == 181) && (mfrc522.uid.uidByte[2] == 141) && (mfrc522.uid.uidByte[3] == 38)) && (button2 == 1) )
  {
    Serial.println("Access Granted to Mrs. Saul Goodman");
    Blynk.virtualWrite(V2, "Access Granted to Saul Goodman" );
    digitalWrite(relay, LOW);
    delay(5000);
    digitalWrite(relay, HIGH);
  }
  else {
    Serial.println("Unregistered user");
    Blynk.virtualWrite(V2, "Unregistered user Trying to Access your Door Lock " );
    Serial.println("Access denied");
    Blynk.virtualWrite(V2, "Access denied");
    digitalWrite(BUZZER, HIGH);
    delay(2000);
    digitalWrite(BUZZER, LOW);
  }
}
// in Blynk app writes values to the Virtual Pin 3
BLYNK_WRITE(V3)
{
  button1 = param.asInt(); // assigning incoming value from pin V3 to a variable


}


// in Blynk app writes values to the Virtual Pin 4
BLYNK_WRITE(V4)
{
  button2 = param.asInt(); // assigning incoming value from pin V4 to a variable

}


