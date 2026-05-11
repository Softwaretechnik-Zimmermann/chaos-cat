const uint8_t PIN_LED_R = 8;
const uint8_t PIN_LED_G = 10;
const uint8_t PIN_LED_B = 12;
const uint8_t SEND_PIN = 6; // Resistor
const uint8_t RECIEVE_PIN = 7; // copper foil to touch
const uint8_t SERVO_PIN = 2;

void setup() {
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  pinMode(SEND_PIN, OUTPUT);
  pinMode(SERVO_PIN,OUTPUT);  
}

enum State {
  PASSIVE,
  HAPPY,
  ANGRY,
  AFRAID
};

State currentState = State::PASSIVE; // Set the initial state
int timer = 0;
int servoTimer = 1;
int passiveToHappyWait = 2; // in Seconds
int happyToAngryWait = 2;
const int minValue = 0;
const int maxValue = 180;
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
  int loopDelay = 10;
  delay(loopDelay);
  if(currentState == State::PASSIVE && timer > passiveToHappyWait * 1000 / loopDelay || currentState == State::HAPPY && timer > happyToAngryWait * 1000 / loopDelay){
    timer = 0;
    nextState();
  }

  if(servoTimer > 90 || servoTimer < 1){
    servoFwd = !servoFwd;
  }
  setServo(servoTimer);
  // Touch detection
  readTouch();
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
  while(digitalRead(RECIEVE_PIN) == LOW){
    touchTime++;
  }
  digitalWrite(SEND_PIN, LOW); 
  if(touchTime > 10){ 
    currentState = State::PASSIVE;
    timer = 0;
  }
}

// Output
void setServo(int pos) {
  int puls = map(pos,minValue,maxValue,400,2400);
  digitalWrite(SERVO_PIN,HIGH);
  delayMicroseconds(puls);
  digitalWrite(SERVO_PIN,LOW);
  delay(19);
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

  digitalWrite(PIN_LED_G, HIGH); // Green LEDs
}

void setAngryState(){
    if(servoFwd){
    servoTimer += 4;
  }
  else{
    servoTimer -=4;
  }

  digitalWrite(PIN_LED_R, HIGH); // Red LEDs
}

void setAfraidState(){
  digitalWrite(PIN_LED_B, HIGH); // Blue LEDs
}
