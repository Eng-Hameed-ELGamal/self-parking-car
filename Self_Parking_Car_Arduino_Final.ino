#include <Servo.h>
Servo myservo;  // create servo object to control a servo

const int EnableL = 5;
const int Left_1 = 6;        // LEFT SIDE MOTOR
const int Left_2 = 7;

const int EnableR = 10;
const int Right_1 = 8;        //RIGHT SIDE MOTOR
const int Right_2 = 9;

const int D0 = 19;        //Raspberry pin 13    LSB
const int D1 = 18;        //Raspberry pin 6
const int D2 = 17;        //Raspberry pin 14
const int D3 = 16;        //Raspberry pin 10    MSB

int a, b, c, d, data;



// Default 125
#define right1_angle  140   //  while moving    (small angle)
#define left1_angle   110   //  while moving    (small angle)
#define default_angle 125   // Straight Angle

#define Moving_Speed  140   // Moving speed for motors
#define Parking_Speed 140


// defines ultrasonic pins numbers
const int trigPin = 2;
const int echoPin = 3;

// defines ultrasonic variables
long duration;
int distance;



void setup() {

  /*  Serial Communication -- To debug using Laptop */
  Serial.begin(9600); // Set the baud rate to 9600

  // 	Define Motors Pins
  pinMode(EnableL, OUTPUT);
  pinMode(Left_1, OUTPUT);     //Setting Left Motor pins to OUTPUT
  pinMode(Left_2, OUTPUT);

  pinMode(EnableR, OUTPUT);
  pinMode(Right_1, OUTPUT);   //Setting Right Motor pins to OUTPUT
  pinMode(Right_2, OUTPUT);

  //Define the moving speed on the PWM pins
  analogWrite(EnableL, Moving_Speed);
  analogWrite(EnableR, Moving_Speed);  
  
  // Define Raspberry pi input pins
  pinMode(D0, INPUT_PULLUP);
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);

  myservo.attach(11);  // attaches the servo on pin 11 to the servo object


  //	Define Ultrasonic Pins
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input


}

void Data()
{
  a = digitalRead(D0);  //Store in these varaibles (0 or 1)
  b = digitalRead(D1);
  c = digitalRead(D2);
  d = digitalRead(D3);

  data = 8 * d + 4 * c + 2 * b + a;  // Convert to decimal value
}

// Move Forward
void Forward()
{
  digitalWrite(Left_1, LOW);     // Set pin 1 of left motor to low
  digitalWrite(Left_2, HIGH);    // Set pin 2 of left motor to high
  //analogWrite(EnableL, 150);

  digitalWrite(Right_1, LOW);    // Set pin 1 of right motor to low
  digitalWrite(Right_2, HIGH);   // Set pin 2 of right motor to high
  //analogWrite(EnableR, 150);


  myservo.write(default_angle);  // Make the servo Straight

}

// Move Backward
void Backward()
{
  digitalWrite(Left_1, HIGH);    // Set pin 1 of left motor to high
  digitalWrite(Left_2, LOW);     // Set pin 2 of left motor to low 
  //analogWrite(EnableL, 150);

  digitalWrite(Right_1, HIGH);  // Set pin 1 of right motor to high
  digitalWrite(Right_2, LOW);   // Set pin 2 of left motor to low
  //analogWrite(EnableR, 150);

  myservo.write(default_angle); // Make the servo Straight


}

// Stop all + return the servo to original position
void Stop()
{
    //set pin1 and pin2 to low for left motor
  digitalWrite(Left_1, LOW);     
  digitalWrite(Left_2, LOW);
  //analogWrite(EnableL, 0);
  
    // Set pin 1 of left motor to high
  digitalWrite(Right_1, LOW);
  digitalWrite(Right_2, LOW);
  //analogWrite(EnableR, 0);

  myservo.write(default_angle);
}

//  trun servo left + move forward
void Left1()
{
  digitalWrite(Left_1, LOW);
  digitalWrite(Left_2, HIGH);
  //analogWrite(EnableL, 150);

  digitalWrite(Right_1, LOW);
  digitalWrite(Right_2, HIGH);
  //analogWrite(EnableR, 150);

  myservo.write(left1_angle); // Make the servo Straight
}


//  trun servo right + move forward
void Right1()
{
  digitalWrite(Left_1, LOW);    // Set pin1 of left motor to low
  digitalWrite(Left_2, HIGH);   // Set pin2 of left motor to high
  //analogWrite(EnableL, 150);

  digitalWrite(Right_1, LOW);   // Set pin1 of right motor to low
  digitalWrite(Right_2, HIGH);  // Set pin1 of right motor to high
  //analogWrite(EnableR, 150); //200

  myservo.write(right1_angle);  // turn servo right
}

void ultrasonic_update_distance()
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
//  Serial.print("Distance = ");
  Serial.println("scanning");
}


void normal_operation()
{
  Data(); // Converts the output of the raspi to decimal
    if(data==0)
    {
//    Serial.println("Forward Order Received");
     Forward();
    }

    else if(data==1)
    {
//    Serial.println("Right1 Order Received");
     Right1();
    }


    else if(data==2)
    {
//    Serial.println("Left1 Order Received");
     Left1();
    }
}


void Parking ()
{
    Forward();
    delay(1250);    // move forward for 1.25s 
    Stop();
    delay(1000);    


    //  angle to right
    myservo.write(100);
    delay(1000);

    //  backward
    digitalWrite(Left_1, HIGH);
    digitalWrite(Left_2, LOW);
    analogWrite(EnableL, 200);  //Set left motor speed to 200

    digitalWrite(Right_1, HIGH);
    digitalWrite(Right_2, LOW);
    analogWrite(EnableR, 200);  //Set right motor speed to 200

    delay(1100);
    Stop();

    myservo.write(150);     // Make servo turn right
    delay(1000);

    //  backward
    digitalWrite(Left_1, HIGH);
    digitalWrite(Left_2, LOW);
    analogWrite(EnableL, 200);

    digitalWrite(Right_1, HIGH);
    digitalWrite(Right_2, LOW);
    analogWrite(EnableR, 200);  //Set right motor speed to 200

    delay(900);
    Stop();

    Forward();
    delay(200);
    Stop();
  
}




bool Park_spot_scan = 0; 
int initial_time = 0;
int Current_time = 0;

void loop()
{  
  ultrasonic_update_distance();
  if (distance < 30) // for obstacles
  {
    normal_operation();  // Keep detecting the lane
//    Serial.println("move");Checks
    Park_spot_scan = 0;
  }

  
  else if (distance > 30 && !Park_spot_scan) // If an empty place spotted
  {
    
    initial_time = millis(); 
    Park_spot_scan = 1;
    
  }
  
  if(Park_spot_scan && distance > 30){  // Checks the width of the parking spot
     normal_operation();
     Current_time = millis();
     if (Current_time - initial_time >= 600){
      Serial.println("Scanning for parking");}
     }
     if (Current_time - initial_time >= 1200){  // Checks if the width is suitable
      Stop();
      Serial.println("Parking Spot Found");
      Parking();
      while (1)
      {
        Serial.println("stop");
      }
     }
  }
 
