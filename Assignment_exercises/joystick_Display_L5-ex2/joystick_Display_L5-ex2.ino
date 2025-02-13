#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

// display connected to the HW I2C (this is the best option)
U8G2_SH1107_SEEED_128X128_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

int cursorX = 0; // Cursor X position
int cursorY = 0; // Cursor Y position (aligned with text baseline)

void setup(void) {
  u8g2.begin();
  Serial.begin(115200);
  cursorX = 64;
  cursorY = 64;
  u8g2.clearBuffer();
  u8g2.drawBox(cursorX, cursorY, 6, 6); // Draw a small rectangle as the cursor
  u8g2.sendBuffer();
}

void loop(void) {

  int val1 = analogRead( A0 );
  int val2 = analogRead( A1 );

/*
  Serial.print( "X-Y coordinate:" );
  Serial.print( val1, DEC );
  Serial.print( "," );
  Serial.println( val2, DEC );
  Serial.println( " " );
*/

  if (val1 > 550) {
    cursorX = cursorX + 5;
    u8g2.clearBuffer();
    u8g2.drawBox(cursorX, cursorY, 6, 6); // Draw a small rectangle as the cursor
    u8g2.sendBuffer();
  }
  else if (val1 < 400) {
    cursorX = cursorX - 5;
    u8g2.clearBuffer();
    u8g2.drawBox(cursorX, cursorY, 6, 6); // Draw a small rectangle as the cursor
    u8g2.sendBuffer();
    if (cursorX < 0){
      cursorX = cursorX + 1;
    }
  }

  if (val2 > 550) {
    cursorY = cursorY + 5;
    u8g2.clearBuffer();
    u8g2.drawBox(cursorX, cursorY, 6, 6); // Draw a small rectangle as the cursor
    u8g2.sendBuffer();
  }
  else if (val2 < 400) {
    cursorY = cursorY - 5;
    u8g2.clearBuffer();
    u8g2.drawBox(cursorX, cursorY, 6, 6); // Draw a small rectangle as the cursor
    u8g2.sendBuffer();
  }

  if (cursorX < 0){
    cursorX = cursorX + 1;
    Serial.println("Out of bounds.");
  } 
  else if (cursorX > 128){
    cursorX = cursorX - 1;
    Serial.println("Out of bounds.");
  }

    if (cursorY < 0){
    cursorY = cursorY + 1;
    Serial.println("Out of bounds.");
  } 
  else if (cursorY > 128){
    cursorY = cursorY - 1;
    Serial.println("Out of bounds.");
  }
}



void moveCursor(int x, int y) {

  if (x > 550) {
    cursorX = cursorX + 5;
    u8g2.clearBuffer();
    u8g2.drawBox(cursorX, cursorY, 6, 6); // Draw a small rectangle as the cursor
    u8g2.sendBuffer();
  }

  else if (x < 400) {
    cursorX = cursorX - 5;
    u8g2.clearBuffer();
    u8g2.drawBox(cursorX, cursorY, 6, 6); // Draw a small rectangle as the cursor
    u8g2.sendBuffer();
  }

  if (y > 550) {
    cursorY = cursorY + 5;
    u8g2.clearBuffer();
    u8g2.drawBox(cursorX, cursorY, 6, 6); // Draw a small rectangle as the cursor
    u8g2.sendBuffer();
  }

  else if (y < 400) {
    cursorY = cursorY - 5;
    u8g2.clearBuffer();
    u8g2.drawBox(cursorX, cursorY, 6, 6); // Draw a small rectangle as the cursor
    u8g2.sendBuffer();
  }
}

void joystick(){
/*
  Serial.print( "X-Y coordinate:" );
  Serial.print( val1, DEC );
  Serial.print( "," );
  Serial.println( val2, DEC );
  Serial.println( " " );
  // delay( 200 );*/
}
