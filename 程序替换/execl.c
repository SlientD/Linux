#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(){
    int res;
    pid_t ret=fork();
    
    if(ret==0){
    //ls实现
    //可变参数列表必须以NULL结尾    
    
    printf("before execl\n");  
    
    //res=execl("/usr/bin/ls","/usr/bin/ls","/",NULL);   //返回值，成功替换无res，失败返回-1，res=-1，可用来标志替换没替换成功
    //res=execlp("ls","ls","/",NULL);    //execlp相对于execl来说是自带路径    
    
   // char *env[]={"AAA=BBB",NULL};
   // execle("./execle","./execle",NULL,env); //手动传入环境变量，上面两个用的是env中的默认环境变量
  
    char *argv[]={"/usr/bin/ls","-l","-a","/",NULL};   //必须以NULL结尾
    execv("/usr/bin/ls",argv);

    }
    else if(ret>0){
        waitpid(ret,NULL,0);
    printf("after execl %d\n",res);
    }
    else{
        perror("fork");
    }
    return 0;
}










