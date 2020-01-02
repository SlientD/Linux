#include "shm.h"

int main(){
    //creat shared memory;
    int ret=CreateShm();
    //link
    char *p=(char *)shmat(ret,NULL,0);
    //print
    printf("reader:%s\n",p);
    return 0;
}
