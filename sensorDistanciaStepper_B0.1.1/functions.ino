/*

    functions

*/

void readDistance() {
  /* Enviar señal de ultrasonido y leer el rebote */
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  /* leer el valor de tiempo entregado por el sensor de distancia
    (valor entre el envío y el recibo del rebote dividido en 2) */
  duracion = pulseIn(echoPin, HIGH);
  /* con el tiempo de duración y la velocidad del sonido (constante)
    se calcula la distancia (52.7) */
  distancia = duracion / 52.7;
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

/*  rutina para realizar los giros hacia la derecha o hacia la izquierda*/
void rotate(int steps) {
  // La variable "dir" pasa a estado alto o bajo, dependiendo del signo del número 
  // presente en la función rotate()
  int dir = (steps > 0) ? HIGH : LOW;
  Serial.print("dir: ");
  Serial.println(dir);
  /* El número de pasos (variable "steps")que el motor stepper va a girar corresponde
     al valor absoluto del número presente en la función rotate(). */
  steps = abs(steps);
  //Serial.println(steps);
  /* Control de pines "ENABLE", "DIRECCIÖN", "PASOS" */
  digitalWrite(DIR_PIN, dir); // le indico al driver del motor stepper qué dirección debe girar

  digitalWrite(EN_PIN, LOW); // le indico al driver del motor stepper qiue energize el motor

  for (int i = 0; i < steps; i++) { // indico al driver del motor stepper cuantos pasos debe moverse el motor
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(usDelay);

    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(usDelay);
/* Estos IF son solo para testear en la terminal serial que se están generando los pasos
   del motor cuando funciona el sensor. Esta parte se puede eliminar si no se necesita 
   publicar la posición del motor cada vez que este de un paso. */
   
/* Si la variable "dir" está en "HIGH" la variable "x" aumenta en 1 */
    if (dir == 1) {
      x ++;
    }
/* Si la variable "dir" está en "LOW" la variable "x" disminuye en 1 */
    if (dir == 0) {
      x --;
    }
    Serial.println(x);
  }
  digitalWrite(EN_PIN, HIGH); // luego de generados todos los pasos indico al driver del motor stepper que desenergize el motor

}

/* El cabezal del eje debe seguir a la posición del sensor Ultrasonido. El sensor ultrasonido es la referencia
   Aquí se mapea el sensor ultrasonido a una escala igual al del eje*/
void setReferencia() {

/* Mapeo: el valor del sensor ultrasonido se escala a un valor entre los extremos del riel.
   si se desea que el riel corra en dirección contraria se debe invertir en la función map()
   las variables maxPasosRiel y minPasos Riel o se pueden invertir los valores de ambas variables
   al inicio del programa. */
   
  distUsonScal = map(soft, minRangUson, maxRangUson, maxPasosRiel, minPasosRiel); // Escalando medición de distancia de sensor ultrasonido a la escala del sensro IR del riel

  /* Si el valor mapeado se encuentra dentro de los rangos del eje
     entonces el valor mapeado es incluido en la variable SetPoint
     (set point es la posición donde quiero que el motor posicione el cabezal en el eje)*/
  
  if (distUsonScal >= minPasosRiel && distUsonScal <= maxPasosRiel)  setPoint = distUsonScal;
}

void GoToHome() {
  while (Home) {
    if (Home = 1) { // Si la interrupción del botón no se ejecuta....
      /* "-1": el signo negativo indica la dirección de giro. El valor entero "1" indica que dará un paso. */
      rotate(-1);
    }
    else { // si se activva la interrupción del botón....
      Home = 0; // se desactiva la función GoToHome
      ContPasosRiel = 0; // se setea el contador de pasos en cero.
      break;
    }
  }
  return;
}


void cambios() {
  Home = 0;
  //digitalWrite(HomeLedPin, HIGH);   // encender o apagar el LED
  detachInterrupt(Interrupcion); // se desactiva la interrupción del botón. No se necesita más después de realizar el seteo.
}

void reset() {
  /* Fin de carrera */
  Interrupcion = 0; // Define a la Interrupción 0 - boton de home
  usDelay = 500; // Tiempo para la señal que da los pasos al driver del stepper
  Home = 1; // inicialización de la variable "Home". El valor 1 indica que la rutina GoToHome parte sin 
            // la activación del interruptor
  HomeLedPin = 3; // este es solo para conectar un led de prueba en el pin 3 si el cabezal llega al Home. No se está usando.
  pinBoton = 2; // El Pin 2  corresponde al botón
}

