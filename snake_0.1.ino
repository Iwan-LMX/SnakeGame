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
  uint8_t y[100];
  uint8_t x[100];
  // int level = 1;
  // int snake_speed = 150;
}Snake;
//player 1 and 2's snakes head positions, length, moving directions
Snake snake1 = {7, 1, 2, RIGHT}
Snake snake2 = {1, 7, 2, Left}


typedef struct Food{
  uint8_t y; //row
  uint8_t x; //column
}Food;
typedef struct Poo{
  uint8_t y; //row
  uint8_t x; //column
  bool poo_eaten = true;
}Poo;
Poo poo1, poo2;


bool game_over = false;
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

void draw_food(void)
{
  int food_out = 0; //food is born in a same position with snake

  if (food_eaten)
  {
    while (food_out == 0)
    {
      food_out = 1;

      food_x = (uint8_t)(random()%8);
      food_y = (uint8_t)(random()%8);

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

  if ((snake1.x == food. ) && (snake2.y == food_y))
  {
    food_eaten = true; //可重新生成食物
    snake1.len++;
  }
  if ((snake1.x == poo.x) && (snake2.y == poo.y))
  {
    food_eaten = true; //可重新生成食物
    snake1.snake_len--;
  }

  for (i = snake_len - 1; i > 0; i--)
  {
    x[i] = x[i - 1];
    y[i] = y[i - 1];
  }
  x[0] = snake_head_x;
  y[0] = snake_head_y;

  check_snake_die();
}
void snake2_move(void)
{
  switch (snake2.snake_dir) {
    case RIGHT:
      snake2.snake_head_x ++;
      break;
    case UP:
      snake2.snake_head_y --;
      break;
    case LEFT:
      snake2.snake_head_x --;
      break;
    case DOWN:
      snake2.snake_head_y ++;
      break;
  }

  if ((snake1.snake_head_x == food_x) && (snake2.snake_head_y == food_y))
  {
    food_eaten = true; //可重新生成食物
    snake1.snake_len++;
    score++;
    level = score / 5 + 1;
    snake_speed -= level;
  }

  for (i = snake_len - 1; i > 0; i--)
  {
    x[i] = x[i - 1];
    y[i] = y[i - 1];
  }
  x[0] = snake_head_x;
  y[0] = snake_head_y;

  check_snake_die();
}
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



void check_snake_die(void)
{
  //撞墙
  if (snake_head_x < 4 || snake_head_x > 96 || snake_head_y < 1 || snake_head_y > 56)
  {
    game_over = true;
  }

  //自己吃自己
  if (snake_len > 4)
  {
    for (int i = 1; i < snake_len; i++)
    {
      if (snake_head_x == x[i] && snake_head_y == y[i])
      {
        game_over = true;
      }
    }
  }
}


void setup()
{
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.setTextColor(WHITE);//开像素点发光
  randomSeed(analogRead(3));//初始化随机种子
}


void loop()
{
  if (game_over)
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