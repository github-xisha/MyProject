/*************************************************************************
	> File Name: func.h
	> Author: xisha
	> Email: 1540348087@qq.com
	> Created Time: Fri 15 Sep 2017 08:22:42 PM CST
 ************************************************************************/

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <strings.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <shadow.h>
#include <unistd.h>
#include <crypt.h>

#define LS "ls"
#define CD "cd"
#define PWD "pwd"
#define PUTS "puts"
#define GETS "gets"
#define REMOVE "remove"
#define MKDIR "mkdir"
#define EXIT "exit"
typedef struct{
	pid_t pid;//子进程的pid
	int tfds;//通过该管道传递内核控制信息
	short busy;//标示进程是否忙碌
}child,*pchild;
typedef struct{
	int len;
	char buf[1000];
}train,*ptrain;
short flag_end;
void help(void);
void error_quit(char *msg);
void get_salt(char *salt,char *passwd);
char* fun_mmap(char *filename,long f_size);
long file_stat(char *file);
void download_file(int new_fd,char filename[30]);
void upload_file(int new_fd,char filename[30]);
int recv_n(int,char*,int);
void send_train(int new_fd,train *p,char *buf);
