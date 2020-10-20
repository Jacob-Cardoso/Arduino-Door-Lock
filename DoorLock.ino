#include <Servo.h> //Include the servo library.
#include <SPI.h> //Include the SPI library.
#include <MFRC522.h> //Include the RFID scanner library.
#include <LiquidCrystal.h> //Include the LCD display library.

#define SS_PIN 10
#define RST_PIN 9

Servo servo;
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create an MFRC522 instance.
LiquidCrystal lcd (A0, A1, A2, A3, A4, A5); // Identify the pins the LCD display is using.

const int ledPin1 = 2; // Constant variables showing the pins the LED's and buzzer are connected to.
const int ledPin2 = 3;
const int buzzer = 4;

bool locked = false; // Declare that the boolean variable locked is false meaning the door is unlocked.

void setup() {
  pinMode(ledPin1, INPUT); // Declare the green LED as an input.
  pinMode(ledPin2, INPUT); // Declare the red LED as an input.
  pinMode(buzzer, OUTPUT); // Declare the buzzer as an output.
  servo.attach(7); // Attatch the servo to pin 7.
  servo.write(0); // Move the servo to the 0 degree position.
  delay(25);
  lcd.begin(16, 2); // Turn on the LCD display as a 16x2 display.

  Serial.begin(9600);   // Initiate a serial communication between the Arduino and computer.
  SPI.begin();      // Initiate  SPI bus for the RFID scanner.
  mfrc522.PCD_Init();   // Initiate MFRC522 RFID scanner.
  Serial.println("Scan Now"); // Print scan now to the serial monitor.
  Serial.println();
  bool locked = true; // Set locked to True meaning the door is locked.
  lcd.setCursor(0, 0); // Set the LCD cursor to the 0, 0 position.
  delay(100);
  lcd.print("Scan Now"); // Print Scan Now to the LCD.
}

void loop() {
  // Detect a new RFID chip being scanned.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select the RFID chip scanned.
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  //Print the UID number on the serial monitor.
  Serial.print("UID tag number :");
  String content = "";
  byte letter; // Find the UID number from the RFID chip and convert each character into one string.
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  Serial.println();
  Serial.print("Message : "); // Print Message: to the serial monitor. 
  content.toUpperCase(); 
  if (content.substring(1) == "13 A5 5D 1A") { // If the RFID UID number matches the one stored here execute the code below.
    Serial.println("Access Granted"); // Print Access Granted to the serial monitor. 
    Serial.println();
    locked = false; // Set locked to false.
    delay(500);
  }

  else { // If the UID number doesn't match the one stored execute the code below.
    Serial.println(" Access Denied"); // Print Access Denied to the serial monitor. 
    locked = true; // Set locked to true.
    delay(500);
  }

  if (locked == true) { // If the locked variable is equal to true execute the code below.
    tone(buzzer, 750, 300); // Beep the buzzer.
    digitalWrite(ledPin1, LOW); // Turn off the green led.
    digitalWrite(ledPin2, HIGH); // Turn on the red LED.
    lcd.setCursor(0, 0); // Set the LCD cursor to 0, 0.
    delay(100);

    lcd.print("Access Denied"); // Print Access Denied to the LCD display.
    servo.write(0); // Move the servo to the 0 degree position.
    delay(200);
  }

  if (locked == false) { // If the locked variable is equal to false execute the code below.
    tone(buzzer, 750, 300); // Beep the buzzer.
    digitalWrite(ledPin2, LOW); // Turn off the red LED.
    digitalWrite(ledPin1, HIGH); // Turn on the green LED.
    lcd.setCursor(0, 0); // Set the LCD cursor to 0, 0.
    delay(100);

    lcd.print("Access Granted"); // Print Access Granted to the LCD display.
    servo.write(330); // Move the servo to the 330 degree position.
    delay(200);
  }
}
