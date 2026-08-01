
#define MAX_SPEED 100   // 0-255, cap for all movement
#define MIN_SPEED 45  // below this PWM the motors just buzz

#define enA 9
#define in1 4
#define in2 5
#define enB 10
#define in3 6
#define in4 7

int xAxis, yAxis;
unsigned long lastMessage = 0;  // when the last good message arrived

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial1.begin(9600);  // HC-12 on the UNO Q's D0/D1 pins
}

void loop() {
  // ---- Listen for a complete message: [255] [X] [Y] ----
  if (Serial1.available() >= 3) {
    int first = Serial1.read();

    if (first == 255) {
      // Knock heard! Make sure both data bytes are really here.
      unsigned long t = millis();
      while (Serial1.available() < 2 && millis() - t < 50) {}

      if (Serial1.available() >= 2) {
        int x = Serial1.read();
        int y = Serial1.read();
        xAxis = x * 4;  // back to 0-1023 for the motor math
        yAxis = y * 4;
        lastMessage = millis();
        drive(xAxis, yAxis);
      }
    }
    // If 'first' wasn't 255 it was noise or a stray data byte:
    // we throw it away and keep looking. This is how the receiver
    // re-finds its place after any hiccup.
  }

  // ---- Failsafe: no message for 300 ms -> stop the car ----
  if (millis() - lastMessage > 300) {
    analogWrite(enA, 0);
    analogWrite(enB, 0);
  }
}

// ---- Same driving math as before, just moved into a function ----
void drive(int xAxis, int yAxis) {
   //xAxis = 1023 - xAxis;
  int motorSpeedA = 0;
  int motorSpeedB = 0;

  // Y-axis: forward and backward
  if (yAxis < 470) {
    digitalWrite(in1, HIGH);  // Motor A backward
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);  // Motor B backward
    digitalWrite(in4, LOW);
    motorSpeedA = map(yAxis, 470, 0, 0, MAX_SPEED);
    motorSpeedB = map(yAxis, 470, 0, 0, MAX_SPEED);
  }
  else if (yAxis > 550) {
    digitalWrite(in1, LOW);   // Motor A forward
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);   // Motor B forward
    digitalWrite(in4, HIGH);
    motorSpeedA = map(yAxis, 550, 1023, 0, MAX_SPEED);
    motorSpeedB = map(yAxis, 550, 1023, 0, MAX_SPEED);
  }

  // X-axis: left and right
  if (xAxis < 470) {
    int xMapped = map(xAxis, 470, 0, 0, MAX_SPEED);
    motorSpeedA = motorSpeedA + xMapped;
    motorSpeedB = motorSpeedB - xMapped;
  }
  if (xAxis > 550) {
    int xMapped = map(xAxis, 550, 1023, 0, MAX_SPEED);
    motorSpeedA = motorSpeedA - xMapped;
    motorSpeedB = motorSpeedB + xMapped;
  }

  motorSpeedA = constrain(motorSpeedA, 0, MAX_SPEED);
  motorSpeedB = constrain(motorSpeedB, 0, MAX_SPEED);

  if (motorSpeedA < MIN_SPEED) motorSpeedA = 0;
  if (motorSpeedB < MIN_SPEED) motorSpeedB = 0;

  analogWrite(enA, motorSpeedA);
  analogWrite(enB, motorSpeedB);
}