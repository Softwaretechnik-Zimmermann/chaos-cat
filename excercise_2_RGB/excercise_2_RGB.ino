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
  RED,
  GREEN,
  BLUE
};

// Set the initial (i.e., starting) state
States state = States::RED;

void nextState() {
  if (state == States::RED) state = States::GREEN;
  else if (state == States::GREEN) state = States::BLUE;
  else state = States::RED;
}

void loop() {
  // put your main code here, to run repeatedly:
  while(1){
    digitalWrite(PIN_LED_R, LOW);
    digitalWrite(PIN_LED_G, LOW);
    digitalWrite(PIN_LED_B, LOW);

    // State-aware actions
    switch(state) {
      case States::RED:
        digitalWrite(PIN_LED_R, HIGH);
        break;
      case States::GREEN:
        digitalWrite(PIN_LED_G, HIGH);
        break;
      case States::BLUE:
        digitalWrite(PIN_LED_B, HIGH);
        break;
    }

    // Simulate some delay and then switch to the next state
    delay(1000);
    nextState();

  }
}
