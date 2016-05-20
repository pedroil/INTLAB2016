/*
 *  Plan B
 *  Move stepper motor according to distance read
 *  from ultrasound
 */

//////////////////////////////////////////////////////////////////
/////////////// el número que hay que modificar //////////////////
//////////////////////////////////////////////////////////////////

                      int maxSteps = 200; 

//////////////////////////////////////////////////////////////////
///////////// rango máximo de pasos para el motor ////////////////
//////////////////////////////////////////////////////////////////

#include <AccelStepper.h>
#include <Ultrasonic.h>

// Define a stepper and the pins it will use
// 1 EN+ , 9 CLK+ , 8 CW+  AccelStepper stepper(1, 9, 8);
AccelStepper stepper(1, 9, 8);
Ultrasonic ultrasonic(5, 6, 10000); // (Trig PIN 5,Echo PIN 6)

// This defines the analog input pin for reading the control voltage
// Tested with a 10k linear pot between 5v and GND
#define ANALOG_IN A0

boolean test = false;

void setup()
{
  // Stepper
  stepper.setSpeed(2000);
  stepper.setMaxSpeed(4000);
  stepper.setAcceleration(1000);
  
  // UltraSound
  if (test) Serial.begin(115200);
  pinMode(4, OUTPUT); // VCC pin
  pinMode(7, OUTPUT); // GND ping
  digitalWrite(4, HIGH); // VCC +5V mode
  digitalWrite(7, LOW);  // GND mode
}

void loop()
{
  // Read new position
  int distance = ultrasonic.Ranging(CM);
  int steps = round(map(distance, 0, 172, maxSteps, 0));
  steps = constrain(steps, 0, maxSteps);

  // Set stepper to mapped steps
  stepper.moveTo(steps);
  stepper.runSpeedToPosition();

  if (test) {
    Serial.print(distance);
    Serial.print("cm   ");
    Serial.println(steps);
  }
}
