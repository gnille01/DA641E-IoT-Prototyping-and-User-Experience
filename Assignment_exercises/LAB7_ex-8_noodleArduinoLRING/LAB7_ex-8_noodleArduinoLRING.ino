#include "defines.h"

#include <WebSockets2_Generic.h>

#include <Adafruit_NeoPixel.h>

// D2 corresponds to 25
#define LED_PIN 25

// how many LEDs there are in the ring
#define LED_COUNT 24

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int r,g,b;

using namespace websockets2_generic;

WebsocketsClient client;

void onEventsCallback(WebsocketsEvent event, String data) 
{
  (void) data;
  
  if (event == WebsocketsEvent::ConnectionOpened) 
  {
    Serial.println("Connnection Opened");
  } 
  else if (event == WebsocketsEvent::ConnectionClosed) 
  {
    Serial.println("Connnection Closed");
  } 
  else if (event == WebsocketsEvent::GotPing) 
  {
    Serial.println("Got a Ping!");
  } 
  else if (event == WebsocketsEvent::GotPong) 
  {
    Serial.println("Got a Pong!");
  }
}

void setup() 
{
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  strip.begin();
  strip.show();
  strip.setBrightness(50);

  Serial.println("\nStarting RP2040-RepeatingClient with WiFiNINA on " + String(BOARD_NAME));
  Serial.println(WEBSOCKETS2_GENERIC_VERSION);
  
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) 
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    return;
  }

  String fv = WiFi.firmwareVersion();
  
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) 
  {
    Serial.println("Please upgrade the firmware");
  }

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);

  // Connect to wifi
  WiFi.begin(ssid, password);

  // Wait some time to connect to wifi
  for (int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++)
  {
    Serial.print(".");
    delay(1000);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("Connected to Wifi, IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Connecting to WebSockets Server @");
    Serial.println(websockets_server_host);
  }
  else
  {
    Serial.println("\nNo WiFi");
    return;
  }

  // run callback when messages are received
  client.onMessage([&](WebsocketsMessage message) 
  {
    Serial.print("Got Message: ");
    Serial.println(message.data());
    Serial.println("");

    String data = message.data();

    // Extract Red (R) value
    int comma1 = data.indexOf(',');
    r = data.substring(0, comma1).toInt();

    // Extract Green (G) value
    int comma2 = data.indexOf(',', comma1 + 1);
    g = data.substring(comma1 + 1, comma2).toInt();

    // Extract Blue (B) value
    b = data.substring(comma2 + 1).toInt();

    // Print the extracted RGB values
    Serial.print("Red: ");
    Serial.print(r);
    Serial.print(", Green: ");
    Serial.print(g);
    Serial.print(", Blue: ");
    Serial.println(b);
    Serial.println("");
  });

  // run callback when events are occuring
  client.onEvent(onEventsCallback);
}

void sendMessage()
{
// try to connect to Websockets server
  bool connected = client.connect(websockets_server_host, websockets_server_port, "/");
  
  if (connected) 
  {
    Serial.println("Connected!");

    String WS_msg = String("Hello to Server from ") + BOARD_NAME;
    client.send(WS_msg);
  } 
  else 
  {
    Serial.println("Not Connected!");
  }
}

void checkToSendMessage()
{
  #define REPEAT_INTERVAL    10000L
  
  static unsigned long checkstatus_timeout = 1000;

  // Send WebSockets message every REPEAT_INTERVAL (10) seconds.
  if (millis() > checkstatus_timeout)
  {
    sendMessage();
    checkstatus_timeout = millis() + REPEAT_INTERVAL;
  }
}

void loop() 
{
  checkToSendMessage();
  
  // let the websockets client check for incoming messages
  if (client.available()) 
  {
    client.poll();
  }

  // Update the LED ring based on the RGB values
  for (int i = 0; i < LED_COUNT; i++) 
  {
    strip.setPixelColor(i, strip.Color(r, g, b)); // Set color for each pixel
  }
  strip.show(); // Refresh the strip to display the new colors
}
