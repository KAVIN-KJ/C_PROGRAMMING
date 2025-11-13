#include<stdio.h>

struct Car{
    char *name;
    int year;

};

int main(){

    // creating a new struct
    struct Car c = {"Corvette",2020};
    struct Car b = {.year = 2026, .name = "Tesla"};
    printf("Car 1 %p",c);
}