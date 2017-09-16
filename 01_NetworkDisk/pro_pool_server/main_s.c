/*************************************************************************
	> File Name: main_s.c
	> Author: xisha
	> Email: 1540348087@qq.com
	> Created Time: Fri 15 Sep 2017 08:23:13 PM CST
 ************************************************************************/

#include"func.h"

void sig(int signum)
{
	char flag='c';
	write(fds_exit[1],&flag,sizeof(flag));
	return;
}

void Daemon()
{
	int i=0;
	if(fork()!=0)
	{
		exit(0);
	}
	setsid();
	chdir("/home/xisha/Daemon");
	umask(0);
	for(i=0;i<3;i++)
	{
		close(i);
	}
}

int main(int argc,char*argv[])
{
	Daemon();
	signal(SIGINT,sig);
	pipe(fds_exit);
	if(argc!=4)
	{
		printf("error args\n");
		return -1;
	}
	int child_num=atoi(argv[3]);
	pchild p=(pchild)calloc(child_num,sizeof(child));
	make_child(p,child_num);
	int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;
	memset(&ser,0,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	ret=bind(sfd,(struct sockaddr*)&ser,sizeof(ser));
	if(-1==ret)
	{
		perror("bind");
		return -1;
	}
	int epfd=epoll_create(1);
	struct epoll_event event,*evs;
	evs=(struct epoll_event *)calloc(child_num+2,sizeof(struct epoll_event));
	memset(&event,0,sizeof(event));
	event.events=EPOLLIN;
	event.data.fd=sfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
	event.events=EPOLLIN;
	event.data.fd=fds_exit[0];
	epoll_ctl(epfd,EPOLL_CTL_ADD,fds_exit[0],&event);
	int i;
	for(i=0;i<child_num;i++)
	{
		event.events=EPOLLIN;
		event.data.fd=p[i].tfds;
		epoll_ctl(epfd,EPOLL_CTL_ADD,p[i].tfds,&event);
	}
	listen(sfd,child_num+2);
	int ret1;
	int new_fd;
	int j;
	char flag;
	while(1)
	{
		memset(evs,0,sizeof(evs));
		ret1=epoll_wait(epfd,evs,child_num+2,-1);
		for(i=0;i<ret1;i++)
		{
			if(evs[i].data.fd==sfd)
			{
				new_fd=accept(sfd,NULL,NULL);
				for(j=0;j<child_num;j++)
				{
					if(p[j].busy==0) 
					{
						send_fd(p[j].tfds,new_fd);
						p[j].busy=1;
						break;
					}
				}
				close(new_fd);
			}
			for(j=0;j<child_num;j++)
			{
				if(evs[i].data.fd==p[j].tfds)
				{
					read(p[j].tfds,&flag,sizeof(flag));
					p[j].busy=0;
				}
			}
			if(evs[i].data.fd==fds_exit[0])
			{
				read(fds_exit[0],&flag,sizeof(flag));
				epoll_ctl(epfd,EPOLL_CTL_DEL,sfd,&event);
				close(sfd);
				for(i=0;i<child_num;i++)
				{
					while(p[i].busy)
					{
						read(p[i].tfds,&flag,sizeof(flag));
						p[i].busy=0;
					}
					event.events=EPOLLIN;
					event.data.fd=p[i].tfds;
					epoll_ctl(epfd,EPOLL_CTL_DEL,p[i].tfds,&event);
					close(p[i].tfds);
				}
				kill(0,SIGKILL);
			}
		}
	}
	return 0;
}
