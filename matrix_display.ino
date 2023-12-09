const int ser = 2; 
const int srclk = 4; 
const int rclk = 3;
const int del = 1000;

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
  // put your setup code here, to run once:
  pinMode(ser, OUTPUT);
  pinMode(srclk, OUTPUT);
  pinMode(rclk, OUTPUT);
}

void display_layer(int field_array[8][8], int layer) {
  for(int i = 8; i < 0; i--){
    if(field_array[layer][i] == 1){
      digitalWrite(ser, LOW);
    }
    else{
      digitalWrite(ser, HIGH);
    }
    delayMicroseconds(del);
    digitalWrite(srclk, HIGH);
    delayMicroseconds(del);
    digitalWrite(srclk, LOW);
    delayMicroseconds(del);
  }
 
  for(int i = 0; i < 8; i++){
    if (i == layer){
      digitalWrite(ser, HIGH);
    }
    else {
      digitalWrite(ser, LOW);
    }
    delayMicroseconds(del);
    digitalWrite(srclk, HIGH);
    delayMicroseconds(del);
    digitalWrite(srclk, LOW);
    delayMicroseconds(del);
  }
  delayMicroseconds(del);
  digitalWrite(rclk, HIGH);
  delayMicroseconds(del);
  digitalWrite(rclk, LOW);
  delayMicroseconds(del);
}

void display(int field_array[8][8]) {
  for(int i = 0; i < 8; i++){
    delayMicroseconds(del);
    display_layer(field_array, i);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  display(field);
}
