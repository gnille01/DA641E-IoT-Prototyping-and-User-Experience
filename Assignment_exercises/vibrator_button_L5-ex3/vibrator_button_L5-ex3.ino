int motorPin = 4; // motor pin
const int buttonPin = 3;  // button pin
int buttonState = 0; // button state
int lastButtonState = HIGH; // last button state
int presses = 0;  // button presses

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 5000;  // 5-second period

void setup() {
  // Initialize motor and button pins
  pinMode(motorPin, OUTPUT);
  pinMode(buttonPin, INPUT); // Corrected to INPUT

  startMillis = millis();  // Initial start time

  Serial.begin(9600); // Start serial monitor
  Serial.println("System initialized.");
  Serial.println("Sampling will start now...");
}

void loop() {
  currentMillis = millis(); // Update current time

  // Count button presses within the period
  countPresses();

  // Check if the 5-second period has ended
  if (currentMillis - startMillis >= period) {
    Serial.println("Sampling period ended.");
    Serial.print("Total button presses detected: ");
    Serial.println(presses);

    // Vibrate based on the number of presses
    Serial.println("Starting vibration...");
    vibrations(presses);

    // Reset for the next cycle
    presses = 0;
    startMillis = millis(); // Reset timer
    Serial.println("Sampling restarted.");
  }
}

void countPresses() {
  int reading = digitalRead(buttonPin); // Read the button state

  // Detect button state change (from HIGH to LOW)
  if (reading == LOW && lastButtonState == HIGH) {
    delay(50); // Debounce delay
    presses++;
    Serial.println("Button pressed.");
  }

  // Save the current state as the last state
  lastButtonState = reading;
}

void vibrations(int amount) {
  for (int i = 0; i < amount; i++) {
    Serial.print("Vibrating... Cycle ");
    Serial.print(i + 1);
    Serial.print(" of ");
    Serial.println(amount);

    digitalWrite(motorPin, HIGH);  // Turn the motor on
    delay(500);                   // Wait for 0.5 seconds
    digitalWrite(motorPin, LOW);   // Turn the motor off
    delay(500);                   // Wait for 0.5 seconds
  }
  Serial.println("Vibration sequence completed.");
}
