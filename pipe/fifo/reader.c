#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
//对命名管道的操作和对文件的操作一样
int main(){
    int fd=open("./myfifo",O_RDONLY);
    if(fd<0){
        perror("open");
        return 1;
    }
    char str[1024]={0};
    while(1){
    ssize_t s=read(fd,str,sizeof(str)-1);
    str[s]='\0';
    if(s>0){
        printf("read：%s\n",str);
    }
    //所有写端关闭写者才关闭
    else if(s==0){
        printf("read done!");
        return 0;
    }
    else if(s<0){
        perror("read");
    }
    }
    close(fd);
}
