//利用管道来实现进程间通信
// 认识匿名管道的基本操作，并且体会管道的特性
//父进程关闭写端时，子进程写端不影响

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(){
  //创建子进程之前创建管道
  int pipefd[2] = {0};
  int ret = pipe(pipefd);
  if(ret < 0){
    perror("pipe error!!!\n");
    return -1;
  }
  int pid = fork();
  if(pid < 0){
    perror("fork error!!!\n");
    return -1;
  }
  //假设父进程写数据，子进程读数据
  else if(pid == 0){
    //子进程
    //子进程暂停，父进程写满会阻塞
    char buf[1024] = {0};
    int ret = read(pipefd[0], buf, 1023);
    close(pipefd[1]);
    printf("buf read: %s\n",buf);
  }
  else{
    //父进程
    //由于父子进程谁先执行不能确定，所以需要先sleep
    //  得以让子进程可以后执行
    sleep(2);
    int total = 0;
    char * ptr = "hello i am so happy!!!";
    int ret = write(pipefd[1], ptr, strlen(ptr));
    total+=ret;
    printf("total = %d\n",total);
    close(pipefd[1]);
  }
  /*while(1){
    printf("---------------------------%d\n",getpid());
    sleep(3);
    }*/
  return 0;
}
