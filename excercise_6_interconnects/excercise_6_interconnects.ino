char* letter = "vier";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.print(letter);
  if (Serial.available()) {
    Serial.println(Serial.readBytes(letter, 1));
  }     
}
// voltage output BIT Sequence: 
//      0 01010110 1
// Start |  data  | stop
//       ASCII Code