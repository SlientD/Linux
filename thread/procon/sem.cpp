#include <stdio.h>
#include <semaphore.h>
#include <iostream>
#include <vector>
#include <unistd.h>
using namespace std;
#define SIZE 1
#define THREADCOUNT 1

pthread_mutex_t mux;
class RingQue{
    public:
        RingQue()
        :_Vec(SIZE){
            _cap=SIZE;
            _WriPos=0;
            _ReaPos=0;
            //生产者生产的时候看有多少空间可以使用
            sem_init(&_ProSem,0,SIZE);
            //消费者初始化时看当前有多少个资源可以消费
            sem_init(&_CusSem,0,0);
            //初始化互斥
            sem_init(&_LockSem,0,1);
        }

        ~RingQue(){
            sem_destroy(&_ProSem);
            sem_destroy(&_CusSem);
            sem_destroy(&_LockSem);

        }
        void Push(int &Data){   //规范：输入型参数用引用
            sem_wait(&_ProSem);
            sem_wait(&_LockSem);
            _Vec[_WriPos]=Data;
            _WriPos=(_WriPos+1)%_cap;
            //资源加一，并且唤醒消费者
            sem_post(&_LockSem);
            sem_post(&_CusSem);

        }
        void Pop(int *Data){
            sem_wait(&_CusSem);
            sem_wait(&_LockSem);
            *Data=_Vec[_ReaPos];
            _ReaPos=(_ReaPos+1)%_cap;
            sem_post(&_LockSem);
            sem_post(&_ProSem);
        }
        int getWri(){
            return _WriPos;
        }
        int getRea(){
            return _ReaPos;
        }
    private:
        vector<int> _Vec;
        size_t _cap;
        int _WriPos;
        int _ReaPos;

        //同步功能的信号量
        //生产者的信号量
        sem_t _ProSem;
        sem_t _CusSem;
        sem_t _LockSem;

};

void *ProEnter(void *arg){
    RingQue *rq=(RingQue *)arg;
    int Data=1;
    while(1){
        rq->Push(Data);
        pthread_mutex_lock(&mux);
        printf("i am producer [%p],i push data in pos[%d],the data is [%d]\n",pthread_self(),(rq->getWri()-1+SIZE)%SIZE,Data);
        Data++;
        pthread_mutex_unlock(&mux);
    }
    return NULL;
}

void *CusEnter(void *arg){
     RingQue *rq=(RingQue *)arg;
     int *Data=new int;
     while(1){
         rq->Pop(Data);
         pthread_mutex_lock(&mux);
         printf("i am custom [%p],i get data in pos[%d],the data is [%d]\n",pthread_self(),(rq->getRea()+1)%SIZE,*Data);
         pthread_mutex_unlock(&mux);
     }
    return NULL;
}
int main(){
    pthread_mutex_init(&mux,NULL);
    RingQue *rq=new RingQue;
    pthread_t pro_tid[THREADCOUNT],cus_tid[THREADCOUNT];
    int i,ret;
    for(i=0;i<THREADCOUNT;i++){
        ret=pthread_create(&pro_tid[i],NULL,ProEnter,rq);
        if(ret!=0){
           perror("pthread_create"); 
        }
        ret=pthread_create(&cus_tid[i],NULL,CusEnter,rq);
        if(ret!=0){
            perror("[thread_create");
        }
    }
    for(i=0;i<THREADCOUNT;i++){
        pthread_join(pro_tid[i],NULL);
        pthread_join(cus_tid[i],NULL);
    }
    pthread_mutex_destroy(&mux);
    delete rq;
    return 0;
}






