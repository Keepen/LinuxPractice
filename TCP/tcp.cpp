//封装tcp通信的流程
//  1.创建套接字
//  2.绑定地址信息
//  3.开始监听
//  4.获取新连接
//  5.传输数据
//    1）发送数据
//    2）接收数据
//  6.关闭套接字


#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

class TcpSocket{
  public:
    TcpSocket():_sockfd(-1){}

    //1.创建套接字
    bool Socket(){
      _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if(_sockfd < 0){
        cout << "创建socket失败" << endl;
        return false;
      }
      return true;
    }

    //2.绑定地址信息
    bool Bind(string& ip, uint16_t& port){
      sockaddr_in addr;
      addr.sin_addr.s_addr = inet_addr(&ip[0]);
      addr.sin_port = htons(port);
      addr.sin_family = AF_INET;
      socklen_t len = sizeof(addr);
      int ret = bind(_sockfd, (sockaddr*)&addr, len);
      if(ret < 0){
        cout << "建立连接失败！！！" << endl;
        return false;
      } 
      return true;
    }

    //3.开始监听
    bool Listen(int backlog = 5){
      int ret = listen(_sockfd, backlog);
      if(ret < 0){
        cout << "监听错误！！！" << endl;
        return false;
      }
      return true;
    }


    //4.建立连接
    bool Connect(string& ip, uint16_t& port){
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(&ip[0]);
      socklen_t len = sizeof(addr);
      int ret = connect(_sockfd, (sockaddr*)&addr, len);
      if(ret < 0){
        cout << "建立连接失败！！！" << endl;
        return false;
      }
      return true;
    }
    
    //5.获取已完成连接
    bool Accept(TcpSocket& newsock){
      sockaddr_in addr;
      socklen_t len = sizeof(addr);
      int fd = accept(_sockfd, (sockaddr*)&addr, &len);
      if(fd < 0){
        cout << "获取新连接失败！！！" << endl;
        return false;
      }
      newsock._sockfd = fd;
      return true;
    }

    //6.发送数据
    bool Send(string& buf){
      int ret = send(_sockfd, &buf[0], 1024, 0);
      if(ret < 0){
        cout << "接收失败！！！" << endl;
        return false;
      }
      return true;
    }

    //7.接收数据
    bool Recv(string& buf){
      char tmp[4096] = {0};
      int ret = recv(_sockfd, &buf[0], 1024, 0);
      if(ret < 0){
        cout << "接收失败！！！" << endl;
        return false;
      }
      else if(ret == 0){
        cout << "连接已断开！！！" << endl;
        return false;
      }
      buf.clear();
      //将实际接收到的数据写入到buf中
      buf.assign(tmp, ret);
      return true;
    }

    //8.关闭套接字
    bool Close(){
      if(_sockfd < 0){
        cout << "关闭出错！！！" << endl;
        return false;
      }
      close(_sockfd);
      return true;
    }

    ~TcpSocket(){
      Close();
    }

  private:
    int _sockfd;
};

