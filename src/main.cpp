#include <Arduino.h>
#include <Keypad.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>

// Keypad Setup
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte rowPins[ROWS] = {22, 24, 26, 28};
byte colPins[COLS] = {30, 32, 34, 36};

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ledstrip setup
#define stripPin 2
#define pixelCount 12

Adafruit_NeoPixel pixels(pixelCount, stripPin, NEO_GRB + NEO_KHZ800);

// lcd setup
LiquidCrystal_I2C lcd(0x3F, 20, 4);

// algemene variablen
String zChips = "1A";
String pChips = "2A";
int ldrWaarde;
int mappedLdrWaarde;
int buttonState1;
int buttonState2;

// pin variablen
#define stap1_1 38
#define stap1_2 40
#define stap1_3 42
#define stap1_4 44

#define stap2_1 39
#define stap2_2 41
#define stap2_3 43
#define stap2_4 45

#define button1 52
#define button2 53

#define LDR A0

void setup()
{
  // ledstrip
  pixels.begin();
  pixels.show();

  // lcd setup
  lcd.init();
  lcd.backlight();
  lcd.print("test");

  // Serial
  Serial.begin(9600);

  // pinModes
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(stripPin, OUTPUT);

  for (int i = 39; i <= 45; i++)
  {
    pinMode(i, OUTPUT);
  }

  for (int i = 22; i <= 36; i++)
  {
    if (i % 2 == 0)
    {
      pinMode(i, OUTPUT);
    }
  }
}

void setStrip(int color1, int color2, int color3)
{
  for (int i = 0; i < pixels.numPixels(); i++)
  {
    pixels.setPixelColor(i, pixels.Color(color1, color2, color3)); // Rood
    pixels.show();
    delay(50);
  }
}

void stappenMotor(int rotaties, int pin1, int pin2, int pin3, int pin4)
{
  for (int i; i <= rotaties; i++)
  {
    digitalWrite(pin4, LOW);
    digitalWrite(pin1, HIGH);
    delay(10);
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
    delay(10);
    digitalWrite(pin2, LOW);
    digitalWrite(pin3, HIGH);
    delay(10);
    digitalWrite(pin3, LOW);
    digitalWrite(pin4, HIGH);
    delay(10);
  }
}

void loop()
{
  ldrWaarde = analogRead(LDR);
  mappedLdrWaarde = map(ldrWaarde, 0, 1023, 0, 255);
  pixels.setBrightness(mappedLdrWaarde);
  Serial.print("strip brightness: ");
  Serial.println(mappedLdrWaarde);
  setStrip(255, 255, 255);

  buttonState1 = digitalRead(button1);
  if (buttonState1 == LOW)
  {
    Serial.println("buttonPressed");
    setStrip(255, 0, 0);
    stappenMotor(20, stap1_1, stap1_2, stap1_3, stap1_4);
  }
}