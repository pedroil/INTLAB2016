/*

  Esqueleto de programa

*/


void setup() {

}

void loop() {
  if (init) {
    initFunction();
  }

  checkActivity();

  if (quiet) {
    goHome();
  } else {
    readDistance();
    mapDistance();
    goTo(mappedDistance);
  }
}
