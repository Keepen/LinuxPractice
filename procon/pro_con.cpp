//使用条件变量实现生产者/消费者模型
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <mutex>
#include <queue>
using namespace std;

const int size = 4; //表示线程的数目
const int Max = 5;  //表示队列的最大容量

class BlockQueue{
  private:
    queue<int> _queue;        //假定数据都是int类型
    int _capacity;
    pthread_mutex_t _mutex;   //互斥锁
    pthread_cond_t _pro;
    pthread_cond_t _con;
  public:

    //1.初始化所有变量
    BlockQueue(int maxq = Max):_capacity(maxq){
      pthread_mutex_init(&_mutex, NULL);
      pthread_cond_init(&_pro, NULL);
      pthread_cond_init(&_con, NULL);
    }

    //2.入队
    bool QueuePush(int data){
      //先加锁，再判断是否已满
      pthread_mutex_lock(&_mutex);
      while(_queue.size() >= _capacity){
        //表示已经满了，就循环等待
        pthread_cond_wait(&_pro, &_mutex);
        //cout << "生产产品 " << _queue.size() << "等待中！！！" << endl;
      }
      //有空间，就直接进行入队操作
      _queue.push(data);
      //cout << "产品" << data << " 生产完毕" << endl;
      cout << "现在有 " << _queue.size() << "个数据"  << endl;
      //唤醒消费者，有数据可以处理了
      pthread_cond_signal(&_con);
      pthread_mutex_unlock(&_mutex);
      return true;
    }

    //3.任务出队
    bool QueuePop(int& data){
      pthread_mutex_lock(&_mutex);
      while(_queue.size() == 0){
        pthread_cond_wait(&_con, &_mutex);
        //cout << "消费产品" << _queue.size() << "等待中！！！" << endl;
      }
      data = _queue.front();
      _queue.pop();
      cout << "产品 " << _queue.size()+1 << "处理完毕" << endl;
      cout << "消费完后现在有 " << _queue.size() << "个数据" << endl;
      pthread_cond_signal(&_pro);
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
  for(int i = 0;i < size;++i){
    cout << endl;
    cout << "生产者线程 " << i << "生产" << endl;
    q->QueuePush(i);
  }
  return NULL;
}

void* thr_con(void* arg){
  BlockQueue* q = (BlockQueue*)arg;
  int data;
  for(int i = 0;i < size;++i){
    //cout << "消费者线程 " << i << "消费" << endl;
    q->QueuePop(data);
    //cout << "消费者线程 " << i << "消费数据 " << data  << "完毕" << endl;
  }
  return NULL;
}

int main(){
  pthread_t pro[size], con[size];
  BlockQueue q;
  for(int i = 0;i < size;++i){
    pthread_create(&pro[i], NULL, thr_pro, (void*)&q);
  }
  for(int i = 0;i < size;++i){
    pthread_create(&con[i], NULL, thr_con, (void*)&q);
  }

  for(int i = 0;i < size;++i){
    pthread_join(pro[i], NULL);
  }
  for(int i = 0;i < size;++i){
    pthread_join(con[i], NULL);
  }
  return 0;
}
