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

//Structs
struct UfoNjan {
  String typeNjan;
  int yPositionNjan;
  int xPositionNjan;
  long randomYPositionNjan = random(20, 100);
  long randomXPositionNjan = random(0, 3);
};

//Globals
bool enableLogNjan;
int rowNjan;
int columnNjan;

int astroidNumberNjan = 20;
UfoNjan astroidsNjan[20];

int shipNumberNjan = 10;
UfoNjan shipsNjan[10];

int counterNjan;

int AstCollAmar = 0;
int elapsedTimeAmar = 0;
int ShipCollAmar = 0;

//Pins
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

void GenerateAstroids() {
  for (int i = 0; i < astroidNumberNjan - 1; i++) {
    UfoNjan astroidNjan;
    astroidNjan.typeNjan = "astroid";
    astroidNjan.yPositionNjan = astroidNjan.randomYPositionNjan;
    astroidNjan.xPositionNjan = astroidNjan.randomXPositionNjan;

    if ((astroidNjan.yPositionNjan % 2) == 0) {
      astroidNjan.yPositionNjan++;
    }
    astroidsNjan[i] = astroidNjan;   
  }
}

void GenerateShips() {
  for (int i = 0; i < shipNumberNjan - 1; i++) {
    UfoNjan shipNjan;
    shipNjan.typeNjan = "ship";
    shipNjan.yPositionNjan = shipNjan.randomYPositionNjan;
    shipNjan.xPositionNjan = shipNjan.randomXPositionNjan;

    if ((shipNjan.yPositionNjan % 2) != 0) {
      shipNjan.yPositionNjan++;
    }
    
    shipsNjan[i] = shipNjan;   
  }
}

void MoveAstroids() {
  for (int i = 0; i < astroidNumberNjan - 1; i++) {
    
    if (astroidsNjan[i].yPositionNjan < 20 && astroidsNjan[i].yPositionNjan > 0) {
      WriteToLcd(astroidsNjan[i].yPositionNjan, astroidsNjan[i].xPositionNjan, "*");
    }

    if (astroidsNjan[i].yPositionNjan == 0 && astroidsNjan[i].xPositionNjan == rowNjan) {
      AstroidinatorLcd.setBacklight(0);
      ++AstCollAmar;
    }

    astroidsNjan[i].yPositionNjan--;
  }
}

void MoveShips() {
  for (int i = 0; i < shipNumberNjan - 1; i++) {
    
    if (shipsNjan[i].yPositionNjan < 20 && shipsNjan[i].yPositionNjan > 0) {
      WriteToLcd(shipsNjan[i].yPositionNjan, shipsNjan[i].xPositionNjan, "<");
    }

    if (shipsNjan[i].yPositionNjan == 0 && shipsNjan[i].xPositionNjan == rowNjan) {
      AstroidinatorLcd.setBacklight(0);
      ++ShipCollAmar;
    }
    shipsNjan[i].yPositionNjan--;
  }
}

void UpdateDisplay() {
  AstroidinatorLcd.clear();
  AstroidinatorLcd.setBacklight(1);
    
  if (counterNjan == 0) {
    GenerateAstroids();
    GenerateShips();
    counterNjan = 100;
  }
  counterNjan--;
  
  ControlJoystick();
  MoveAstroids();
  MoveShips();

  UpdateScores();
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

  WriteToLcd(0, rowNjan,"=");
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
  WriteToLcd(0, 0, "Astroidinator");
  WriteToLcd(0, 1, "Date: 18/04/2018");
  WriteToLcd(0, 2, "By: Nordin & Alex");
  WriteToLcd(0, 3, "Version: V0");
  delay(2000);
  AstroidinatorLcd.clear();
}

void InitStartGame() {
  WriteToLcd(0,0,"Start!");
  delay(2000);
  AstroidinatorLcd.clear();
  tmrGame.setInterval(250, UpdateDisplay);
  GenerateAstroids();
  GenerateShips();
}

void InitGlobals() {
  Serial.println("Init globals");

  enableLogNjan = true;
  rowNjan = 1;
  columnNjan = 19;
  counterNjan = 100;

  Serial.println("Finished.");
}

void InitPins() {
  Serial.println("Init pins..");

  pinMode(aiYaxisNjan, INPUT);
  
  Serial.println("Finished.");
}

void UpdateScores()
{
  //  aantal ast coll
  //  aantal sh coll
  //  AstCollAmar = stubAstColl + 3; zo meteen niet meer nodig, waarde komt van Nordin
  
  elapsedTimeAmar++;

  AstCollAmar = AstCollAmar - ShipCollAmar;
  
  WriteToLcd(0, 3, "Score:");
  WriteToLcd(10, 3, "Time:");

  //WriteToLcd(7, 3, "*");
  WriteToLcd(7, 3, (String)AstCollAmar);
  WriteToLcd(16, 3, (String)elapsedTimeAmar);

  ShipCollAmar = 0;
  DebugLogging("ast: " + (String)AstCollAmar, 0);
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

