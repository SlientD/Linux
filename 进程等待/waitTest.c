#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#if 0
int main(){
    pid_t ret=fork();
    if(ret>0){
    //father
    printf("father:%d\n",getpid());
    int status=0;
    //阻塞式函数，要等到子函数执行完才会向下执行
    pid_t res=wait(&status);        //wait返回等待子进程的pid，status为输出型参数保存子程序的退出状态
    printf("status=%d,res=%d\n",status,res);
    
    //退出码用一个字节表示正常退出高八位为退出状态，=1是stutas为256，=2 512.。。
    //被信号所杀第八位表示，第八位为coredump标志，低七位为终止信号
    if(status&0xff){
        //异常终止
        printf("异常终止，终止信号为：%d\n",status&0x7f);
    }
    else{
        printf("正常终止，退出信号为：%d\n",(status>>8)&0xff);
    }
    }
    else if(ret==0){
    //child
    int  count=100;
    sleep(3);
    printf("child:%d\n",getpid());  
    while(count--){
    sleep(1);
    printf("child:%d\n",getpid()); 
    }
    exit(1);    //包含在stdlib头文件中
    }
    else{
        //error
        perror("fork");
    }
}
#endif

//注：wait只等待一个进程
int main(){
    pid_t ret=fork();
    if(ret==0){
        //child1
        printf("child1:%d\n",getpid());
        exit(1);
    }
    ret=fork();                                                                                                
    if(ret==0){                                                                                                      
    //child2                                                                                                      
        printf("child2:%d\n",getpid());                                                                              
        exit(2);                                                                                                     
   }     
    int status=0;
    pid_t res=wait(&status);  //wait只等待一个子进程，因而其中一个等待进程会变成僵尸
                              //因而wait调用次数要和子进程的数目一样
                              //wait调用次数多，多出来的wait就会调用出错
                              //次数少，会出现僵尸进程
                              //如果有多个子进程，任何一个子进程都会触发wait的返回，针对所有子进程有时不太科学
                              //针对某个子进程，用waitpid
    printf("%d,%d\n",status,res);
    while(1){}
}









