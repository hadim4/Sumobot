// ----- IR Sensor -----

int IRSensor = 2;     // 

// ----- Ultrasonic Sensor -----
int trig = 7;         // trig pin
int echo = 8;         // echo pin

// ----- Motors ------
//left side motors
int motorA1 = 6;
int motorA2 = 3;
//Right side motors
int motorB1 = 4;
int motorB2 = 5;

// ----- PWM -----
int pwma = 10;
int pwmb = 9;

// ----- Setup -----
void StartState() {
  leftState(0);
}

void setup() {
  Serial.begin(9600);

  // IR setup
  pinMode(IRSensor, INPUT);

  // Ultrasonic setup
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // Motor setup
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(pwma, OUTPUT); 
  pinMode(pwmb, OUTPUT); 

  StartState();
}



// ------ MOVEMENT ------
void forwardState(int speed) {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);

  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);

  analogWrite(pwma, speed);
  analogWrite(pwmb, speed);
}

void backwardState(int speed) {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);

  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);

  analogWrite(pwma, speed);
  analogWrite(pwmb, speed);
}

void stop(){
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);

  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);

  analogWrite(pwma, 0);
  analogWrite(pwmb, 0);
}

void leftState(int speed) {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);

  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);

  analogWrite(pwma, speed);
  analogWrite(pwmb, speed);
}

void rightState(int speed) {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);

  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);

  analogWrite(pwma, speed);
  analogWrite(pwmb, speed);
}



 // ------ SENSORS ------
int getDistance(int print_in_serial) {
  // ----- Read Ultrasonic Sensor -----
  float time_us, distance_cm;

  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  time_us = pulseIn(echo, HIGH);
  distance_cm = (time_us * 0.0343) / 2;
  /*if (distance_cm > 80) { // Too close will result in max distance
    distance_cm = 0; 
  } */

  if (print_in_serial == 1) {
    Serial.print("Distance: ");
    Serial.print(distance_cm);
    Serial.println(" cm");
  }
  return distance_cm;
}

int getColor(int print_in_serial) {
  // ----- Read IR Sensor -----
  int color = digitalRead(IRSensor);

  if (color == 1) {
    color = 0;
  } else {
    color = 1;
  }

  if (print_in_serial == 1) {
    if (color == 0) {
      Serial.println("Black");
    } else {
      Serial.println("White");
    }
  }

  return color;
}



// ------ PLANS ------
boolean targetCheck() // Check if enemy bot is in front of us 
{
  float distance = getDistance(1);
  if (distance < 80) { // If enemy bot is within 80 cm
    return true;
  } else {
    return false;
  }
}

int plan_A() { // Ram Into Enemy Bot If It's Within Ring
  if (targetCheck() == false) { // If no enemy bot is detected
    stop();
    return 2;
  } else { // enemy bot detected
    forwardState(255);
  }

  while (true) {
    if (getColor(0) == 1) { // If white line is detected
      stop();
      return 1;
    }
    if (targetCheck() == false) { // If no enemy bot is detected
      stop();
      return 2;
    }
    delay(200);
  }
}

boolean plan_B() { // Start spinning to find enemy bot
  while (true) {
    leftState(150);
    delay(150);
    stop();
    delay(50);
    if (targetCheck() == true) { // If enemy bot is detected
      return true;
    }
  }
}

boolean test() {
  rightState(200);
  targetCheck();
}


// ------ LOOP ------
void loop() {
  plan_A();
  Serial.println("DONE");
  delay(150);
}
