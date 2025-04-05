#include <stdio.h>

int main(int argc, char *argv[]){
char nombre[5][45]; int i;
for(i=0;i<5;i++)
{
    printf("introducir el nombre de k[%d]",i);
    scanf("%s",&nombre[i]);
}
for(i=4;i>=0;i--)
    printf("Los nombres han sido %s /n",nombre[i]);

return 1;
}