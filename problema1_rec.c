#include <stdio.h>


int pseudofibonacci(int n){

    if(n<=0){
        return 0;
    }
    else if(n==1){
        return 1;
    }
    else {

        return pseudofibonacci(n-1) + pseudofibonacci(n-2) + 1;
    }
}

int main(){

        int n;

        scanf("%d", &n);

        printf("El enesimo termino es: %d\n", pseudofibonacci(n));

    return 0;
}