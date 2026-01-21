#include <Arduino.h>
#include <Keypad.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Stepper.h>
#include <HCSR04.h>

//stappenmotoren setup
const int stepsPerRevolution = 200;
Stepper stepper1(stepsPerRevolution, 38, 42, 40, 44); // IN1, IN3, IN2, IN4
Stepper stepper2(stepsPerRevolution, 39, 43, 41, 45); // IN1, IN3, IN2, IN4

//ultrasone
#define trigPin 9
#define echoPin 10

UltraSonicDistanceSensor ultraSone(trigPin, echoPin);

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
String pass = "1234";
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
#define button1 52
#define button2 53

#define LDR A0

#define buzzerPin 19

void setup()
{
  // ledstrip
  pixels.begin();
  for (size_t i = 0; i < pixels.numPixels(); i++)
  {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255)); // wit
  }
  pixels.show();

  // stepper
  stepper1.setSpeed(60);
  stepper2.setSpeed(60);

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

//functie voor ledstrip van kleur te veranderen
void setStrip(int R, int G, int B)
{
  for (size_t i = 0; i < pixels.numPixels(); i++)
  {
    pixels.setPixelColor(i, pixels.Color(R, G, B)); // Rood
    pixels.show();
    delay(50);
  }
}

// functie voor ultrasone sensor lezen en toon starten
void checkItem() {
  distance = ultraSone.measureDistanceCm();
  Serial.println(distance);
  if (distance <= 30)
  {
    tone(buzzerPin, 1000);
  }
  else {
    noTone(buzzerPin);
  }
}

void partyMode() {
  for(int i = 0; i < 7; i++){
    int red = random(0, 255);
    int green = random(0, 255);
    int blue = random(0, 255);

    setStrip(red, green, blue);
    Serial.println(i);
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
  if (buttonState1 == LOW or msg == zChips)
  {
    setStrip(255, 0, 0);
    lcd.setCursor(0,1);
    lcd.print("zoute chips");
    // for loop werkt niet
    stepper1.step(stepsPerRevolution);
    stepper1.step(stepsPerRevolution);
    stepper1.step(stepsPerRevolution);
    stepper1.step(stepsPerRevolution);
    stepper1.step(stepsPerRevolution);
    stepper1.step(stepsPerRevolution);
    msg = "";   // maak msg leeg voor volgende 
    delay(100);
    setStrip(255, 255, 255);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Vending Machine");
  }
  if (buttonState2 == LOW or msg == pChips)
  {
    setStrip(0, 0, 255);
    lcd.setCursor(0,1);
    lcd.print("paprika chips");
    // for loop werkt niet
    stepper2.step(stepsPerRevolution);
    stepper2.step(stepsPerRevolution);
    stepper2.step(stepsPerRevolution);
    stepper2.step(stepsPerRevolution);
    stepper2.step(stepsPerRevolution);
    stepper2.step(stepsPerRevolution);
    msg = "";   // maak msg leeg voor volgende 
    delay(100);
    setStrip(255, 255, 255);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Vending Machine");
  }
  checkItem();
  if (msg == pass) {
    partyMode();
    msg = "";
  }
}