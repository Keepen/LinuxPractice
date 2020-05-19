//使用POSIX信号量实现生产者/消费者模型
#include <iostream>
#include <thread>
#include <semaphore.h>
#include <vector>
using namespace std;

const int size = 4;   //最多4个线程
const int Max = 5;    //最多5个节点数据

class BlockQueue{
  private:
    vector<int> _queue;
    int _capacity;
    int _write;         //写指针
    int _read;          //读指针
    sem_t _lock;        //相当于锁互斥量
    sem_t _empty_space; //记录空闲空间
    sem_t _data_space;  //记录已经存放的数据量
  public:
    //1.初始化
    BlockQueue():_queue(Max),_capacity(Max),_write(0),_read(0){
      sem_init(&_lock, 0, 1);   //互斥锁的计数0/1
      sem_init(&_empty_space, 0, _capacity);
      sem_init(&_data_space, 0, 0);
    }

    //2.任务入队
    bool QueuePush(int val){
      sem_wait(&_empty_space);
      sem_wait(&_lock);
      _queue[_write] = val;
      _write = (_write + 1) % _capacity;
      sem_post(&_lock);
      sem_post(&_data_space);
      return true;
    }

    //3.任务出队
    bool QueuePop(int& val){
      sem_wait(&_data_space);
      sem_wait(&_lock);
      val = _queue[_read];
      _read = (_read + 1) % _capacity;
      sem_post(&_lock);
      sem_post(&_empty_space);
      return true;
    }

    //4.销毁
    ~BlockQueue(){
      sem_destroy(&_lock);
      sem_destroy(&_empty_space);
      sem_destroy(&_data_space);
    }
};

void thr_pro(BlockQueue* q){
  int val = 0;
  int i = 0;
  for(int i = 0;i < size;++i){
    q->QueuePush(val);
    cout << "生产者线程 " << i << "生产 " << val << "完毕！！！" << endl;
    ++val;
    //++i;
  }
}

void thr_con(BlockQueue* q){
  int val = 0;
  int i = 0;
  for(int i = 0;i < size;++i){
    q->QueuePop(val);
    cout << "消费者线程 " << i << "处理数据 " << val << "完毕！！！" << endl;
    //++i;
  }
}

int main(){
  BlockQueue q;
  vector<thread> pro(4);
  vector<thread> con(4);

  for(int i = 0;i < size;++i){
    pro[i] = thread(&thr_pro, &q);
  }

  for(int i = 0;i < size;++i){
    con[i] = thread(&thr_con, &q);
  }

  for(int i = 0;i < size;++i){
    pro[i].join();
    con[i].join();
  }

  return 0;
}

