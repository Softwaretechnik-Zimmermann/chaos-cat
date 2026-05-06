const uint8_t PIN_LED_B = 8;
const uint8_t PIN_LED_G = 10;
const uint8_t PIN_LED_R = 12;
const uint8_t EchoPin = 5;
const uint8_t TrigPin = 4;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  

  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  pinMode(EchoPin, INPUT);               // Set echo pin as input
  pinMode(TrigPin, OUTPUT);

  Serial.println("Ultrasonic sensor:");
}

void loop() {
  unsigned long distance = readDistance();  // Call the function to read the sensor data and get the distance
  Serial.println(distance);           // Print the distance value
  //Serial.println(" microsec");            // Print " cm" to indicate the unit of measurement
  //delay(400);                       // Delay for 400 milliseconds before repeating the loop
}

// Function to read the sensor data and calculate the distance
unsigned long readDistance() {
  digitalWrite(TrigPin, LOW);   // Set trig pin to low to ensure a clean pulse
  delayMicroseconds(2);         // Delay for 2 microseconds
  digitalWrite(TrigPin, HIGH);  // Send a 10 microsecond pulse by setting trig pin to high
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);  // Set trig pin back to low

  // Measure the pulse width of the echo pin and calculate the distance value
  unsigned long distance = pulseIn(EchoPin, HIGH); 
  return distance;
}
