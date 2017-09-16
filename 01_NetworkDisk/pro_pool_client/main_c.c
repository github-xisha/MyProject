/*************************************************************************
	> File Name: main_c.c
	> Author: xisha
	> Email: 1540348087@qq.com
	> Created Time: Fri 15 Sep 2017 08:23:13 PM CST
 ************************************************************************/

#include "func.h"

void sig(int signum)
{
	flag_end=0;
	return;
}

int main(int argc,char* argv[])
{
	if(argc!=4)
	{
		printf("error args\n");
		return -1;
	}
	flag_end=1;
	struct spwd *sp;
	char *passwd;
	char salt[512]={0};
	passwd=getpass("请输入密码:");
	if((sp=getspnam(argv[3]))==NULL)error_quit("获取用户名和密码");
	get_salt(salt,sp->sp_pwdp);
	//if(!strcmp((char*)(sp->sp_pwdp),crypt(passwd,salt)))
	if(!strcmp(sp->sp_pwdp,crypt(passwd,salt)))
	{
		signal(SIGINT,sig);
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
		ret=connect(sfd,(struct sockaddr*)&ser,sizeof(ser));
		if(-1==ret)
		{
			perror("connect");
			return -1;
		}
		train t;
		char buf[1000];
		char tem_buf[1000];
		int len;
		while(1)
		{
			memset(buf,0,sizeof(buf));
			memset(tem_buf,0,sizeof(tem_buf));
			fflush(stdout);
			printf("Operation:");
			scanf("%s",buf);
			send_train(sfd,&t,buf);
			if(!strcmp(buf,EXIT))
			{
				break;
			}
			if(!strcmp(buf,CD)) 
			{
				scanf("%s",tem_buf);
				send_train(sfd,&t,tem_buf);
				memset(tem_buf,0,sizeof(tem_buf));
				recv_n(sfd,(char*)&len,sizeof(len));
				recv_n(sfd,tem_buf,len);
				/*printf("Current path:");
				printf("%s\n",tem_buf);*/
			}
			if(!strcmp(buf,PWD))
			{
				recv_n(sfd,(char*)&len,sizeof(len));
				recv_n(sfd,buf,len);
				printf("Current path:");
				printf("%s\n",buf);
			}
			if(!strcmp(buf,LS))
			{
				recv_n(sfd,(char*)&len,sizeof(len));
				recv_n(sfd,buf,len);
				printf("%s\n",buf);
			}
			if(!strcmp(buf,REMOVE))
			{
				scanf("%s",tem_buf);
				send_train(sfd,&t,tem_buf);
			}
			if(!strcmp(buf,MKDIR))
			{
				scanf("%s",tem_buf);
				send_train(sfd,&t,tem_buf);
			}
			if(!strcmp(buf,GETS))
			{
				scanf("%s",tem_buf);
				send_train(sfd,&t,tem_buf);
				download_file(sfd,tem_buf);
			}
			if(!strcmp(buf,PUTS))
			{
				scanf("%s",tem_buf);
				send_train(sfd,&t,tem_buf);
				upload_file(sfd,tem_buf);
			}
			if(flag_end==0)
			{
				send_train(sfd,&t,"exit");
				break;
			}
		}
		close(sfd);
	}
	return 0;
}
