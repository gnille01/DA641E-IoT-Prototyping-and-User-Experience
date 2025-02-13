
// Joystick running average

const int avgSize = 10;  // determines the amount of samples to use to make the average
float avgBuffer1[avgSize];
float avgBuffer2[avgSize];
int avgIndex = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  // read current data, and
  // store readings in array
  avgBuffer1[avgIndex] = analogRead(A0);
  avgBuffer2[avgIndex] = analogRead(A1);
  avgIndex++;

  // reset the index
  if (avgIndex >= avgSize) avgIndex = 0;

  // create variables to calculate the average
  float avg1 = 0;
  float avg2 = 0;

  for (int i = 0; i < avgSize; i++) {

    avg1 += avgBuffer1[i];
    avg2 += avgBuffer2[i];
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