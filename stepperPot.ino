// ProportionalControl.pde
// -*- mode: C++ -*-
//
// Make a single stepper follow the analog value read from a pot or whatever
// The stepper will move at a constant speed to each newly set posiiton, 
// depending on the value of the pot.
//
// Copyright (C) 2012 Mike McCauley
// $Id: ProportionalControl.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

// Define a stepper and the pins it will use
// 1 EN+ , 9 CLK+ , 8 CW+  AccelStepper stepper(1, 9, 8); 
AccelStepper stepper(1, 9, 8); 


// This defines the analog input pin for reading the control voltage
// Tested with a 10k linear pot between 5v and GND
#define ANALOG_IN A0

void setup()
{ 
  stepper.setSpeed(1000);
  stepper.setMaxSpeed(3000);
  stepper.setAcceleration(1000);
 
}

void loop()
{
  // Read new position
  int analog_in = analogRead(ANALOG_IN);
  stepper.moveTo((analog_in)*4);
  stepper.runSpeedToPosition();
  delayMicroseconds(100);
}
