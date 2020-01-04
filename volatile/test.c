#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
//volatile int flag=1;
int flag=1;
void Handler(int signo){
    printf("signo=%d\n",signo);
    flag=0;
}
int main(){
    signal(2,Handler);
    while(flag){
    }
}
