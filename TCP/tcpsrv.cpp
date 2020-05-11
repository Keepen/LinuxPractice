#include "tcp.hpp"
#include "select.hpp"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
  if(argc != 3){
    cout << "./tcpsrv ip port" << endl;
    return -1;
  }
  string ip(argv[1]);
  uint16_t port = atoi(argv[2]);
  TcpSocket sock;
  sock.Socket();
  sock.Bind(ip, port);
  sock.Listen();
  cout << "开始监听" << endl;

  Select s;
  s.Add(sock);
  while(1){
    vector<TcpSocket> list;
    bool ret = s.Wait(list);
    if(ret == false){
      perror("select 出错\n");
      return -1;
    }
    for(auto i : list){
      //如果是监听套接字，就获取新连接
      if(i.GetFd() == sock.GetFd()){
        TcpSocket newsock;
        bool ret = i.Accept(newsock);
        if(ret == false){
          continue;
        }
        s.Add(newsock);
        cout << "listen add" << endl;
        sleep(2);
      }
      else{
        //普通的通信套接字
        string buf;
        buf.clear();
        bool ret = i.Recv(buf);
        if(!ret){
          s.Del(i);
          i.Close();
          continue;
        }
        cout << "client say : " << buf << endl;
        buf.clear();
        cin >> buf;
        ret = i.Send(buf);
        if(!ret){
          s.Del(i);
          i.Close();
          continue;
        }
      }
    }
  }
  sock.Close();
  return 0;
}

