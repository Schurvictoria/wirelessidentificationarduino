#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//задаем адрес LCD экрана 0x3F, 16 символов, 2 строки
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];
word minid1;
word minid2;

void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  lcd.init(); // Инициализируем экран
  //включаем подсветку
  lcd.backlight();
  //Устанавливаем позицию начиная с которой выводится текст.
  lcd.setCursor(0, 0);
  //выводим строку 1
  lcd.print("Wireless animal");
  //аналогично выводим вторую строку
  lcd.setCursor(0, 1);
  lcd.print("ID");
//  for (byte i = 0; i < 6; i++) {
//    key.keyByte[i] = 0xFF;
//  }
}
 
void loop() {

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
   return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

    
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }

    lcd.setCursor(0, 0);
     //выводим строку 1
    lcd.print("                    ");
    lcd.setCursor(0, 1);
    lcd.print("                   ");
    lcd.setCursor(5, 1);
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    if (minid1==0x66F8&minid2==0x4CA5){
     lcd.setCursor(0, 0);
     lcd.print("OWNER: V CHIR");
     lcd.setCursor(0, 1);
      lcd.print("NAME: Ciha");
       rfid.PICC_HaltA();
       rfid.PCD_StopCrypto1();
    return;
    }
    if (minid1==0xC514&minid2==0x8014){
     lcd.setCursor(0, 0);
     lcd.print("OWNER: none");
     lcd.setCursor(0, 1);
      lcd.print("NAME: Wid Dog");
       rfid.PICC_HaltA();
       rfid.PCD_StopCrypto1();
    return;
    }
    if (minid1==0x0&minid2==0x0){}
    else{ 
     lcd.setCursor(0, 0);
     lcd.print("NO IN BASE");
     lcd.setCursor(0, 1);
     lcd.print(minid1, HEX);
     lcd.print(minid2, HEX);     
      }
    
    //lcd.print(minid1, HEX);
    Serial.println();

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

    return;

}
/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
    minid1=(buffer[0]*0x100+buffer[1]);
    minid2=(buffer[2]*0x100+buffer[3]);
  }
}
