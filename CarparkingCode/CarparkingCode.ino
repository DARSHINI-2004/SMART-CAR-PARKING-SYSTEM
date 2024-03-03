#include <LiquidCrystal_I2C.h>

#include <Servo.h> 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2); 

Servo myservo;

#define ir_enter 2
#define ir_back 4
#define servoPin 3
#define ir_car1 5
#define ir_car2 6
#define ir_car3 7
#define ir_car4 8
int entrySensorState = 0;     
int exitSensorState = 0;       


int S1 = 0, S2 = 0, S3 = 0, S4 = 0;
int flag1 = 0, flag2 = 0;
int slot = 4;

void setup() {
  pinMode(ir_enter, INPUT);
  pinMode(ir_back, INPUT);
  myservo.attach(servoPin); 
  myservo.write(0);   
  Serial.begin(9600);

  pinMode(ir_car1, INPUT);
  pinMode(ir_car2, INPUT);
  pinMode(ir_car3, INPUT);
  pinMode(ir_car4, INPUT);

  pinMode(ir_enter, INPUT);
  pinMode(ir_back, INPUT);

  myservo.attach(3);
  myservo.write(90);

 
  lcd.begin();
  lcd.backlight();
  lcd.setCursor (0,0);
  lcd.print("WELCOME TO CAR");
  lcd.setCursor (0,1);
  lcd.print("PARKING SYSTEM ");
  delay(5000);
  lcd.clear();  

  Read_Sensor();

  int total = S1 + S2 + S3 + S4;
  slot = slot - total;
}

void loop() {
  entrySensorState = digitalRead(ir_enter);
  exitSensorState = digitalRead(ir_back);

  if (entrySensorState == LOW) {
    rotateServo(90); // Rotate the servo motor by 90 degrees for entry
    delay(1000);      // Adjust the delay as needed for the motor to reach the position
  }

  if (exitSensorState == LOW) {
    rotateServo(90);   // Rotate the servo motor to the initial position (0 degrees) for exit
    delay(1000);      // Adjust the delay as needed for the motor to reach the position
  }  
  Read_Sensor();
  
   
  lcd.setCursor (0, 0);
  if (S1 == 1) {
    lcd.print("S1:full   ");
  }
  else {
    lcd.print("S1:0      ");
  }
  

  lcd.setCursor (9, 0);
  if (S2 == 1) {
    lcd.print("S2:full   ");
  }
  else {
    lcd.print("S2:0    ");
  }

  lcd.setCursor (0, 1);
  if (S3 == 1) {
    lcd.print("S3:full    ");
  }
  else {
    lcd.print("S3:0     ");
  }

  lcd.setCursor (9, 1);
  if (S4 == 1) {
    lcd.print("S4:full     ");
  }
  else {
    lcd.print("S4:0      ");
  }
  
  if(S1==1 && S2==1 && S3==1 && S4==1)
  {
    lcd.setCursor (0, 0);
      lcd.print(" Parking Full    ");
      lcd.setCursor (0, 1);
      lcd.print(" ENTRY CLOSED !!   ");
      delay(6000);
  }
}

void Read_Sensor() {
  S1 = 0, S2 = 0, S3 = 0, S4 = 0;

  if (digitalRead(ir_car1) == 0) {
    S1 = 1;
  }
  if (digitalRead(ir_car2) == 0) {
    S2 = 1;
  }
  if (digitalRead(ir_car3) == 0) {
    S3 = 1;
  }
  if (digitalRead(ir_car4) == 0) {
    S4 = 1;
  }

}
void rotateServo(int angle) {
  myservo.write(angle);   // Rotate the servo motor to the specified angle
  delay(1000); 
  myservo.write(0);    // Adjust the delay as needed for the motor to reach the position
}