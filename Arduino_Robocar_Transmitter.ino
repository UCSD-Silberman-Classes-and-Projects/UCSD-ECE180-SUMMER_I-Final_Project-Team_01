/*
   Robot Car Transmitter - STAGE 5 (FINAL)
   Upload to the Nano (joystick side).

   Two drivers, one steering wheel:
     - GESTURES (letters from the laptop over USB) drive by default
     - The JOYSTICK takes over the moment it leaves center
     - After the joystick sits centered for 1 second, gestures rule again

   Letters understood (from Stage 3/4):
     F=forward  B=backward  L=left  R=right  S=stop

   Wiring: unchanged. HC-12 TX->D2, RX->D3, VCC->5V, GND->GND.
   Joystick X->A0, Y->A1 as always.
*/

#include <SoftwareSerial.h>

SoftwareSerial hc12(2, 3);  // RX = D2, TX = D3

// Joystick "center zone": raw readings inside this range count
// as "not being touched". Raw center is about 512.
#define CENTER_LOW  420
#define CENTER_HIGH 600

// How long (ms) the joystick must sit centered before
// handing control back to gestures.
#define RELEASE_TIME 1000

// The gesture position (from the laptop). Starts centered = stopped.
byte gestureX = 127;
byte gestureY = 127;

unsigned long lastJoystickMove = 0;  // when the stick was last off-center
bool joystickInCharge = false;

void setup() {
  hc12.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // ---- 1. Always read the real joystick ----
  int rawX = analogRead(A0);  // 0-1023
  int rawY = analogRead(A1);

  bool stickOffCenter =
      rawX < CENTER_LOW || rawX > CENTER_HIGH ||
      rawY < CENTER_LOW || rawY > CENTER_HIGH;

  if (stickOffCenter) {
    joystickInCharge = true;             // human grabbed the wheel!
    lastJoystickMove = millis();
  }
  else if (joystickInCharge &&
           millis() - lastJoystickMove > RELEASE_TIME) {
    joystickInCharge = false;            // stick idle 1s -> gestures rule
  }

  // ---- 2. Always collect the latest gesture letter (if any) ----
  if (Serial.available() > 0) {
    char c = Serial.read();
    Serial.print("Got: "); Serial.println(c);
    if      (c == 'F' || c == 'f') { gestureX = 127; gestureY = 254; }
    else if (c == 'B' || c == 'b') { gestureX = 127; gestureY = 0;   }
    else if (c == 'L' || c == 'l') { gestureX = 0;   gestureY = 127; }
    else if (c == 'R' || c == 'r') { gestureX = 254; gestureY = 127; }
    else if (c == 'S' || c == 's') { gestureX = 127; gestureY = 127; }
  }

  // ---- 3. Whoever is in charge gets radioed out ----
  byte outX, outY;
  if (joystickInCharge) {
    outX = min(rawX / 4, 254);   // real stick position
    outY = min(rawY / 4, 254);
  } else {
    outX = gestureX;             // hand position
    outY = gestureY;
  }

  hc12.write((byte)255);  // knock
  hc12.write(outX);
  hc12.write(outY);

  delay(20);
}