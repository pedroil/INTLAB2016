/*
 * 
 *  funciones 
 * 
 */
 
void initFuntion(){
  
}

void calcSoft() {
  
  // add value only if in range
  if (distance != -1) {
    distanceBuffer[count] = distance;
    count ++;
    count %= distanceBuffer.length;
  }
  
  // calculate de average of all values
  long val = 0;
  for (int i = 0; i < b; i++) {
    val += distanceBuffer[i];
  }
  soft = val / distanceBuffer.length;
}

void goHome(){
  
}

boolean checkActivity(){
  
}

void readDistance(){
  
}

void mapDistance(){
  
}

void goTo(int d){
  
}

