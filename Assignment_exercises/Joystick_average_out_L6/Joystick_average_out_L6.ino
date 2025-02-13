
// Joystick Average

float avg1 = 0;
float avg2 = 0;
int avgSize = 10;  // determines the amount of samples to use to make the average

void setup() {
  Serial.begin(115200);
}

void loop() {
delay(3000);
  for (int i = 0; i < avgSize; i++) {
    avg1 += analogRead(A0);
    avg2 += analogRead(A1);
    delay(1);  // note that this delay is needed to stabilise the ADC
  }
  avg1 /= avgSize;
  avg2 /= avgSize;


  Serial.print("X-Y coordinate:");
  Serial.print(int(avg1));
  Serial.print(",");
  Serial.println(int(avg2));
  Serial.println(" ");
  delay(200);

}