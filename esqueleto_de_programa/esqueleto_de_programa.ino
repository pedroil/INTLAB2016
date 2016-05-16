/*
 * 
 * Esqueleto
 * 
 */


void setup() {

}

void loop() {
  if (init) {
    initFunction();
  }

  if (checkActivity()) {
    goHome();
  } else {
    readDistance();
    mapDistance();
    goTo(mappedDistance);
    printValues();
  }
}
