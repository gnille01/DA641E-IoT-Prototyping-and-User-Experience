

#include <Adafruit_NeoPixel.h>
#include <WiFiNINA.h>

// LED_PIN refers to the pin where the LED ring is connected to, D2 corresponds to 25
#define LED_PIN 25

// LED_COUNT indicates how many LEDs there are in the ring, if you had a different NeoPixel
// configuration, like a strip, you should simply add the total amount of LEDs and put it here
#define LED_COUNT 24

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 5;     // THIS IS THE CHANGE TO BE MADE, from 2 to 3
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int num = 0;



void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(50);

  // setRingRed(false);
  
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  pinMode(LEDR, OUTPUT); // RED in RGB LED

  Serial.begin(9600); // Start serial monitor
}

void loop() {
// Read the state of the pushbutton
buttonState = digitalRead(buttonPin);


if (buttonState == LOW) {
num++;
digitalWrite(LEDR, HIGH);
if (num > 2) {
  num = 0;
}
delay(300);
}

if (buttonState == HIGH) {
digitalWrite(LEDR, LOW);
}

switch (num) {
  case 0:
    setLights(true, 255, 0, 0);
    Serial.println("LED RED");
    break;
  
  case 1:
    setLights(true, 0, 255, 0);
    Serial.println("LED GREEN");
    break;

  case 2:
    setLights(true, 0, 0, 255);
    Serial.println("LED BLUE");
    break;

  default:
    // Optional: handle unexpected cases
    break;
}
}

/*
void setRingRed() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));
  }
  strip.show();
}

void setRingGreen(bool off) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(off ? 0 : 0, 255, 0));
  }
  strip.show();
}

    void setRingBlue(bool off) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(off ? 0 : 0, 0, 255));
  }
  strip.show();
}
*/

void setLights(bool off, int r, int g, int b) {
  if (!off){
    strip.clear();
  }
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}
