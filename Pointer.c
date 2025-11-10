#include "stdio.h"

typedef struct coordinate{
    int x,y,z;
} coordinate;

void dummyfunction(int arr[]){
    printf("Size of the array : %d\n",sizeof(arr));
}

int main(){
    int num[10];
    int *arr= num;
    for(int i=0;i<10;i++){
        *(arr+i) = i;
    }
    for(int i=0;i<10;i++){
        printf("%d ",*(arr+i));
    }
    coordinate c1[] = {{
        .x = 1,.y=2,.z=3
    }};
    int *cord = (int*)c1;
    printf("(%d, %d, %d)\n",cord[0],cord[1],cord[2]);
    int x = 10;
    int *ptr = &x;
    printf("%d\n",sizeof(num));
    dummyfunction(num); 
}   