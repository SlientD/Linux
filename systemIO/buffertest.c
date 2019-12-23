#include <stdio.h>
#include <unistd.h>

int main(){
    printf("printf");   //无  /n   因为他会刷新缓冲区
    const char *p1="buffer";
    const char *p2="test-fprintf";
    fprintf(stdout,"%s%s",p1,p2);
    fwrite("fwire",1,6,stdout);
    write(1,"write",5);
    sleep(3);
    return 0;
}
