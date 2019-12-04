/* ArduinoNano-RecordLEDStatus.ino: Record LED Status, Version 1.0            */
/* Copyright (c) 2016-2019 ArdeshirV@protonmail.com, Licensed under GPLv3+    */
#include <Arduino.h>
#include <LiquidCrystal.h>
/* LCD Pin Map:
   LCD GND    Pin01 to GND
   LCD VCC    Pin02 to VCC
   LCD VEE    Pin03 to GND
   LCD RS     Pin04 to D12
   LCD R/W    Pin05 to GND
   LCD Enable Pin06 to D11
   LCD D4     Pin11 to D5
   LCD D5     Pin12 to D4
   LCD D6     Pin13 to D3
   LCD D7     Pin14 to D2
   LCD LightV Pin15 to VCC
   LCD LightG Pin16 to R-GND */
#define BETWEEN(value, num1, num2) (value > num1 && value < num2)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//---------------------------------------------------------------------------------
const int MaxLCDRow = 2;
const int MaxLCDCol = 16;
const int Left = 1;
const int Play = 4;
const int Right = 2;
const int Record = 3;
const int Nothing = 0;
const char *chrArrKeys[5] = {"Nothing", "Left", "Right", "Record", "Play"};
//---------------------------------------------------------------------------------
int StepPhase = 0;
int key = Nothing;
int sensorPin = A0;
int sensorValue = 0;
bool LEDStatus = HIGH;
int intLoopCounter = 0;
int intStartPlayTime = 0;
bool RecordIsEnable = false;
int intRecordPlayIndex = 0;
int intLastTimeElapsed = 0;
int intLastStatusChangedTime = 0;

struct RecordStatus {
  int intStatus;
  int intDuration;
};

int intRecordCounts = 0;
RecordStatus rec[300];
char LCDLines[MaxLCDRow][MaxLCDCol];
//---------------------------------------------------------------------------------
int GetKey(int value) {
  int Result = Nothing;
  if (BETWEEN(value, 80, 490)) Result = Play;
  if (value > 490 && value < 550) Result = Right;
  if (value > 550 && value < 1010) Result = Record;
  if (value > 1010 && value < 1025) Result = Left;
  return Result;
}
//---------------------------------------------------------------------------------
void UpdateLCD() {
  //lcd.clear();
  for (int row = 0; row < MaxLCDRow; row++) {
    int intLen = strlen(LCDLines[row]);
    while (intLen < MaxLCDCol)
      LCDLines[row][intLen++] = ' ';
    lcd.setCursor(0, row);
    lcd.print(LCDLines[row]);
  }
  lcd.display();
}
//---------------------------------------------------------------------------------
void setup() {
  key = Nothing;
  intLoopCounter = 0;
  pinMode(13, OUTPUT);
  pinMode(10, OUTPUT);
  lcd.begin(MaxLCDCol, MaxLCDRow);
  sprintf(LCDLines[0], "Arduino Nano");
  UpdateLCD();
  delay(2000);
  rec[0].intDuration = 0;
}
//---------------------------------------------------------------------------------
void defunion() {
  union u {
    int i;
    char c[2];
  } uu;
  uu.c[0] = 9;
  uu.c[1] = 9;
  sprintf(LCDLines[0], "uu.i = %d", uu.i);
  UpdateLCD();
}
//---------------------------------------------------------------------------------
void loop() {
  int milliseconds = millis();
  digitalWrite(13, LEDStatus);
  LEDStatus = (LEDStatus == HIGH) ? LOW : HIGH;
  //digitalWrite(13, HIGH); // Play as default status.
  int local_key = GetKey(sensorValue = analogRead(sensorPin));

  switch (local_key) {
    case Nothing:
      //digitalWrite(10, LOW);
      break;
    case Left:
      //digitalWrite(10, LOW);
      break;
    case Right:
      //if (RecordIsEnable)
      digitalWrite(10, HIGH);
      break;
    case Record:
      intRecordCounts = 0;
      RecordIsEnable = true;
      //digitalWrite(10, LOW);
      break;
    case Play:
      intStartPlayTime = milliseconds;
      intRecordPlayIndex = 0;
      RecordIsEnable = false;
      //digitalWrite(10, LOW);
      break;
    default:
      //digitalWrite(10, LOW);
      break;
  }

  if (RecordIsEnable) {
    rec[intRecordCounts].intStatus = key;
    rec[intRecordCounts].intDuration = milliseconds - intLastStatusChangedTime;
    intRecordCounts++;
  } else {// Play is enable then
    if (intRecordPlayIndex >= intRecordCounts)
      intRecordPlayIndex = 0;
    if (rec[intRecordPlayIndex].intStatus == Right)
      digitalWrite(10, HIGH);
    else
      digitalWrite(10, LOW);

    if (milliseconds - intStartPlayTime - intLastTimeElapsed >
        rec[intRecordPlayIndex].intDuration) {
      intStartPlayTime = milliseconds;
      intRecordPlayIndex++;
    }
  }

  //defunion(); return;
  if (local_key != Nothing) {
    key = local_key;
    intLastStatusChangedTime = milliseconds;
  }
  sprintf(LCDLines[0], "Uptime: %us", milliseconds / 1000);
  //sprintf(LCDLines[0], "Duration = %d", rec[0].intDuration);
  sprintf(LCDLines[1], "Key = %s", chrArrKeys[key]);
  UpdateLCD();
  if (++intLoopCounter >= 10000) intLoopCounter = 0;
  intLastTimeElapsed = milliseconds;
  delay(100);
}
//---------------------------------------------------------------------------------

