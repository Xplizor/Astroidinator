/*
   Created: Nordin Jansen, 09feb2018
   Info: Astroidinator lcd

   History
   21mar2018: initial creation
*/

//Libraries
#include <LiquidCrystal_I2C.h>
#include <SimpleTimer.h>

//Objects
LiquidCrystal_I2C AstroidinatorLcd(0x27, 20, 4);
SimpleTimer tmrGame;

//Globals
bool enableLogNjan;
int rowNjan;
int columnNjan;

//Pins
int diSwitchNjan = 2;
int aiXaxisNjan = A0;
int aiYaxisNjan = A1;

void setup() {
  Serial.begin(115200);
  Serial.println("Init..");

  InitGlobals();
  InitPins();
  InitLcd();
  InitCredentials();
  InitStartGame();
}

void loop() {
  tmrGame.run();
}

void UpdateDisplay() {
  AstroidinatorLcd.clear();
  AstroidinatorLcd.setBacklight(1);
  
  WriteToLcd(columnNjan, 2, "*");

  ControlJoystick();
      
  DebugLogging(String(map(analogRead(aiYaxisNjan), 0, 1023, 0, 3)), 2);
  WriteToLcd(0, rowNjan,"=");
 
  if (columnNjan == 0) {
    if (rowNjan == 2) {
       AstroidinatorLcd.setBacklight(0);
    }
  }
 
  if (columnNjan == 0) {
    columnNjan = 19;
  } else {
    columnNjan--;
  }
}

void ControlJoystick() {
  switch (map(analogRead(aiYaxisNjan), 0, 1023, 0, 3)) {
    case 0:
      switch (rowNjan) {
        case 1:
          rowNjan = 0;
          break;
        case 2:
          rowNjan = 1;
          break;
      }
      break;
    case 2:
      switch (rowNjan) {
        case 0:
          rowNjan = 1;
          break;
        case 1:
          rowNjan = 2;
          break;
      }
      break;
  }
}

void WriteToLcd(int a_x, int a_y, String a_text) {
  AstroidinatorLcd.setCursor(a_x, a_y);
  AstroidinatorLcd.print(a_text);
}

void InitLcd(){
  AstroidinatorLcd.init();
  AstroidinatorLcd.setBacklight(1);  
}

void InitCredentials() {
  WriteToLcd(0,0,"Game name:");
  WriteToLcd(0,1,"Made by:");
  WriteToLcd(0,2,"Date:");
  WriteToLcd(0,3,"Version:");
  delay(2000);
  AstroidinatorLcd.clear();
}

void InitStartGame() {
  WriteToLcd(0,0,"Start!");
  delay(2000);
  AstroidinatorLcd.clear();
  tmrGame.setInterval(120, UpdateDisplay);
}

void InitGlobals() {
  Serial.println("Init globals");

  enableLogNjan = true;
  rowNjan = 1;
  columnNjan = 19;

  Serial.println("Finished.");
}

void InitPins() {
  Serial.println("Init pins..");

  pinMode(diSwitchNjan, INPUT);
  pinMode(aiXaxisNjan, INPUT);
  pinMode(aiYaxisNjan, INPUT);
  
  Serial.println("Finished.");
}

void DebugLogging(String a_text, int a_infoType) {
  if (enableLogNjan) {
    switch (a_infoType) {
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
    }
  }
}
