#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ultrasonic
#define trigPin 7
#define echoPin 6
#define buzzPin 13

// liquid crytal
# define lcdVcc A3

// speed
# define delay 200

// water level
# define stop   5 
// 10 cm  = 80%
# define full   10    
// 25 cm  = 20%
# define drain  25      

int percentage(float x) { return map(x, drain, full, 20, 80); }

bool stopped = false;

void setup() {
  // pins
  pinMode(buzzPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // lcd
  pinMode(lcdVcc, OUTPUT); 
  digitalWrite(lcdVcc, HIGH); // Input 5volts
  lcd.init();                 // Initialize
  lcd.backlight();            // Turn on screen backlight

  // serial
  Serial.begin(115200);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  const long duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  const float distance = duration * 0.034 / 2;

  const bool isFull = distance <= full;
  const bool isStop = distance <= stop; 
  const bool isDrain = distance >= drain;

  if (isStop) {
    digitalWrite(buzzPin, LOW);
    stopped = true;  

    lcd.setCursor(0, 0);  // draw at the top left
    lcd.print("Stopped..");
  }

  if (!stopped && (isFull || isDrain)) {
    digitalWrite(buzzPin, HIGH);
    
    if (isFull) {
      lcd.setCursor(0, 0);  // draw at the top left
      lcd.print("Overflowed!");
    }
    else if (isDrain) {
      lcd.setCursor(0, 0);  // draw at the top left
      lcd.print("Running out..");
    }
  }
  else if (stopped && !(isFull || isDrain)) {
    stopped = false;
  }

  if (!isFull && !isDrain) {
    lcd.setCursor(0, 0);  // draw distance (cm) at the top left
    lcd.print(String(distance) + "cm");

    lcd.setCursor(0, 1);  // draw percentage at the bottom left
    lcd.print(String(percentage(distance)) + "%");

    delay(delay);
    lcd.clear();
  }
  else {
    delay(delay);
    lcd.clear();
  }
}
