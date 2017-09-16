/*************************************************************************
	> File Name: func_c.c
	> Author: xisha
	> Email: 1540348087@qq.com
	> Created Time: Fri 15 Sep 2017 08:23:13 PM CST
 ************************************************************************/

#include "func.h"

void help(void)
{
	printf("第一个参数为用户名\n");
	exit(-1);
}
void error_quit(char *msg)
{
	perror(msg);
	exit(-2);
}
void get_salt(char *salt,char *passwd)
{
	int i,j;
	for(i=0,j=0;passwd[i]&&j!=3;++i)
	{
		if(passwd[i]=='$') ++j;
	}
	strncpy(salt,passwd,i-1);
}


//循环发送
int send_n(int sfd,char* p,int len)
{
	int total=0;
	int ret;
	while(total<len)
	{
		ret=send(sfd,p+total,len-total,0);
		total=total+ret;
	}
	return 0;
}

//循环接收
int recv_n(int sfd,char* p,int len)
{
	int total=0;
	int ret;
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

//下载文件
void download_file(int sfd,char *filename)
{
	time_t start_time,stop_time;
    struct tm * pstart;
    struct tm * pstop;
    time(&start_time);
    pstart=localtime(&start_time);
    //程序执行时间测试
    clock_t start,stop;
    double runtime;
    start=clock();
    
    int len;
	train t;
	short flag_mmap=0;
	char *pmmap,*cur_mmap;
	char buf[1000]={0};
	char tem_buf[1000]={0};
	long f_size,recv_size,cur_size,check,l=0;
	memset(buf,0,sizeof(buf));
	memset(tem_buf,0,sizeof(tem_buf));
	int fd;
	fd=open(filename,O_CREAT|O_RDWR,0666);
	if(-1==fd)
	{
		perror("open");
		return;
	}
	cur_size=file_stat(filename);
	printf("cur_size=%ld\n",cur_size);
	sprintf(tem_buf,"%ld",cur_size);
	send_train(sfd,&t,tem_buf);//发送文件大小给服务器端，不存在发0
	recv_n(sfd,(char*)&len,sizeof(len));//接收要接收的数据大小
	recv_n(sfd,buf,len);
	recv_size=atoi(buf);
	printf("f_size=%ld\n",recv_size);
	printf("----------:0.00%s\r","%");
	fflush(stdout);
	check=recv_size/10;
	int count=1,i;
	lseek(fd,cur_size,SEEK_SET);
	while(1)
	{
        if(flag_end==0)
		{
			send_train(sfd,&t,"0");
			break;
		}else{
			send_train(sfd,&t,"1");
		}
        recv_n(sfd,(char*)&len,sizeof(len));
		l=l+len;
		if(len>0)
		{
			memset(buf,0,sizeof(buf));
			recv_n(sfd,buf,len);
			write(fd,buf,len);

			if(l>check)
			{
				for(i=0;i<count;i++)
				{
					printf("0");
				}
				for(i=0;i<10-count;i++)
				{
					printf("-");
				}
				printf(":%5.2f%s\r",(double)l*100/recv_size,"%");
				fflush(stdout);
				count++;
				check=check+recv_size/10;
			}
		}else{
			printf("\nfile download success\n");
			break;
		}
	}
    
    stop=clock();
    runtime=(double)(stop-start)/1000000;
    printf("runtime=%lfs\n",runtime);
    time(&stop_time);
    pstop=localtime(&stop_time);
    printf("start:%d,stop:%d,start=%ld,stop=%ld\n",pstart->tm_sec,pstop->tm_sec,start_time,stop_time);
	
    close(fd);
}  


//上传文件
void upload_file(int new_fd,char filename[30])
{
	train t,t_end;
	long big=0;
	char flag='c';
	struct stat f_stat;
	memset(&f_stat,0,sizeof(f_stat));
	off_t f_size;
	int i,count=1;
	long num=0,temp;
	char buf[1000];
	memset(buf,0,sizeof(buf));
	recv_n(new_fd,(char*)&big,sizeof(big));
	recv_n(new_fd,buf,big);
	big=atoi(buf);
	int fd;
	fd=open(filename,O_RDWR,0666);
	if(-1==fd)
	{
		perror("open");
		return;
	}
	fstat(fd,&f_stat);
	f_size=f_stat.st_size;
	printf("f_size=%ld,big=%ld\n",f_size,big);
	if(big!=0)
	{
		lseek(fd,big,SEEK_SET);
		f_size=f_size-big;
	}
	printf("f_size=%ld,big=%ld\n",f_size,big);
	temp=f_size/10;
	printf("---------- 00.00%s\r","%");
	fflush(stdout);
	while(memset(&t,0,sizeof(t)),(t.len=read(fd,t.buf,sizeof(t.buf)))>0)
	{
		memset(&t_end,0,sizeof(t_end));
		if(flag_end==0)
		{
			send_train(new_fd,&t_end,"0");
			break;
		}else{
			send_train(new_fd,&t_end,"1");
		}

		send_n(new_fd,(char*)&t,4+t.len);
		num=num+t.len;
		if(num>=temp)
		{
			temp=temp+f_size/10;
			printf("---------- ");
			printf("%5.2f%s\r",(double)num/f_size*100,"%");
			for(i=0;i<count;i++)
			{
				printf("0");
			}
			printf("\r");
			count++;
			fflush(stdout);
		}
	}
	close(fd);
	if(flag_end==1)
	{
		t.len=0;
		send_n(new_fd,(char*)&t,4+t.len);
		printf("file upload success\n");
	}
}
