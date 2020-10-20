#include <Servo.h> //Include the servo library.
#include <SPI.h> //Include the SPI library.
#include <MFRC522.h> //Include the RFID scanner library.
#include <LiquidCrystal.h> //Include the LCD display library.
#include <BlynkSimpleStream.h> // Include the Blynk library.
#include <SoftwareSerial.h> // Import the serial library to be used in the code.

#define SS_PIN 10
#define RST_PIN 9
#define BLYNK_PRINT SwSerial // Tells the Arduino and Blynk app to use the Serial/USB option.

Servo servo;
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
LiquidCrystal lcd (A0, A1, A2, A3, A4, A5); // Identify the pins the LCD display is using.
SoftwareSerial SwSerial(10, 11); // RX, TX Declares the serial pins used by Blynk.

char auth[] = "oOyVlMzjR6WDcE17gF9h8qCj-srHvkyz"; // Auth token used by Bylnk.

const int ledPin1 = 2; // Constant variables showing the pins the LED's and buzzer are connected to.
const int ledPin2 = 3;
const int buzzer = 4;

bool locked = false; // Declare that the boolean variable locked is false meaning the door is unlocked.

void setup() {
  Serial.begin(9600); // Initalise serial connection.
  Blynk.begin(Serial, auth);
  while (Blynk.connect() == false) {
    SwSerial.begin(9600); // Initiate a serial communication between the Arduino, computer and phone.
    Serial.begin(9600); // Initiate a serial communication between the Arduino and computer.
    Blynk.begin(Serial, auth); // Begin the Blynk program using the serial pins and auth token.
  }
  pinMode(ledPin1, INPUT); // Declare the green LED as an input.
  pinMode(ledPin2, INPUT); // Declare the red LED as an input.
  pinMode(buzzer, OUTPUT); // Declare the buzzer as an output.
  servo.attach(7);  // Attatch the servo to pin 7.
  servo.write(0); // Move the servo to the 0 degree position.
  delay(25);
  lcd.begin(16, 2); // Turn on the LCD display as a 16x2 display.
  SPI.begin();      // Initiate  SPI bus for the RFID scanner.
  mfrc522.PCD_Init();   // Initiate MFRC522 RFID scanner.
  bool locked = true; // Set the locked variable to true.

}

void loop() {

  Blynk.run(); // Run the Blynk Program.
}

BLYNK_WRITE(V1){ // Code for the virtual pin on the blynk app. This pin is told to be button 1 and should execute the funtion stated below it.

  int b1 = param.asInt();

  if (b1 == 0) // If button 1 is pressed run the lock function.
  {
    lock();
  }
}

BLYNK_WRITE(V2){ // Code for the virtual pin on the blynk app. This pin is told to be button 2 and should execute the funtion stated below it.

  int b2 = param.asInt();

  if (b2 == 0) // If button 2 is pressed run the unlock function.
  {
    unlock();
  }
}

void lock() {
  tone(buzzer, 750, 300); // Beep the buzzer.
  digitalWrite(ledPin1, LOW); // Turn off the green led.

  digitalWrite(ledPin2, HIGH); // Turn on the red LED.
  lcd.setCursor(0, 0); // Set the LCD cursor to 0, 0.
  delay(100);

  lcd.print("Access Denied"); // Print Access Denied to the LCD display.
  servo.write(0);  // Move the servo to the 0 degree position.
  delay(200);
}

void unlock() {
  tone(buzzer, 750, 300);  // Beep the buzzer.
  digitalWrite(ledPin2, LOW); // Turn off the red LED.

  digitalWrite(ledPin1, HIGH); // Turn on the green LED.
  lcd.setCursor(0, 0); // Set the LCD cursor to 0, 0.
  delay(100);

  lcd.print("Access Granted"); // Print Access Granted to the LCD display.
  servo.write(330); // Move the servo to the 330 degree position.
  delay(200);
}
