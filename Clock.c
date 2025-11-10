#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "stdbool.h"
int main(){
    system("clear");
    int hours = 11;
    int minutes = 59;
    int seconds = 55;
    bool am = false;
    
    while(1){
        // system("clear");
        printf("DIY CLOCK : %02d : %02d : %02d (%s)\n",hours,minutes,seconds,(am ? "a.m" : "p.m"));
        sleep(1);
        system("clear");
        seconds+=1;
        if(seconds>=60){
            minutes+=1;
            seconds%=60;
        }
        if(minutes>=60){
            hours+=1;
            if(hours==12) am=!am;
            minutes%=60;
        }
        if(hours>12){
            hours%=12;
        }
    }
}