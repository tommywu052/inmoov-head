#include <Servo.h>

Servo myservo,myservo2,myservo3,myservo4;           // create servo object to control a servo

int pos = 0;            // variable to store the servo position

void setup() {
   Serial.begin(9600);
   myservo.attach(9);    // attaches the Top eye servo on pin 9 to the servo object
   myservo2.attach(10); //Botton Eye
   myservo3.attach(6); //Neck
   myservo4.attach(11); //Jaw
   myservo3.write(90);
   myservo2.write(0);
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("start");
}

void loop() { 
  String s;
  String jawAngle;
  String eyesLRAngle;
  String eyesUpDownAngle;
  String neckAngle;
  int cc = 0;
  
  // 正轉 180度，從 0 度旋轉到 180 度，每次 1 度
//  for (pos = 0; pos <= 180; pos += 1)  
//  {     
//    myservo.write(pos);    // 告訴 servo 走到 'pos' 的位置
//    Serial.println("To Right Eyes Angle");
//    Serial.println(pos);    
//    delay(15);        // 等待 15ms 讓 servo 走到指定位置
//  }
//    for (pos = 0; pos <= 180; pos += 1)  
//  {     
//    Serial.println("To Bottom Eyes Angle");
//    Serial.println(pos);  
//    myservo2.write(pos);    // 告訴 servo 走到 'pos' 的位置
//    delay(15);        // 等待 15ms 讓 servo 走到指定位置
//  }
////
////  // 反轉 180度，從 180 度旋轉到 0 度，每次 1 度 
//  for (pos = 180; pos >= 0; pos -= 1) 
//  { 
//    myservo.write(pos);   // 告訴 servo 走到 'pos' 的位置
//    Serial.println("To Left Eyes Angle");
//    Serial.println(pos);  
//
//    delay(15);   // 等待 15ms 讓 servo 走到指定位置
//  }
//
//    for (pos = 180; pos >= 0; pos -= 1) 
//  { 
//    myservo2.write(pos);   // 告訴 servo 走到 'pos' 的位置
//    Serial.println("To Top Eyes Angle");
//    Serial.println(pos);  
//    delay(15);   // 等待 15ms 讓 servo 走到指定位置
//  }

//    for (pos = 0; pos <= 180; pos += 1)  
//  {     
//    myservo3.write(pos);    // 告訴 servo 走到 'pos' 的位置
//    delay(15);        // 等待 15ms 讓 servo 走到指定位置
//  }
//
//    for (pos = 180; pos >= 0; pos -= 1) 
//  { 
//    myservo3.write(pos);   // 告訴 servo 走到 'pos' 的位置
//
//    delay(15);   // 等待 15ms 讓 servo 走到指定位置
//  }

if (Serial.available() > 0) {
        s = Serial.readString();
        s.trim();
        cc = s.length();
        Serial.println(s); 

         
        if (s.indexOf("jawAngle") > -1) {
           delay(5);      
           String jawAngle = s.substring(8,cc);
           
           //Serial.println(jawAngle);      
           //delay(1000); 
           myservo4.write(jawAngle.toInt());   // 告訴 Jaw servo 走到 'pos' 的位置
           delay(500); 
           Serial.print("Adjust JAW Angle to ");
           Serial.println(jawAngle);
        }

        if (s.indexOf("neckAngle") > -1) {
           delay(5);      
           String neckAngle = s.substring(9,cc);
           
           //Serial.println(jawAngle);      
           //delay(1000); 
           myservo3.write(neckAngle.toInt());   // 告訴 Jaw servo 走到 'pos' 的位置
           delay(500); 
           Serial.print("Adjust Neck Angle to ");
           Serial.println(neckAngle);
        }
        
        if (s.indexOf("eyesLRAngle") > -1) {
           delay(5);      
           String eyesLRAngle = s.substring(11,cc);
           
           //Serial.println(jawAngle);      
           //delay(1000); 
           myservo.write(eyesLRAngle.toInt());   // 告訴 Jaw servo 走到 'pos' 的位置
           delay(500); 
           Serial.print("Adjust eyesLRAngle Angle to ");
           Serial.println(eyesLRAngle);
        }

        if (s.indexOf("eyesUpDownAngle") > -1) {
           delay(5);      
           String eyesUpDownAngle = s.substring(15,cc);
           
           //Serial.println(jawAngle);      
           //delay(1000); 
           myservo2.write(eyesUpDownAngle.toInt());   // 告訴 Jaw servo 走到 'pos' 的位置
           delay(500); 
           Serial.print("Adjust eyesUpDownAngle Angle to ");
           Serial.println(eyesUpDownAngle);
        }
        
        if (s.equals("talk")) {
           delay(5);            
           //delay(1000); 
           myservo4.write(180);   // 告訴 Jaw servo 走到 'pos' 的位置
           delay(500); 
           myservo4.write(90);   // 告訴 Jaw servo 走到 'pos' 的位置
           //myservo4.write(90);   // 告訴 Jaw servo 走到 'pos' 的位置
           Serial.println("JAW Angle");
        }
        if (s.equals("openjaw")) {
           delay(5);            
           //delay(1000); 
           myservo4.write(180);   // 告訴 Jaw servo 走到 'pos' 的位置
           Serial.println("Open JAW");
        }
        if (s.equals("closejaw")) {
           delay(5);            
           myservo4.write(90);   // 告訴 Jaw servo 走到 'pos' 的位置
           //myservo4.write(90);   // 告訴 Jaw servo 走到 'pos' 的位置
           Serial.println("Close JAW");
        }
        if (s.equals("eyeright")) {
          for (pos = 0; pos <= 180; pos += 1)  
          {     
            myservo.write(pos);    // 告訴 servo 走到 'pos' 的位置
            Serial.println("To Right Eyes Angle");
            Serial.println(pos);    
            delay(15);        // 等待 15ms 讓 servo 走到指定位置
          }
           Serial.println("move eye to right");
        }

        if (s.equals("eyeleft")) {
          for (pos = 180; pos >= 0; pos -= 1)   
          {     
            myservo.write(pos);    // 告訴 servo 走到 'pos' 的位置
            Serial.println("To Right Eyes Angle");
            Serial.println(pos);    
            delay(15);        // 等待 15ms 讓 servo 走到指定位置
          }
           Serial.println("move eye to left");
        }

        if (s.equals("eyetop")) {
          for (pos = 0; pos <= 180; pos += 1)  
          {     
            myservo2.write(pos);    // 告訴 servo 走到 'pos' 的位置
            Serial.println("To Top Eyes Angle");
            Serial.println(pos);    
            delay(15);        // 等待 15ms 讓 servo 走到指定位置
          }
           Serial.println("move eye to right");
        }

        if (s.equals("eyebottom")) {
          for (pos = 180; pos >= 0; pos -= 1)  
          {     
            myservo2.write(pos);    // 告訴 servo 走到 'pos' 的位置
            Serial.println("To Bottom Eyes Angle");
            Serial.println(pos);    
            delay(15);        // 等待 15ms 讓 servo 走到指定位置
          }
           Serial.println("move eye to right");
        }

        if (s.equals("neckturn")) {
           delay(5); 
           myservo3.write(140);   // 告訴 Neck servo 走到 'pos' 的位置
           delay(1500); 
           myservo3.write(100);   // 告訴 Neck servo 走到 'pos' 的位置
        }



        
        
 }

//  Serial.println(180);  

// delay(5); 
// myservo3.write(75);   // 告訴 Neck servo 走到 'pos' 的位置
// delay(15); 
//  myservo3.write(130);   // 告訴 Neck servo 走到 'pos' 的位置
//  Serial.println("Neck Angle");
//  Serial.println(130);  

}
