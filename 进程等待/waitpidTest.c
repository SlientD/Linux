#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
     pid_t ret1=fork();
     if(ret1==0){ 
      //child1                       
      printf("child1:%d\n",getpid());                                                                 
     exit(1);
     } 
    pid_t ret2=fork();                                                                                                
      if(ret2==0){                                                                                                      
       //child2                                                                                                      
       printf("child2:%d\n",getpid());                                                                              
       exit(2);                                                                                                     
     }     
    int status=0;
    //pid_t waitpid(pid,status,int options)
    //pid=-1   等待任意子进程 >0 等待指定进程
    //status=NULL表示不关注子进程退出状态
    //option =WNOHANG （宏定义为1）  表示非阻塞状态,轮询式的wait就是没等到子进程就直接走了,因而要搭配循环，否则也会产生僵尸进程  
    //能够更灵活的控制代码，充分利用等待时间，坏处是代码复杂,,,,可用信号机制代替
    pid_t res1=waitpid(ret1,&status,0);
    int count1=0;
    while(res1!=ret1){
    res1=waitpid(ret1,&status,0);
    count1++;
    }
    printf("child1:%d,%d,%d\n",status,res1,count1);
    int count2=0;
    pid_t res2=waitpid(ret2,&status,WNOHANG);
    while(res2!=ret2){
        res2=waitpid(ret2,&status,WNOHANG);
        count2++;
     //   printf("%d,%d \n",res2,count2);
    }
    printf("child2:%d,%d,%d\n",status,res2,count2);
        while(1){}
}
