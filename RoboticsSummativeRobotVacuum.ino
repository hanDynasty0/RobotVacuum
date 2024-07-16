/*
Authors: Hazel Bains, Han Fang
Date of latest update: Jan 21, 2024

This program implements a robotic vacuum cleaner (similar to a Roomba), specifically for how the robot will move around, avoid obstacles and avoid getting stuck
Measurements are read from a forwards-facing ultrasonic sensor to determine distance from objects in front, and move accordingly
*/

//Import the library for the motor shield
#include <AFMotor.h> 

AF_DCMotor leftMotor(3); //Motor 3 on the shield used for the left motor and wheel
AF_DCMotor rightMotor(2); //Motor 2 on the shield used for the right motor and wheel

//Analog pins 0 and 1 used for ultrasonic input from the front of the robot
#define echoFront A0
#define trigFront A1

long prevDistance; //Variable for the previous distance read by the ultrasonic sensor
long randomNumForTurnDirection; //Variable to store a random number and decide which way the robot will turn

//setup establishes the pin modes for the ultrasonic sensor and the speed and initial motion of the motors, and initializes the previous distance variable and random function
void setup() {
  //Declare the pin modes for the ultrasonic sensor
  pinMode(echoFront, INPUT);
  pinMode(trigFront, OUTPUT);

  leftMotor.setSpeed(120); //Sets the speed of the left motor to 120 out of a maximum speed of 255, since the left motor spins slower than the right motor
  leftMotor.run(RELEASE); //The left motor is initially resting, no rotation
  rightMotor.setSpeed(100); //Sets the speed of the right motor to 100 out of a maximum speed of 255, since the right motor spins faster than the left motor
  rightMotor.run(RELEASE); //The right motor is initially resting, no rotation

  prevDistance = -3; //Initialize the previous distance read by the ultrasonic sensor as -3, since the ultrasonic sensor hasn't read anything yet
  randomSeed(analogRead(A4)); //randomize using noise from analog pin 4
}

//loop continuously reads in distance measurements from the ultrasonic sensor and determines how the robot should move
void loop() {
  
  randomNumForTurnDirection = random(2); //Let the random number be 0 or 1

  goForwards(); //Move the robot forwards

  long distanceFront = readFrontDistance(); //Read the forward distance from the ultrasonic sensor

  //If the difference between the current and previous values of the forward distance is within 2 cm, assume the robot is stuck
  //Thus, the robot will move backwards and turn in some direction, then read and update the new forward distance
  if(abs(prevDistance - distanceFront) < 2){
    goBackwards();
    turn();
    distanceFront = readFrontDistance();
  }

  //If objects in front of the robot are within 30 cm, the robot keep turning in some direction and read and update the new forward distance
  //Until there are no objects within 30 cm in front of the robot
  while(distanceFront < 30){
    turn();
    distanceFront = readFrontDistance();
  }

  prevDistance = distanceFront; //Update the value of the previous forward distance as the current forward distance
}

//Function for reading the distance from the front of the robot to the closest object in front using the ultrasonic sensor
long readFrontDistance(){
  //Alternate ultrasonic sensor between high and low to read in measurements
  digitalWrite(trigFront, LOW);
  delayMicroseconds(2);
  digitalWrite(trigFront, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigFront, LOW);

  //Declares variables used for distance calculations, and uses measurements to calculate and return distance from objects in front in cm
  long frontDuration = pulseIn(echoFront, HIGH);
  long frontDistance = (frontDuration/2)/29.1;
  return frontDistance;
}

//Function to make the robot move forwards
void goForwards(){
  //Robot moves forward for half a second
  leftMotor.run(FORWARD);
  rightMotor.run(FORWARD);
  delay(500);

  //Robot pauses for half a second. This is to slow down the robot, allowing the vacuum to suck up more particles
  //Allow the ultrasonic sensor to read measurements while the robot is still
  //And prevent the motor shield from overheating too fast
  leftMotor.run(RELEASE);
  rightMotor.run(RELEASE);
  delay(500);
}

//Function to make the robot go backwards
void goBackwards(){
  //Robot moves forward for 750 milliseconds.
  leftMotor.run(BACKWARD);
  rightMotor.run(BACKWARD);
  delay(750);

  //Robot pauses for 750 milliseconds. This is to slow down the robot
  //Allow the ultrasonic sensor to read measurements while the robot is still
  //And prevent the motor shield from overheating too fast.
  leftMotor.run(RELEASE);
  rightMotor.run(RELEASE);
  delay(750);
}

//Function to make the robot turn in some direction depending on the randomly selected number
void turn(){
  //If the random number is 0, turn right
  if(randomNumForTurnDirection == 0){
    turnRight();
  }
  //Otherwise, turn left
  else{
    turnLeft();
  }
}

//Function to make the robot turn right
void turnRight(){
  //The right motor moves backwards while the left motor moves forwards, for 500 milliseconds, to turn right.
  leftMotor.run(FORWARD);
  rightMotor.run(BACKWARD);
  delay(500);

  //Robot pauses for half a second. This is to slow down the robot, allowing the vacuum to suck up more particles
  //Allow the ultrasonic sensor to read measurements while the robot is still
  //And prevent the motor shield from overheating too fast.
  leftMotor.run(RELEASE);
  rightMotor.run(RELEASE);
  delay(500);
}

//Function to make the robot turn left
void turnLeft(){
  //The left motor moves backwards while the right motor moves forwards, for 500 milliseconds, to turn left.
  leftMotor.run(BACKWARD);
  rightMotor.run(FORWARD);
  delay(500);

  //Robot pauses for half a second. This is to slow down the robot, allowing the vacuum to suck up more particles
  //Allow the ultrasonic sensor to read measurements while the robot is still
  //And prevent the motor shield from overheating too fast.
  leftMotor.run(RELEASE);
  rightMotor.run(RELEASE);
  delay(500);
}