/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read new NUID from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to the read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the type, and the NUID if a new card has been detected. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(5, 6); // RX, TX

#define SS_PIN 2
#define RST_PIN 10
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[3];

void setup() 
{ 
  mySerial.begin(115200);
  Serial.begin(115200);

  pinMode(9,OUTPUT);

  for(int i=4;i<9;i++)
  {
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
  }
  
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) 
  {
    key.keyByte[i] = 0xFF;
  } 
}
 
void loop() 
{
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;
    
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) 
  {
    /*
    for(int i=0; i<4; i++)
    {
    digitalWrite(4, (rfid.uid.uidByte[i] & 1));
    digitalWrite(5, (rfid.uid.uidByte[i] & 2)>>1);
    digitalWrite(6, (rfid.uid.uidByte[i] & 4)>>2);
    digitalWrite(7, (rfid.uid.uidByte[i] & 8)>>3);
    digitalWrite(8, HIGH);
    delay(2);
    digitalWrite(8, LOW);
    digitalWrite(4, (rfid.uid.uidByte[i] & 16)>>4);
    digitalWrite(5, (rfid.uid.uidByte[i] & 32)>>5);
    digitalWrite(6, (rfid.uid.uidByte[i] & 64)>>6);
    digitalWrite(7, (rfid.uid.uidByte[i] & 128)>>7);
    digitalWrite(8, HIGH);
    delay(2);
    digitalWrite(8, LOW);
    }
    */
      mySerial.print(String(rfid.uid.uidByte[0],HEX)+String(rfid.uid.uidByte[1],HEX)+String(rfid.uid.uidByte[2],HEX)+String(rfid.uid.uidByte[3],HEX));
      Serial.print(String(rfid.uid.uidByte[0],HEX)+String(rfid.uid.uidByte[1],HEX)+String(rfid.uid.uidByte[2],HEX)+String(rfid.uid.uidByte[3],HEX));

      digitalWrite(9, HIGH);
      delay(200);
      digitalWrite(9,LOW);
  
  }

  
  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}
