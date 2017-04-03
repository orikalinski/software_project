#include <stdio.h>

#include<stdlib.h>

void readCoordinates(int dim, double* coordinates){
  int i=0;
  while (i < dim && scanf("%lf", &coordinates[i]) == 1)
    i++;

}





