#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//www.elegoo.com
//2016.12.09

// Arduino pin numbers
const int SW_pin1 = 2; // digital pin connected to switch output
const int X_pin1 = A0; // analog pin connected to X output
const int Y_pin1 = A1; // analog pin connected to Y output

const int SW_pin2 = 1; // digital pin connected to switch output
const int X_pin2 = A2; // analog pin connected to X output
const int Y_pin2 = A3; // analog pin connected to Y output


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
  delay(1000);

  printField ();
  Serial.print("\n");
  delay(500);
}






Adafruit_SSD1306 oled(128, 64, &Wire, -1);

#define RIGHT 0
#define LEFT  1
#define UP    2
#define DOWN  3

//摇杆相关变量
#define pinX  A0
#define pinY  A1

int valueX = 0;
int valueY = 0;
unsigned char keyValue = 0;


// this is the matrix of the board it can map to the led board
const uint8_t block[] PROGMEM = {
  0xf0, //B1111000
  0xb0, //B1011000
  0xd0, //B1101000
  0xf0, //B1111000
};
typedef struct Snake{
  int y; //head row
  int x; //head column
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
Snake snake1 = {7, 1, 2, RIGHT, 0}
Snake snake2 = {1, 7, 2, Left, 0}


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
  static unsigned char keyUp = 1;

  valueX = analogRead(pinX);
  valueY = analogRead(pinY);

  if (keyUp && ((valueX <= 10) || (valueX >= 1010) || (valueY <= 10) || (valueY >= 1010)))
  {
    delay(10);
    keyUp = 0;
    if (valueX <= 10)
    {
      if (snake_dir != LEFT)
      {
        snake_dir = RIGHT;
      }
    }
    else if (valueX >= 1010)
    {
      if (snake_dir != RIGHT)
      {
        snake_dir = LEFT;
      }
    }
    else if (valueY <= 10)
    {
      if (snake_dir != UP)
      {
        snake_dir = DOWN;
      }
    }
    else if (valueY >= 1010)
    {
      if (snake_dir != DOWN)
      {
        snake_dir = UP;
      }
    }
  } else if ((valueX > 10) && (valueX < 1010) && (valueY > 10) && (valueY < 1010))keyUp = 1;
  return 0;
}

void draw_snake(int x, int y)
{
  oled.drawBitmap(x, y, block, 4, 4, 1);
}

void show_score(int x, int y, int data)
{
  oled.setCursor(x, y);//设置显示位置
  oled.println(data);

}

void screen(void)
{
  oled.clearDisplay();//清屏
  oled.setTextSize(1); //设置字体大小
  oled.drawRect(0, 1, 102, 62, 1);
  oled.drawRect(0, 0, 102, 64, 1);
  oled.setCursor(104, 12);//设置显示位置
  oled.println("LEVE");
  oled.setCursor(104, 40);//设置显示位置
  oled.println("SCOR");

  show_score(110, 25, level);
  show_score(110, 53, score);

  for (i = 0; i < snake_len; i++)
  {
    draw_snake(x[i], y[i]);
  }

  draw_snake(food_x, food_y);

  oled.display();

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
void draw_food(void)
{
  int food_out = 0; //food is born in a same position with snake

  if (food_eaten)
  {
    while (food_out == 0)
    {
      food_out = 1;

      food_x = (uint8_t)(random(8)); // random number from 0 to 7
      food_y = (uint8_t)(random(8));

      for (int i = snake1.len - 1; i > 0; i--) //from tail to head to check if food is generate in the snake body
      {
        if (food_x == snake1.x[i] && food_y == snake1.y[i])
        {
          food_out = 0;
        }
      }
      for (int i = snake2.len - 1; i > 0; i--) //from tail to head to check if food is generate in the snake body
      {
        if (food_x == snake2.x[i] && food_y == snake2.y[i])
        {
          food_out = 0;
        }
      }
    }
  }

  food_eaten = false;
}
//generate a poo
void draw_poo(void){}
void snake1_move(void)
{

  switch (snake1.dir) {
    case RIGHT:
      snake1.x ++;
      break;
    case UP:
      snake1.y --;
      break;
    case LEFT:
      snake1.x --;
      break;
    case DOWN:
      snake1.y ++;
      break;
  }
  snake1.eaten = false;
  if ((snake1.x == food.x ) && (snake1.y == food.y))
  {
    food.eaten = true; //allow to create a new food
    snake1.eaten = true;
    snake1.len++;
  }
  //eat it's own poo
  if ((snake1.x == poo1.x) && (snake1.y == poo1.y))
  {
    poo1.poo_eaten = true; //allow player1 to poo
    snake1.len--;
  }
  //eat player2's poo
  if ((snake1.x == poo2.x) && (snake1.y == poo2.y))
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
    snake1.x[0] = snake1.x;
    snake1.y[0] = snake1.y;
  }
}
void snake2_move(void)
{
  switch (snake2.dir) {
    case RIGHT:
      snake2.x ++;
      break;
    case UP:
      snake2.y --;
      break;
    case LEFT:
      snake2.x --;
      break;
    case DOWN:
      snake2.y ++;
      break;
  }
  snake2.eaten = false;
  if ((snake2.x == food.x ) && (snake2.y == food.y))
  {
    food.eaten = true; //allow to create a new food
    snake2.eaten = true;
    snake2.len++;
  }
  //eat player1's poo
  if ((snake2.x == poo1.x) && (snake2.y == poo1.y))
  {
    poo1.poo_eaten = true; //allow player1 to poo
    snake2.len--;
  }
  //eat player2's poo
  if ((snake2.x == poo2.x) && (snake2.y == poo2.y))
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
    snake2.x[0] = snake2.x;
    snake2.y[0] = snake2.y;
  }
}

//here you can add the logic for penalty
void draw_game_over()
{
  oled.clearDisplay();//清屏
  oled.setTextSize(2); //设置字体大小
  oled.setCursor(10, 10);//设置显示位置

  oled.println("GAME OVER");
  oled.setTextSize(1); //设置字体大小
  oled.setCursor(30, 35);//设置显示位置
  oled.println("LEVE:");
  oled.setCursor(30, 55);//设置显示位置
  oled.println("SCOR:");

  show_score(80, 35, level);
  show_score(80, 55, score);

  oled.display();
}
//set for light up the led light
void setup()
{
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.setTextColor(WHITE);
  randomSeed(analogRead(3));
}


void loop()
{
  if(win1 || win2)
  {
    draw_game_over();
  } else {
    keyScan();
    snake1_move();
    snake2_move();
    draw_food();
    screen();
  }
  delay(snake_speed);
}