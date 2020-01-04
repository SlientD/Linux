#include <signal.h>
#include <stdio.h>

void MySignal(int signo){
    printf("signo:%d\n",signo);
}


int main(){
    //signal 替换了原有的函数指针，signal的返回值是旧的函数指针
    int i;
    for(i=0;i<=62;i++)
    signal(i,MySignal);
    while(1){
        
    }
    return 0;


}
