/* Variables de suavizado */
long soft;
int b = 99;   // estos 2 numeros deben ser iguales
int buf[99]; // buffer de suavizado

int x = 0;

/* Fin de carrera */
int Interrupcion = 0; // Define a la Interrupción 0 - boton de home
float usDelay = 500; // Tiempo para la señal que da los pasos al driver del stepper
int  Home = 1;
int  HomeLedPin = 3;
int  pinBoton = 2; // El Pin 2  corresponde al botón

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
int ContPasosRiel = 0;
int pasos;

int maxRangUson = 300 ; // Maximo rango de distancia de sensor ultrasonido
int minRangUson = 2; // Minimo rango de distancia de sensor ultrasonido
int setPoint = 0;

long duracion, distancia; // Duration used to calculate distance
int count;
int numero = 0;


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
  count = 0;
  soft = 0;

  GoToHome();
  x = 0;
}

void loop() {

  /* Enviar señal de ultrasonido y leer el rebote */
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  /* leer el valor de tiempo entregado por el sensor de distancia (valor entre el envío y el recibo del rebote dividido en 2) */

  duracion = pulseIn(echoPin, HIGH);

  /* con el tiempo de duración y la velocidad del sonido (constante) se calcula la distancia (52.7) */

  distancia = duracion / 52.7;

  /* Consideramos los límites de medición del sensor */
  /* Si el valor de distancia se encuentra fuera de los límites */
  /* no se realiza cálculo. Si el valor se encuentra dentro de los límites */
  /* se incluye este valor de distancia en las rutinas de calculo */

  if (distancia >= maxRangUson || distancia <= minRangUson) {
    /* Send a negative number to computer and Turn LED ON
      to indicate "out of range" */
    // Serial.println("-1");
    distancia = -1;
    digitalWrite(LEDPin, HIGH);
    GoToHome();

  }
  else {
    /* Publicar en monitor serial el valor de distancia */
    //Serial.println(distancia);
    /* apagar el led que indica distancia fuera de rango */
    digitalWrite(LEDPin, LOW);
  }

  calcSoft(); // Rutina de calculo de valor de distancia promediado
  //Serial.println(ContPasosRiel);

  Serial.print("ContPasosRiel: ");
  Serial.println(ContPasosRiel);

  if (ContPasosRiel < setPoint) {

    //opcion de codigo 1: se desplaza el total de pasos entre ContPosRiel y setPoint
    pasos = setPoint - ContPasosRiel;
    rotate(pasos);
    ContPasosRiel = ContPasosRiel + pasos;
    Serial.print("ContPasosRiel: ");
    Serial.println(ContPasosRiel);

    // opcion de codigo 2: Se desplaza de a 1 paso a la vez hasta que ContPosRiel=setPoint
    //rotate(1);  //Valor de pasos con signo negativo indicando dirección
    //ContPasosRiel ++;

  }

  if (ContPasosRiel > setPoint) {
    // opcion de codigo 1: se desplaza el total de pasos entre ContPosRiel y setPoint
    pasos = setPoint - ContPasosRiel;
    rotate(pasos);
    ContPasosRiel = ContPasosRiel + pasos;
    Serial.print("ContPasosRiel: ");
    Serial.println(ContPasosRiel);

    // opcion de codigo 2: Se desplaza de a 1 paso a la vez hasta que ContPosRiel=setPoint
    //rotate(-1);  //Valor de pasos con signo negativo indicando dirección
    //ContPasosRiel --;

  }


  /* Imprimir en terminal serial posición */
  //Serial.println(ContPasosRiel);
  //Serial.println(setPoint);
  setReferencia(); // Leer el parámetro de distancia desde el sensor de ultrasonido (SetPoint)

  /* Delay de 30ms antes de la siguiente lectura de la variable "soft" */
  delay(1);
}

/* Subrutina de calculo de suavizado */

void calcSoft() {
  if (distancia != -1) {
    buf[count] = distancia;
    count ++;
    count %= b;
  }

  long val = 0;
  for (int i = 0; i < b; i++) {
    val += buf[i];
  }
  soft = val / b;
  //Serial.println(soft);
}


/* Subrutina para el control de los pines del driver stepper motor */

/* Giro a la derecha o hacia la izquierda*/
void rotate(int steps) {
  int dir = (steps > 0) ? HIGH : LOW;
  Serial.print("dir: ");
  Serial.println(dir);
  steps = abs(steps);
  //Serial.println(steps);
  /* Control de pines "ENABLE", "DIRECCIÖN", "PASOS" */
  digitalWrite(DIR_PIN, dir);

  digitalWrite(EN_PIN, LOW);

  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(usDelay);

    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(usDelay);

    if (dir == 1) {
      x ++;
    }
    if (dir == 0) {
      x --;
    }
    Serial.println(x);
  }
  digitalWrite(EN_PIN, HIGH);

}

/*  */
void setReferencia() {

  int distUsonScal = map(soft, minRangUson, maxRangUson, maxPasosRiel, minPasosRiel); // Escalando medición de distancia de sensor ultrasonido a la escala del sensro IR del riel

  if (distUsonScal >= minPasosRiel && distUsonScal <= maxPasosRiel)  setPoint = distUsonScal;
  //Serial.print("-----");
  Serial.print("setPoint: ");
  Serial.println(setPoint);

}

void GoToHome() {
  // interrupción
  while (Home) {
    if (Home = 1) {
      rotate(-1);
    }
    else {
      Home = 0;
      ContPasosRiel = 0;
      break;
    }
  }
  return;
}


void cambios()
{
  Home = 0;
  //digitalWrite(HomeLedPin, HIGH);   // encender o apagar el LED
  detachInterrupt(Interrupcion);
}



