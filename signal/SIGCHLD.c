#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//void Handle(int signo){
//    printf("child exit");
//  while(1){
//  int ret=waitpid(-1,NULL,WNOHANG);
//  if(ret>0){               //回收到子进程 
//  printf("wait=%d\n",ret);
//  }
//  else{                       //无子进程可回收
//  break;
//  }
//  }
//  //wait(NULL);  //会产生僵尸进程，因为信号同时来时可能只处理一个，来不及处理全部的
//}

int main(){
   signal(SIGCHLD,SIG_IGN);   //fater ignore child , child will be distroy by itself

    int i=0;
    pid_t ret;
//   signal(SIGCHLD,Handle);
    for(i;i<20;i++){
        ret=fork();
        if(ret==0){
            printf("child%d=%d\n",i,getpid());
            exit(0);
        }
    }
    while(1){}
    return 0;
}
