#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int main(){
  int pipefd[2];
  int ret = pipe(pipefd);
  if(ret < 0){
    perror("pipe error\n");
    return -1;
  }
  int pid1 = fork();
  //进入子进程，程序替换，实现ps -ef
  if(pid1 == 0){
    //将标准输出重定向到管道的写端，用来传递给grep
    dup2(pipefd[1], 1);
    execl("/usr/bin/ps", "ps", "-ef", NULL);
    exit(0);
  }
  int pid2 = fork();
  if(pid2 == 0){
    //关闭管道的写端，避免阻塞
    close(pipefd[1]);
    //将标准输入重定向到管道的读端，从pid1进程的结果读入
    dup2(pipefd[0], 0);
    execl("/usr/bin/grep", "grep", "sshd", NULL);
    exit(0);
  }
  close(pipefd[0]);
  close(pipefd[1]);
  waitpid(pid1, NULL, 0);
  waitpid(pid2, NULL, 0);

  return 0;
}
