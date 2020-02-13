#include <iostream>
#include <stdio.h>
#include <queue>
#include <pthread.h>

#define PTHREADCOUNT 4

class BlockQueue{
    public:
        BlockQueue(int cap){
            _cap=cap;
            pthread_mutex_init(&_QueMutex,NULL);
            pthread_cond_init(&_concond,NULL);
            pthread_cond_init(&_procond,NULL);
        }
        ~BlockQueue(){
            
        }
        int Pop(int *Data){
            pthread_mutex_lock(&_QueMutex);
            while(_Queue.empty()){
                pthread_cond_wait(&_concond,&_QueMutex);
            }
            *Data=_Queue.front();
            _Queue.pop();
            pthread_mutex_unlock(&_QueMutex);
            pthread_cond_signal(&_procond);
            return 0;
        }
        int Push(int &Data){
            pthread_mutex_lock(&_QueMutex);
            while(IsFull()){
                pthread_cond_wait(&_procond,&_QueMutex);
            }
            _Queue.push(Data);
            pthread_mutex_unlock(&_QueMutex);
            pthread_cond_signal(&_concond);
            return 0;
        }
        bool IsFull(){
            if(_Queue.size()==_cap){
                return true;
            }
            return false;
        }
private:
    std::queue<int> _Queue;
    //定义的Queue的容量
    size_t _cap;
    pthread_mutex_t _QueMutex;
    pthread_cond_t _concond;
    pthread_cond_t _procond;
};

void *CONSUMER(void *arg){
    BlockQueue *bq=(BlockQueue *)arg;
    int Data;
    while(1){
        bq->Pop(&Data);
        printf("i am Consumer  [%p],i put the data[%d] \n",pthread_self(),Data);

    }
    return NULL;
}
void *PRODUCER(void *arg){
    BlockQueue *bq=(BlockQueue *)arg;
    int Data=1;
    while(1){
        bq->Push(Data);
        printf("i am Producer [%p],i put the data[%d]\n ",pthread_self(),Data);
        Data++;
    }
    return NULL;
}





int main(){
    
    pthread_t con_tid[PTHREADCOUNT],pro_tid[PTHREADCOUNT];
    int i;
    int ret=-1;
    BlockQueue* bq=new BlockQueue(4);
    for(i=0;i<PTHREADCOUNT;i++){
        ret=pthread_create(&con_tid[i],NULL,CONSUMER,bq);
        if(ret!=0)
            perror("pthread_create:");
        ret=pthread_create(&pro_tid[i],NULL,PRODUCER,bq);
        if(ret!=0)
            perror("pthread_create:");
        
    }
    for(i=0;i<PTHREADCOUNT;i++){
        pthread_join(con_tid[i],NULL);
        pthread_join(pro_tid[i],NULL);
    }
    delete bq;

}

