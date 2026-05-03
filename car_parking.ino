#include <LiquidCrystal.h>
#include <Servo.h>
#include <DHT.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // pins for lcd

Servo myservo1;

const int IR1 = 8;  // Entry IR sensor
const int IR2 = 9;  // Exit IR sensor

// Servo angles
const int servoOpen = 90;   
const int servoClose = 0;   

int Slot = 5;            
const int maxSlots = 5;  // Max slot capacity

// Flags to prevent multiple detections
bool entryDetected = false;
bool exitDetected = false;

#define DHTPIN 13          // Pin for temp sensorr
#define DHTTYPE DHT11     
DHT dht(DHTPIN, DHTTYPE);

const int buzzerPin = 10;  // pin for buzzer
const float tempThreshold = 30.0;  // threshold temp

void setup() {
  Serial.begin(9600);  // Initializing serial communication 
  
  // Initializing LCD
  analogWrite(6, 75);  // contrast
  lcd.begin(16, 2);
  
  // Initializing IR sensor pins
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  
  // Initializing servo
  myservo1.attach(7);
  myservo1.write(servoClose); 
  
  // Initializing buzzer
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW); 
  
  // Initializing DHT11
  dht.begin();
  displaySlots();
}

void loop() {
  // Read IR sensor states
  int sensor1 = digitalRead(IR1);
  int sensor2 = digitalRead(IR2);
  
  // Detect entry (sensor1 LOW)
  if (sensor1 == LOW && !entryDetected) {
    delay(50);  // Debounce delay
    if (digitalRead(IR1) == LOW) {  // Confirm the sensor state
      entryDetected = true;
      handleEntry();
    }
  }
  
  // Detect exit (sensor2 LOW)
  if (sensor2 == LOW && !exitDetected) {
    delay(50);  // Debounce delay
    if (digitalRead(IR2) == LOW) {  // Confirm the sensor state
      exitDetected = true;
      handleExit();
    }
  }
  
  // Reset entry flag when sensor1 is not triggered
  if (sensor1 == HIGH && entryDetected) {
    entryDetected = false;
  }
  
  // Reset exit flag when sensor2 is not triggered
  if (sensor2 == HIGH && exitDetected) {
    exitDetected = false;
  }
  
  // Check temp and activate buzzer if temp is too high
  checkTemperature();

  delay(100);  // Small delay to avoid bouncing
}

// Function to handle car entry
void handleEntry() {
  if (Slot > 0) {
    // Open gate
    myservo1.write(servoOpen);
    Serial.println("Gate opened for entry.");
    delay(2000);  // Wait for gate to open and car to pass
    
    // Close gate
    myservo1.write(servoClose);
    Serial.println("Gate closed after entry.");
    
    // Update slot count
    Slot--;
    Serial.print("Slot decremented: ");
    Serial.println(Slot);
    
    // Update LCD
    displaySlots();
  } else {
    // Parking full
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("    SORRY :(    ");  
    lcd.setCursor(0, 1);
    lcd.print("  Parking Full  "); 
    Serial.println("Parking Full");
    delay(3000);
    lcd.clear();
    
    // Optionally, display welcome and slot count again
    displaySlots();
  }
}

// Function to handle car exit
void handleExit() {
  if (Slot < maxSlots) {
    // Open gate
    myservo1.write(servoOpen);
    Serial.println("Gate opened for exit.");
    delay(2000);  // Wait for gate to open and car to exit
    
    // Close gate
    myservo1.write(servoClose);
    Serial.println("Gate closed after exit.");
    
    // Update slot count
    Slot++;
    Serial.print("Slot incremented: ");
    Serial.println(Slot);
    
    // Update LCD
    displaySlots();
  } 
}

// Function to display available slots on LCD
void displaySlots() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  CAR PARKING    ");
  lcd.setCursor(0, 1);
  lcd.print("  Slots Left: ");
  lcd.print(Slot);
  Serial.print("  Slots Left: ");
  Serial.println(Slot);
}

// Function to check temperature and activate buzzer if too high
void checkTemperature() {
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("Failed to read temperature!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println("°C");
    
    // Check if temperature exceeds threshold
    if (temp > tempThreshold) {
      digitalWrite(buzzerPin, HIGH);  // Turn on the buzzer
      
      // Display warning message
      Serial.println("FIRE DETECTED, EVACUATE QUICKLY AS POSSIBLE!");
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" FIRE DETECTED ");
      lcd.setCursor(0, 1);
      lcd.print(" EVACUATE NOW ");
    } else {
      digitalWrite(buzzerPin, LOW);   // Turn off the buzzer
      displaySlots();  // Reset LCD to display slot info
    }
  }
}
