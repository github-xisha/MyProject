/*************************************************************************
	> File Name: func.h
	> Author: xisha
	> Email: 1540348087@qq.com
	> Created Time: Fri 15 Sep 2017 08:22:42 PM CST
 ************************************************************************/

#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/mman.h>
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
#include <time.h>
#include <unistd.h>

#define LOG "log.txt"
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
int fds_exit[2];
void write_log(char *p,short flag);
long file_stat(char *file);
int send_n(int sfd,char* p,int len);
int recv_n(int sfd,char* p,int len);
char* fun_mmap(char *filename,long f_size);
void send_data(int new_fd,char *filename);
void recv_data(int sfd);
void send_fd(int fdw,int fd);
void recv_fd(int fdr,int* fd);
void child_handle(int fdr);
void make_child(pchild p,int num);
void send_train(int new_fd,train *p,char *buf);
