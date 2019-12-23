#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    int fp=open("tmp.txt",O_RDWR|O_CREAT,0667);
    if(fp<0){
        perror("open");
        return 0;
    }
    dup2(fp,1);
    //printf("%s\n","hello");
    printf("你是猪");
    return 0;
}
