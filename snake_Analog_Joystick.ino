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
