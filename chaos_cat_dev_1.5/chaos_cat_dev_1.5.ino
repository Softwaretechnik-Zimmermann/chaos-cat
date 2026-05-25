#include <Servo.h>

const uint8_t SERVO_PIN = 2;
const int SERVO_MIN_PULSE = 420;
const int SERVO_MAX_PULSE = 2420;

const uint8_t SEND_PIN = 4; // Resistor
const uint8_t RECIEVE_PIN = 5; // copper foil to touch

const uint8_t PIN_LED_R = 6;
const uint8_t PIN_LED_G = 7;
const uint8_t PIN_LED_B = 10;

const uint8_t PWMA_MOTOR = 3;
const uint8_t BREAKER_B_MOTOR = 8;
const uint8_t BREAKER_A_MOTOR = 9;
const uint8_t PWMB_MOTOR = 11;
const uint8_t DIRA_MOTOR = 12;
const uint8_t DIRB_MOTOR = 13;

const uint8_t BUZZER_PIN = A2;

Servo tailServo;

void setup() {
  tailServo.attach(SERVO_PIN, SERVO_MIN_PULSE, SERVO_MAX_PULSE);
  pinMode(SEND_PIN, OUTPUT);
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  pinMode(BREAKER_B_MOTOR,OUTPUT);  
  pinMode(BREAKER_A_MOTOR,OUTPUT);  
  pinMode(DIRA_MOTOR,OUTPUT);  
  pinMode(DIRB_MOTOR,OUTPUT);  
  pinMode(BUZZER_PIN, OUTPUT);
}

enum State {
  PASSIVE,
  HAPPY,
  ANGRY,
  AFRAID
};

State currentState = State::PASSIVE; // Set the initial state

int timer = 0;
long time = 0;
int servoTimer = 1;

int passiveToHappyWait = 2; // in Seconds
int happyToAngryWait = 20;

const int servoMinAngle = 0;
const int servoMaxAngle = 90;
bool servoFwd = true;

void loop() {
  resetState();

  switch(currentState) {
    case State::PASSIVE:
      setPassiveState();
      break;
    case State::HAPPY:
      setHappyState();
      break;
    case State::ANGRY:
      setAngryState();
      break;
    case State::AFRAID:
      setAfraidState();
      break;
  }

  timer++;
  time++;

  int loopDelay = 10;
  delay(loopDelay);
  if(currentState == State::PASSIVE && timer > passiveToHappyWait * 1000 / loopDelay || currentState == State::HAPPY && timer > happyToAngryWait * 1000 / loopDelay){
    timer = 0;
    nextState();
  }

  if(servoTimer > servoMaxAngle || servoTimer < 1){
    servoFwd = !servoFwd;
  }

  setServo(servoTimer);
  readTouch();
  readUltrasonic();
}

// Input - capacitive Sensor
void readTouch(){
  long touchTime = 0;
  pinMode(RECIEVE_PIN, OUTPUT);
  digitalWrite(RECIEVE_PIN, LOW); 
  digitalWrite(SEND_PIN, LOW); 
  delayMicroseconds(10);
  pinMode(RECIEVE_PIN, INPUT);

  digitalWrite(SEND_PIN, HIGH); 
  while(digitalRead(RECIEVE_PIN) == LOW){
    touchTime++;
  }
  digitalWrite(SEND_PIN, LOW); 
  if(touchTime > 10){ 
    currentState = State::PASSIVE;
    timer = 0;
  }
}

// Input - ultrasonic Sensor
void readUltrasonic(){
  // TODO
  // if range is large: currentState = State::AFRAID
}

// Output - Servo
void setServo(int pos) {
  tailServo.write(constrain(pos, servoMinAngle, servoMaxAngle));
}

// Output - Buzzer
void updateBuzzer(){

  switch(currentState){
    case PASSIVE:
      if(millis()-lastSound>7000){
        lastSound=millis();
        tone(BUZZER_PIN,700,150);
      }
      break;

    case HAPPY:
      if(!secondTone && millis()-lastSound>3000){
        lastSound=millis();
        tone(BUZZER_PIN,900,100);
        happyTime=millis();
        secondTone=true;
      }
      if(secondTone && millis()-happyTime>120){
        tone(BUZZER_PIN,1200,100);
        secondTone=false;
      }
      break;

    case ANGRY:
      if(millis()-lastSound>800){
        lastSound=millis();
        tone(BUZZER_PIN,300,150);
      }
      break;

    case AFRAID:
      if(millis()-lastSound>8000){
        lastSound=millis();
        // SOS
        tone(BUZZER_PIN,1200,150); delay(250);
        tone(BUZZER_PIN,1200,150); delay(250);
        tone(BUZZER_PIN,1200,150); delay(250);

        tone(BUZZER_PIN,1200,450); delay(550);
        tone(BUZZER_PIN,1200,450); delay(550);
        tone(BUZZER_PIN,1200,450); delay(550);

        tone(BUZZER_PIN,1200,150); delay(250);
        tone(BUZZER_PIN,1200,150); delay(250);
        tone(BUZZER_PIN,1200,150); delay(250);
      }
      break;
  }
}

// Output - Motor
void driveForward(int speed){ // TODO Überarbeiten
  if(currentState == State::HAPPY){
    if (time % 1000 > 1 && time % 1000 < 400){ 
      driveRight(speed);
    } else if (time % 1000 > 600 && time % 1000 < 900){
      driveLeft(speed);
    } else {
    drive(true, true, speed, speed);
    }
  }
  if(currentState == State::ANGRY){
    if (time % 100 > 1 && time % 100 < 30){ 
      driveRight(speed);
    } else if (time % 100 > 60 && time % 100 < 90){
      driveLeft(speed);
    } else {
    drive(true, true, speed, speed);
    }
  }
}

void driveBackward(int speed){
  drive(false, false, speed, speed);
}

void driveLeft(int speed){
  drive(true, true, 0, speed);
}

void driveRight(int speed){
  drive(true, true, speed, 0);
}

void drive(bool leftFWD, bool rightFWD, int speedL, int speedR){ // FWD = False means backwards, speed range from 0 to 255
  if (leftFWD){
    digitalWrite(DIRA_MOTOR, HIGH); // forward direction A
  } else {    
    digitalWrite(DIRA_MOTOR, LOW); // backward direction A
  }
  digitalWrite(BREAKER_A_MOTOR, LOW); // Brake A off
  analogWrite(PWMA_MOTOR, speedL); // Spins the motor A 
  if (rightFWD){
    digitalWrite(DIRB_MOTOR, HIGH); // forward direction B
  } else {    
    digitalWrite(DIRB_MOTOR, LOW); // backward direction B
  }
  digitalWrite(BREAKER_B_MOTOR, LOW); // Brake B off
  analogWrite(PWMB_MOTOR, speedR); // Spins the motor B
}


// State Logic
void nextState() {
  if (currentState == State::PASSIVE) currentState = State::HAPPY; 
  else if (currentState == State::HAPPY) currentState = State::ANGRY; 
}

void resetState(){
  // Reset LEDs
  digitalWrite(PIN_LED_R, LOW);
  digitalWrite(PIN_LED_G, LOW);
  digitalWrite(PIN_LED_B, LOW);
}

void setPassiveState(){
  if(servoFwd){
    servoTimer++;
  }
  else{
    servoTimer--;
  }
  digitalWrite(BREAKER_A_MOTOR, HIGH); // Brake A on
  digitalWrite(BREAKER_B_MOTOR, HIGH); // Brake B on
  // White LEDs
  digitalWrite(PIN_LED_R, HIGH);
  digitalWrite(PIN_LED_G, HIGH);
  digitalWrite(PIN_LED_B, HIGH);
}

void setHappyState(){
  if(servoFwd){
    servoTimer += 2;
  }
  else{
    servoTimer -=2;
  }
  driveForward(255);
  digitalWrite(PIN_LED_G, HIGH); // Green LEDs
}

void setAngryState(){
    if(servoFwd){
    servoTimer += 4;
  }
  else{
    servoTimer -=4;
  }
  driveForward(255);
  digitalWrite(PIN_LED_R, HIGH); // Red LEDs
}

void setAfraidState(){
  digitalWrite(PIN_LED_B, HIGH); // Blue LEDs
  digitalWrite(BREAKER_A_MOTOR, HIGH); // Brake A on
  digitalWrite(BREAKER_B_MOTOR, HIGH); // Brake B on
  delay(500);
  // try driving backwards
  driveBackward(255);
  delay(1000);
}
