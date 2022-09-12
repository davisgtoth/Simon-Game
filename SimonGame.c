/*
* Author: Davis Toth
* Purpose: to create a Simon Game where the goal is to memorize a random sequence of LEDs lit up
*/

//constants
#define one_second 1000
#define max_sequence 5
#define GOOD 0
#define BAD 1
#define NEITHER 2

//other symbolic constants:
int const WIN = 8;
int const LOSE = 13;

int const RED = 12;
int const YELLOW = 11;
int const BLUE = 10;
int const GREEN = 9;

int const RBUTTON = 7;
int const YBUTTON = 6;
int const BBUTTON = 5;
int const GBUTTON = 4;

//variables and arrays
int gameArr[max_sequence];
int buttonArr[max_sequence]; 
int gameIndex;
bool waitingForInput;
unsigned long startTime;
unsigned long currentTime;
int count;
int holder;

//function prototypes:
void gameStart(void);
void printPattern(void);
int checkButton(int);
void gameWin(void);
void gameLose(void);

//functions:

//function that inititates the start of a new game
void gameStart(void) {
  for(int index = 0; index < max_sequence; index++) {
    gameArr[index] = random(GREEN, RED + 1);
    buttonArr[index] = gameArr[index] - 5;
  }

  digitalWrite(RED, HIGH);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(BLUE, HIGH);
  digitalWrite(GREEN, HIGH);
  delay(2 * one_second);
  
  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(GREEN, LOW);
  delay(one_second);

  waitingForInput = false;
  gameIndex = 0;
  return;
}

//function that lights up the pattern of LEDs
void printPattern(void) {
  delay(one_second);
  for(int index = 0; index <= gameIndex; index++) {
    digitalWrite(gameArr[index], HIGH);
    delay(0.5 * one_second);
    digitalWrite(gameArr[index], LOW);
    delay(0.5 * one_second);
  }
  return;
}

/*
 * Purpose: to check to see if the correct button has been pushed
 * 
 * Param: check - the index of the correct button to be pushed in the button array 
 * 
 * Returns: good - if the user pushes the correct button
 *          bad - if the user pushes the wrong button
 *          neither - if no buttons are pushed
 */
int checkButton(int check) {
  int result = NEITHER;
  if(digitalRead(buttonArr[check]) == HIGH) {
    while(digitalRead(buttonArr[check]) == HIGH)
      digitalWrite(gameArr[check], HIGH);
    delay(0.3 * one_second);
    digitalWrite(gameArr[check], LOW);
    result = GOOD;
  }
  else {
    for(int val = -3; val < 0; val++) {
      if(digitalRead(buttonArr[check] + val) == HIGH) {
        while(digitalRead(buttonArr[check] + val) == HIGH)
          digitalWrite(gameArr[check] + val, HIGH);
        delay(0.3 * one_second);
        digitalWrite(gameArr[check] + val, LOW);
        result = BAD;
      }
    }
    for(int val = 3; val > 0; val--) {
      if(digitalRead(buttonArr[check] + val) == HIGH) {
        while(digitalRead(buttonArr[check] + val) == HIGH)
          digitalWrite(gameArr[check] + val, HIGH);
        delay(0.3 * one_second);
        digitalWrite(gameArr[check] + val, LOW);
        result = BAD;
      }
    }
  }
  return result;
}

//function that signifies a player has won the game
void gameWin(void) {
  for(int index = 0; index < 3; index++) {
    digitalWrite(WIN, HIGH);
    delay(0.5 * one_second);
    digitalWrite(WIN, LOW);
    delay(0.5 * one_second);
  }
  gameStart();
  return;
}

//function that signifies a player has lost the game
void gameLose(void) {
  for(int index = 0; index < 3; index++) {
    digitalWrite(LOSE, HIGH);
    delay(0.5 * one_second);
    digitalWrite(LOSE, LOW);
    delay(0.5 * one_second);
  }
  gameStart();
  return;
}

//initialization
void setup() {
  pinMode(WIN, OUTPUT);
  pinMode(LOSE, OUTPUT);
  
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  
  pinMode(RBUTTON, INPUT);
  pinMode(YBUTTON, INPUT);
  pinMode(BBUTTON, INPUT);
  pinMode(GBUTTON, INPUT);
  
  randomSeed(analogRead(0));

  gameStart();
}

//main program loop
void loop() {
  if(waitingForInput == false) {
    if(gameIndex == max_sequence) {
      gameWin();
    }
    else {
      printPattern();
      waitingForInput = true;
      startTime = millis();
      count = 0;
    }
  }
  else {
    currentTime = millis();
    if(currentTime - startTime > 10 * one_second) {
      gameLose();
    }
    else {
      holder = checkButton(count);
      if(holder == GOOD) {
        count++;
        if(count > gameIndex) {
          gameIndex++;
          waitingForInput = false;
          delay(0.5 * one_second);
        }
      }
      else if(holder == BAD) {
        gameLose();
      }
    }
    delay(0.01 * one_second);
  }
}