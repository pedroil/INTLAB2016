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

                       int maxSteps = 1850;

//////////////////////////////////////////////////////////////////
///////////// rango máximo de pasos para el motor ////////////////
//////////////////////////////////////////////////////////////////

boolean test = false;

// Define a stepper and the pins ~ 1 EN+ , 9 CLK+ , 8 CW+ 
AccelStepper stepper(1, 9, 8);

// Define an ultrasonic sensor ~ (Trig PIN 5,Echo PIN 6)
Ultrasonic ultrasonic(5, 6, 10000); 

/* Variables de suavizado */
long soft;
int b = 200;   // estos 2 numeros deben ser iguales
int buf[200];  // buffer de suavizado
int count;

void setup() {
  // Stepper
  stepper.setSpeed(200);
  stepper.setMaxSpeed(300);
  stepper.setAcceleration(50);
 // stepper.direction(1);

  // UltraSound
  pinMode(4, OUTPUT); // VCC pin
  pinMode(7, OUTPUT); // GND ping
  digitalWrite(4, HIGH); // VCC +5V mode
  digitalWrite(7, LOW);  // GND mode

  // if test init serial
  if (test) Serial.begin(115200);

  count = 0;
}

void loop() {
  // Read new position
  int distance = ultrasonic.Ranging(CM);

    buf[count] = distance;
    count ++;
    count %= b;
  long val = 0;
  
  for (int i = 0; i < b; i++) {
    val += buf[i];
  }
  
  soft = val / b;
  
  // int steps = round(map(distance, 0, 172, maxSteps, 0));
  
  int steps = round(map(soft, 10, 150, maxSteps, 0));
  steps = constrain(steps, 0, maxSteps);

  // Set stepper to mapped steps
  stepper.moveTo(steps);
  stepper.runSpeedToPosition();

  // if test then print values to serial
  if (test) {
    Serial.print(soft);
    Serial.print("cm   ");
    Serial.println(steps);
  }
}
