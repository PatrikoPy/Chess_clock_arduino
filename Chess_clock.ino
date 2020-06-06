#include <LiquidCrystal.h>

#define btnBLACK  22
#define btnWHITE  23
#define sensorPin  0

const int interval = 1000;
unsigned long previousMillis = 0;
int whiteTimer = 0;
int blackTimer = 0;
int sensorValue = 0;
short playerMode = 0;
bool start = false;
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(26, 27, 50, 51, 52, 53);

void DisplayTimer(int timeLeft, int col = 0)
{
  lcd.setCursor(col, 1);
  lcd.print(timeLeft / 60);
  lcd.print(":");
  if (timeLeft % 60 > 9)
  {
    lcd.print(timeLeft % 60);
  }
  else
  {
    lcd.print("0");
    lcd.print(timeLeft % 60);
  }
}
void DisplayPlayer(short player)
{
  switch (player)
  {
    case 1:
      {
        lcd.setCursor(0, 0);
        lcd.print(" BLACK   >WHITE<");
        DisplayTimer(whiteTimer, 10);
        break;
      }
    case 2:
      {
        lcd.setCursor(0, 0);
        lcd.print(">BLACK<   WHITE ");
        DisplayTimer(blackTimer, 1);
        break;
      }
  }
}
void SetTimer()
{
  sensorValue = analogRead(sensorPin);
  sensorValue = map(sensorValue, 0, 1023, 3, 1);
  switch (sensorValue)
  {
    case 1:
      whiteTimer = 60;
      blackTimer = 60;
      break;
    case 2:
      whiteTimer = 180;
      blackTimer = 180;
      break;
    case 3:
      whiteTimer = 300;
      blackTimer = 300;
      break;
  }
  DisplayTimer(whiteTimer, 10);
  DisplayTimer(blackTimer, 1);
}

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print(" BLACK    WHITE ");
  pinMode(btnWHITE, INPUT);
  pinMode(btnBLACK, INPUT);

}

void loop() {
  unsigned long currentMillis = millis();


  if (start == true)
  {
    if ((unsigned long)(currentMillis - previousMillis) >= interval && whiteTimer > 0 && blackTimer > 0)
    {
      previousMillis = currentMillis;
      (playerMode == 1) ? whiteTimer-- : blackTimer--;
    }
    (playerMode == 1) ? DisplayPlayer(1) : DisplayPlayer(2);
    if (whiteTimer == 0 || blackTimer == 0)
    {
      start = false;
      if (whiteTimer == 0)
      {
        lcd.setCursor(0, 0);
        lcd.print(">WINNER<>LOOSER<");
      }
      else if (blackTimer == 0)
      {
        lcd.setCursor(0, 0);
        lcd.print(">LOOSER<>WINNER<");
      }
    }
    if (digitalRead(btnWHITE) == HIGH)
      playerMode = 2;
    else if (digitalRead(btnBLACK) == HIGH)
      playerMode = 1;
  }
  else
  {
    SetTimer();
    if (digitalRead(btnWHITE) == HIGH || digitalRead(btnBLACK) == HIGH)
      start = true;
    playerMode = 1;
    delay(200);
  }

}
