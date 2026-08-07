#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// HC-SR04 Ultrasonic Sensor pins
#define trigPin 9
#define echoPin 10

// Motor A pins (Left motor)
#define motorEnableA 5
#define motorIn1 6
#define motorIn2 7

// Motor B pins (Right motor)
#define motorEnableB 11
#define motorIn3 8
#define motorIn4 12

// Potentiometer pin
#define potPin A0

// Distance limit to stop the motors
const int stopDistance = 20; // Adjust as needed (in cm)

// Initialize the LCD with I2C address 0x27, 16 columns, and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2); 

long duration;
int distance;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  
  // Set up motor pins
  pinMode(motorEnableA, OUTPUT);
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  
  pinMode(motorEnableB, OUTPUT);
  pinMode(motorIn3, OUTPUT);
  pinMode(motorIn4, OUTPUT);
  
  // Set up ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize the LCD
  lcd.init();      // Initialize the LCD
  lcd.backlight(); // Turn on the LCD backlight
  lcd.print("Motor Speed:");
}

void loop() {
  // Ultrasonic sensor reading
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Convert to cm

  // Read potentiometer for speed control
  int potValue = analogRead(potPin);
  int motorSpeed = map(potValue, 0, 1023, 0, 178); // 70% of max speed

  // Display speed on LCD
  lcd.setCursor(0, 1);
  lcd.print("Speed: ");
  lcd.print(motorSpeed);
  lcd.print("   "); // Clear extra characters if overwritten

  // Motor control based on distance
  if (distance >= stopDistance) {
    // Run motors if no object within stop distance
    analogWrite(motorEnableA, motorSpeed);
    analogWrite(motorEnableB, motorSpeed);

    // Set direction (forward)
    digitalWrite(motorIn1, HIGH);
    digitalWrite(motorIn2, LOW);
    digitalWrite(motorIn3, HIGH);
    digitalWrite(motorIn4, LOW);
  } else {
    // Stop motors if object is within stop distance
    analogWrite(motorEnableA, 0);
    analogWrite(motorEnableB, 0);
  }

  delay(100); // Short delay for stability
}
