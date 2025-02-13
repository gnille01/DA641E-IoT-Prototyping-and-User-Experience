
#include <Arduino_LSM6DSOX.h>

float Ax, Ay, Az;
float r_magnitude;

const int avgSize = 10; // amount of samples to make the average
float avgBuffer[avgSize];
int avgIndex = 0;
float avg = 0;  // variable to calculate the average

int motorPin = 2; // Vibrator
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 5000;  // 5-second period

void setup() {
  // put your setup code here, to run once:

  pinMode(motorPin, OUTPUT);

  startMillis = millis();  // Initial start time

  Serial.begin(9600);

  while(!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println("Hz");
  Serial.println();

}

void loop() {
  
  currentMillis = millis(); // Update current time

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(Ax, Ay, Az);

    // Call modulus function to calculate the magnitude
    modulus();
  }
  avgBuffer[avgIndex] = r_magnitude; // Store magnitutde in array
  avgIndex++;

  // reset the index
  if (avgIndex >= avgSize) avgIndex = 0;

  for (int i = 0; i < avgSize; i++) {
    avg += avgBuffer[i];
  }

  avg /= avgSize;
  if (avg > 1.14 || avg < 1.12) {
    Serial.println("Movement detected!");
    Serial.println("Magnitude: ");
    Serial.print(avg);
    Serial.println();
    startMillis = millis(); // Reset timer
  } else {
    if (currentMillis - startMillis >= period) {
      Serial.println("Device is idle.");
      /*
      Serial.print("Magnitude: ");
      Serial.print(avg);
      Serial.println("");*/
      vibrations();
    }
    
  }
  
  delay(100);
}

void modulus() {
  // Magnitude of the vector - 1-dimensional quantity instead of 3
  r_magnitude = sqrt(Ax * Ax + Ay * Ay + Az * Az);
}

void vibrations() {
  digitalWrite(motorPin, HIGH);
  delay(500);                   // Wait for 0.5 seconds
  digitalWrite(motorPin, LOW);   // Turn the motor off
  delay(500);                   // Wait for 0.5 seconds
  Serial.println("*Vibrating*");
}