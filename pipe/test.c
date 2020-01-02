#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#if 0
int main(){
    //使用pipe创建一对文件描述符，通过文件描述符操作内核中的管道
   int fd[2];
   //用文件描述符创建一个管道  ret保存pipe返回结果，返回0表示创建成功，失败返回错误码
   int ret=pipe(fd);
   if(ret==0){
   char *str="qwerty";
   write(fd[1],str,strlen(str));
   char get[1024];
   read(fd[0],get,sizeof(get)-1);
   get[1024]='\0';
   printf("%s\n",get);
   }
   //什么时候会创建失败？文打开太多以至于无法分配文件的时候
   else{
       perror("pipe");
   }
    //管道使用完后需要及时关闭文件描述符
    close(fd[0]);
    close(fd[1]);
    return 0;
}
#endif

#if 0
//两个进程之间通过管道进行资源共享
int main(){
    int fd[2];
    int ret=pipe(fd);
    if(ret!=0){
        perror("pipe");
    }
    ret=fork();
    if(ret>0){
        //父进程进行写
        char *str="fater..tell..son";
        write(fd[1],str,strlen(str)); 
        wait(NULL);
        char end[1024]={0};
        //每份数据读一下就被删掉了，因而只能读一次，且不能一份数据一个进程读一部分另一个进程再读另一部分，因为有同步互斥机制的限制
        //管道为空时，读不到，就阻塞了
        read(fd[0],end,sizeof(end)-1);
        printf("father read=%s\n",end);
        //wait(NULL);
    }
    else if(ret==0){
        char end[1024]={0};
        read(fd[0],end,sizeof(end)-1);
        printf("cild read=%s\n",end);
    }
    else{
        perror("fork");
    }
    close(fd[0]);
    close(fd[1]);



}
#endif

//管道的最大长度
int main(){
    int fd[2];
    int ret=pipe(fd);
    int count=0;
    while(1){
        write(fd[1],"a",1);
        count++;
        printf("count=%d\n",count);
    }
    close(fd[0]);
    close(fd[1]);

}






