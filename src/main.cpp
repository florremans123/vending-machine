#include <Arduino.h>
#include <Keypad.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Stepper.h>

//stapenmotoren setup
const int stepsPerRevolution = 200;
Stepper stepper1(stepsPerRevolution, 38, 40, 42, 44);
Stepper stepper2(stepsPerRevolution, 39, 41, 43, 45);

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

#define trigPin 9
#define echoPin 10

void setup()
{
  // ledstrip
  pixels.begin();
  for (size_t i = 0; i < pixels.numPixels(); i++)
  {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255)); // wit
  }
  pixels.show();y

  // stepper
  stepper1.setSpeed(60);

  // lcd setup
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Vending Machine");

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
    tone(8, 440, 20);
  }
}

void loop()
{

  ldrWaarde = analogRead(LDR);
  mappedLdrWaarde = map(ldrWaarde, 0, 1023, 0, 255);
  pixels.setBrightness(mappedLdrWaarde);
  
  

  char key = kpd.getKey();
  if (key != NO_KEY)
  {
    Serial.println(key);
    if (key != '#')
    {
      tempMsg += key;
      a++;
      Serial.println(key);
      lcd.setCursor(a,4);
      lcd.print(key);
    }
    if (key == '#')
    {
      msg = tempMsg;
      tempMsg = "";
      Serial.println(msg);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Vending Machine");
      a = 0;
    }
  }

  buttonState1 = digitalRead(button1);
  buttonState2 = digitalRead(button2);
  if (buttonState1 == LOW or msg == "A1")
  {
    setStrip(255, 0, 0);
    lcd.setCursor(0,1);
    lcd.print("zoute chips");
    stepper1.step(stepsPerRevolution);
    msg = "";
    delay(100);
    ultrasone();
    setStrip(255, 255, 255);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Vending Machine");
  }
  if (buttonState2 == LOW or msg == "A2")
  {
    setStrip(0, 0, 255);
    lcd.setCursor(0,1);
    lcd.print("paprika chips");
    stepper2.step(stepsPerRevolution);
    msg = "";
    delay(100);
    ultrasone();
    setStrip(255, 255, 255);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Vending Machine");
  }
  //ultrasone();
}