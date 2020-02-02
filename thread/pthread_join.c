#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void *MyThread(void * arg){
        int count=5;
        while(count--){
                printf("2:in  thread,%lu\n",pthread_self());
                    sleep(3);
                        
        }

}
int main(){
     pthread_t thread;
      pthread_create(&thread,NULL,MyThread,NULL);

       int ret=pthread_join(thread,NULL);
       if(ret==0){
            printf("quit\n"); 

       }
       while(1){
             printf("1.In main thread,%lu\n",pthread_self());                                                        sleep(2);
                 
       }
        
}
