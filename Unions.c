#include<stdio.h>
#include<string.h>


typedef union config{
    int totalconfig[4];
    struct
    {
        int io;
        int mode;
        int val;
        int id;
    };
    
} config;
config PIN_Config;
int main(){
    PIN_Config.io = 1;
    PIN_Config.mode = 4;
    PIN_Config.val = 4579;
    PIN_Config.id = 100001;
    for(int i=0;i<4;i++){
        printf("%d ",PIN_Config.totalconfig[i]);
    }
    printf("\n");
}


/*



typedef union node{
    int x;
    int y;
    // char* str;
    char c;
} node;

int main(){
    node nd;
    // nd.str = "Kavin Raja";
    // nd.srt = 10;
    // int sz = sizeof(nd);
    // printf("%d\n",sz);
    // nd.x = 200000000000000000000000;
    // sz = sizeof(nd);    
    // printf("%d\n",sz);
    nd.x = 10;
    printf("(%d, %d, %c)\n",nd.x,nd.y,nd.c);
    nd.y = 20;
    printf("(%d, %d, %c)\n",nd.x,nd.y,nd.c);
    nd.c = '@';
    printf("(%d, %d, %c)\n",nd.x,nd.y,nd.c);
    nd.c = 'A';
    printf("(%d, %d, %c)\n",nd.x,nd.y,nd.c);
}



*/