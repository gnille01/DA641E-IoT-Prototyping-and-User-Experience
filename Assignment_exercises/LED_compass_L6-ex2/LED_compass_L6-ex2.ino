#include <MadgwickAHRS.h>
#include <Arduino_LSM6DS3.h>
#include <Adafruit_NeoPixel.h>

// Madgwick filter and IMU
Madgwick filter;

// LED ring definitions
#define LED_PIN 25           // Pin where the LED ring is connected
#define LED_COUNT 24         // Number of LEDs in the ring
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  while (!Serial);

  // Initialize IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // Start the filter at 104Hz (IMU frequency)
  filter.begin(104);

  // Initialize LED ring
  strip.begin();
  strip.show();
  strip.setBrightness(50); // Set brightness to 50%
}

void loop() {
  float aix, aiy, aiz;
  float gix, giy, giz;
  float ax, ay, az;
  float gx, gy, gz;
  float heading;

  // Check if IMU data is available
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    // Read acceleration and gyroscope data
    IMU.readAcceleration(aix, aiy, aiz);
    IMU.readGyroscope(gix, giy, giz);

    // Convert raw data to meaningful units
    ax = convertRawAcceleration(aix);
    ay = convertRawAcceleration(aiy);
    az = convertRawAcceleration(aiz);
    gx = convertRawGyro(gix);
    gy = convertRawGyro(giy);
    gz = convertRawGyro(giz);

    // Update Madgwick filter with IMU data
    filter.updateIMU(gx, gy, gz, ax, ay, az);

    // Get heading (yaw) from the filter
    heading = filter.getYaw();

    // Convert heading to degrees (0 to 360) and map to LED index
    int ledIndex = mapHeadingToLEDIndex(heading);

    // Update LED ring to show heading
    setRingLED(ledIndex);

    // Debug: print heading and LED index
    Serial.print("Heading: ");
    Serial.print(heading);
    Serial.print(" -> LED Index: ");
    Serial.println(ledIndex);
  }
}

float convertRawAcceleration(float aRaw) {
  // Conversion for 4G accelerometer range
  return (aRaw * 4.0) / 32768.0;
}

float convertRawGyro(float gRaw) {
  // Conversion for 2000 degrees/second gyroscope range
  return (gRaw * 2000.0) / 32768.0;
}

// Map heading (-180 to 180 degrees) to LED index (0 to LED_COUNT-1)
int mapHeadingToLEDIndex(float heading) {
  // Convert heading from radians to degrees
  float headingDegrees = heading * 180.0 / PI;

  // Normalize to 0-360 degrees
  if (headingDegrees < 0) {
    headingDegrees += 360.0;
  }

  // Map 0-360 degrees to 0-(LED_COUNT-1)
  return int((headingDegrees / 360.0) * LED_COUNT) % LED_COUNT;
}

// Set the LED corresponding to the heading and turn off others
void setRingLED(int index) {
  strip.clear(); // Clear all LEDs

  // Set the specified LED to a color (e.g., bright green)
  strip.setPixelColor(index, strip.Color(0, 255, 0));

  // Update LED ring
  strip.show();
}
