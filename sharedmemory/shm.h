//创建一个共享内存
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>

int CreateShm(){
    //key要通过ftok（pathname，proj_id）生成，参数变化生成的key就会变
    key_t key=ftok(".",0x2);
    //失败返回-1
    if(key==-1){
        perror("key");
        return 1;
    }
    //int shmget(key_t key,size_t size,int shmflg)
    //key为共享内存的名字，size为共享内存大小，返回值为一个非负整数即共享内存段的标识码，失败返回-1
    //shmflg由九个权限标志组成，    IPC_CREAT共享内存对象存在打开，不存在打开
    //                              IPC_EXCL  不存在创建，存在失败    与上一个结合表示只创建不打开
    //                              0666表示权限
    int ret=shmget(key,20,IPC_CREAT|0666);
    if(ret<0){
        perror("shmget");
        return 1;
    }
    printf("shmget return =%d\n",ret);
    return ret;
}

