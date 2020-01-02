#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(){
    int fd=open("myfifo",O_WRONLY);
    if(fd<0){
        perror("open");
    } 
    while(1){
    char str[1024]; 
    ssize_t s=read(1,str,1024);
    write(fd,str,strlen(str));
    }
    close(fd);
    return 0;

}
