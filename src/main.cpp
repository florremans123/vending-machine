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
int delayStap = 50;
int a = 0;
int notes[] = {NOTE_A4, NOTE_B4, NOTE_C3};

float duration, distance;

String msg;
String tempMsg;

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

#define trigPin = 9;
#define echoPin = 10;

void setup()
{
  // ledstrip
  pixels.begin();
  pixels.show();

  // lcd setup
  lcd.init();
  lcd.backlight();

  // Serial
  Serial.begin(9600);

  // pinModes
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(stripPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
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

void setStrip(int R, int G, int B)
{
  for (size_t i = 0; i < pixels.numPixels(); i++)
  {
    pixels.setPixelColor(i, pixels.Color(R, G, B)); // Rood
    pixels.show();
    delay(50);
  }
}

void stappenMotor(int rotaties, int pin1, int pin2, int pin3, int pin4)
{
  for (int i = 0; i <= rotaties; i++)
  {
    digitalWrite(pin4, LOW);
    digitalWrite(pin1, HIGH);
    delay(delayStap);
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
    delay(delayStap);
    digitalWrite(pin2, LOW);
    digitalWrite(pin3, HIGH);
    delay(delayStap);
    digitalWrite(pin3, LOW);
    digitalWrite(pin4, HIGH);
    delay(delayStap);
  }
}

void ultrasone() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH),
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.0343) / 2;

  if (distance < 40) {
    tone(8, notes[1], 20);
  }
}

void loop()
{

  ldrWaarde = analogRead(LDR);
  mappedLdrWaarde = map(ldrWaarde, 0, 1023, 0, 255);
  pixels.setBrightness(mappedLdrWaarde);
  setStrip(255, 255, 255);

  char key = kpd.getKey();
  if (key != NO_KEY)
  {
    Serial.println(key);
    if (key != '#')
    {
      tempMsg += key;
      a++;
      Serial.println(key);
    }
    if (key == '#')
    {
      msg = tempMsg;
      tempMsg = "";
      Serial.println(msg);
    }
  }

  buttonState1 = digitalRead(button1);
  buttonState2 = digitalRead(button2);
  if (buttonState1 == LOW or msg == "A1")
  {
    setStrip(255, 0, 0);
    lcd.print("zoute chips");
    stappenMotor(10, stap1_1, stap1_2, stap1_3, stap1_4);
    msg = "";
  }
  if (buttonState2 == LOW or msg == "A2")
  {
    setStrip(0, 0, 255);
    lcd.print("paprika chips");
    stappenMotor(10, stap2_1, stap2_2, stap2_3, stap2_4);
    msg = "";
  }

  ultrasone()
}