const int center_trig = 5;
const int center_echo = 6;

const int left_trig = 7;
const int left_echo = 10;

const int right_trig = 4;
const int right_echo = 1;

const int right_motor = 13;
const int right_break = 8;
const int right_speed = 11;

const int left_motor = 12;
const int left_break = 9;
const int left_speed = 3;

const int start_pin = 2;

bool forward = false;
volatile bool start = false;
int counter;

void setup() {
  pinMode(center_trig, OUTPUT);
  pinMode(center_echo, INPUT);
  
  pinMode(left_trig, OUTPUT);
  pinMode(left_echo, INPUT);

  pinMode(right_trig, OUTPUT);
  pinMode(right_echo, INPUT);

  pinMode(right_motor, OUTPUT);
  pinMode(right_break, OUTPUT);

  pinMode(left_motor, OUTPUT);
  pinMode(left_break, OUTPUT);

  pinMode(start_pin, INPUT);

  attachInterrupt(digitalPinToInterrupt(start_pin), start_or_stop, HIGH);
  
  delay(10);
}

void loop() {
  if(start){
    if(has_obstacle()){
      stop_moving(1000);
      go_backward(800);
      stop_moving(1000);

      if(distance_from(left_trig, left_echo) < distance_from(right_trig, right_echo)){
        turn_right(200);
      }
      else{
        turn_left(200);
      }
      stop_moving(1000);
      forward = false;
      counter = 0;
    }
    else{
      if(counter <= 15){
        if(!forward){
          go_forward(90);
          forward = true;  
        }
        if(counter == 15){
          forward = false;
        }
      }
      else{
        if(!forward){
          go_forward(75);
          forward = true;  
        }
      }
      counter++;
    }
  }
  delay(80);
}

void start_or_stop(){
  start = true;
}

bool has_obstacle(){
  return distance_from(center_trig, center_echo) < 23 
            || distance_from(left_trig, left_echo) < 16 
            || distance_from(right_trig, right_echo) < 16;
}

int distance_from(int trig_pin, int echo_pin){
  long duration;
  int distance;
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo_pin, HIGH);
  
  // Calculating the distance
  distance = duration*0.034/2;
  
  return distance;
}

void go_forward(int vel){  
  digitalWrite(right_motor, HIGH);
  digitalWrite(right_break, LOW);
  analogWrite(right_speed, vel);

  digitalWrite(left_motor, HIGH);
  digitalWrite(left_break, LOW);
  analogWrite(left_speed, vel);
}

void go_backward(int mil){
  digitalWrite(right_motor, LOW);
  digitalWrite(right_break, LOW);
  analogWrite(right_speed, 90);

  digitalWrite(left_motor, LOW);
  digitalWrite(left_break, LOW);
  analogWrite(left_speed, 90);

  delay(mil);
}

void turn_right(int mil){  
  digitalWrite(right_motor, LOW);
  digitalWrite(right_break, LOW);
  analogWrite(right_speed, 255);

  digitalWrite(left_motor, HIGH);
  digitalWrite(left_break, LOW);
  analogWrite(left_speed, 255);

  delay(mil);
}

void turn_left(int mil){
  digitalWrite(right_motor, HIGH);
  digitalWrite(right_break, LOW);
  analogWrite(right_speed, 255);

  digitalWrite(left_motor, LOW);
  digitalWrite(left_break, LOW);
  analogWrite(left_speed, 255);

  delay(mil);
}

void stop_moving(int mil){
  digitalWrite(right_break, HIGH);
  digitalWrite(left_break, HIGH);

  delay(mil);
}
