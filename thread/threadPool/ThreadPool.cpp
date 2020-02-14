#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <queue>
#include <pthread.h>
using namespace std;

#define ThreadCount 4

//处理函数
typedef void (*Handler_t)(int);//定义一个函数指针：返回值为void，参数为int
//线程池队列中存放的结构：数据+处理方式
class ThreadTask{
    public:
        ThreadTask()
        :_Data(-1)
        ,_Handler(NULL)
        {

        }
        ThreadTask(int Data,Handler_t Handler){
            _Data=Data;
            _Handler=Handler;
        }
        void Run(){
            _Handler(_Data);
        }
    private:
        int _Data;
        Handler_t  _Handler; //结构里包含一个处理函数的指针   

};

class ThreadPool{
    public:
        ThreadPool(){
            _Cap=ThreadCount;
           //理应创建线程的时候再赋值的，不过这样简单也不影响解释
            _Cur=0;
            pthread_mutex_init(&_Mutex,NULL);
            pthread_cond_init(&_Cond,NULL);
            _IsQuit=false;

            //创建线程
            bool IsCreate=ThreadCreate();
            if(!IsCreate){
                printf("ThreadPool create thread fail");
                exit(1);
            }
        }
        ~ThreadPool(){
            pthread_mutex_destroy(&_Mutex);
            pthread_cond_destroy(&_Cond);
        }


        //在队列中放数据：队列里存放的是一个结构体指针：数据+处理方式
         bool Push(ThreadTask *Tt){ 
            pthread_mutex_lock(&_Mutex);  //保护临界资源_Que
            if(_IsQuit){    //如果要销毁线程池的话就停止往队列里放数据了！
                pthread_mutex_unlock(&_Mutex);
                return false;  
            }
            _Que.push(Tt);      
            pthread_mutex_unlock(&_Mutex);
            //放完后需要通知有个date可以使用了
            pthread_cond_signal(&_Cond);
            return true;
            //放完数据以后，我们需要告诉线程（在等待队列里的线程），我们现在有数据需要处理了
            //如果现在线程都是忙着的
         }
            


        //取队列中的数据
        bool Pop(ThreadTask **Tt){
          *Tt=_Que.front();
          _Que.pop();
          return true;
        }
        bool PoolClear(){
            pthread_mutex_lock(&_Mutex);
            _IsQuit=true;
            pthread_mutex_unlock(&_Mutex);
            //当线程池队列中没有数据的时候，线程都在等待队列中等待唤醒，因而要被唤醒才能让线程执行到ThreadQuit函数
            //while(cur > 0)    是没有必要的的用if就可以实现所有的退出
            if(_Cur > 0){
                pthread_cond_broadcast(&_Cond);

            }
            return true;
        }
    private:
        //为什么入口函数为static？  因为入口函数规定了自己的格式，在类中定义的函数都含有隐式的this指针，static可以屏蔽掉this
        //创建出来的线程是用来处理队列里的数据的
    static void * ThreadStart(void *arg){
         ThreadPool * tp=(ThreadPool *)arg;
         while(1){
             pthread_mutex_lock(&tp->_Mutex); //确保只有一个线程对tp->Pop
             while(tp->_Que.empty()){ 
                 if(tp->_IsQuit){
                     tp->ThreadQuit(); //确保在等待队列中等待的线程退出
                 }
                 //cond_wait就只是将该线程放入队列，然后释放锁，接收到信号再去抢锁，即运行wait这个线程就会去等待队列里等，因此要放入一个循环进行判断！
                 pthread_cond_wait(&tp->_Cond,&tp->_Mutex);
             }
             ThreadTask *tt;
            tp->Pop(&tt);    
            pthread_mutex_unlock(&tp->_Mutex);  
            //pop一个，不需要去通知在产生一个数据，因为来数据是用户的需求，不能没有硬要生产
            //数据已经拿到了，不需要对队列保护了
            tt->Run();   //在该线程中运行结构体中的函数来做任务
         }
            return NULL;
    } 
        void ThreadQuit(){
            _Cur--;           //cur也是个临界资源避免对他同时操作
            pthread_mutex_unlock(&_Mutex);
            pthread_exit(NULL);
        }
        bool ThreadCreate(){
         pthread_t tid;
         int ret=-1,i=0;
// 并不需要拿这个线程id去做什么，我只是创建够线程池中够数的线程就行了，然后使用
           for(; i < ThreadCount;i++)
           {
             ret=pthread_create(&tid,NULL,ThreadStart,this); 
             if(ret!=0){
             printf("create thread is fail\n");
             return false;
             }
            _Cur++;
           }
           return true;
       }
    
        
    
    
    
    
    
    
    private:

        //初始的时候线程的个数
        size_t _Cap;
        //当前的线程池有多少个数量
        size_t _Cur;
        //线程安全队列
        queue<ThreadTask *> _Que;
        //互斥机制：互斥锁
        pthread_mutex_t _Mutex;
        //同步机制
        pthread_cond_t _Cond;

        //线程退出标志,线程池要毁的时候置为真
        bool _IsQuit;
        

};



void Printf(int Data){
    printf("i am Handler,i print [%d]\n",Data);
}






int main(){
    ThreadPool *tp=new ThreadPool();
    for(int i=0;i < 10;i++){
        ThreadTask *tt=new ThreadTask(i,Printf);
        tp->Push(tt);
    }

    //保证数据处理完在clear
    sleep(5);
    tp->PoolClear();
    delete tp;
}
















