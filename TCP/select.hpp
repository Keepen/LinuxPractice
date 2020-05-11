#include <iostream>
#include <sys/select.h>
#include <time.h>
#include <vector>
#include <sys/select.h>
#include "tcp.hpp"
using namespace std;

using namespace std;

class Select{
  public:
    Select():maxfd(10){
      //初始化清空集合
      FD_ZERO(&_rfds);
    }

    //1.添加事件到集合中
    bool Add(TcpSocket& sock){
      int fd = sock.GetFd();
      //添加到集合中
      FD_SET(fd, &_rfds);
      //每次插入都要更新最大描述符
      maxfd = maxfd > fd ? maxfd : fd;
      return true;
    }

    //2.移除事件
    bool Del(TcpSocket& sock){
      int fd = sock.GetFd();
      FD_CLR(fd, &_rfds);
      for(int i = maxfd;i >= 0;--i){
        if(FD_ISSET(i, &_rfds)){
          maxfd = i;
          break;
        }
      }
      return true;
    }

    //3.开始监控，返回就绪的事件集合
    bool Wait(vector<TcpSocket>& list, int timeout = 3000){
      struct timeval tv;
      tv.tv_usec = 0;
      tv.tv_sec = timeout / 1000;
      fd_set tmp = _rfds;
      int nfds = select(maxfd+1, &tmp, NULL, NULL, &tv);
      if(nfds < 0){
        cout << "select error" << endl;
        return false;
      }
      else if(nfds == 0){
        cout << "time out " << endl;
        return false;
      }
      for(int i = 0;i < maxfd;++i){
        if(FD_ISSET(i, &tmp)){
          TcpSocket s;
          s.SetFd(i);
          list.push_back(s);
        }
      }
      return true;
    }
  private:
    int maxfd;
    fd_set _rfds; //可读事件集合
};
