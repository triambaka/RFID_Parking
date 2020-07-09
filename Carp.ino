#include<SPI.h>
#include <MFRC522.h>
#include<Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SS_PIN 10
#define RST_PIN 9
#define buzz 1

Servo toll;
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
Adafruit_SSD1306 display(-1);

int IRSensor1 = 7; // connect ir sensor to arduino pin 2
int IRSensor2 = 8;
int IRSensor3 = 4; // connect ir sensor to arduino pin 2
int IRSensor4 = 2;
int IR[4];

void setup() 
{
Serial.begin(9600);
 toll.attach(7);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(buzz,OUTPUT);
  toll.write(20);
  pinMode (IRSensor1, INPUT); // sensor pin INPUT
  pinMode (IRSensor2, INPUT); // sensor pin INPUT
  pinMode (IRSensor3, INPUT); // sensor pin INPUT
  pinMode (IRSensor4, INPUT); // sensor pin INPUT
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  

  // Clear the buffer.
  display.clearDisplay();

  // Display Text
  display.setTextSize(1);
  display.setTextColor(WHITE);
 
  display.display();
  
 // display.clearDisplay();

}

void loop()
{
  int statusSensor1 = digitalRead (IRSensor1);
  int statusSensor2 = digitalRead (IRSensor2);
  int statusSensor3 = digitalRead (IRSensor3);
  int statusSensor4 = digitalRead (IRSensor4);
  
    if (statusSensor1 == 1)
    {
      IR[0]=1;
      Serial.println("1:EMPTY"); // LED LOW
    }
  if (statusSensor2 == 1)
    {
    IR[1]=1;
    Serial.println("2:EMPTY"); // LED LOW
    } // LED LOW
  if (statusSensor3 == 1)
    {
    IR[2]=1;
    Serial.println("3:EMPTY"); 
    } // LED LOW
  if (statusSensor4 == 1)
    {
    IR[3]=1;
    Serial.println("4:EMPTY"); 
    } // LED LOW


    
  if (statusSensor1 == 0)
   {
    IR[0]=1;
    Serial.println("1:FULL"); // LED HIGH
   }
  if (statusSensor2 == 0)
   {
    IR[1]=0;
    Serial.println("2:FULL"); // LED LOW
   } // LED LOW
  if (statusSensor3 == 0)
   {
    IR[2]=0;
    Serial.println("3:FULL"); // LED LOW
   } // LED LOW
  if (statusSensor4 == 0)
  {
    IR[3]=0;
    Serial.println("4:FULL"); // LED LOW
   } // LED LOW
        display.setCursor(0,0);
        display.println("EMPTY SLOTS:");
      //  display.setTextSize(1);
      //  display.setTextColor(WHITE);
        display.setCursor(83,0);
  
    for(int i =1 ; i < 4; i++)
    {
      if(IR[i]==1)
      { Serial.print(i+1);
        display.print(i+1);
        display.print(" ");
       
      }
    }

   if(IR[1]==0 && IR[2]==0 && IR[3]==0)
    {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.print("PARKING FULL");
    }
   display.display();
  delay(2000);
  display.clearDisplay();
   if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i!=3 ? "" : "");
  }
  strID.toUpperCase();
  Serial.println(strID);
  if((strID.indexOf("04:38:C2:AA")>=0 || strID.indexOf("04:40:C2:AA")>=0 || strID.indexOf("04:48:C2:AA")>=0 || strID.indexOf("04:51:C2:AA")>=0 ) && !( IR[1]==0 && IR[2]==0 && IR[3]==0))
  {
    Serial.println("YES");
    digitalWrite(buzz,LOW);  // buzzer disabled 
    toll.write(110);         // open
    delay(1300);             //wait
    toll.write(20);          //close
  }
  else if(!(strID.indexOf("14B74283")>=0 || strID.indexOf("0431C1AA")>=0 || strID.indexOf("0429C1AA")>=0 || strID.indexOf("047CF3AA")>=0 ) || ( IR[1]==0 && IR[2]==0 && IR[3]==0)){
    Serial.println("NO");
    digitalWrite(buzz,HIGH);
    toll.write(20);            // buzzer enabled
    delay(500);                 //beep
    digitalWrite(buzz,LOW);     // stop beep 
    }
}
