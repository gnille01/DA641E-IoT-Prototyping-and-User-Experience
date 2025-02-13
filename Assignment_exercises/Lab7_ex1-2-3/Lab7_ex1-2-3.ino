
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>


// display connected to the HW I2C (this is the best option)
U8G2_SH1107_SEEED_128X128_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

int item_index = 0;   // Index of item
int items_length = 0;   // Amount of items in Main Menu
int sub_items_length = 0; // Amount of items in submenu
// int menuSel = 0;    // Menu selection
int state = 0; // 0: MAIN_MENU , 1:SUB_MENU , 1:LOGOGO



void setup() {
  // put your setup code here, to run once:

  u8g2.begin();
  Serial.begin(115200);


}

void loop() {
  // read analog values of joystick
  int joystick_x = analogRead( A0 );
  int joystick_y = analogRead( A1 );
  // Call joystick func 
  joystick(joystick_x, joystick_y);
  

  String items[] = { String("item1"), String("item2"), String("item3")};  // Main menu
    items_length = sizeof(items) / sizeof(items[0]);
  
  String sub_items[] = { String("sub_item1"), String("sub_item2"), String("sub_item3")}; // A submenu
    sub_items_length = sizeof(sub_items) / sizeof(sub_items[0]);
  //Serial.println(items_length);
  
  switch (state) {
    case 0:
      showMenu(items, items_length, item_index);
      break;
    
    case 1:
      showMenu(sub_items, sub_items_length, item_index); // submenu
      break;

    case 2:
      drawLogo();
      break;
    
    default:
      break;
  } 
}

void joystick(int val1, int val2) {
  if (val1 > 600) {
    if (val1 > 1000){
      Serial.println("CLICK");
      // menuSel++;
    }
    else if (val1 < 900) {
      Serial.println("RIGHT");
      if (state == 0) { // MAIN MENU
        if (item_index == 0) {
        state = 1; // SUB MENU
        }
        else {
          state = 2; // LOGO
        }
      }
      Serial.print(state);
      Serial.println(" state");
      Serial.print(item_index);
      Serial.println(" index");
    }
  }
  else if (val1 < 400) {
    Serial.println("LEFT");
    if (state == 1 || state == 2) {
      state = 0; // MAIN MENU
    }
    Serial.println(state);
  }

  if (val2 > 600) {       
    Serial.println("UP");
    item_index--;       // subtract from item index 
     if (item_index < 0){ // if item index is lower than first item
    //Serial.println(items_length - items_length);
    item_index = items_length - 1; // set item index to highest item
    }
    Serial.println(state);
  }
  else if (val2 < 400) {
    Serial.println("DOWN");
    item_index++;
    if (item_index == items_length) { // if item index is higher than last item
      item_index = 0;  // set item index to lowest item
    }
    Serial.println(state);
  }
  delay(50);
}

void showMenu(String items[], int itemN, int selected) {
  int itemY = 14;

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB12_tr);

  
  // Loop through itmes in array
  for (int i = 0; i < itemN; i++) {
    if (i == selected) {
      u8g2.drawBox(0, itemY - 12, 128, 15);
      u8g2.setFontMode(0);
      u8g2.setDrawColor(0);
    } else {
      u8g2.setFontMode(0);
      u8g2.setDrawColor(1);
    }

  // Draw curr item
  u8g2.drawStr(0, itemY, items[i].c_str());
      u8g2.setDrawColor(1);

  // Update vertical position 
  itemY += 16;
  }

  u8g2.sendBuffer(); // Render to display
}

void drawLogo(void)
{
  u8g2.clearBuffer();
    u8g2.setFontMode(1);	// Transparent
// #ifdef MINI_LOGO

    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb16_mf);
    u8g2.drawStr(0, 22, "U");
    
    u8g2.setFontDirection(1);
    u8g2.setFont(u8g2_font_inb19_mn);
    u8g2.drawStr(14,8,"8");
    
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb16_mf);
    u8g2.drawStr(36,22,"g");
    u8g2.drawStr(48,22,"\xb2");
    
    u8g2.drawHLine(2, 25, 34);
    u8g2.drawHLine(3, 26, 34);
    u8g2.drawVLine(32, 22, 12);
    u8g2.drawVLine(33, 23, 12);
      u8g2.sendBuffer();
}