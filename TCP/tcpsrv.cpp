#include "tcp.hpp"
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
  TcpSocket newsock;
  while(1){
    sock.Accept(newsock);
    string buf;
    buf.clear();
    newsock.Recv(buf);
    cout << "client say : " << buf << endl;
    buf.clear();
    cin >> buf;
    newsock.Send(buf);
  }
  sock.Close();
  return 0;
}

