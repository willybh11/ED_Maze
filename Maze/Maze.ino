/*Property of Elf-Is Studios*/

#define pinN 11 //R
#define pinE 10 //B
#define pinS 9 //Y
#define pinW 8  //G

#define xPin A0
#define yPin A1
#define startPin 4
#define togglePin 7 // high easy | low hard

#define buzzPin 13

boolean grid[7][7][4];
boolean moved = false;
boolean finished = false;

int spawnQuadrant;
int randomStart[] = {0, 0};

int starts[4][2] = {
  {0, 0},
  {4, 0},
  {0, 3},
  {0, 6}
};

int finish[2] = {6, 6};
int coords[2] = {0, 0};
int last[2];

int melody[8] = {
  293.66,
  293.66,
  293.66,
  293.66,
  329.63,
  293.66,
  329.63,
  369.99
};

int durations[8] = {
  200,
  100,
  100,
  200,
  200,
  200,
  200,
  800
};

boolean easyGrid[7][7][4] = {{ //row,col - N,E,S,W
    {false, true, true, false}, //0,0
    {false, true, true, true},
    {false, false, true, true},
    {false, true, true, false},
    {false, false, true, true},
    {false, true, false, false},
    {false, false, true, true}
  },
  { {true, false, false, false}, //1,0
    {true, false, true, false},
    {true, false, false, false},
    {true, false, false, false},
    {true, true, true, false},
    {false, true, false, true},
    {true, false, false, true}
  },
  { {false, true, false, false}, //2,0
    {true, true, false, true},
    {false, false, true, true},
    {false, false, true, false},
    {true, true, true, false},
    {false, true, false, true},
    {false, false, true, true}
  },
  { {false, true, true, false}, //3,0
    {false, true, false, true},
    {true, false, true, true},
    {true, true, true, false},
    {true, false, false, true},
    {false, true, true, false},
    {true, false, true, true}
  },
  { {true, true, true, false}, //4,0
    {false, true, false, true},
    {true, false, true, true},
    {true, false, false, false},
    {false, true, true, false},
    {true, false, false, true},
    {true, false, true, false}
  },
  { {true, false, true, false}, //5,0
    {false, false, true, false},
    {true, true, false, false},
    {false, false, true, true},
    {true, false, true, false},
    {false, true, false, false},
    {true, false, false, true}
  },
  { {true, true, false, false}, //6,0
    {true, true, false, true},
    {false, false, false, true},
    {true, true, false, false},
    {true, true, false, true},
    {false, true, false, true},
    {false, false, false, true}
  }
};

void makeNoise() {
  Serial.println("Move failed.");
  return;//TODO: implement
}

void movePlayer(int dir) { //0=up, 1=right, 2=down, 3 = left
  if (grid[coords[0]][coords[1]][dir]) { //if that direction is open
    switch (dir) {
      case 0: coords[1]--; break;
      case 1: coords[0]++; break;
      case 2: coords[1]++; break;
      case 3: coords[0]--; break;
    }
    //    Serial.println("moved: ", dir);

  } else {
    makeNoise();
  }
}

boolean path(int dir) {
  return (grid[coords[0]][coords[1]][dir]);
}

void powerLights() {

  digitalWrite(pinN, LOW);
  digitalWrite(pinE, LOW);
  digitalWrite(pinS, LOW);
  digitalWrite(pinW, LOW);

  if (!grid[coords[0]][coords[1]][0]) digitalWrite(pinN, HIGH); //N
  if (!grid[coords[0]][coords[1]][1]) digitalWrite(pinE, HIGH); //E
  if (!grid[coords[0]][coords[1]][2]) digitalWrite(pinS, HIGH); //S
  if (!grid[coords[0]][coords[1]][3]) digitalWrite(pinW, HIGH); //W
}

char pos(int val) {
  if (val < 400) return ('L');      //Low position
  else if (val > 600) return ('H'); //High position
  else return ('N');                //Neutral position
}

int easyStart() {
  Serial.println("easy start");
  int start = rand() % 4;
  memcpy(coords, starts[start], sizeof(coords));
  finished = false;
}

void hardStart() {
  Serial.print("hard start\nQuadrant: ");
  spawnQuadrant = rand() % 3;
  Serial.println(spawnQuadrant);
  Serial.print(coords[0]);
  Serial.print(",");
  Serial.println(coords[1]);
  if (spawnQuadrant == 0) {
    randomStart[0] = rand() % 4;
    randomStart[1] = rand() % 4;
    finished = false;
  } else if (spawnQuadrant == 1) {
    randomStart[0] = rand() % 3 + 4;
    randomStart[1] = rand() % 4;
    finished = false;
  } else {
    randomStart[0] = rand() % 4;
    randomStart[1] = rand() % 3 + 4;
    finished = false;
  }
  memcpy(coords, randomStart, sizeof(coords));
  Serial.print(coords[0]);
  Serial.print(",");
  Serial.println(coords[1]);
}

void setup() {

  Serial.begin(9600);

  memcpy(grid, easyGrid, sizeof(grid));
  int start = rand() % 4;
  memcpy(coords, starts[start], sizeof(coords));

  pinMode(pinN, OUTPUT);
  pinMode(pinE, OUTPUT);
  pinMode(pinS, OUTPUT);
  pinMode(pinW, OUTPUT);

  pinMode(startPin, INPUT);
  pinMode(togglePin, INPUT);
//  digitalWrite(startPin, HIGH);
//  digitalWrite(togglePin, HIGH);


}

void loop() {
  powerLights();
  moved = false;

  Serial.print(digitalRead(startPin));
  Serial.println(digitalRead(togglePin));  

  if (digitalRead(startPin) == HIGH) {
    if (digitalRead(togglePin) == HIGH) {
      easyStart();
    } else {
      hardStart();
    }
    delay(500);
  }


  switch (pos(analogRead(xPin))) {
    case 'H':
      if (path(3)) {
        coords[1]--;
        delay(500);
      }
      else {
        tone(buzzPin, 50);
        delay(200);
        noTone(buzzPin);
      }
      moved = true;
      break;
    case 'L':
      if (path(1)) {
        coords[1]++;
        delay(500);
      }
      else {
        tone(buzzPin, 50);
        delay(200);
        noTone(buzzPin);
      }
      moved = true;
      break;
  }
  if (!moved) {
    switch (pos(analogRead(yPin))) {
      case 'H':
        if (path(0)) {
          coords[0]--;
          delay(500);
        }
        else {
          tone(buzzPin, 50);
          delay(200);
          noTone(buzzPin);
        }

        break;
      case 'L':
        if (path(2)) {
          coords[0]++;
          delay(500);
        }
        else {
          tone(buzzPin, 50);
          delay(200);
          noTone(buzzPin);
        }
        break;
    }
  }

  if (coords[0] == finish[0] && coords[1] == finish[1] && !finished) {
    finished = true;
    for (int i = 0; i < 8; i++) {
      tone(buzzPin, melody[i]);
      delay(durations[i] - 20);
      noTone(buzzPin);
      delay(20);
    }
  }
}
