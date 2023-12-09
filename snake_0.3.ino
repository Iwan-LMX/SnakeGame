#include <string.h>

// Arduino pin numbers
const int SW_pin1 = 2; // digital pin connected to switch output
const int X_pin1 = A0; // analog pin connected to X output
const int Y_pin1 = A1; // analog pin connected to Y output

const int SW_pin2 = 1; // digital pin connected to switch output
const int X_pin2 = A2; // analog pin connected to X output
const int Y_pin2 = A3; // analog pin connected to Y output
const int Delay = 500;

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

void setup() {
  pinMode(SW_pin1, INPUT);
  digitalWrite(SW_pin1, HIGH);

  pinMode(SW_pin2, INPUT);
  digitalWrite(SW_pin2, HIGH);
   
  Serial.begin(9600);
}
void printField () {
    for (int i = 0; i < 8; i ++) {
      for (int j = 0; j < 8; j++) {
        Serial.print(field[i][j]);
        Serial.print("\r");
      }
    Serial.print("\n");
    }
}

#define RIGHT 0
#define LEFT  1
#define UP    2
#define DOWN  3

int valueX = 0;
int valueY = 0;

typedef struct Snake{
  int heady; //head row
  int headx; //head column
  uint8_t len;
  uint8_t dir;
  //x-axis represend column, y-axis represend row
  bool eaten;
  uint8_t y[100];
  uint8_t x[100];
  // int level = 1;
  // int snake_speed = 150;
}Snake;
//player 1 and 2's snakes head positions, length, moving directions
Snake snake1 = {7, 1, 2, RIGHT, 0};
Snake snake2 = {1, 7, 2, Left, 0};


typedef struct Food{
  uint8_t y; //row
  uint8_t x; //column
  bool eaten = true;
}Food;
Food food;
typedef struct Poo{
  uint8_t y; //row
  uint8_t x; //column
  bool poo_eaten = true;
  uint8_t time; // exist time for a poo
}Poo;
Poo poo1, poo2;

//record the win state for 2 players
bool win1 = false;
bool win2 = false;
//int score = 0;
int i;

//scan controler/key for moving
void keyScan(void)
{
  //judge player1's directoin
  valueX = X_pin1 - 512;
  valueY = Y_pin1 - 512;
  if(abs(valueX) > abs(valueY)){
    if(valueX > 0)
      snake1.dir = UP;
    else
      snake1.dir = DOWN;
  }else if(abs(valueX) < abs(valueY){
    if(valueY > 0)
      snake1.dir = RIGHT;
    else
      snake1.dir = LEFT;
  }
  //judge player2's direction
  valueX = X_pin2 - 512;
  valueY = Y_pin2 - 512;
  if(abs(valueX) > abs(valueY)){
    if(valueX > 0)
      snake2.dir = UP;
    else
      snake2.dir = DOWN;
  }else if(abs(valueX) < abs(valueY){
    if(valueY > 0)
      snake2.dir = RIGHT;
    else
      snake.dir = LEFT;
  }
}

void draw_snake(int r, int c){  field[r][c] =1;}
void draw_food(int r, int c){  field[r][c] =1;}
void draw_food(){
    if(!poo1.poo_eaten){
        field[poo1.y][poo1.x]=1;
    }
    if(!poo2.poo_eaten){
        field[poo2.y][poo2.x]=1;
    }
}
void screen(void)
{
    memset(field, 0, 64*sizeof(int))
    //draw snake1
    for (i = 0; i < snake1.len; i++)
    {
        draw_snake(snake1.y[i], snake1.x[i]);
    }
    //draw snake2
    for (i = 0; i < snake2.len; i++)
    {
        draw_snake(snake2.y[i], snake2.x[i]);
    }
    //draw food in
    draw_food(food.y, food.x);
    //draw poo in
    draw_poo();
}
bool check_snake_die(Snake snakeA, Snake snakeB)
{
  //too much short!
  if(snakeA.len <= 1){
    return true;
  }
  //head on border
  if (snakeA.x < 0 || snakeA.x > 7 || sankeA.y < 0 || snakeA.y > 7)
  {
    return true;
  }

  //head on self or opponent
  if (snakeA.len > 4)
  {
    for (int i = 1; i < snakeA.len; i++)
    {
      if (snakeA.x == snakeA.x[i] && snakeA.y == sankeA.y[i])
      {
        return true;
      }
    }
  }
  for (int i = 0; i<snakeB.len; i++){
    if(snakeA.x == snakeB.x[i] && snakeA.y == snakeB.y[i]){
      return true;
    }
  }
  //default: no winner now
  return false;
}
void generate_food(void)
{
  int food_out = 0; //food is born in a same position with snake

  if (food.eaten)
  {
    while (food_out == 0)
    {
      food_out = 1;

      food.x = (uint8_t)(random(8)); // random number from 0 to 7
      food.y = (uint8_t)(random(8));

      for (int i = snake1.len - 1; i > 0; i--) //from tail to head to check if food is generate in the snake body
      {
        if (food.x == snake1.x[i] && food.y == snake1.y[i])
        {
          food_out = 0;
        }
      }
      for (int i = snake2.len - 1; i > 0; i--) //from tail to head to check if food is generate in the snake body
      {
        if (food.x == snake2.x[i] && food.y == snake2.y[i])
        {
          food_out = 0;
        }
      }
    }
  }

  food_eaten = false;
}
//generate a poo
void generate_poo(void){
    //player1 use skill, and poo1 has been eaten
    if(SW_pin1 && poo1.poo_eaten){
        poo1.poo_eaten = false;
        poo1.x = snake1.x[snake1.len - 1];
        poo1.y = sanke1.y[snake1.len - 1];
        poo1.time = 5000; //I think 5s is eough.
    }
    //player2 use skill, and poo2 has been eaten
    if(SW_pin2 && poo2.poo_eaten){
        poo2.poo_eaten = false;
        poo2.x = snake2.x[snake2.len - 1];
        poo2.y = sanke2.y[snake2.len - 1];
        poo2.time = 5000; //I think 5s is eough.
    }
}
//refreshing poo
void refresh_poo(int time){
    if(!poo1.poo_eaten){
        poo1.time -= time;
        //time is up it should be disappear.
        if(poo1.time <= 0)
            poo1.poo_eaten = true;
    }
    if(!poo2.poo_eaten){
        poo2.time -= time;
        //time is up it should be disappear.
        if(poo2.time <= 0)
            poo2.poo_eaten = true;
    }
}
void snake1_move(void)
{
  switch (snake1.dir) {
    case RIGHT:
      snake1.headx ++;
      break;
    case UP:
      snake1.heady --;
      break;
    case LEFT:
      snake1.headx --;
      break;
    case DOWN:
      snake1.heady ++;
      break;
  }
  snake1.eaten = false;
  if ((snake1.headx == food.x ) && (snake1.heady == food.y))
  {
    food.eaten = true; //allow to create a new food
    snake1.eaten = true;
    snake1.len++;
  }
  //eat it's own poo
  if ((snake1.headx == poo1.x) && (snake1.heady == poo1.y))
  {
    poo1.poo_eaten = true; //allow player1 to poo
    snake1.len--;
  }
  //eat player2's poo
  if ((snake1.headx == poo2.x) && (snake1.heady == poo2.y))
  {
    poo2.poo_eaten = true; //allow player2 to poo
    snake1.len--;
  }
  win2 = check_snake_die(snake1, snake2);
  //player1 eat a food
  if(snake1.eaten){
    for (i = snake1.len - 1; i > 0; i--)
    {
      snake1.x[i] = snake1.x[i - 1];
      snake1.y[i] = snake1.y[i - 1];
    }
    snake1.x[0] = snake1.headx;
    snake1.y[0] = snake1.heady;
  }
}
void snake2_move(void)
{
  switch (snake2.dir) {
    case RIGHT:
      snake2.headx ++;
      break;
    case UP:
      snake2.heady --;
      break;
    case LEFT:
      snake2.headx --;
      break;
    case DOWN:
      snake2.heady ++;
      break;
  }
  snake2.eaten = false;
  if ((snake2.headx == food.x ) && (snake2.heady == food.y))
  {
    food.eaten = true; //allow to create a new food
    snake2.eaten = true;
    snake2.len++;
  }
  //eat player1's poo
  if ((snake2.headx == poo1.x) && (snake2.heady == poo1.y))
  {
    poo1.poo_eaten = true; //allow player1 to poo
    snake2.len--;
  }
  //eat player2's poo
  if ((snake2.headx == poo2.x) && (snake2.heady == poo2.y))
  {
    poo2.poo_eaten = true; //allow player2 to poo
    snake2.len--;
  }
  win1 = check_snake_die(snake2, snake1);
  //playe2 eat a food
  if(snake2.eaten){
    for (i = snake2.len - 1; i > 0; i--)
    {
      snake2.x[i] = snake2.x[i - 1];
      snake2.y[i] = snake2.y[i - 1];
    }
    snake2.x[0] = snake2.headx;
    snake2.y[0] = snake2.heady;
  }
}

void loop() {
  Serial.print("Switch1:  ");
  Serial.print(digitalRead(SW_pin1));
  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(analogRead(X_pin1));
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.println(analogRead(Y_pin1));
  Serial.print("\n\n");

  Serial.print("Switch2:  ");
  Serial.print(digitalRead(SW_pin2));
  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(analogRead(X_pin2));
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.println(analogRead(Y_pin2));
  Serial.print("\n\n");
  delay(Delay);

  printField ();
  Serial.print("\n");
  delay(500);

    if(win1 || win2)
    {
        //shoking the player
        //draw_game_over();
    } else {
        keyScan();
        snake1_move();
        snake2_move();
        generate_food();
        generate_poo();
        screen();
     }
    //here need a number for game speed, I just assume it's 500
    //refresh the existed poo
    refresh_poo(Delay);
}