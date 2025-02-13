#include <MadgwickAHRS.h>
#include <Arduino_LSM6DS3.h>
#include <Adafruit_NeoPixel.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

// Madgwick filter and IMU
Madgwick filter;

// LED ring definitions
#define LED_PIN 25            // Pin where the LED ring is connected
#define LED_COUNT 24          // Number of LEDs in the ring
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Alarm thresholds
const float pitchRawThreshold = 10.0;  // Pitch alarm threshold (raw units)
const float rollRawThreshold = 10.0;   // Roll alarm threshold (raw units)
const float yawLowerThreshold = 170.0; // Lower yaw alarm threshold
const float yawUpperThreshold = 210.0; // Upper yaw alarm threshold
const float thresholdHysteresis = 2.0; // Deadband to avoid rapid switching
bool alarmActive = false;

// Sensor update delay
const int sensorUpdateInterval = 50; // 50ms delay (20Hz update rate)
unsigned long lastSensorUpdate = 0;

// OLED Display
U8G2_SH1107_SEEED_128X128_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  Serial.begin(115200); // Start Serial communication for debugging

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
  strip.setBrightness(50);

  // Initialize OLED display
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr); // Choose a readable font
  u8g2.drawStr(0, 10, "System Initialized");
  u8g2.sendBuffer();

  Serial.println("System Initialized.");
  Serial.println("Monitoring Raw Yaw, Pitch, Roll...");
}

void loop() {
  if (millis() - lastSensorUpdate >= sensorUpdateInterval) {
    lastSensorUpdate = millis();

    float aix, aiy, aiz;
    float gix, giy, giz;
    float ax, ay, az;
    float gx, gy, gz;
    float rawYaw, rawPitch, rawRoll;

    // Check if IMU data is available
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
      // Read acceleration and gyroscope data
      IMU.readAcceleration(aix, aiy, aiz);
      IMU.readGyroscope(gix, giy, giz);

      // Use raw acceleration and gyroscope values
      ax = aix;
      ay = aiy;
      az = aiz;
      gx = gix;
      gy = giy;
      gz = giz;

      // Update Madgwick filter with IMU data
      filter.updateIMU(gx, gy, gz, ax, ay, az);

      // Get raw yaw, pitch, and roll (do not convert to degrees)
      rawYaw = filter.getYaw(); 
      rawPitch = filter.getPitch();
      rawRoll = filter.getRoll();

      // Update LED ring to show raw yaw
      int ledIndex = mapHeadingToLEDIndex(rawYaw);
      setRingLED(ledIndex);

      // Check and handle alarm conditions
      checkAlarm(rawYaw, rawPitch, rawRoll);

      // Update OLED Display with warnings
      updateDisplay(rawYaw, rawPitch, rawRoll);

      // Print raw attitude data and alarm status
      Serial.print("Yaw (raw): ");
      Serial.print(rawYaw);
      Serial.print(" | Pitch (raw): ");
      Serial.print(rawPitch);
      Serial.print(" | Roll (raw): ");
      Serial.print(rawRoll);

      if (alarmActive) {
        Serial.println(" --> Alarm ACTIVE! Threshold exceeded.");
      } else {
        Serial.println(" --> Alarm OFF.");
      }
    }
  }
}

// Map yaw (0 to 2π radians) to LED index (0 to LED_COUNT-1)
int mapHeadingToLEDIndex(float rawYaw) {
  return int((rawYaw / (2.0 * PI)) * LED_COUNT) % LED_COUNT;
}

// Set the LED corresponding to raw yaw and turn off others
void setRingLED(int index) {
  strip.clear(); // Clear all LEDs
  strip.setPixelColor(index, strip.Color(0, 255, 0)); // Set green LED
  strip.show();
}

// Check for alarm conditions using raw values
void checkAlarm(float rawYaw, float rawPitch, float rawRoll) {
  // Define alarm condition with hysteresis
  bool yawInAlarm = (rawYaw < (yawLowerThreshold - thresholdHysteresis)) || 
                    (rawYaw > (yawUpperThreshold + thresholdHysteresis));
  bool pitchInAlarm = abs(rawPitch) > (pitchRawThreshold + thresholdHysteresis);
  bool rollInAlarm = abs(rawRoll) > (rollRawThreshold + thresholdHysteresis);

  bool inAlarmCondition = yawInAlarm || pitchInAlarm || rollInAlarm;

  // Ensure the alarm only resets when parameters return to safe range
  if (!inAlarmCondition) {
    alarmActive = false; // Deactivate alarm if conditions are within safe range
    return;
  }

  if (!alarmActive && inAlarmCondition) {
    alarmActive = true; // Activate alarm when thresholds are exceeded
  }
}

// Update OLED display with warnings
void updateDisplay(float rawYaw, float rawPitch, float rawRoll) {
  u8g2.clearBuffer(); // Clear the display buffer

  u8g2.setFont(u8g2_font_ncenB08_tr); // Choose a readable font

  u8g2.drawStr(0, 10, "Attitude Warnings:");

  if (rawYaw < yawLowerThreshold || rawYaw > yawUpperThreshold) {
    u8g2.drawStr(0, 30, "YAW: OUT OF RANGE");
  }

  if (abs(rawPitch) > pitchRawThreshold) {
    u8g2.drawStr(0, 50, "PITCH: OUT OF RANGE");
  }

  if (abs(rawRoll) > rollRawThreshold) {
    u8g2.drawStr(0, 70, "ROLL: OUT OF RANGE");
  }

  if (!alarmActive) {
    u8g2.drawStr(0, 90, "ALL SYSTEMS NORMAL");
  }

  u8g2.sendBuffer(); // Send data to the display
}
