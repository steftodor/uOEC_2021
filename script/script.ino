#include <Servo.h>

//// pins
int left_Forward_pin = 9;
int left_Backward_pin = 10;
int right_Forward_pin = 11;
int right_Backward_pin = 12;
int pingPin = 3; // Trigger Pin of Ultrasonic Sensor
int echoPin = 2; // Echo Pin of Ultrasonic Sensor
int servo_pin = 5;
// variables
int moisture_detected = 0;
float moisture_Value = 0; 
int arm_angle = 10;



#define moisture_Sensor A0 
#define left_Forward left_Forward_pin
#define left_Backward left_Backward_pin
#define right_Forward right_Forward_pin
#define right_Backward right_Backward_pin
#define servoPin servo_pin

Servo arm_servo;

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

float analyze_moisture(){
  moisture_Value = analogRead(moisture_Sensor);
  if (moisture_Value > 100){
    moisture_detected = moisture_detected + 1;
    Serial.println("Moisture Found");
  }else
  {
    Serial.println("No Moisture");
  }
  
  return moisture_Value;
  }


void setup() {
     pinMode(left_Forward_pin, OUTPUT);
  pinMode(left_Backward_pin, OUTPUT);
   pinMode(right_Forward_pin, OUTPUT);
    pinMode(right_Backward_pin, OUTPUT);
    pinMode(pingPin, OUTPUT);
    pinMode(echoPin, INPUT);
    
 Serial.begin(9600);
 Serial.setTimeout(1);
  Serial.println("STARTUP");
    arm_servo.attach(servoPin);
     arm_servo.write(arm_angle);

}

void loop() {
  if (Serial.available()) {
    char serial_in = (char) Serial.read();
    // measure moisture 
    if(serial_in == 'm'){
      Serial.println("measure moisture: ");
       Serial.println(analyze_moisture());
    }
    // measure distance
    else if(serial_in == 'o'){
      Serial.println("measure distance: ");
      long duration, inches, cm;
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   cm = microsecondsToCentimeters(duration);
   Serial.print(cm);
   Serial.println("cm");
    }
    // emergency stop
    else if(serial_in == 'p'){
      Serial.println("stop");
            digitalWrite(right_Forward_pin, LOW);
      digitalWrite(left_Forward_pin, LOW);
            digitalWrite(right_Backward_pin, LOW);
       digitalWrite(left_Backward_pin, LOW);
    }
    // move forward
        else if(serial_in == 'w'){
      Serial.println("forward");
       digitalWrite(right_Forward_pin, HIGH);
       digitalWrite(left_Forward_pin, HIGH);
      delay(1000);
      digitalWrite(right_Forward_pin, LOW);
      digitalWrite(left_Forward_pin, LOW);
    }
    // move backwards
     else if(serial_in == 's'){
      Serial.println("backward");
      digitalWrite(right_Backward_pin, HIGH);
       digitalWrite(left_Backward_pin, HIGH);
      delay(1000);
      digitalWrite(right_Backward_pin, LOW);
       digitalWrite(left_Backward_pin, LOW);
    }
    // move left
     else if(serial_in == 'a'){
            Serial.println("left");
      digitalWrite(right_Forward_pin, HIGH);
       digitalWrite(left_Backward_pin, HIGH);
      delay(1000);
      digitalWrite(right_Forward_pin, LOW);
       digitalWrite(left_Backward_pin, LOW);
    }

    // move right
     else if(serial_in == 'd'){
      Serial.println("right");
            digitalWrite(right_Backward_pin, HIGH);
       digitalWrite(left_Forward_pin, HIGH);
      delay(1000);
      digitalWrite(right_Backward_pin, LOW);
       digitalWrite(left_Forward_pin, LOW);
    }
    // return total moisture detections
else if(serial_in == 'n'){
      Serial.print("moisture_detected_total: ");
     Serial.println(moisture_detected);
    delay(10);
  }
  // move arm up
  else if(serial_in == 'u'){
           Serial.print("MOVE ARM UP ANGLE: ");
        Serial.println(arm_angle);
    arm_angle = arm_angle -= 10;
     arm_servo.write(arm_angle);
  }
    // move arm down
  else if(serial_in == 'j'){
       Serial.print("MOVE ARM DOWN ANGLE: ");
        Serial.println(arm_angle);
     arm_angle = arm_angle += 10;
      arm_servo.write(arm_angle);
  }
  }
}
