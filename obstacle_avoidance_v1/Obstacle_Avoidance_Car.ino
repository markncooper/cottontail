//www.elegoo.com

#include <Servo.h>  //servo library
Servo myservo;      // create servo object to control servo

int Echo = A4;  
int Trig = A5; 

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define carSpeed 250
#define SERVO_CENTER 30
int rightDistance = 0, leftDistance = 0, middleDistance = 0;

void forward(){ 
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Forward");
}

void back() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Back");
}

void left() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); 
  Serial.println("Left");
}

void right() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Right");
}

void stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("Stop!");
} 

//Ultrasonic distance measurement Sub function
int Distance_test() {
  digitalWrite(Trig, LOW);   
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);   
  float Fdistance = pulseIn(Echo, HIGH);  
  Fdistance= Fdistance / 58;       
  return (int)Fdistance;
}  

void setup() { 
  myservo.attach(3);  // attach servo on pin 3 to servo object
  Serial.begin(9600);     
  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT);  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  stop();
} 

// Reassess if we can keep going forward periodically
int distantTraveledForward = 0;

void loop() { 
    myservo.write(SERVO_CENTER);  //setservo position according to scaled value
    delay(150); 
    middleDistance = Distance_test();

    if (distantTraveledForward > 6 || middleDistance <= 20){
        distantTraveledForward = 0;
        stop();
        delay(500);                         
        myservo.write(SERVO_CENTER - 30);          
        delay(1000);      
        rightDistance = Distance_test();
        
        delay(500);
        myservo.write(SERVO_CENTER);              
        delay(1000);                                                  
        myservo.write(SERVO_CENTER + 30);              
        delay(1000); 
        leftDistance = Distance_test();
        
        delay(500);
        myservo.write(SERVO_CENTER);              
        delay(1000);

        if (middleDistance > 20 && leftDistance > 20 && rightDistance > 20){
          forward();
        }
        else if(rightDistance > leftDistance) {
          right();
          delay(500);
        }
        else if(rightDistance < leftDistance) {
          left();
          delay(500);
        }
        else if((rightDistance <= 30) || (leftDistance <= 30)) {
          // TODO: should we turn around in a random direction here?
          back();
          delay(500);
        }else {
          Serial.println("Turning around");
          left();
          delay(300);          
        }
    }  
    else {
        forward();
    }

    distantTraveledForward ++;
}

