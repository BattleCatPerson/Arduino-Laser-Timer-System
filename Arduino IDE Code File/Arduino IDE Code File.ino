#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define TRANSMIT 2
#define RECEIVER 4
#define BUTTON 6
#define TRANSMIT2 8
#define RECEIVER2 9

bool stop = false;
bool stop2 = false;

bool started = false;
bool buttonDown = false;
double timer = 0;
double timer2 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(TRANSMIT, OUTPUT);
  pinMode(RECEIVER, INPUT);
  digitalWrite(TRANSMIT, HIGH);

  pinMode(TRANSMIT2, OUTPUT);
  pinMode(RECEIVER2, INPUT);
  digitalWrite(TRANSMIT2, HIGH);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

}

void loop() {
  int result = digitalRead(RECEIVER);
  int button = digitalRead(BUTTON);
  int result2 = digitalRead(RECEIVER2);
  if (button == HIGH && buttonDown && !started)
  {
    timer = millis() / (double)1000;
    timer2 = millis() / (double)1000;
    started = true;
    buttonDown = false;
    Serial.println("HI");
    stop = false;
  }
  if (button == LOW)
  {
    buttonDown = true;
  }
  else
  {
    buttonDown = false;
  }

  String final = "";
  if (!stop && started)
  {
    if (result == HIGH)
    {
      Serial.println("THERE IS AN OBSTACLE");
      stop = true;
      timer = (millis() / (double)1000) - timer;
      Serial.println(timer);
      final = timer;
      final += "\n";
    }
    else
    {
      final = ((millis() / (double) 1000) - timer);
      final += "\n";
    }
  }
  else if (started)
  {
    final += timer;
    final += "\n";
  }

  if (!stop2 && started)
  {
    if (result2 == HIGH)
    {
      Serial.println("THERE IS AN OBSTACLE");
      stop2 = true;
      timer2 = (millis() / (double)1000) - timer2;
      Serial.println(timer2);
      final += timer2;
    }
    else
    {
      final += ((millis() / (double) 1000) - timer2);
    }
  }
  else if (started)
  {
    final += timer2;
  }

  if ((!stop || !stop2) && started)
  {
    display.clearDisplay();
    display.setTextSize(3);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(final);
    display.display();
  }
  if (stop && stop2 && started)
  {
    display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);
    display.println("RESULT: ");             // Start at top-left corner
    display.println(final);
    display.display();
    started = false;
    stop = false;
    stop2 = false;
  }
  
  
}