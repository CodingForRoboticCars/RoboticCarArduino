/* PLEASE READ!
 * Description: This code allows a second car in a line of two to follow the first car along a route determined by the layout of tape on the surface, 
 * as car one is line tracking. Car two senses car one using an ultrasonic, going forward until the front distance becomes too large, then car one has been "lost" and car two will 
 * look at about 30˚ on each side and takes measurements. It will then make a slight turn in whichever direction the values were measured as smaller.
 * 
 * This code requires two robotics cars, an ultrasonic sensor, colored tape, and line tracking modules.
 * Car 1 (lead car) should be equipped with the line tracking modules. Car 2 should be equipped with the ultrasonic.
 * 
 */



#include <Servo.h> //servo library
Servo myservo; // create servo object to control servo
int Echo = A4; //controls the ultrasonic
int Trig = A5; //controls the ultrasoic
int in1 = 9;  
int in2 = 8;
//motors must be set to these four pins
int in3 = 7;
int in4 = 6;
int ENA = 11; //controls the right motors
int ENB = 5; //controls the left motors
int ABS = 120;  //controls the speed of the car by voltage
                //car usually won't move below 80
int middleDistance = 0;  //where the front distance is recorded
int rDist, lDist; //left and right distances

void _mForward() 
//the function that tells car one to move forward
{
 analogWrite(ENA,ABS); //ABS is the voltage supplied
 //ENA (set to pin 11) controls the right motors
 analogWrite(ENB,ABS); //ABS is the voltage supplied
 //ENB (set to pin 5) controls the left motors
 digitalWrite(in1,LOW);
 digitalWrite(in2,HIGH);
 digitalWrite(in3,LOW);
 digitalWrite(in4,HIGH);
 Serial.println("Go forward!");
 //serial printout viewable in serial monitor
}

void _mleft() //function to turn car left (delay time for turn varies)
{
 analogWrite(ENA,135);  //voltage sent to motors is higher so turns are faster
 analogWrite(ENB,135);
 digitalWrite(in1,LOW);
 digitalWrite(in2,HIGH);
 digitalWrite(in3,HIGH);
 digitalWrite(in4,LOW);
 Serial.println("veer left!");
 //serial printout viewable in serial monitor
}

void _mright() //function to turn car right (delay time for turn varies)
{
 analogWrite(ENA,135);  //voltage sent to motors is higher so turns are faster
 analogWrite(ENB,135);
 digitalWrite(in1,HIGH);
 digitalWrite(in2,LOW);
 digitalWrite(in3,LOW);
 digitalWrite(in4,HIGH);
 Serial.println("veer right!");
 //serial printout viewable in serial monitor
} 

void _mStop() //function to stop car indefinately until other command
{
  digitalWrite(ENA,LOW);  //sets motor voltage to lowest possible
  digitalWrite(ENB,LOW);
  Serial.println("Stop!");
  //serial printout viewable in serial monitor
} 

//For Ultrasonic Distance Measurements:
int Distance_test()   
{
    digitalWrite(Trig, LOW);   
            //sets up the sensor
    delayMicroseconds(2);
    digitalWrite(Trig, HIGH);  
    delayMicroseconds(20);
    digitalWrite(Trig, LOW);   
    float Fdistance = pulseIn(Echo, HIGH);  
            //receives ultrasonic waves
    Fdistance= Fdistance/58;    
            //converts to centimeters
    return (int)Fdistance;      
            //returns distance as integer in cm
}  

void setup() //executes before every run
{ 
  myservo.attach(3);// attach servo on pin 3 to servo object
  Serial.begin(9600);     
  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT);  
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  _mStop();
} 

void loop() 
{ 
    myservo.write(90);//setservo position according to scaled value  
    middleDistance = Distance_test(); //records front distance

    delay(300);

    Serial.print("middleDistance="); //prints reading for test purposes
    Serial.println(middleDistance);
    
    if (middleDistance < 100 && middleDistance > 30)
    {
      _mForward(); //distance is within range so car 1 is still there
    }
    else if (middleDistance >= 100) //car 1 is no longer in front
    {
      _mStop(); //stops to take readings
      //look right and look left because the car has gone out of view
      myservo.write(125); //slight left turn of servo and ultrasonic
      delay(90); //delay so servo can turn all the way
      lDist = Distance_test();
      myservo.write(55); //slight right turn of servo and ultrasonic
      delay(110);
      rDist = Distance_test();//within d test we could take 2 and return only the 2nd val if the 2 are within reasonable amt
      Serial.print("lDistance: ");
      Serial.println(lDist);   //prints readings for testing purposes
      Serial.print("rDistance: ");
      Serial.println(rDist);
      if (rDist < lDist && rDist < 150) //car must be on right because distance is shorter
      {
        _mright();
        delay(5); //delay time for turn is short because voltage is high
      }
      else if (lDist < rDist && lDist < 150) //car must be on left because distance is shorter
      {
        _mleft();
        delay(5); //delay time for turn is short because voltage is high
      }
    }
    else if (middleDistance < 30) //car or other object got too close
    {
      _mStop();  //stop so that car two doesn't run into anything
      delay(150); 
    }
}
   

