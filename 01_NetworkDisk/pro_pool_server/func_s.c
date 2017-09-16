/*************************************************************************
	> File Name: fun_s.c
	> Author: xisha
	> Email: 1540348087@qq.com
	> Created Time: Fri 15 Sep 2017 08:23:13 PM CST
 ************************************************************************/

#include "func.h"

//记录日志
void write_log(char *p,short flag)
{
	FILE *fd;
	time_t timep;
    struct tm *ptime;
    time(&timep);
    ptime=localtime(&timep);
	char *cur_path=getcwd(NULL,0);
	char pre_path[100]="/home/xisha/Daemon/log";
	chdir(pre_path);
	fd=fopen(LOG,"a+");
	if(flag==1)
    {
        fwrite(">>",1,1,fd);
        strcat(p," ");
        fwrite(p,strlen(p),1,fd);
    }
    if(flag==0)
	{
		char time_buf[100]={0};
		memset(time_buf,0,sizeof(time_buf));

        sprintf(time_buf,"    %d-%d-%d %d:%d",1900+ptime->tm_year,1+ptime->tm_mon,ptime->tm_mday,ptime->tm_hour,ptime->tm_min);
		strcat(p,time_buf);
		strcat(p,"\n");
        fwrite(p,strlen(p),1,fd);
	}
	fclose(fd);
	chdir(cur_path);
	return;
}

//mmap映射
char* fun_mmap(char *filename,long f_size)
{
	int fd;
	char *pmmap;
	fd=open(filename,O_RDWR);
	pmmap=(char *)mmap(NULL,f_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	close(fd);
	return pmmap;
}


//发送数据
void send_data(int new_fd,char *filename)
{
    train t;
	int len;
	int big;
	char flag='c';
	short flag_mmap=0;
	short flag_end=1;
	char buf[1000]={0};
	char tem_buf[1000]={0};
	char *pmmap,*cur_mmap,*pre_mmap;
	memset(buf,0,sizeof(buf));
	memset(tem_buf,0,sizeof(tem_buf));
	memset(&t,0,sizeof(t));
	long f_size,recv_size,send_size;
	int fd;
	fd=open(filename,O_CREAT|O_RDWR,0666);
	if(-1==fd)
	{
		perror("open");
		return;
	}
	f_size=file_stat(filename);
	recv_n(new_fd,(char*)&len,sizeof(len));//接收客户端相同文件的大小，若不存在则为0
	recv_n(new_fd,buf,len);
	recv_size=atoi(buf);
	send_size=f_size-recv_size;
	sprintf(t.buf,"%ld",send_size);
	t.len=strlen(t.buf);
	send_n(new_fd,(char*)&t,4+t.len);//发送文件大小，是需要发送的数据大小而不是文件的实际大小
    
    if(send_size>104857600)
    //if(send_size<100)
	{
		pmmap=fun_mmap(filename,f_size);
		cur_mmap=pmmap;
		cur_mmap=cur_mmap+recv_size;
		flag_mmap=1;
	}else{
		lseek(fd,recv_size,SEEK_SET);
	}

	while(memset(&t,0,sizeof(t)),send_size>0)
	{
		if(flag_mmap==0)
		{
            t.len=read(fd,t.buf,sizeof(t.buf));
            send_size=send_size-t.len;
            if(t.len==0)
			{
				break;
			}

		}else{
			if(send_size>sizeof(t.buf))
            {
                memcpy(t.buf,cur_mmap,sizeof(t.buf));
                t.len=sizeof(t.buf);
            }else{
                memcpy(t.buf,cur_mmap,send_size);
                t.len=send_size;
            }
		    cur_mmap=cur_mmap+sizeof(t.buf);
            send_size=send_size-sizeof(t.buf);
		}
		memset(tem_buf,0,sizeof(tem_buf));
		recv_n(new_fd,(char*)&len,sizeof(len));//接收是否继续下载的请求
		recv_n(new_fd,tem_buf,len);
		flag_end=atoi(tem_buf);
        
        if(flag_end==0)
		{
			break;
		}
		send_n(new_fd,(char*)&t,4+t.len);
	}
	if(flag_end==1)
	{
		t.len=0;
		send_n(new_fd,(char*)&t,4+t.len);
	}
	if(flag_mmap==1)
	{
		close(fd);
        munmap(pmmap,f_size);
	}else{
		close(fd);
	}
}

//接收数据
void recv_data(int sfd)
{
	int len;
	int flag_end=1;
	train t;
	memset(&t,0,sizeof(t));
	char buf[1000];
	char tem_buf[1000];
    char buf110[110]={0};
    int fd;
	char flag='c';
	memset(buf,0,sizeof(buf));
	memset(tem_buf,0,sizeof(tem_buf));
	recv_n(sfd,(char*)&len,sizeof(len));
	recv_n(sfd,buf,len);
	strcpy(buf110,buf);
    write_log(buf110,0);
	long f_size;
	f_size=file_stat(buf);
	printf("f_size=%ld\n",f_size);
	sprintf(tem_buf,"%ld",f_size);
	send_train(sfd,&t,tem_buf);//发送此时文件的大小，不存在则发送0字节
	printf("f_size=%ld\n",f_size);
	fd=open(buf,O_CREAT|O_RDWR,0666);
	if(-1==fd)
	{
		perror("open");
		return;
	}

	lseek(fd,f_size,SEEK_SET);
	while(1)
	{
		memset(buf,0,sizeof(buf));
		recv_n(sfd,(char*)&len,sizeof(len));//接收是否继续上传的请求
		recv_n(sfd,buf,len);
		flag_end=atoi(buf);
		if(flag_end==0)
		{
			break;
		}
		memset(buf,0,sizeof(buf));
		recv_n(sfd,(char*)&len,sizeof(len));
		if(len>0)
		{
			recv_n(sfd,buf,len);
			write(fd,buf,len);
		}else{
			break;
		}
	}
	close(fd);
}  

//获取文件信息
long file_stat(char *file)
{
	int fd;
	struct stat p;
	memset(&p,0,sizeof(p));
	off_t f_size;
	fd=open(file,O_RDONLY);
	if(fd==-1)
	{
		f_size=0;
	}else{
		fstat(fd,&p);
		f_size=p.st_size;
	}
	return f_size;
}


//发送描述符
void send_fd(int fdw,int fd)
{
	struct msghdr msg;
	memset(&msg,0,sizeof(msg));
	struct cmsghdr *cmsg;
	int len=CMSG_LEN(sizeof(int));
	cmsg=(struct cmsghdr *)calloc(1,len);
	cmsg->cmsg_len=len;
	cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
	*(int*)CMSG_DATA(cmsg)=fd;
	msg.msg_control=cmsg;
	msg.msg_controllen=len;
	char buf1[10]="hello";
	char buf2[10]="world";
	struct iovec iov[2];
	iov[0].iov_base=buf1;
	iov[0].iov_len=5;
	iov[1].iov_base=buf2;
	iov[1].iov_len=5;
	msg.msg_iov=iov;
	msg.msg_iovlen=2;
	int ret=sendmsg(fdw,&msg,0);
	if(-1==ret)
	{
		perror("sendmsg");
		return;
	}
}

//接收描述符
void recv_fd(int fdr,int* fd)
{
	struct msghdr msg;
	memset(&msg,0,sizeof(msg));
	struct cmsghdr *cmsg;
	int len=CMSG_LEN(sizeof(int));
	cmsg=(struct cmsghdr *)calloc(1,len);
	cmsg->cmsg_len=len;
	cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
	msg.msg_control=cmsg;
	msg.msg_controllen=len;
	char buf1[10]="hello";
	char buf2[10]="world";
	struct iovec iov[2];
	iov[0].iov_base=buf1;
	iov[0].iov_len=5;
	iov[1].iov_base=buf2;
	iov[1].iov_len=5;
	msg.msg_iov=iov;
	msg.msg_iovlen=2;
	int ret=recvmsg(fdr,&msg,0);
	if(-1==ret)
	{
		perror("sendmsg");
		return;
	}
	*fd=*(int*)CMSG_DATA(cmsg);
}
//循环发送
int send_n(int sfd,char *p,int len)
{
	int ret,total=0; 
	while(total<len)
	{
		ret=send(sfd,p+total,len-total,0);
		total=total+ret;
	}
	return 0;
}

//循环接收
int recv_n(int sfd,char * p,int len)
{
	int ret,total=0;
	while(total<len)
	{
		ret=recv(sfd,p+total,len-total,0);
		total=total+ret;
	}
	return 0;
}	


//发送小火车
void send_train(int new_fd,train *p,char *buf)
{
	memset(p,0,sizeof(p));
	strcpy(p->buf,buf);
	p->len=strlen(p->buf);
	send_n(new_fd,(char*)p,4+p->len);
	return;
}

//子进程处理函数
void child_handle(int fdr)
{
	int new_fd;
	char flag='c';
	train t;
	int len,count;
	char buf[1000];
	char tem_buf[1000];
	DIR *fdir;
	struct dirent *dir_infor;
	recv_fd(fdr,&new_fd);
	while(1)
	{
		memset(&t,0,sizeof(t));
		memset(buf,0,sizeof(buf));
		memset(tem_buf,0,sizeof(tem_buf));
		count=0;
		recv_n(new_fd,(char*)&len,sizeof(len));
		recv_n(new_fd,buf,len);
		if(!strcmp(buf,EXIT))
		{
			write(fdr,&flag,sizeof(flag));
			close(new_fd);
			write_log(buf,0);
			recv_fd(fdr,&new_fd);
		}
		if(!strcmp(buf,PWD)) 
		{
			send_train(new_fd,&t,getcwd(NULL,0));
			write_log(buf,0);
		}
		if(!strcmp(buf,CD)) 
		{
			recv_n(new_fd,(char*)&len,sizeof(len));
			recv_n(new_fd,tem_buf,len);
			chdir(tem_buf);
			send_train(new_fd,&t,getcwd(NULL,0));
			write_log(buf,1);
			write_log(tem_buf,0);
		}
		if(!strcmp(buf,LS)) 
		{
			fdir=opendir(getcwd(NULL,0));
			struct dirent *dir_infor;
			write_log(buf,0);
			memset(buf,0,sizeof(buf));
			while((dir_infor=readdir(fdir))!=NULL)
			{
				if(strcmp(dir_infor->d_name,".")&&strcmp(dir_infor->d_name,".."))
				{
					strcat(buf,dir_infor->d_name);
					strcat(buf," ");
					count++;
					if(count%4==0) strcat(buf,"\n"); //为什么不能使用sprintf(buf,"%s ",dir_infor->d_name);
				}
			}
			if(count>0)send_train(new_fd,&t,buf);
			else{
				strcpy(buf,"Empty");
				send_train(new_fd,&t,buf);	
			}
			//write_log(buf,0);
		}
		if(!strcmp(buf,REMOVE))
		{
			recv_n(new_fd,(char*)&len,sizeof(len));
			recv_n(new_fd,tem_buf,len);
			if(unlink(tem_buf))
			{
				fdir=opendir(tem_buf);
				dir_infor=readdir(fdir);
				rmdir(tem_buf);
			}
			write_log(buf,1);
			write_log(tem_buf,0);
		}
		if(!strcmp(buf,MKDIR))
		{
			recv_n(new_fd,(char*)&len,sizeof(len));
			recv_n(new_fd,tem_buf,len);
			//umask(002);//若此处不设置，可能会导致mkdir创建目录的实际权限与设置的权限不符，可以在命令行输入umask查看系统的掩码值
            mkdir(tem_buf,0775);
            write_log(buf,1);
			write_log(tem_buf,0);
		}
		if(!strcmp(buf,GETS)) 
		{
			recv_n(new_fd,(char*)&len,sizeof(len));
			recv_n(new_fd,tem_buf,len);
			send_data(new_fd,tem_buf);
			write_log(buf,1);
			write_log(tem_buf,0);
		}
		if(!strcmp(buf,PUTS))
		{
			write_log(buf,1);
			//write_log(tem_buf,0);
			recv_data(new_fd);
		}
	}
}
//创建多个子进程
void make_child(pchild p,int num)
{
	int i;
	int fds[2];
	pid_t pid;
	for(i=0;i<num;i++)
	{
		socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
		pid=fork();
		if(0==pid)
		{
			close(fds[1]);
			child_handle(fds[0]);
		}
		close(fds[0]);
		p[i].pid=pid;
		p[i].tfds=fds[1];
		p[i].busy=0;
	}
}
