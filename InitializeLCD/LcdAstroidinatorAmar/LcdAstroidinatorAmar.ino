// Created by Alex Mares
// Date: 21-03-2018
// Class: IC.16AO.a
//
// Info: LcdFunctions

#include <SimpleTimer.h>
#include <LiquidCrystal_I2C.h>

//Objects
LiquidCrystal_I2C AstroidinatorLcd(0x27, 20, 4);
SimpleTimer tmrGame;

//Globals
bool enableLog;
int stubAstColl = 0;
int elapsedTime = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("Init..");
  InitGlobals();

  InitLcd();
  InitCredentials();
  InitStartGame();
}

void loop()
{
  tmrGame.run();
}

void InitLcd()
{
  AstroidinatorLcd.init();
  AstroidinatorLcd.setBacklight(1);
  AstroidinatorLcd.setCursor(0, 0);
  //AstroidinatorLcd.print("Alex Mares");
}
void InitCredentials()
{
  WriteToLcd(0, 0, "Astroidinator");
  WriteToLcd(0, 1, "Date: 06/04/2018");
  WriteToLcd(0, 2, "By: Nordin & Alex");
  WriteToLcd(0, 3, "Version: V0");
  delay(2000);
  AstroidinatorLcd.clear();
}
void InitStartGame()
{
  WriteToLcd(0, 0, "Start!");
  delay(1000);
  AstroidinatorLcd.clear();
  tmrGame.setInterval(500, UpdateDisplay);
}
void InitGlobals()
{
  Serial.println("Init globals..");
  enableLog == true;
  Serial.println("Finished.");
}
void WriteToLcd(int a_x, int a_y, String a_text)
{
  AstroidinatorLcd.setCursor(a_x, a_y);
  AstroidinatorLcd.print(a_text);
}
void UpdateDisplay()
{
  AstroidinatorLcd.clear();
  //ergens asteroids bewegen, ook hierin collision detecteren
  //ergens spaceships bewegen, ook hierin collision detecteren
  UpdateScores();
}

void UpdateScores()
{
  //aantal ast coll
  //aantal sh coll
  elapsedTime++;
  stubAstColl = stubAstColl + 3;//zo meteen niet meer nodig, waarde komt van Nordin

  //WriteToLcd(0, 3, "Score: ''  Time: ..");
  WriteToLcd(0, 3, "Score:");
  WriteToLcd(12, 3, "Time:");

  WriteToLcd(7, 3, "*");
  WriteToLcd(8, 3, (String)stubAstColl);
  WriteToLcd(17, 3, (String)elapsedTime);

  DebugLogging("ast: " + (String)stubAstColl, 0);
}

void DebugLogging(String a_text, int a_infoType)
{
  if (enableLog == true)
  {
    switch (a_infoType)
    {
      case 0:
        Serial.println("ERROR: " + a_text);
        break;
      case 1:
        Serial.println("WARNING: " + a_text);
        break;
      case 2:
        Serial.println("INFO: " + a_text);
        break;
      default:
        Serial.println(a_text);
        break;
    }
  }
}

