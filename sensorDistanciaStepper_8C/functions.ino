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

  distUsonScal = map(soft, minRangUson, maxRangUson, maxPasosRiel, minPasosRiel); // Escalando medición de distancia de sensor ultrasonido a la escala del sensro IR del riel

  if (distUsonScal >= minPasosRiel && distUsonScal <= maxPasosRiel)  setPoint = distUsonScal;
}

void GoToHome() {
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


void cambios() {
  Home = 0;
  //digitalWrite(HomeLedPin, HIGH);   // encender o apagar el LED
  detachInterrupt(Interrupcion);
}

void reset() {
  /* Fin de carrera */
  Interrupcion = 0; // Define a la Interrupción 0 - boton de home
  usDelay = 500; // Tiempo para la señal que da los pasos al driver del stepper
  Home = 1;
  HomeLedPin = 3;
  pinBoton = 2; // El Pin 2  corresponde al botón
}

