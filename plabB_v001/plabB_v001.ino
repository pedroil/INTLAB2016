/*
    Plan B
    Move stepper motor according to distance read
    from ultrasound
*/

#include <AccelStepper.h>
#include <Ultrasonic.h>

//////////////////////////////////////////////////////////////////
/////////////// el número que hay que modificar //////////////////
//////////////////////////////////////////////////////////////////

                       int maxSteps = 200;

//////////////////////////////////////////////////////////////////
///////////// rango máximo de pasos para el motor ////////////////
//////////////////////////////////////////////////////////////////

boolean test = false;

// Define a stepper and the pins ~ 1 EN+ , 9 CLK+ , 8 CW+ 
AccelStepper stepper(1, 9, 8);

// Define an ultrasonic sensor ~ (Trig PIN 5,Echo PIN 6)
Ultrasonic ultrasonic(5, 6, 10000); 


void setup() {
  // Stepper
  stepper.setSpeed(2000);
  stepper.setMaxSpeed(4000);
  stepper.setAcceleration(1000);

  // UltraSound
  pinMode(4, OUTPUT); // VCC pin
  pinMode(7, OUTPUT); // GND ping
  digitalWrite(4, HIGH); // VCC +5V mode
  digitalWrite(7, LOW);  // GND mode

  // if test init serial
  if (test) Serial.begin(115200);
}

void loop() {
  // Read new position
  int distance = ultrasonic.Ranging(CM);
  int steps = round(map(distance, 0, 172, maxSteps, 0));
  steps = constrain(steps, 0, maxSteps);

  // Set stepper to mapped steps
  stepper.moveTo(steps);
  stepper.runSpeedToPosition();

  // if test then print values to serial
  if (test) {
    Serial.print(distance);
    Serial.print("cm   ");
    Serial.println(steps);
  }
}
