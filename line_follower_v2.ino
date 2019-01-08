const int right_infrared = 7;
const int middle_infrared = 2;
const int left_infrared = 5;

const int center_trig = 10;
const int center_echo = 6;

const int right_dir = 13;
const int right_break = 8;
const int right_pwm = 11;

const int left_dir = 12;
const int left_break = 9;
const int left_pwm = 3;

const int start_pin = 4;
bool start = false;
int counter = 0;

volatile bool state = true;

void setup() {
  pinMode(center_trig, OUTPUT);
  pinMode(center_echo, INPUT);

  pinMode(right_infrared, INPUT);
  pinMode(left_infrared, INPUT);
  pinMode(middle_infrared, INPUT);
  
  pinMode(right_dir, OUTPUT);
  pinMode(right_break, OUTPUT);
  pinMode(right_pwm, OUTPUT);

  pinMode(left_dir, OUTPUT);
  pinMode(left_break, OUTPUT);
  pinMode(left_pwm, OUTPUT);

  pinMode(start_pin, INPUT);

  attachInterrupt(digitalPinToInterrupt(middle_infrared), line_found, RISING);

  delay(1000);
}

void loop() {
  if(start){
    if(state){
      if(has_obstacle()){
        avoid_obstacle();
        state = false;
      }
      else{
        follow_line();  
      }
    }
    else{
      go_around();
      stop_moving(500);
      counter = 0;
    }
  }
  else{
    if(digitalRead(start_pin) == HIGH){
      start = true;
    }
  }
  delay(150);
}

void avoid_obstacle(){
  stop_moving(1000);
  turn_right(400, 255);
  stop_moving(1000);
  go_forward(750, 150);
  stop_moving(1000);
  turn_left(500, 255);
  stop_moving(1000);
  go_forward(750, 150);
  stop_moving(1000);
}

void line_found(){
  state = true;
}

void go_around(){
  while(true){
    turn_left(250, 255);
    stop_moving(500);
    for(int i = 0; i < 5; i++){
     go_forward(150, 180);
     stop_moving(300);
      if(state && check_for_line()){
        return;
      }
      else{
        state = false;
      }
    }
  }
}

bool check_for_line(){
  int sum = 0;
  for(int i = 0; i < 10; i++){
    if(digitalRead(middle_infrared) == 1){
      sum++;
    }
    delay(100);
  }
  return sum > 5;
}

void follow_line(){
  if(digitalRead(right_infrared) == 1 && digitalRead(left_infrared) == 1){
    stop_moving(500);
    go_forward(350, 100);
    stop_moving(500);
    turn_right(400, 255);
    stop_moving(500);
    counter = 0;
  }
  else if(digitalRead(right_infrared) == 1){
      stop_moving(100);
      turn_right(120, 255);
      stop_moving(100);
      counter = 0;
    }
  else if(digitalRead(left_infrared) == 1){
    stop_moving(100);
    turn_left(120, 255);
    stop_moving(100);
    counter = 0;
  }
  else{
    if(counter > 4){
      go_forward(0, 100);  
    }
    else{
      go_forward(0, 160);  
    }
    counter++;
  }
}

void drive(uint8_t r_dir, uint8_t r_break, int r_pwm, uint8_t l_dir, uint8_t l_break, int l_pwm, int mil){
  digitalWrite(right_dir, r_dir);
  digitalWrite(right_break, r_break);
  analogWrite(right_pwm, r_pwm);

  digitalWrite(left_dir, l_dir);
  digitalWrite(left_break, l_break);
  analogWrite(left_pwm, l_pwm);

  delay(mil);
}

void go_forward(int mil, int pwm){
  drive(HIGH, LOW, pwm, HIGH, LOW, pwm, mil);
}

void turn_left(int mil, int pwm){
  drive(HIGH, LOW, pwm, LOW, LOW, pwm, mil);
}

void turn_right(int mil, int pwm){
  drive(LOW, LOW, pwm, HIGH, LOW, pwm, mil);
}

void stop_moving(int mil){
  drive(HIGH, HIGH, 100, HIGH, HIGH, 100, mil);
}

bool has_obstacle(){
  int distance = distance_from(center_trig, center_echo);
  return distance < 14 && distance > 3;
}

int distance_from(int trig_pin, int echo_pin){
  long duration;
  int distance;
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  
  duration = pulseIn(echo_pin, HIGH);
  distance = duration*0.034/2;
  
  return distance;
}
