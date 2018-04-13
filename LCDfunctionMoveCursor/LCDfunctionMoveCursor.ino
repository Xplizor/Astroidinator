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
struct Ufo {
  String type;
  int yPosition;
  int xPosition;
  long randomYPosition = random(20, 100);
  long randomXPosition = random(0, 3);
<<<<<<< Updated upstream
};
=======
}
>>>>>>> Stashed changes

//Globals
bool enableLogNjan;
int rowNjan;
int columnNjan;

int astroidNumber = 20;
Ufo astroids[20];

int shipNumber = 10;
Ufo ships[10];

int counter;

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
  for (int i = 0; i < astroidNumber - 1; i++) {
    Ufo astroid;
    astroid.type = "astroid";
    astroid.yPosition = astroid.randomYPosition;
    astroid.xPosition = astroid.randomXPosition;

    if ((astroid.yPosition % 2) == 0) {
      astroid.yPosition++;
    }
    
    astroids[i] = astroid;   
  }
}

void GenerateShips() {
  for (int i = 0; i < shipNumber - 1; i++) {
    Ufo ship;
    ship.type = "ship";
    ship.yPosition = ship.randomYPosition;
    ship.xPosition = ship.randomXPosition;

    if ((ship.yPosition % 2) != 0) {
      ship.yPosition++;
    }
    
    ships[i] = ship;   
  }
}

void MoveAstroids() {
  for (int i = 0; i < astroidNumber - 1; i++) {
    
    if (astroids[i].yPosition < 20 && astroids[i].yPosition > 0) {
      WriteToLcd(astroids[i].yPosition, astroids[i].xPosition, "*");
    }

    if (astroids[i].yPosition == 0 && astroids[i].xPosition == rowNjan) {
      AstroidinatorLcd.setBacklight(0);
    }

    astroids[i].yPosition--;
  }
}

void MoveShips() {
  for (int i = 0; i < shipNumber - 1; i++) {
    
    if (ships[i].yPosition < 20 && ships[i].yPosition > 0) {
      WriteToLcd(ships[i].yPosition, ships[i].xPosition, "<");
    }

    if (ships[i].yPosition == 0 && ships[i].xPosition == rowNjan) {
      AstroidinatorLcd.setBacklight(0);
    }

    ships[i].yPosition--;
  }
}

void UpdateDisplay() {
  AstroidinatorLcd.clear();
  AstroidinatorLcd.setBacklight(1);
    
  if (counter == 0) {
    GenerateAstroids();
    GenerateShips();
    counter = 100;
  }
  counter--;
  
  ControlJoystick();
  MoveAstroids();
  MoveShips();

  
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
  tmrGame.setInterval(250, UpdateDisplay);
  GenerateAstroids();
  GenerateShips();
}

void InitGlobals() {
  Serial.println("Init globals");

  enableLogNjan = true;
  rowNjan = 1;
  columnNjan = 19;
  counter = 100;

  Serial.println("Finished.");
}

void InitPins() {
  Serial.println("Init pins..");

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

