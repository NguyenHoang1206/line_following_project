#include <ServoC.h>
#include <Arduino.h>

int stop_radar_distance = 12;// Khoảng cách phát hiện vật cản
//Kết nối SRF 05 OR 04
const int trigPin = A1; // kết nối chân trig với chân 11 arduino
const int echoPin = A0; // kết nối chân echo với chân 12 arduino

//L298 kết nối arduino
const int motorA1      = 10;  // kết nối chân IN1 với chân 3 arduino
const int motorA2      = 8;  // kết nối chân IN2 với chân 4 arduino
const int motorAspeed  = 9;  // kết nối chân ENA với chân 5 arduino
const int motorB1      = 7; // kết nối chân IN3 với chân 7 arduino
const int motorB2      = 6; // kết nối chân IN4 với chân 8 arduino
const int motorBspeed  = 5;  // kết nối chân ENB với chân 6 arduino
//dong co A dieu khien banh xe trai, dong co B dieu khien banh xe phai

//kết nối của 3 cảm biến hồng ngoại (dò line )
const int L_S = 13; // cb dò line phải
const int S_S = 12; // cb dò line giữa
const int R_S = 11; //cb dò line trái

// ket noi chan pwm
const int servopwm = 3;// ket noi chan pwm cua servo voi chan D3 cua arduino

int left_sensor_state;// biến lưu cảm biến hồng ngoại line trái
int s_sensor_state;   // biến lưu cảm biến hồng ngoại line giữa
int right_sensor_state;// biến lưu cảm biến hồng ngoại line phải

long radar_duration; //
int radar_distance;  // biến khoảng cách

ServoC myservo; // bien dieu khien servo


void setup() {
  pinMode(L_S, INPUT); // chân cảm biến khai báo là đầu vào
  pinMode(R_S, INPUT);
  pinMode(S_S, INPUT);
  pinMode(motorA1, OUTPUT); // chan motor khai bao la dau ra
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(motorAspeed, OUTPUT);
  pinMode(motorBspeed, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
  analogWrite(motorAspeed, 120); // tốc độ động cơ a ban đầu 120 ( 0 - 255)
  analogWrite(motorBspeed, 120 );// tốc độ động cơ b ban đầu 120 ( 0 - 255)
  delay(2000);

}

void forward() { // chương trình con xe robot đi tiến

  digitalWrite (motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  digitalWrite (motorB1, HIGH);
  digitalWrite(motorB2, LOW);
  Serial.println("forward");
}

void backward() {
  digitalWrite (motorA1, HIGH); // cho xe robot chạy lùi 1 đoạn
  digitalWrite(motorA2, LOW);
  digitalWrite (motorB1, LOW);
  digitalWrite(motorB2, HIGH);
  Serial.println("backward");
}


void turnRight() {

  digitalWrite (motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite (motorB1, HIGH);
  digitalWrite(motorB2, LOW);
  Serial.println("turnright");
}

void turnLeft() {

  digitalWrite (motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  digitalWrite (motorB1, LOW);
  digitalWrite(motorB2, LOW);
  Serial.println("turnleft");
}

void Stop() {

  digitalWrite (motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite (motorB1, LOW);
  digitalWrite(motorB2, LOW);
  Serial.println("stop");
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  radar_duration = pulseIn(echoPin, HIGH);
  radar_distance = radar_duration * 0.034 / 2;
  delay(50);
  Serial.print("radar_distance: ");
  Serial.println(radar_distance);

  left_sensor_state = digitalRead(L_S);
  s_sensor_state = digitalRead(S_S);
  right_sensor_state = digitalRead(R_S);

  if ((digitalRead(L_S) == 0) && (digitalRead(S_S) == 1) && (digitalRead(R_S) == 0)) {
    forward(); // đi tiến
    Serial.println("forward");
  }

  else if ((digitalRead(L_S) == 1) && (digitalRead(S_S) == 1) && (digitalRead(R_S) == 0)) {
    turnLeft(); // rẻ trái
    Serial.println("turnleft");
  }
  else if ((digitalRead(L_S) == 1) && (digitalRead(S_S) == 0) && (digitalRead(R_S) == 0)) {
    turnLeft(); // rẻ trái
    Serial.println("turnleft");
  }

  else if ((digitalRead(L_S) == 0) && (digitalRead(S_S) == 1) && (digitalRead(R_S) == 1)) {
    turnRight(); // rẻ phải
    Serial.println("turnright");
  }
  else if ((digitalRead(L_S) == 0) && (digitalRead(S_S) == 0) && (digitalRead(R_S) == 1)) {
    turnRight(); // rẻ phải
    Serial.println("turnright");
  }

  else if ((digitalRead(L_S) == 1) && (digitalRead(S_S) == 1) && (digitalRead(R_S) == 1)) {
    Stop(); // stop
    Serial.println("stop");
  }



  if (radar_distance < stop_radar_distance) // nếu khoảng cách nhỏ hơn giới hạn
  {

    backward();
    delay(300);

    Stop();
    delay(200);


    turnLeft(); // cho xe robot xoay sang trái
    Serial.println("turnleft");
    delay(280);
    Stop();
    delay(200);


    forward();
    delay(550);
    Stop();
    delay(200);


    turnRight();//cho xe xoay sang phai
    delay(500);
    Stop();
    delay(200);


    myservo.write(90);
    Serial.println("rotate90");
    delay(500);
    
    while(radar_distance <= 20){
    forward();
    Serial.println("forward");
    delay(200);
    }

    Stop();
    delay(500);

    forward();//////////////////////////cho xe robot xoay phải 1 đoạn
    Serial.println("turnright");
    delay(370);
    Stop();
    Serial.println("stop");
    delay(200);
    
    forward(); //cho xe robot đi thẳng 1 doan
    Serial.println("forward");
    while (left_sensor_state == LOW) {

      left_sensor_state = digitalRead(L_S);
      s_sensor_state = digitalRead(S_S);
      right_sensor_state = digitalRead(R_S);
      Serial.println("in the first while");

    }
  }
}

