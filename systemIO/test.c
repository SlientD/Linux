#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main(){
    //文件描述i符
   // close(1);       //关闭标准输出，即下面的输出不能在屏幕上显示了
    printf("%d\n",stdin->_fileno);
    printf("%d\n",stdout->_fileno);
    printf("%d\n",stderr->_fileno);
    
    //文件描述符分配规则：最小未使用    --->可实现重定向但依赖操作系统底层结构，真正用的是dup2
    close(1);
    int fd=open("test.txt",O_RDWR|O_CREAT|O_APPEND,0666);
    fprintf(stderr,"fd=%d",fd);
    printf("hello");                //会打印到test.txt文件里，因为此时文件描述符为1即stdout指向的结构体变为test.txt
    //printf(...)=fprintf(stdout,..);
    
    //打开
    //          打开什么文件？   如何打开   权限
    //
    int fd1=open("tmp.txt",O_RDWR|O_CREAT,0664);//返回值保存文件描述符
    if(fd1<0){
        perror("open");
    }
    else{
        printf("fd1=%d\n",fd1);
    }
    const char* lp="linuxtest";
    
    
    
    
    
    //写
   int ret= write(fd1,lp,strlen(lp));
   printf("ret=%d\n",ret);
   // write(fd,"linuxtest",9);
   //读
   char arr[1024]={0};
    //刚刚写完光标在文件最后所以要移动光标到前面才可以读 
   lseek(fd1,0,SEEK_SET);
   ret=read(fd1,arr,sizeof(arr)-1);

   printf("ret=%d\n",ret);
   printf("arr=%s\n",arr);
   //关闭
   sleep(20);
   close(fd);
   while(1){}
   return 0;
}
