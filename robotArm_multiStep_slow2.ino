#include <Servo.h>

// Define servo objects
Servo claw;
Servo arm;
Servo forearm;
Servo swivel;

// define servo pins
const int swivelPin = 3; 
const int armPin = 6;
const int forearmPin = 5;
const int clawPin = 9;

void setup() {
  // Attach servos to their respective pins
  claw.attach(clawPin);
  arm.attach(armPin);
  forearm.attach(forearmPin);
  swivel.attach(swivelPin);
  

  // Initialize the serial communication
  Serial.begin(9600);
  Serial.println("Enter servo number (1-4) followed by angle (0-180) separated by space, e.g., '1 90':");
}

void moveServoTo(Servo &servo, int targetAngle, int duration) {\
  if (duration == 0) {//Are we asking for immediate movement?
    servo.write(targetAngle);
    return;
  }
  int currentAngle = servo.read();
  int steps = abs(targetAngle - currentAngle);
  int stepDuration = duration / steps;

  for (int i = 0; i < steps; i++) {
    currentAngle += (targetAngle - currentAngle) > 0 ? 1 : -1;
    servo.write(currentAngle);
    delay(stepDuration);
  }
}

void moveArm(int angles[][3], int steps, int delayTime) {
  // angles: a 2D array containing the servo number, the angle for each step, and the duration per step in ms
  // steps: the number of steps to perform
  // delayTime: the delay in milliseconds between each step

  for (int i = 0; i < steps; i++) {
    int servoNumber = angles[i][0];
    int angle = angles[i][1];
    int duration = angles[i][2];

    // Check if the values are within the valid range
    if (servoNumber >= 1 && servoNumber <= 4 && angle >= 0 && angle <= 180 && duration >=0 && duration <= 5000) {
      // Move the corresponding servo to the desired angle
      switch (servoNumber) {
        case 1: //swivel min 0, max 180
          //swivel.write(angle)
          moveServoTo(swivel, angle, duration);
          break;

        case 2: //forearm min 50, max 130
          if (angle >= 50 && angle <= 130) {
            //forearm.write(angle);
            moveServoTo(forearm, angle, duration);
          }
          break;

        case 3: //arm min 55, max 155
          if (angle >= 55 && angle <= 155) {
            //arm.write(angle);
            moveServoTo(arm, angle, duration);
          }
          break;

        case 4: //claw min 0, max, 180
          if (angle >= 0 && angle <= 180) {
            //claw.write(angle);
            moveServoTo(claw, angle, duration);
          }
          break;
      }
      Serial.print("Servo ");
      Serial.print(servoNumber);
      Serial.print(" moved to angle ");
      Serial.println(angle);
    } else {
      Serial.println("Invalid input. Please enter a valid servo number (1-4), angle (0-180), and duration (0-5000).");
      return;
    }

    delay(delayTime);
  }
}


void loop() {
  // Move the arm in multiple steps
 
  int angles[][3] = {
    {1, 60, 100},     // swivel to the left
    {2, 50, 200},     // move forearm down
    {4, 180, 100},    // open the claw
    {3, 155, 1000},  // extend the arm
    {4, 0, 1700},    // close the claw
    {1, 90, 1000},     // swivel to the home position
    {2, 90, 1000},     // put the forearm into the home position
    {3, 90, 1000},     // move the arm to the home position
    {1, 130, 1000},    // swivel to the right
    {3, 155, 1000},  // extend the arm far
    {2, 50, 500},   // move the forearm down
    {4, 180, 500},  // open the claw
    {3, 90, 0},     // move the arm to the home position
    {1, 90, 0},     // swivel to the homeposition
    {2, 90, 0},     // move the forearm to the home position
    {4, 90, 0}      // move the claw to the home position
    };




  moveArm(angles, 16, 500);

  // Wait for a new command
  //Serial.println("Enter servo number (1-4) followed by angle (0-180) separated by space, e.g., '1 90':");
  //while (Serial.available() > 0) {
  //  Serial.read();
  //}
}