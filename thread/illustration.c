#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#if 0
//线程内存共享
int g_count=0;
void *ThreadEnter1(void *arg){
    
    while(1){
        printf("In TreadEnter1\n");
       ++ g_count;
        sleep(1);
    }
    return NULL;
}
void *ThreadEnter2(void *arg){                                                                                                     
     int *p=(int *)arg;
      while(1){         
                printf("In TreadEnter2\n");    
                ++*p;                                   
                sleep(1);                                                                                                              
            }                 
      return NULL;    
  } 
void *ThreadEnter3(void *arg){                                                                                                     
      int *p=(int *)arg;
      while(1){         
                printf("In TreadEnter3\n");    
                ++*p;                                   
                sleep(1);                                                                                                              
            }                 
      return NULL;    
  } 
int main(){
    //共享全局变量
    pthread_t tid1;
    pthread_create(&tid1,NULL,ThreadEnter1,NULL);
   // pthread_detach(tid1);//分离后共享内存等性质不会发生改变，就只是该线程有没有结束创建者不再管了而已。主进程结束后都结束了
    int count1=10;
    printf("--------共享全局变量--------------\n");
    while(count1--){
        printf("In MainThread :g_count=%d\n",g_count);
        sleep(1);
    }
    pthread_cancel(tid1);
    pthread_join(tid1,NULL);
    //共享栈上变量
    int tmp=0;
    pthread_t tid2;
    pthread_create(&tid2,NULL,ThreadEnter2,&tmp);
    printf("--------共享栈------------\n");
    int count2=10;
    while(count2--){
        printf("In MainThread :g_count=%d\n",tmp);
        sleep(1);
    }
    pthread_cancel(tid2);                                                                                                   
    pthread_join(tid2,NULL);
    //共享堆
    int *p=(int *)malloc(4);
    pthread_t tid3;
    pthread_create(&tid3,NULL,ThreadEnter3,p);                                                                                  
    printf("--------共享堆------------\n");
    int count3=10;
    while(count3--){
        printf("In MainThread :g_count=%d\n",*p);
        sleep(1);                                                          
    }
    pthread_cancel(tid3);                                                                                                       
    pthread_join(tid3,NULL);
    return 0;
}
#endif

#if 0
//线程异常终止的情况
void *ThreadEnter(void *arg){
    (void) arg;
    while(1){
        int *p=NULL;
        *p=0;       //一个进程中的一个线程异常终止时会导致整个进程全部终止
        printf("in ThreadEnter *p=%d\n",*p);
        sleep(1);
    }
}
int main(){
    pthread_t tid;
    pthread_create(&tid,NULL,ThreadEnter,NULL);
    while(1){
        sleep(1);
    }
    return 0;
}
#endif

#if 0
//多线程利用多核资源
//  前提保证机器确定有多核
void *ThreadEnter1(void *arg){
    while(1);

}
void *ThreadEnter2(void *arg){
    while(1);
}

int main(){
    pthread_t tid1;
    pthread_t tid2;
    pthread_create(&tid1,NULL,ThreadEnter1,NULL);
    pthread_create(&tid1,NULL,ThreadEnter2,NULL);
    while(1);
    return 0;
}
//线程不是越多越好，大多一定程度，cpu达到上线后，线程继续增多，非但不能提高效率，反而会降低效率（线程多了，调度开销也大效率会受影响
//具体多个线程合适？没有准确答案，由于线程也要分cou密集型和io密集型，要看CPU/IO密集型任务的比重，毕竟IO密集型任务不占用cpu
#endif

#if 0
//Q4:通过多线程提高程序效率：
//解决问题：一个很大的数组，把数组的每个元素都进行乘方运算再放回数组
#include <sys/time.h> 
#define SIZE 10000000
#define THREAD_NUM 1
typedef struct Arg{
    int *arr;
    int begin;
    int end;
}Arg;
int64_t GetUs(){          //为什么用int64_t 而不用int 因为要返回的是时间戳，现在的时间戳用int已经存不下了(int 最多能存21亿的数)  不用uint64_t当遇到闰秒时，这个时间戳可能为负数
    struct timeval tv;         //看gettimeofday函数介绍
    gettimeofday(&tv,NULL);
    return tv.tv_sec*1000000+tv.tv_usec;
    
}
void Cal(int *arr,int begin,int end){
    int i=begin;
    for(i;i<end;i++){
       arr[i]=arr[i]*arr[i]; 
    }
}
void *Enter(void *arg){
    Arg *t=(Arg *)arg;
    Cal(t->arr,t->begin,t->end);
    return NULL;
}

int main(){
    
    int *arr=(int *)malloc(SIZE*sizeof(int));
    //记录开始时间
    int64_t start1=GetUs();
    //Cal(arr,0,SIZE);
    int64_t end1=GetUs();
    //用线程
    pthread_t tid[THREAD_NUM];
    Arg arg[THREAD_NUM];
    int i=0;

    for(i=0;i<THREAD_NUM;i++){
        arg[i].arr=arr;
        arg[i].begin=SIZE/THREAD_NUM*i;
        arg[i].end=SIZE/THREAD_NUM*(i+1);
    }
    int64_t start2=GetUs();
        for(i=0;i<THREAD_NUM;i++){
        pthread_create(&tid[i],NULL,Enter,&arg[i]);
    }
    
    for(i=0;i<THREAD_NUM;i++)
    {
        pthread_join(tid[i],NULL);
    }
    int64_t end2=GetUs();
    printf("using time1=%ld\n",end1-start1);
    printf("using time2=%ld\n",end2-start2);

    return 0;
}
#endif

#if 0
int g_count=0;
//互斥量，互斥锁
pthread_mutex_t mutex;//使用前一定要对其初始化
void *Enter(void *arg){
    int i;
    for(i=0;i<50000;i++){
        //如果当前锁正在被其他线程使用时，该线程会在lock处阻塞
        pthread_mutex_lock(&mutex);
        g_count++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(){
    pthread_mutex_init(&mutex,NULL);
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,Enter,NULL);
    pthread_create(&tid2,NULL,Enter,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid1,NULL);
    printf("%d\n",g_count);
    pthread_mutex_destroy(&mutex);
    return 0;
}
#endif

//黄牛抢票
//
#define THREAD_NUM 4
int tickets=100;
pthread_mutex_t mutex;
void *ENTER(void *arg){
    int *p=(int *)arg;
    while(1){
        pthread_mutex_lock(&mutex);
        if(tickets>0){
        printf("i am number %d,i get :%d\n",*p,tickets);
        tickets--;
        
        }
        else{
        pthread_mutex_unlock(&mutex);
            delete p;
            p=NULL;
            break;
        }
        pthread_mutex_unlock(&mutex);
    }
}
int main(){
  pthread_mutex_init(&mutex,NULL);
  pthread_t tid[THREAD_NUM];
  int i;
  for(i=0;i<THREAD_NUM;i++){
      int *p=new int;
      *p=i;
      pthread_create(&tid[i],NULL,ENTER,(void *)p);


  }
  for(i=0;i<THREAD_NUM;i++){
      pthread_join(tid[i],NULL);
  }
  pthread_mutex_destroy(&mutex);
  return 0;
}




