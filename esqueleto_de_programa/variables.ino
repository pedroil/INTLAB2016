/*
 * 
 *  variables
 * 
 */


boolean init = true; // sólo al inicio para ir al home
int distance, softDistance, mappedDistance; // ditancia real, suavizada y mapeada
int distanceBuffer[99]; // buffer de suavizado de la distancia
int count = 0; // contador útil
