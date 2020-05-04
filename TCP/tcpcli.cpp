#include "tcp.hpp"

int main(int argc, char* argv[]){
  if(argc != 3){
    cout << "./tcpcli ip port" << endl;
    return -1;
  }
  string ip(argv[1]);
  uint16_t port = atoi(argv[2]);
  TcpSocket sock;
  sock.Socket();
  //sock.Bind(ip, port);
  sock.Connect(ip, port);
  while(1){
    string buf;
    cin >> buf;
    sock.Send(buf);
    buf.clear();
    sock.Recv(buf);
    cout << "server said :  " << buf << endl;

  }
  sock.Close();
  return 0;

}
