//线程控制相关函数------------>都有一个前缀pthrea   c语言全是前缀，c++用命名空间来解决这个问题，但是命名空间也有自身的问题
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void *MyTread(void * arg){
    while(1){
        printf("2:in second thread,%lu\n",pthread_self());
        sleep(3);
    }
}
int main(){
    //1.创建线程
    pthread_t thread;
    pthread_create(&thread,NULL,MyTread,NULL);
    while(1){
        printf("1.In main thread,%lu\n",pthread_self());
        sleep(2);
    }




    return 0;
}
