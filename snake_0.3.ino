#include <string.h>

// Direction definitions
#define RIGHT 0
#define LEFT  1
#define UP    2
#define DOWN  3

// Joystick definitions
#define JOYSTICK_LOWER_THRESHOLD -256
#define JOYSTICK_UPPER_THRESHOLD 256

// Arduino pin numbers
const int SW_pin1 = 2; // digital pin connected to switch output
const int X_pin1 = A0; // analog pin connected to X output
const int Y_pin1 = A1; // analog pin connected to Y output

const int SW_pin2 = 3; // digital pin connected to switch output
const int X_pin2 = A2; // analog pin connected to X output
const int Y_pin2 = A3; // analog pin connected to Y output
const int Delay = 500;

////// Snake game struct definitions //////
typedef struct Snake {
  int headY; // row of head cell
  int headX; // column of head cell
  uint8_t len;
  uint8_t dir;
  // x-axis represent column, y-axis represent row
  uint8_t y[100];
  uint8_t x[100];
  // int level = 1;
  // int snake_speed = 150;
} Snake;

typedef struct Food {
  uint8_t y; // row
  uint8_t x; // column
  bool eaten = true;
} Food;

typedef struct Poo {
  uint8_t y; // row
  uint8_t x; // column
  bool eaten = true;
  int time; // exist time for a poo
} Poo;

////// Global game state variables //////
// Joystick controller values
int valueX = 0;
int valueY = 0;

// Player 1 and 2's snakes' head positions, lengths and moving directions
Snake snake1 = {7, 1, 2, RIGHT};
// Snake snake2 = {1, 7, 2, LEFT};

Food food;
Poo poo1, poo2;

int field[8][8] = {
  {0,0,0,1,0,0,1,0},
  {0,0,0,1,0,0,1,0},
  {0,0,0,1,0,0,1,0},
  {0,0,0,1,0,0,1,0},
  {0,0,0,1,0,0,1,0},
  {0,0,0,1,0,0,1,0},
  {0,0,0,1,0,0,1,0},
  {0,0,0,1,0,0,0,0}
};

// win state for 2 players
bool win1 = false;
bool win2 = false;
//int score = 0;

////// Function signatures //////
void getInput(const int pinX, const int pinY);
void getInputs(void);
void drawSnake(int r, int c);

// Helper function for getInputs
// Sets snake position based on joystick state
void getInput(const int pinX, const int pinY, struct Snake *snake) {
  valueX = analogRead(pinX) - 512;
  valueY = analogRead(pinY) - 512;
  bool valueXLow = valueX < JOYSTICK_LOWER_THRESHOLD;
  bool valueXHigh = valueX > JOYSTICK_UPPER_THRESHOLD;
  bool valueYLow = valueY < JOYSTICK_LOWER_THRESHOLD;
  bool valueYHigh = valueY > JOYSTICK_UPPER_THRESHOLD;

  if (valueXLow) {
    Serial.print("valueXLow = true\n");
  } else {
    Serial.print("valueXLow = false\n");
  }
  if (valueXHigh) {
    Serial.print("valueXHigh = true\n");
  } else {
    Serial.print("valueXHigh = false\n");
  }

  if (!valueXLow && !valueXHigh) {
    Serial.print("first block\n");
    if (valueYLow) {
      snake->dir = DOWN;
    } else if (valueYHigh) {
      snake->dir = UP;
    }
  } else if (!valueYLow && !valueYHigh) {
    Serial.print("second block\n");
    if (valueXLow) { 
      snake->dir = LEFT;
    } else if (valueXHigh) {
      snake->dir = RIGHT;
    }
  } else {
    Serial.print("third block\n");
    if (abs(valueX) > abs(valueY)) {
      if (valueX > 0) {
        snake->dir = RIGHT;
      } else {
        snake->dir = LEFT;
      }
    } else {
      if (valueY > 0) {
        snake->dir = UP;
      } else {
        snake->dir = DOWN;
      }
    }
  }
}

// Sets snake1 and snake2 directions based on current position of joysticks
void getInputs(void) {
  // judge player1's direction
  getInput(X_pin1, Y_pin1, &snake1);
  //judge player2's direction
  // getInput(X_pin2, Y_pin2, snake2);
}

// Sets snake cell at position (r, c) on grid
void drawSnake(int r, int c) {
  field[r][c] = 1;
}

// Sets food cell at position (r, c) on grid
void drawFood(int r, int c) {
  field[r][c] = 1;
}

// Sets poo1 and poo2 cells at their coordinates on grid unless eaten
void drawPoo() {
  if (!poo1.eaten) {
    field[poo1.y][poo1.x] = 1;
  }
  if (!poo2.eaten){
      field[poo2.y][poo2.x] = 1;
  }
}

void renderDisplay(void) {
    memset(field, 0, 64*sizeof(int));
    //draw snake1
    for (int i = 0; i < snake1.len; i++) {
        drawSnake(snake1.y[i], snake1.x[i]);
    }
    //draw snake2
    // for (int i = 0; i < snake2.len; i++) {
    //    drawSnake(snake2.y[i], snake2.x[i]);
    // }
    //draw food in
    drawFood(food.y, food.x);
    //draw poo in
    drawPoo();
}

bool check_snake_die(Snake snakeA, Snake snakeB)
{
  // too much short!
  if(snakeA.len <= 1){
    return true;
  }
  //head on border
  if (snakeA.headX < 0 || snakeA.headX > 7 || snakeA.headY < 0 || snakeA.headY > 7)
  {
    return true;
  }

  //head on self or opponent
  if (snakeA.len > 4)
  {
    for (int i = 1; i < snakeA.len; i++)
    {
      if (snakeA.headX == snakeA.x[i] && snakeA.headY == snakeA.y[i])
      {
        return true;
      }
    }
  }
  for (int i = 0; i < snakeB.len; i++){
    if(snakeA.headX == snakeB.x[i] && snakeA.headY == snakeB.y[i]){
      return true;
    }
  }
  //default: no winner now
  return false;
}

void doFoodCheck(void)
{
  int food_out = 0; // food is born in a same position with snake

  if (food.eaten) {
    while (food_out == 0) {
      food_out = 1;

      food.x = (uint8_t)(random(8)); // random number from 0 to 7
      food.y = (uint8_t)(random(8));

      for (int i = snake1.len - 1; i > 0; i--) { //from tail to head to check if food is generate in the snake body
        if (food.x == snake1.x[i] && food.y == snake1.y[i]) {
          food_out = 0;
        }
      }
//      for (int i = snake2.len - 1; i > 0; i--) { //from tail to head to check if food is generate in the snake body
//        if (food.x == snake2.x[i] && food.y == snake2.y[i])
//        {
//          food_out = 0;
//        }
//      }
    }
  }

  food.eaten = false;
}
//generate a poo
void doPooCheck(void){
    //player1 use skill, and poo1 has been eaten
    if(SW_pin1 && poo1.eaten
){
        poo1.eaten
     = false;
        poo1.x = snake1.x[snake1.len - 1];
        poo1.y = snake1.y[snake1.len - 1];
        poo1.time = 5000; //I think 5s is eough.
    }
    //player2 use skill, and poo2 has been eaten
    if (SW_pin2 && poo2.eaten){
        poo2.eaten
     = false;
        //poo2.x = snake2.x[snake2.len - 1];
        //poo2.y = snake2.y[snake2.len - 1];
        poo2.time = 5000; //I think 5s is eough.
    }
}

//refreshing poo
void refresh_poo(int time){
    if (!poo1.eaten) {
        poo1.time -= time;
        //time is up it should be disappear.
        if(poo1.time <= 0)
            poo1.eaten
         = true;
    }
    if (!poo2.eaten){
        poo2.time -= time;
        //time is up it should be disappear.
        if(poo2.time <= 0)
            poo2.eaten
         = true;
    }
}
void moveSnake1(void) {
  switch (snake1.dir) {
    case RIGHT:
      snake1.headX++;
      break;
    case UP:
      snake1.headY--;
      break;
    case LEFT:
      snake1.headX--;
      break;
    case DOWN:
      snake1.headY++;
      break;
  }
  if (snake1.headX < 0) {
    snake1.headX = 7;
  }
  if (snake1.headY < 0) {
    snake1.headY = 7;
  }
  snake1.headX = snake1.headX % 8;
  snake1.headY = snake1.headY % 8;
  if ((snake1.headX == food.x ) && (snake1.headY == food.y))
  {
    food.eaten = true; //allow to create a new food
    snake1.len++;
  }
  //eat it's own poo
  if ((snake1.headX == poo1.x) && (snake1.headY == poo1.y))
  {
    poo1.eaten
 = true; //allow player1 to poo
    snake1.len--;
  }
  //eat player2's poo
  if ((snake1.headX == poo2.x) && (snake1.headY == poo2.y))
  {
    poo2.eaten
 = true; //allow player2 to poo
    snake1.len--;
  }
  //win2 = check_snake_die(snake1, snake2);
  //player1 eat a food
  for (int i = snake1.len - 1; i > 0; i--) {
      snake1.x[i] = snake1.x[i - 1];
      snake1.y[i] = snake1.y[i - 1];
  }
  snake1.x[0] = snake1.headX;
  snake1.y[0] = snake1.headY;
}

//void moveSnake2(void)
//{
//  switch (snake2.dir) {
//    case RIGHT:
//      snake2.headX ++;
//      break;
//    case UP:
//      snake2.headY --;
//      break;
//    case LEFT:
//      snake2.headX --;
//      break;
//    case DOWN:
//      snake2.headY ++;
//      break;
//  }
//  if (snake2.headX < 0) {
//    snake2.headX = 7;
//  }
//  if (snake2.headY < 0) {
//    snake2.headY = 7;
//  }
//  snake2.headX = snake2.headX % 8;
//  snake2.headY = snake2.headY % 8;
//  if ((snake2.headX == food.x ) && (snake2.headY == food.y))
//  {
//    food.eaten = true; //allow to create a new food
//    snake2.len++;
//  }
//  //eat player1's poo
//  if ((snake2.headX == poo1.x) && (snake2.headY == poo1.y))
//  {
//    poo1.eaten
// = true; //allow player1 to poo
//    snake2.len--;
//  }
//  //eat player2's poo
//  if ((snake2.headX == poo2.x) && (snake2.headY == poo2.y))
//  {
//    poo2.eaten
// = true; //allow player2 to poo
//    snake2.len--;
//  }
//  win1 = check_snake_die(snake2, snake1);
//  //playe2 eat a food
//  for (int i = snake2.len - 1; i > 0; i--) {
//    snake2.x[i] = snake2.x[i - 1];
//    snake2.y[i] = snake2.y[i - 1];
//  }
//  snake2.x[0] = snake2.headX;
//  snake2.y[0] = snake2.headY;
//}

void loop() {
  Serial.print("Switch1:  ");
  Serial.print(digitalRead(SW_pin1));
  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(analogRead(X_pin1));
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.print(analogRead(Y_pin1));
  Serial.print("\n\n");

  Serial.print("Switch2:  ");
  Serial.print(digitalRead(SW_pin2));
  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(analogRead(X_pin2));
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.print(analogRead(Y_pin2));
  Serial.print("\n\n");
  delay(Delay);

  printField();
  Serial.print("\n");
  delay(2000);

  if (win1 || win2) {
      //shoking the player
      //draw_game_over();
  } else {
    getInputs();
    moveSnake1();
    //moveSnake2();
    doFoodCheck();
    doPooCheck();
    renderDisplay();
  }
  // here need a number for game speed, I just assume it's 500
  // refresh the existed poo
  refresh_poo(Delay);
}

// runs on startup
void setup() {
  pinMode(SW_pin1, INPUT);
  digitalWrite(SW_pin1, HIGH);

  pinMode(SW_pin2, INPUT);
  digitalWrite(SW_pin2, HIGH);
   
  Serial.begin(9600);
}

// prints field to Arduino serial output
void printField() {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        Serial.print(field[i][j]);
        Serial.print("\r");
      }
    Serial.print("\n");
    }
}
