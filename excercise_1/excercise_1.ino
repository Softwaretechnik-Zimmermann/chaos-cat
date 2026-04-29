const uint8_t PIN_LED = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LED, OUTPUT);
}


void loop() {
  // put your main code here, to run repeatedly:
  while(1){
    digitalWrite(PIN_LED, HIGH);
    delay(500);
    digitalWrite(PIN_LED, LOW);
    delay(500);
  }
}
