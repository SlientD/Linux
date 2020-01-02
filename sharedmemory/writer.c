#include "shm.h"
#include <string.h>
int main(){
    //create shared memory
    int ret=CreateShm();
    //link
    char *p=(char *)shmat(ret,NULL,0);
    //write
    strcpy(p,"hehe");
    return 0;
}
