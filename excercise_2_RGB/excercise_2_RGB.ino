const uint8_t PIN_LED_B = 8;
const uint8_t PIN_LED_G = 10;
const uint8_t PIN_LED_R = 12;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);

}

enum States {
  PASSIVE,
  HAPPY,
  ANGRY,
  AFRAID
};

// Set the initial (i.e., starting) state
States state = States::PASSIVE;

void nextState() {
  if (state == States::PASSIVE) state = States::HAPPY; // after time
  else if (state == States::HAPPY) state = States::ANGRY; // after time if not petted
  else if (state == States::ANGRY) state = States::AFRAID; // reaching end of table
  // else state = States::PASSIVE; loop
}

void loop() {
  // put your main code here, to run repeatedly:
  while(1){
    digitalWrite(PIN_LED_R, LOW);
    digitalWrite(PIN_LED_G, LOW);
    digitalWrite(PIN_LED_B, LOW);

    // State-aware actions
    switch(state) {
      case States::PASSIVE:
        digitalWrite(PIN_LED_R, HIGH);
        digitalWrite(PIN_LED_G, HIGH);
        digitalWrite(PIN_LED_B, HIGH);
        break;
      case States::HAPPY:
        digitalWrite(PIN_LED_G, HIGH);
        break;
      case States::ANGRY:
        digitalWrite(PIN_LED_R, HIGH);
        break;
      case States::AFRAID:
        digitalWrite(PIN_LED_B, HIGH);
        break;
    }

    // Simulate some delay and then switch to the next state
    delay(3000);
    nextState();

  }
}
