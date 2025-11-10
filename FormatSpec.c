#include<stdio.h>

int main(){

    int a = 21;
    float pri = 19.99;
    double pi = 3.14159265123456735;
    char cur = '$';
    char name[] = "KJKR";

    // printf("%d\n",a);
    // printf("%f\n",pri);
    // printf("%lf\n",pi);

    // width
    int num1 = 1;
    int num2 = 10;
    int num3 = 100;
    // spacing before printing
    printf("%4d\n",num1);
    printf("%4d\n",num2);
    printf("%4d\n",num3);
    // spacing after printing (or) left align
    printf("%-4d\n",num1);
    printf("%-4d\n",num2);
    printf("%-4d\n",num3);
    // preceed with zeros
    printf("%04d\n",num1);
    printf("%04d\n",num2);
    printf("%04d\n",num3);
    // print with signs
    printf("%+d %+d\n",100,-100);
    // precision
    float x1 = 99.99; 
    float x2 = 1.5;
    float x3 = -100;
    printf("\nPrecision : \n\n");
    printf("%+7.2f\n",x1);
    printf("%+7.2f\n",x2);
    printf("%+7.2f\n",x3);

    return 0;
}