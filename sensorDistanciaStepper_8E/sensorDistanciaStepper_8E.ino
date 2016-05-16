/*
 * 
 * vuelve al home cada 100000 loops
 * 
 */

/* Variables de suavizado */
long soft;
int b = 99;   // estos 2 numeros deben ser iguales
int buf[99];  // buffer de suavizado

int x = 0;

/* variables para determinar actividad */
int frameCount = 0;

/* Fin de carrera */
int Interrupcion = 0; // Define a la Interrupción 0 - boton de home
float usDelay = 500; // Tiempo para la señal que da los pasos al driver del stepper
int  Home = 1;
int  HomeLedPin = 3;
int  pinBoton = 2; // El Pin 2  corresponde al botón


int ContPasosRiel = 0;
int pasos;

int maxRangUson = 250 ; // Maximo rango de distancia de sensor ultrasonido
int minRangUson = 1; // Minimo rango de distancia de sensor ultrasonido
int setPoint = 0;

long duracion, distancia; // Duration used to calculate distance
int count;
int numero = 0;

int distUsonScal;

/* Pines usados para sensor de distancia */
#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin
#define LEDPin 9  // Onboard LED 13

/* Pines usados para control de driver stepper motor */
#define DIR_PIN 12 // CW
#define STEP_PIN 11 // CLK
#define EN_PIN 13

/* Variables usadas en el movimiento del stepper motor */
#define banda 5         // Banda de tolerancia entre medidas
#define minPasosRiel 0    // Minimo numero de pasos del riel
#define maxPasosRiel 3900  // Maximo numero de pasos del riel

void setup() {
  Serial.begin (115200);

  /* Configuración de pines como entrada o salida */

  /* pines driver stepper motor */
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, HIGH);

  /* Pines sensor de distancia */
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)

  pinMode(HomeLedPin, OUTPUT);
  /* Pin Botón */
  pinMode(pinBoton, INPUT);  //El pin conectado al botón se configura como entrada
  digitalWrite(pinBoton, HIGH ); // Activación de la resistencia interna pull-up
  attachInterrupt(Interrupcion, cambios, LOW); // la interrupción0 se genera cuando el pin 2 posee un valor HIGH y ejecuta la subrutina "cambios".

  /* Inicialización de variables usadas en el sensor de distancia */

  for (int i = 0; i < b; i++) {
    /* llenar el buffer de distancias sensadas con ceros */
    buf[i] = 0;
  }

  //
  count = 0;
  soft = 0;

  GoToHome();
  x = 0;
}

void loop() {

  readDistance();


  /* si es que está fuera del rango del ultrasonido */
  if (distancia >= maxRangUson || distancia <= minRangUson) {
    distancia = -1;
    digitalWrite(LEDPin, HIGH);
  }
  else {
    /* Publicar en monitor serial el valor de distancia */
    //Serial.println(distancia);
    /* apagar el led que indica distancia fuera de rango */
    digitalWrite(LEDPin, LOW);
  }

  calcSoft();

  if (ContPasosRiel != setPoint) {
    //opcion de codigo 1: se desplaza el total de pasos entre ContPosRiel y setPoint
    pasos = setPoint - ContPasosRiel;
    rotate(pasos);
    ContPasosRiel = ContPasosRiel + pasos;
  }

  setReferencia(); // Leer el parámetro de distancia desde el sensor de ultrasonido (SetPoint)

  /* Delay de 30ms antes de la siguiente lectura de la variable "soft" */
  delay(1);
  frameCount++;

  if (frameCount % 100000 == 0) {
    setPoint = 0;
  }
}





