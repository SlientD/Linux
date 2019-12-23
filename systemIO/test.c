#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main(){
    //打开
    //          打开什么文件？   如何打开   权限
    //
    int fd=open("tmp.txt",O_RDWR|O_CREAT,0664);//返回值保存文件描述符
    if(fd<0){
        perror("open");
    }
    else{
        printf("fd=%d\n",fd);
    }
    const char* lp="linuxtest";
    //写
   int ret= write(fd,lp,strlen(lp));
   printf("ret=%d\n",ret);
   // write(fd,"linuxtest",9);
   //读
   char arr[1024]={0};
    //刚刚写完光标在文件最后所以要移动光标到前面才可以读 
   lseek(fd,0,SEEK_SET);
   ret=read(fd,arr,sizeof(arr)-1);

   printf("ret=%d\n",ret);
   printf("arr=%s\n",arr);
   //关闭
   sleep(20);
   close(fd);
   while(1){}
   return 0;
}
