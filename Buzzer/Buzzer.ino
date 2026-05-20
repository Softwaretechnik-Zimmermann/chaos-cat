const int buzzer = 8;

enum State {PASSIVE, HAPPY, ANGRY, AFRAID};
State state = PASSIVE;

unsigned long lastSound = 0;
unsigned long happyTime = 0;
bool secondTone = false;

void setup() {
  pinMode(buzzer, OUTPUT);
}

void loop() {

  // Testzustände
  if (millis() > 10000 && millis() < 20000) state = HAPPY;
  else if (millis() > 20000 && millis() < 30000) state = ANGRY;
  else if (millis() > 30000) state = AFRAID;

  switch(state) {

    case PASSIVE:
      if (millis() - lastSound > 7000) {
        lastSound = millis();
        tone(buzzer, 700, 150);
      }
      break;

    case HAPPY:

      if (!secondTone && millis() - lastSound > 3000) {
        lastSound = millis();

        tone(buzzer, 900, 100);

        happyTime = millis();
        secondTone = true;
      }

      if (secondTone && millis() - happyTime > 120) {
        tone(buzzer, 1200, 100);
        secondTone = false;
      }

      break;

    case ANGRY:
      if (millis() - lastSound > 800) {
        lastSound = millis();
        tone(buzzer, 300, 150);
      }
      break;

    case AFRAID:
      if (millis() - lastSound > 8000) {
        lastSound = millis();

        // SOS
        tone(buzzer,1200,150); delay(250);
        tone(buzzer,1200,150); delay(250);
        tone(buzzer,1200,150); delay(250);

        tone(buzzer,1200,450); delay(550);
        tone(buzzer,1200,450); delay(550);
        tone(buzzer,1200,450); delay(550);

        tone(buzzer,1200,150); delay(250);
        tone(buzzer,1200,150); delay(250);
        tone(buzzer,1200,150); delay(250);
      }
      break;
  }
}