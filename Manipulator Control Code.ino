#include <Servo.h>
#include <math.h>

Servo servo9; 
Servo servo10;
Servo servo11;

int joystickXPin = A0; 
int joystickYPin = A1; 
int buttonPin = 0;

void setup() {
  Serial.begin(9600);
  servo9.attach(9);   
  servo10.attach(10);   
  servo11.attach(11);   

  pinMode(buttonPin, INPUT);  // button input on joystick
  digitalWrite(buttonPin, HIGH);  // button unclicked is set as a HIGH signal
}

void loop() {
 
  int buttonState = digitalRead(buttonPin);
  int xValue = analogRead(joystickXPin); // Read X-axis value
  int yValue = analogRead(joystickYPin); // Read Y-axis value

  double angle = (atan2(xValue - 512, yValue - 512) * (180 / PI)); //calculates angle on joystick based on X and Y readings (-180 to 180)
  double absX = abs(map(xValue, 0, 1023, 100, -100));
  double absY = abs(map(yValue, 0, 1023, -100, 100));
  double force = sqrt(absX*absX + absY*absY); //calculates force of joystick input, how much it is pushed from the center

  force = constrain(force, 0, 100); //displays force as range 0 to 100
    
  if (force < 20) { //if the force is lower than 20% stay in this mode (keeps it from being twitchy from noise when no input)

    force = 0;
    angle = 0;

    if (buttonState == HIGH) { // if the button is not pressed get the motors to not pull any cables
    
    servo9.write(0);  // servo at 0 degrees
    servo10.write(0);  // servo at 0 degrees
    servo11.write(127); // servo at 270 degrees bc it's flipped in the assembly, is the equivalent 0 (these servos have 270 degrees of motion, input of 127 represents it)
    }

    else if (buttonState == LOW) { // if the button is pressed get the motors to pull the cables to their completely closed positions
    
    servo9.write(127); // servo at 270 degrees, fully pulls cable 1
    servo10.write(127); // servo at 270 degrees, fully pulls cable 2
    servo11.write(0); // servo at 0 degrees, fully pulls cable 3
    }

  }

  Serial.println(force); // writes force in serial monitor
  Serial.println(angle); // writes angle in serial monitor

  // the rest is for if the force is greater than 20%

  if (angle > 0 && angle < 60) { //region 1
   
    servo9.write(0);
    servo10.write((force/100)*127); 
    servo11.write((map(angle, 0, 60, 0, 127)));

    //servo9.write(0);
    //servo10.write(0);
  }

  if (angle > 60 && angle < 120) { //region 2

    servo9.write((map(angle, 60, 120, 0, 127)));
    servo10.write((force/100)*127); 
    servo11.write(127);

  }

  if (angle > 120 && angle < 180) { //region 3

    servo9.write((force/100)*127);
    servo10.write((map(angle, 120, 180, 127, 0))); 
    servo11.write(127);

  }

  if (angle < -120 && angle > -180) { //region 4
    
    servo9.write((force/100)*127); 
    servo10.write(0);
    servo11.write((map(angle, -120, -180, 0, 127))); 

  } 

  if (angle < -60 && angle > -120) { //region 5

    servo9.write((map(angle, -60, -120, 0, 127))); 
    servo10.write(0);
    servo11.write(127-(force/100)*127); 

  } 

  if (angle < 0 && angle > -60) { //region 6
    
    servo9.write(0); 
    servo10.write((map(angle, 0, -60, 127, 0)));
    servo11.write(127-(force/100)*127); 

  } 
  Serial.println(force);
  delay(10); 
}
