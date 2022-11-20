#include <Servo.h>
#include "MotorDriver.h"
    const int trig = 13;     // chân trig của HC-SR04
    const int echo = 2;     // chân echo của HC-SR04
    const int led = 13;     // chân echo của HC-SR04
    const byte LEFT = 1;
    const byte RIGHT = 0;
    const byte MID = -1;

    int const InitPos = 0;  
    int const EndPos = 120;
    int const MidPos = 50;
    int distance;           // biến lưu khoảng cách
    int disLeft;           // biến lưu khoảng cách
    int disMid;           // biến lưu khoảng cách

    int disRight;           // biến lưu khoảng cách
    int gap=300;
    int danger=30;
    Servo servo;  
    MotorDriver m;

    int pos = 0;   
    void setup()
    {
        Serial.begin(9600);     // giao tiếp Serial với baudrate 9600
        pinMode(trig,OUTPUT);   // chân trig sẽ phát tín hiệu
        pinMode(echo,INPUT);    // chân echo sẽ nhận tín hiệu
        servo.attach(9);
        servo.write(MidPos);  

    }
     
    int doDistance(){
      unsigned long duration; // biến đo thời gian
        int distance;           // biến lưu khoảng cách
        
        /* Phát xung từ chân trig */
        digitalWrite(trig,0);   // tắt chân trig
        delayMicroseconds(2);
        digitalWrite(trig,1);   // phát xung từ chân trig
        delayMicroseconds(5);   // xung có độ dài 5 microSeconds
        digitalWrite(trig,0);   // tắt chân trig
        
        /* Tính toán thời gian */
        // Đo độ rộng xung HIGH ở chân echo. 
        duration = pulseIn(echo,HIGH);  
        // Tính khoảng cách đến vật.
        distance = int(duration/2/29.412);
        
        /* In kết quả ra Serial Monitor */
        return distance;

    }

    int scan(bool dir){
      int distance;
      if (dir == LEFT){
      servo.write(InitPos);  
      delay(gap);
      distance  = doDistance();
       
      }else if (dir == RIGHT){
      servo.write(EndPos); 
      delay(gap);
             
      distance  = doDistance();
  
      }else if (dir == MID){
      servo.write(MidPos);
     delay(gap);

      distance  = doDistance();
      }
       servo.write(MidPos);
     delay(gap);
      return  distance;
    }
    

    void tien(MotorDriver m){
       m.motor(4,FORWARD,255);
       m.motor(3,FORWARD,255);  
       m.motor(2,BACKWARD,255);
       m.motor(1,BACKWARD,255);  
    }
    void lui(MotorDriver m){
       m.motor(1,FORWARD,255);
       m.motor(2,FORWARD,255);  
       m.motor(3,BACKWARD,255);
       m.motor(4,BACKWARD,255);  
    }
  void quayTrai(MotorDriver m){
       m.motor(1,FORWARD,255);
       m.motor(2,FORWARD,255);  
       m.motor(3,FORWARD,255);
       m.motor(4,FORWARD,255);  
    }
        
    void quayPhai(MotorDriver m){
       m.motor(1,BACKWARD,255);
       m.motor(2,BACKWARD,255);  
       m.motor(3,BACKWARD,255);
       m.motor(4,BACKWARD,255);  
    }   
    void dung(MotorDriver m){
      m.motor(1,BRAKE,255);
       m.motor(2,BRAKE,255);  
       m.motor(3,BRAKE,255);
       m.motor(4,BRAKE,255); 
    }
    int fullScan(){
        disLeft= scan(LEFT);
        
        disRight= scan(RIGHT);
        disMid= scan(MID);
        servo.write(MidPos); 

        
        if(disLeft>disRight>disMid) return disLeft;
        if(disLeft<disRight<disMid) return disMid;
        if(disLeft>disRight<disMid) return disRight;
        else return 0;


    }
     void loop()
    {
        distance=fullScan();
        
        
        if (distance<=15) {
          //dung(m);
          //lui(m);
         
          Serial.println("disLeft ");
          Serial.println(disLeft);
          Serial.println("disRight ");
          Serial.println(disRight);
          if(disRight>disLeft>danger){
            quayPhai(m);
          }
          else if(disLeft>disRight>danger){
            quayTrai(m);
          }   
          else{
            quayPhai(m);
            quayPhai(m);
          }       
          digitalWrite(led,0);


         }

        else  {
              tien(m); 
          }
      
    }