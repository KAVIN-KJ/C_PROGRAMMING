#include <stdio.h>
 
typedef enum{
    SUNDAY = 10000,
    MONDAY = 1,
    TUESDAY = 1,
    WEDNESDAY = 10,
    THURSDAY = 10,
    FRIDAY = 1000,
    SATURDAY = 10000
} week;
 

int main()
{
    printf("%d\n", MONDAY);
}