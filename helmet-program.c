#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN 150   // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 450   // This is the 'maximum' pulse length count (out of 4096)
#define USMIN 600      // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX 2400     // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50  // Analog servos run at ~50 Hz updates

uint8_t servonum = 0;
int buttonPin = 2;
int ledPin = 6;
int buttonState = 0;
int globalPos = 1;
unsigned long previousMillis = 0;
unsigned long interval = 5000;
int a = 60;

int mainServoL = 0;
int mainServoR = 1;
int mainServoClosed = 5;
int mainServoOpen = 90;
int servoBrowCenter = 4;
int servoBrowCenterOpen = 40;
int servoBrowCenterClosed = 120;
int animDelay = 1;

void setup() {
  Serial.begin(9600);
  Serial.println("Boot Up");
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  pinMode(buttonPin, INPUT_PULLUP);
  delay(50);
  pwm.sleep();
}

int getAngleToPulse(int angle) {
  return map(angle, 0, 180, SERVOMIN, SERVOMAX);
}
int getAngleToMilli(int angle) {
  return map(angle, 0, 180, USMIN, USMAX);
}

void loop() {

  buttonState = digitalRead(buttonPin);
  if (buttonState == 0) {
    Serial.println("Wake up");
    pwm.wakeup();

    if (globalPos > 0) {
      Serial.println("Opening");
      // CHEEKS
      for (uint16_t pulselen = 90; pulselen >= 20; pulselen--) {
        pwm.setPWM(9, 0, getAngleToPulse(90 + 20 - pulselen));
        pwm.setPWM(8, 0, getAngleToPulse(pulselen));
      }
      delay(animDelay);
      Serial.println("1. Cheeks Open");
      // NOSE Side
      for (uint16_t pulselen = 86; pulselen >= 10; pulselen--) {
        pwm.setPWM(6, 0, getAngleToPulse(86 + 10 - pulselen));
        pwm.setPWM(7, 0, getAngleToPulse(pulselen));
      }
      delay(animDelay);
      Serial.println("2. Nose Side Open");
      //BROW Center
      for (uint16_t pulselen = servoBrowCenterClosed; pulselen >= servoBrowCenterOpen; pulselen--) {
        pwm.setPWM(servoBrowCenter, 0, getAngleToPulse(pulselen));
      }
      delay(animDelay);
      Serial.println("3. Brow Center Open");
      // BROW Side
      for (uint16_t pulselen = 30; pulselen <= 90; pulselen++) {
        pwm.setPWM(2, 0, getAngleToPulse(pulselen));
        pwm.setPWM(3, 0, getAngleToPulse(30 + 90 - pulselen));
      }
      delay(animDelay);
      Serial.println("4. Brow Side Open");
      // NOSE Center
      for (uint16_t pulselen = 110; pulselen >= 1; pulselen--) {
        pwm.setPWM(5, 0, getAngleToPulse(pulselen));
      }
      delay(animDelay);
      Serial.println("5. Nose Center Open");
      // EYES
      analogWrite(ledPin, 0); 
      // MAIN SERVOS
      for (uint16_t microsec = 750; microsec < 1950; microsec +=5) {
        pwm.writeMicroseconds(mainServoL, microsec);
        pwm.writeMicroseconds(mainServoR, (1950+750-microsec));
      }
      globalPos = 0;
      delay(animDelay);
      Serial.println("6. Main Open");
      //delay(100);
      //Serial.println("Sleep");
      //pwm.sleep();
    } else {
      Serial.println("Closing");
      //MAIN SERVOS
      for (uint16_t microsec = 1950; microsec > 750; microsec-=5) {
        pwm.writeMicroseconds(mainServoL, microsec);
        pwm.writeMicroseconds(mainServoR, (1950+750-microsec));
      }
      delay(animDelay);
      //NOSE Center
      for (uint16_t pulselen = 1; pulselen <= 110; pulselen++) {
        pwm.setPWM(5, 0, getAngleToPulse(pulselen));
      }
      
      delay(animDelay);
      
      // BROW Side
      for (uint16_t pulselen = 90; pulselen >= 30; pulselen--) {
        pwm.setPWM(2, 0, getAngleToPulse(pulselen));
        pwm.setPWM(3, 0, getAngleToPulse(30 + 90 - pulselen));
      }
      delay(animDelay);
      


      for (uint16_t pulselen = servoBrowCenterOpen; pulselen <= servoBrowCenterClosed; pulselen++) {
        pwm.setPWM(servoBrowCenter, 0, getAngleToPulse(pulselen));
      }
      delay(animDelay);
      // NOSE Side
      for (uint16_t pulselen = 10; pulselen <= 86; pulselen++) {
        pwm.setPWM(6, 0, getAngleToPulse(86 + 10 - pulselen));
        pwm.setPWM(7, 0, getAngleToPulse(pulselen));
      }
      delay(animDelay);
      //CHEEKS
      for (uint16_t pulselen = 20; pulselen <= 90; pulselen++) {
        pwm.setPWM(9, 0, getAngleToPulse(90 + 20 - pulselen));
        pwm.setPWM(8, 0, getAngleToPulse(pulselen));
      }
      delay(animDelay);
      //EYES
      analogWrite(ledPin, 255);
      globalPos = 1;
      delay(100);
      Serial.println("Sleep");
      pwm.sleep();
    }
    delay(500);
  }
  delay(10);
}
