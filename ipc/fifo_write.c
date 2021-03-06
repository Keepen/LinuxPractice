//命名管道的使用
//  命名管道特性：
//    1.若管道文件以只读方式打开，则会阻塞，直到该管道文件以只写方式打开
//    2.若管道文件以只写方式打开，则会阻塞，直到该管道文件以只读方式打开
//    3.若管道文件以读写方式打开，则不会阻塞

#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
  char* file = "./test.fifo";
  umask(0);
  int ret = mkfifo(file, 0664);
  if(ret < 0 && errno != EEXIST){
    perror("mkfifo error\n");
    return -1;
  }
  printf("create fifo OK\n");
  int fd = open(file, O_WRONLY);
  if(fd < 0){
    perror("open error\n");
    return -1;
  }
  printf("open fifo OK\n");

  while(1){
    char buf[1024] = {0};
    scanf("%s", buf);
    write(fd, buf, strlen(buf));
  }

  close (fd);
  return 0;
}
