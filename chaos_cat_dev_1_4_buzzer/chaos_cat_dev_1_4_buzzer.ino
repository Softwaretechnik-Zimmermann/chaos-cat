#include <Servo.h>

const uint8_t SERVO_PIN = 2;
const int SERVO_MIN_PULSE = 420;
const int SERVO_MAX_PULSE = 2420;
const uint8_t PWMA_MOTOR = 3;
const uint8_t SEND_PIN = 4; // Resistor
const uint8_t RECIEVE_PIN = 5; // copper foil to touch
const uint8_t PIN_LED_R = 6;
const uint8_t PIN_LED_G = 7;
const uint8_t BREAKER_B_MOTOR = 8;
const uint8_t BREAKER_A_MOTOR = 9;
const uint8_t PIN_LED_B = 10;
const uint8_t PWMB_MOTOR = 11;
const uint8_t DIRA_MOTOR = 12;
const uint8_t DIRB_MOTOR = 13;

const uint8_t BUZZER_PIN = A0; // A0 noch frei

Servo tailServo;

void resetState();
void setPassiveState();
void setHappyState();
void setAngryState();
void setAfraidState();
void nextState();

void setServo(int pos);

void readTouch();
void readUltrasonic();

void driveForward(int speed);
void driveBackward(int speed);
void driveLeft(int speed);
void driveRight(int speed);
void drive(bool leftFWD, bool rightFWD, int speedL, int speedR);

void updateBuzzer();

enum State {
  PASSIVE,
  HAPPY,
  ANGRY,
  AFRAID
};

State currentState = State::PASSIVE;
int timer = 0;
long time = 0;
int servoTimer = 1;
int passiveToHappyWait = 2;
int happyToAngryWait = 20;
const int minValue = 0;
const int maxValue = 180;
bool servoFwd = true;

unsigned long lastSound = 0;
unsigned long happyTime = 0;
bool secondTone = false;

void setup() {
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  pinMode(SEND_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  tailServo.attach(SERVO_PIN, SERVO_MIN_PULSE, SERVO_MAX_PULSE);

  pinMode(BREAKER_B_MOTOR,OUTPUT);  
  pinMode(BREAKER_A_MOTOR,OUTPUT);  
  pinMode(DIRA_MOTOR,OUTPUT);  
  pinMode(DIRB_MOTOR,OUTPUT);  
}

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

  updateBuzzer();

  timer++;
  time++;

  int loopDelay = 10;
  delay(loopDelay);

  if(currentState == State::PASSIVE && timer > passiveToHappyWait * 1000 / loopDelay || currentState == State::HAPPY && timer > happyToAngryWait * 1000 / loopDelay){
    timer = 0;
    nextState();
  }

  if(servoTimer > 180 || servoTimer < 1){
    servoFwd = !servoFwd;
  }

  setServo(servoTimer);

  readTouch();
  readUltrasonic();
}

// Input read

void readTouch(){

  long touchTime = 0;

  pinMode(RECIEVE_PIN, OUTPUT);

  digitalWrite(RECIEVE_PIN, LOW);
  digitalWrite(SEND_PIN, LOW);

  delayMicroseconds(10);

  pinMode(RECIEVE_PIN, INPUT);

  digitalWrite(SEND_PIN, HIGH);

  while(digitalRead(RECIEVE_PIN) == LOW && touchTime < 1000){
    touchTime++;
  }

  digitalWrite(SEND_PIN, LOW);

  if(touchTime > 10){
    currentState = State::PASSIVE;
    timer = 0;
  }
}

void readUltrasonic(){
  // TODO
  // if range is large: currentState = State::AFRAID
}

// Output - Servo

void setServo(int pos) {
  tailServo.write(constrain(pos, minValue, maxValue));
}

// Output - Motor

void driveForward(int speed){

  if(currentState == State::HAPPY){

    if(time % 1000 < 400){
      driveRight(speed);
    }
    else if(time % 1000 > 600 && time % 1000 < 900){
      driveLeft(speed);
    }
    else{
      drive(true,true,speed,speed);
    }
  }

  else if(currentState == State::ANGRY){

    if(time % 100 < 30){
      driveRight(speed);
    }
    else if(time % 100 > 60 && time % 100 < 90){
      driveLeft(speed);
    }
    else{
      drive(true,true,speed,speed);
    }
  }
}

void driveBackward(int speed){
  drive(false,false,speed,speed);
}

void driveLeft(int speed){
  drive(true,true,0,speed);
}

void driveRight(int speed){
  drive(true,true,speed,0);
}

void drive(bool leftFWD, bool rightFWD, int speedL, int speedR){

  if(leftFWD){
    digitalWrite(DIRA_MOTOR,HIGH);
  } else {
    digitalWrite(DIRA_MOTOR,LOW);
  }

  digitalWrite(BREAKER_A_MOTOR,LOW);

  analogWrite(PWMA_MOTOR,speedL);

  if(rightFWD){
    digitalWrite(DIRB_MOTOR,HIGH);
  } else {
    digitalWrite(DIRB_MOTOR,LOW);
  }

  digitalWrite(BREAKER_B_MOTOR,LOW);

  analogWrite(PWMB_MOTOR,speedR);
}

// State Logic

void nextState(){

  if(currentState == State::PASSIVE)
    currentState = State::HAPPY;

  else if(currentState == State::HAPPY)
    currentState = State::ANGRY;
}

void resetState(){

  digitalWrite(PIN_LED_R,LOW);
  digitalWrite(PIN_LED_G,LOW);
  digitalWrite(PIN_LED_B,LOW);
}

void setPassiveState(){

  if(servoFwd){
    servoTimer++;
  }
  else{
    servoTimer--;
  }

  digitalWrite(BREAKER_A_MOTOR,HIGH);
  digitalWrite(BREAKER_B_MOTOR,HIGH);

  digitalWrite(PIN_LED_R,HIGH);
  digitalWrite(PIN_LED_G,HIGH);
  digitalWrite(PIN_LED_B,HIGH);
}

void setHappyState(){

  if(servoFwd){
    servoTimer += 2;
  }
  else{
    servoTimer -=2;
  }

  driveForward(255);

  digitalWrite(PIN_LED_G,HIGH);
}

void setAngryState(){

  if(servoFwd){
    servoTimer +=4;
  }
  else{
    servoTimer -=4;
  }

  driveForward(255);

  digitalWrite(PIN_LED_R,HIGH);
}

void setAfraidState(){

  digitalWrite(PIN_LED_B,HIGH);

  digitalWrite(BREAKER_A_MOTOR,HIGH);
  digitalWrite(BREAKER_B_MOTOR,HIGH);

  delay(500);

  driveBackward(255);

  delay(1000);
}

// Buzzer

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