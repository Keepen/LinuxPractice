//使用条件变量与互斥锁来实现生产者与消费者模型
#include <iostream>
#include <unistd.h>
#include <mutex>
#include <pthread.h>
#include <queue>
using namespace std;
#define MAXQ 10
class Queue{
  public:
    Queue(){
      _max = MAXQ;
      pthread_mutex_init(&_mutex, NULL);
      pthread_cond_init(&_con, NULL);
      pthread_cond_init(&_pro, NULL);
    }

    //1.生产者生产数据，数据入队
    bool QueuePush(int data){
      //1.判断队列是否还有空间放数据
      pthread_mutex_lock(&_mutex);
      while(_queue.size() == _max){
        pthread_cond_wait(&_pro, &_mutex);
      }
      _queue.push(data);
      pthread_mutex_unlock(&_mutex);
      pthread_cond_signal(&_con);
      return true;
    }

    //2.消费者从缓冲区中拿出数据，数据出队
    bool QueuePop(int& data){
      pthread_mutex_lock(&_mutex);
      while(_queue.empty()){
        pthread_cond_wait(&_con, &_mutex);
      }
      data = _queue.front();
      _queue.pop();
      pthread_cond_signal(&_pro);
      pthread_mutex_unlock(&_mutex);
      return true;
    }

    ~Queue(){
      pthread_mutex_destroy(&_mutex);
      pthread_cond_destroy(&_con);
      pthread_cond_destroy(&_pro);
    }
  private:
    int _max;     //队列内结点的最大数量
    queue<int> _queue;
    pthread_mutex_t _mutex;
    pthread_cond_t _con;
    pthread_cond_t _pro;
};

void* thr_con(void* arg){
  Queue* _q = (Queue*)arg;
  int i = 1;
  while(1){
    _q->QueuePop(i);
    cout << "消费者消费第"  << i++ << "个产品\n";
  }
  return NULL;
}

void* thr_pro(void* arg){
  Queue* _q = (Queue*)arg;
  int i = 1;
  while(1){
    cout << "生产者生产第" << i << "个产品\n";
    _q->QueuePush(i++);
  }

  return NULL;
}

int main(){
  pthread_t con[4],pro[4];
  Queue q;

  for(int i = 0;i < 4; i++){
    pthread_create(&con[i], NULL, thr_con, (void*)&q);
  }
  for(int i = 0;i < 4; i++){
    pthread_create(&pro[i], NULL, thr_pro, (void*)&q);
  }

  for(int i = 0;i < 4; ++i){
    pthread_join(con[i], NULL);
  }
  for(int i = 0;i < 4; ++i){
    pthread_join(pro[i], NULL);
  }

  return 0;
}
