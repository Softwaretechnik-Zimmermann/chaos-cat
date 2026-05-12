const uint8_t BUZZER_PIN = 3; // plus Buzzer

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

int x1 = 1135; // 440hz

void loop() {
  tone(BUZZER_PIN, 523, 200);
  delay(220);
  tone(BUZZER_PIN, 523, 200);
  delay(220);
  tone(BUZZER_PIN, 523, 200);
  delay(220);
  tone(BUZZER_PIN, 659, 200);
  delay(220);
  tone(BUZZER_PIN, 523, 400);
  delay(800);
}