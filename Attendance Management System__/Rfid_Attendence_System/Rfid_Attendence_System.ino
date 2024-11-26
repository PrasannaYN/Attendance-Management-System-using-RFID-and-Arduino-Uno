#include <SPI.h>                
#include <MFRC522.h>            //RFID Scanner
#include <Wire.h>               // I2C
#include <LiquidCrystal_I2C.h>  // LCD 

LiquidCrystal_I2C lcd(0x27, 16, 2);  

#define SS_PIN 10  //RX slave select
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

String card_ID = "";  //card UID

//Add as many cards you want
String Name1 = "18702153";  //first UID card     52220209207     
String Name2 = "3512121173";     //second UID card  18705153

// String Name3 = "522201209207";   //third UID card....3512221173



int NumbCard[2];  //the number of cards. in my case I have 6 cards.
int j = 0;        //increased by one for every user you add
int statu[2];     //the number of cards. in my case I have 6 cards.
int s = 0;        //increased by one for every user you add

int const RedLed = 6;
int const GreenLed = 5;
int const BlueLed = 4; 
int const Buzzer = 8;


String Name;    //user name
String Number;  //user number
int n;          //The number of card you want to detect (optional)
int ID = 1;

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);  // Initialize serial communications with the PC
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card

  Serial.println("CLEARSHEET");                                             // clears starting at row 1
  Serial.println("LABEL,ID,Date,Name,USN ,RFID Card ID,Time IN,Time OUT");  // make four columns (Date,Time,[Name:"user name"]line 48 & 52,[Number:"user number"]line 49 & 53)

  pinMode(RedLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(BlueLed,OUTPUT);

  lcd.setCursor(2, 0);
  lcd.print("Good Morning");  //Wlecome Message will appear in the beginning.
  delay(2000);
}

void loop() {
  //look for new card
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;  //got to start of loop if there is no card present
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;  //if read card serial(0) returns 1, the uid struct contians the ID of the read card.
  }

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    card_ID += mfrc522.uid.uidByte[i];
  }

  //Serial.println(card_ID);//Uncomment this line to scan the card ID and appear it on Serial monitor.

  if (card_ID == Name1) {
    Name = "add name 1 ";    //user name
    Number = "add your id ";  //user number
    j = 0;                  //increased by one for every user you add
    s = 0;                  //++1
  } else if (card_ID == Name2) {
    Name = "add name 2 ";   //user name
    Number = "add id";  //user number
    j = 1;                  //++1
    s = 1;  
    }               //++1
  // } else if (card_ID == Name3) {
  //   Name = "Student 3 Name ";    //user name
  //   Number = "Student 3 USN";  //user number
  //   j = 2;                  //++1
  //   s = 2;                  //++1
  // }
  else {
    digitalWrite(GreenLed, LOW);
    digitalWrite(RedLed, HIGH);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Not Registered");
    digitalWrite(Buzzer,HIGH);
    delay(600);
    digitalWrite(Buzzer,LOW);
    goto cont;  //go directly to line 188
  }

  if (NumbCard[j] == 1 && statu[s] == 0) {
    statu[s] = 1;
    NumbCard[j] = 0;
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Good Bye");
    lcd.setCursor(0, 1);
    lcd.print(Name);
    Serial.print("DATA,");  //send the Name to excel
    Serial.print(ID);
    Serial.print(",");
    Serial.print("DATE");
    Serial.print(",");
    Serial.print(Name);
    Serial.print(",");
    Serial.print(Number);  //send the Number to excel
    Serial.print(",");
    Serial.print(card_ID);  //send the card ID to excel
    Serial.print(",");
    Serial.print("");
    Serial.print(",");
    Serial.println("TIME");
    ID = ID + 1;
    digitalWrite(BlueLed, HIGH);
    digitalWrite(RedLed, LOW);
    digitalWrite(Buzzer, HIGH);
    delay(60);
    digitalWrite(Buzzer, LOW);
    delay(60);
    digitalWrite(Buzzer, HIGH);
    delay(60);
    digitalWrite(Buzzer, LOW);
  } 

  else if (NumbCard[j] == 0) {
    NumbCard[j] = 1;
    statu[s] = 0;
    n++;  //(optional)
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Welcome");
    lcd.setCursor(0, 1);
    lcd.print(Name);
    Serial.print("DATA,");  //send the Name to excel
    Serial.print(ID);
    Serial.print(",");
    Serial.print("DATE");
    Serial.print(",");
    Serial.print(Name);
    Serial.print(",");
    Serial.print(Number);  //send the Number to excel
    Serial.print(",");
    Serial.print(card_ID);  //send the card ID to excel
    Serial.print(",");
    Serial.print("TIME");
    Serial.print(",");
    Serial.println("");
    digitalWrite(GreenLed, HIGH);
    digitalWrite(RedLed, LOW);
    digitalWrite(Buzzer, HIGH);
    delay(300);
    digitalWrite(Buzzer, LOW);
    ID = ID + 1;
  } else if (statu[s] == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(Name);
    lcd.setCursor(0, 1);
    lcd.print("Already Left");
  }
  delay(2000);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Good Morning");

cont:
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Good Morning");
  digitalWrite(BlueLed, LOW);
  digitalWrite(GreenLed, LOW);
  digitalWrite(RedLed, LOW);
  card_ID = "";

  //if you want to close the Excel when all card had detected and save Excel file in Names Folder. in my case i have just 2 card (optional)
  // if(n==2){
  //   Serial.println("SAVEWORKBOOKAS,Names/WorkNames");
  //   Serial.println("FORCEEXCELQUIT");
  //   }
}
