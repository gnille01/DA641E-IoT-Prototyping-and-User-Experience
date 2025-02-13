// Pin assignments
const int pinX = A0;
const int pinY = A1;

// Filtering parameters
const int avgSize = 50;  // Window size for average
float avgBufferX[avgSize];
float avgBufferY[avgSize];
int avgIndex = 0;
float prevSmoothedX = 0.0;
float prevSmoothedY = 0.0;
float weight = 0.1;  // Smoothing factor for EMA

// Counters to track printed values
int counter = 0;
const int maxPrints = 50;

void setup() {
  Serial.begin(115200);
  delay(5000);
  // Print CSV header
  Serial.println("RawX,RawY,SimpleAvgX,SimpleAvgY,RunningAvgX,RunningAvgY,EMA_X,EMA_Y");
  // Initialize buffers
  for (int i = 0; i < avgSize; i++) {
    avgBufferX[i] = 0;
    avgBufferY[i] = 0;
  }
}

void loop() {
  if (counter >= maxPrints) {
    // Stop after maxPrints rows of output
    while (true);
  }

  // Raw data
  int rawX = analogRead(pinX);
  int rawY = analogRead(pinY);

  // Simple average
  float sumX = 0;
  float sumY = 0;
  for (int i = 0; i < avgSize; i++) {
    sumX += analogRead(pinX);
    sumY += analogRead(pinY);
    delay(1);  // Stabilize ADC readings
  }
  float simpleAvgX = sumX / avgSize;
  float simpleAvgY = sumY / avgSize;

  // Running average
  avgBufferX[avgIndex] = rawX;
  avgBufferY[avgIndex] = rawY;
  avgIndex = (avgIndex + 1) % avgSize;

  float runningAvgX = 0;
  float runningAvgY = 0;
  for (int i = 0; i < avgSize; i++) {
    runningAvgX += avgBufferX[i];
    runningAvgY += avgBufferY[i];
  }
  runningAvgX /= avgSize;
  runningAvgY /= avgSize;

  // Exponential Moving Average
  float smoothedX = filter(rawX, weight, prevSmoothedX);
  float smoothedY = filter(rawY, weight, prevSmoothedY);
  prevSmoothedX = smoothedX;
  prevSmoothedY = smoothedY;

  // Print all data in one row
  Serial.print(rawX);
  Serial.print(",");
  Serial.print(rawY);
  Serial.print(",");
  Serial.print(int(simpleAvgX));
  Serial.print(",");
  Serial.print(int(simpleAvgY));
  Serial.print(",");
  Serial.print(int(runningAvgX));
  Serial.print(",");
  Serial.print(int(runningAvgY));
  Serial.print(",");
  Serial.print(int(smoothedX));
  Serial.print(",");
  Serial.println(int(smoothedY));

  counter++;
  delay(200);
}

float filter(float rawVal, float w, float prevVal) {
  return w * rawVal + (1.0 - w) * prevVal;
}
