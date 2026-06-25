const uint8_t SEND_PIN = 6;
const uint8_t RECIEVE_PIN = 7; // foil to touch

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  

  pinMode(SEND_PIN, OUTPUT);

  Serial.println("Capacitive sensor:");
}

void loop() {
  long time = 0;
  pinMode(RECIEVE_PIN, OUTPUT);
  digitalWrite(RECIEVE_PIN, LOW); 
  digitalWrite(SEND_PIN, LOW); 
  delayMicroseconds(10);
  pinMode(RECIEVE_PIN, INPUT);

  digitalWrite(SEND_PIN, HIGH); 
  while(digitalRead(RECIEVE_PIN) == LOW){
    time++;
  }

  digitalWrite(SEND_PIN, LOW); 
  Serial.println(time);

  delay(100);
}