#include <Adafruit_GFX.h>
#include <ArducamSSD1306.h>
#include <Fonts/LemonRegular8pt8b.h>
#include <Fonts/LemonRegular7pt8b.h>

#define OLED_RESET  7

ArducamSSD1306 display(OLED_RESET);
char degree = char(176);

const unsigned char pi_logo[] PROGMEM = {
  // 'pi-png-12, 16x16px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x3f, 0x80, 0x02, 0x02, 0x40, 0x40, 
  0x02, 0x01, 0x80, 0x40, 0x02, 0x11, 0x80, 0x40, 0x02, 0x01, 0x80, 0x40, 0x01, 0x03, 0xc0, 0x80, 
  0x00, 0x83, 0xc1, 0x00, 0x00, 0x6f, 0xf6, 0x00, 0x00, 0x4c, 0x27, 0x00, 0x00, 0x88, 0x11, 0x80, 
  0x01, 0x1c, 0x18, 0x80, 0x01, 0x77, 0xc4, 0x80, 0x01, 0xc1, 0x83, 0x80, 0x02, 0xc1, 0x83, 0x40, 
  0x04, 0x81, 0x81, 0x20, 0x04, 0x81, 0x81, 0x20, 0x04, 0x81, 0xc1, 0x20, 0x04, 0xc3, 0x63, 0x20, 
  0x05, 0xfc, 0x1f, 0xe0, 0x03, 0x38, 0x1c, 0xc0, 0x02, 0x18, 0x18, 0x40, 0x03, 0x08, 0x10, 0x40, 
  0x01, 0x0c, 0x10, 0x80, 0x01, 0x8f, 0x71, 0x80, 0x00, 0xcf, 0x73, 0x00, 0x00, 0x38, 0x1c, 0x00, 
  0x00, 0x08, 0x10, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// set up temperature offsets used
// to place each of the four readings
int x1 = 0;
int y1 = 33;
int x2 = 66;
int y2 = 57;
int temperaturePos[4][2] = {
  {x1, y1},
  {x2, y1},
  {x1, y2},
  {x2, y2}
};

void setup() {
  // put your setup code here, to run once:
    display.begin();
  display.clearDisplay();
  drawLoading();
  display.display();
  Serial.begin(9600);
}

char newChar;
bool loaded = false;
bool waiting = true;
String message = "";
void loop() {
  // put your main code here, to run repeatedly:
  while (waiting) {
    if (Serial.available()) {
      newChar = char(Serial.read());
      if (newChar == 'A') {
        waiting = true;
        loaded = false;
        continue;
      }
      if (newChar == 'B') {
        loaded = true;
        waiting = false;
        continue;
      }
      message += newChar;
      waiting = true;
      loaded = false;
    }
  }
  if (loaded) {
    display.clearDisplay();
    drawHeader();
    drawTemps(getValue(message, ',', 0), getValue(message, ',', 1), getValue(message, ',', 2), getValue(message, ',', 3));
    display.display();
    message = "";
    loaded = false;
    waiting = true;
  }
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


void drawLoading() {
  display.drawBitmap(48, 14, pi_logo, 32, 32, WHITE);
  display.setFont(&Lemon_Regular7pt8b);
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(24, 60);
  display.print("Loading...");
  display.display();
}

void drawHeader() {
  display.setTextColor(WHITE, BLACK);
  display.setFont(&Lemon_Regular7pt8b);
  display.setTextSize(1);
  display.setTextWrap(false);
  display.setCursor(16,12);
  display.print("Pi");
  display.print(degree);
  display.print(" Monitor");
}

void drawTemp(String temp, int position) {
  int offsetX = temperaturePos[position][0];
  int offsetY = temperaturePos[position][1];
  
  display.setFont(&Lemon_Regular8pt8b);
  display.setTextSize(1);
  display.setCursor(offsetX, offsetY);
  display.fillRoundRect(offsetX + 13, offsetY - 15, 49, 21, 4, WHITE);
  display.setTextColor(WHITE, BLACK);
  display.print(String(position + 1));
  display.setCursor(offsetX+15, offsetY);
  display.setTextColor(BLACK, WHITE);
  display.print(temp + degree);
  display.print("C");
}

void drawTemps(String a, String b, String c, String d) {
  drawTemp(a, 0);
  drawTemp(b, 1);
  drawTemp(c, 2);
  drawTemp(d, 3);
}
