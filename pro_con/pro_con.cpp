//生产者消费者模型：
//    实现一个线程安全的队列
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <mutex>
#include <queue>
using namespace std;
#define MAX 10    //定义队列内部界定啊的最大数量是10
#define SIZE 4    //线程的数目

class BlockQueue{
  private:
    queue<int> _queue;
    int _capacity;            //控制队列内结点最大数量
    pthread_mutex_t _mutex;   //互斥锁
    pthread_cond_t _pro;
    pthread_cond_t _con;
  public:
    //1.初始化所有变量
    BlockQueue(int capa = MAX):_capacity(capa){
      pthread_mutex_init(&_mutex, NULL);
      pthread_cond_init(&_pro, NULL);
      pthread_cond_init(&_con, NULL);
    }

    //2.入队
    bool QueuePush(int data){
      //先加锁
      pthread_mutex_lock(&_mutex);
      //循环判断队列内部是否已满
      while(_queue.size() == _capacity){
        //如果已经满，就等待
        pthread_cond_wait(&_pro, &_mutex);
      }
      //未满，则进行入队操作
      _queue.push(data);
      //唤醒消费者
      pthread_cond_signal(&_con);
      //解锁，释放锁资源，让消费者线程可以操作
      pthread_mutex_unlock(&_mutex);
      return true;
    }

    //3.出队
    bool QueuePop(int& data){
      //先加锁
      pthread_mutex_lock(&_mutex);
      //循环判断队列是否已经为空
      while(_queue.empty()){
        //如果为空，进行等待
        pthread_cond_wait(&_con, &_mutex);
      }
      //如果不为空，正常出队
      _queue.pop();
      //唤醒生产者线程
      pthread_cond_signal(&_pro);
      //解锁，释放锁资源，让生产者线程可以操作临界资源
      pthread_mutex_unlock(&_mutex);
      return true;
    }


    //4.销毁所有变量
    ~BlockQueue(){
      pthread_mutex_destroy(&_mutex);
      pthread_cond_destroy(&_pro);
      pthread_cond_destroy(&_con);
    }
};

void* thr_pro(void* arg){
  BlockQueue* q = (BlockQueue*)arg;
  //直接生产数据
  for(int i = 0;i < SIZE; ++i){
    q->QueuePush(i);
    cout << "生产者生产第" << i << "个数据" << endl;
  }
  return NULL;
}

void* thr_con(void* arg){
  BlockQueue* q = (BlockQueue*)arg;
  for(int i = 0; i < SIZE; ++i){
    q->QueuePop(i);
    cout << "消费者消费第" << i << "个数据" << endl;
  }
  return NULL;
}




int main(){
  pthread_t pro[SIZE], con[SIZE];   //创建4个生产者，4个消费者
  BlockQueue q;     //定义一个线程安全队列变量
  for(int i = 0; i < SIZE; ++i){
    pthread_create(&pro[i], NULL, thr_pro, (void*)&q);
  }
  for(int i = 0; i < SIZE; ++i){
    pthread_create(&con[i], NULL, thr_con, (void*)&q);
  }

  //等待线程退出
  for(int i = 0; i < SIZE; ++i){
    pthread_join(pro[i], NULL);
  }
  for(int i = 0; i < SIZE; ++i){
    pthread_join(con[i], NULL);
  }
  return 0;
}


