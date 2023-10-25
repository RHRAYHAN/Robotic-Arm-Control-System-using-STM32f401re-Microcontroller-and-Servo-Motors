#include <stdio.h>
#include <stdlib.h>

int main()
{
int num, x;
int y = 0;
int z = 0;

printf (" enter the number: \n ");
scanf ("%d", &num);

for (x=1;x<=num; x++){

y = (x*x) + z;
z=y;

}
printf ("the perfect square this is %d : "3, z);

}

