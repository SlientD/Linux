//线程控制相关函数------------>都有一个前缀pthrea   c语言全是前缀，c++用命名空间来解决这个问题，但是命名空间也有自身的问题
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void *MyThread1(void * arg){
    int count=5;
    while(count--){
        printf("2:in  thread1,%lu\n",pthread_self());
        sleep(3);
    }
    pthread_exit(NULL);    //返回值为一个void*  无用 让这个线程退出，注意与exit（让一个进程推出，调用后整个进程都退出了）区别
}
void *MyThread2(void *arg){
pthread_cancel(pthread_self());
    while(1){
        printf("2:in thread2,%lu\n",pthread_self());
        sleep(2);
//   pthread_cancel(pthread_self());
    }
}
int main(){
    //1.创建线程
    pthread_t thread1;
    pthread_create(&thread1,NULL,MyThread1,NULL);
    pthread_t thread2;
    pthread_create(&thread2,NULL,MyThread2,NULL);
    int count=5;
    while(count--){
         printf("1.In main thread,%lu\n",pthread_self());
         sleep(2);
    }
   // pthread_cancel(thread2);
 int ret=pthread_join(thread2,NULL);
 if(ret==0){
    printf("quit\n"); 
 }
    while(1){
        printf("1.In main thread,%lu\n",pthread_self());
        sleep(2);
    }
    //若主函数没有这个while，主函数在创建出线程后他自己也会往后跑，当执行完return 0时main主线程结束了即进程结束了那么他创建的所有线程也就全都结束了
    



    return 0;
}
