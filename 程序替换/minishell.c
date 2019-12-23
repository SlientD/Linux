//借助进程控制内容，开发一个mini的shell程序
//实现功能顺序：
//          1.打印一个提示符，并且让用户输入一个指令
//          2.解析客户输入的指令，找到对应的可执行程序
//          3.创建子进程，子进程序替换，来加载可执行程序
//          4.父进程进行进程等待子进程结束
//          5.子进程结束，父进程从wait中返回，循环执行1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
int Split(char input[],char *output[]){
    //函数strtoke
    //strtoke(str,"分隔符集合"）
    //strtok 会破坏原来的字符串，把分隔符都改成\0
    //返回的都是每个子串的第一个字符的地址
    //strtok必须循环调用，并且第一次和后面的参数不一样
    int res=0;
    char *substr;
    int i=0;
    substr=strtok(input," ");
    while (substr!=NULL){
       // printf("%s\n",substr);
       output[i]=substr;
       i++;
        substr=strtok(NULL," ");  //strtok函数可以记录上次切分的位置，在上次结果的基础上继续切分
    }
    output[i]=NULL;
    i++;
    return i;
}
void CreatProcess(char *argv[],int n){
    //void(n);   //消除没有n的警告
    int ret=fork();
    if(ret==0){
        ret=execvp(argv[0],argv);
        //if(ret==-1)   是不需要这个条件的，如果替换成功了就不会执行下面的代码
            perror("exec");
            exit(0);
    }
    else if(ret>0){
        int status=0;
        wait(&status);
    }
    else{
        perror("fork");
    }

}
int main(){
    while(1){
    //1.打印提示符
    printf("[linxi@localhost ~]");
    fflush(stdout);
    //2.解析用户指令：
    //2.1接收指令-->字符串
    char command[1024]={0};
    gets(command);                //一次读一行，scanf遇空格就停了，getline只在c++中才有
    printf("%s\n",command);
   //2.2解析：目的要把执行那个程序识别出来
   //将命令和命令行参数分开，即按空格将字符串切分，切分结果应该是一个字符串数组
   char *argv[1024]={0};
   int n=Split(command,argv);     //数组是能将改变带出来的
   CreatProcess(argv,n); 

    }
}
