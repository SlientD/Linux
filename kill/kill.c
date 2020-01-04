#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int arg,char *argv[]){
    if(arg!=3){
        printf("参数应该有三个：kill 【选项】【pid】\n");
        return 1;
    }   
    int choose=atoi(argv[1]+1);
    pid_t pid=atoi(argv[2]);
    int ret=kill(pid,choose);
    if(ret<0)//成功返回0，错误返回-1
    {
        perror("kill:");
    }
    return 0;


}
