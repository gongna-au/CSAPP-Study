#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<ctype.h>
 
#define SRV_PORT 9999
void catch_child(int signum){
  (void)signum;
  while(waitpid(0,NULL,WNOHANG)>0);
  return ;
}
 
 
int main(){
 
 
  int lfd,cfd;
  pid_t pid;
  int ret,i;
  char buf[BUFSIZ];
  char str[BUFSIZ];
 
  struct sockaddr_in srv_addr,clt_addr;
  socklen_t clt_addr_len;
 
  bzero(&srv_addr,sizeof(srv_addr));    //将地址结构清零
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_port= htons(SRV_PORT);
  srv_addr.sin_addr.s_addr= htonl(INADDR_ANY);
 
 
  lfd = socket(AF_INET,SOCK_STREAM,0);
  bind(lfd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
  listen(lfd,128);
  clt_addr_len =sizeof(clt_addr);
  while(1){
    cfd = accept(lfd,(struct sockaddr*)&clt_addr,&clt_addr_len);
    printf("receive from %s at PORT %d\n",
        inet_ntop(AF_INET,&clt_addr.sin_addr,str,sizeof(str)),
        ntohs(clt_addr.sin_port));
    pid = fork();
    if(pid<0){
      perror("fork error");
      exit(1);
    }else if(pid==0){
      close(lfd);
      break;
 
    }else{
      struct sigaction act;
 
      act.sa_handler = catch_child;
      sigemptyset(&act.sa_mask);
      act.sa_flags = 0;
      ret=sigaction(SIGCHLD,&act,NULL);
      if(ret!=0){
        perror("sigaction error");
        exit(1);
      }
 
      close(cfd);
      continue;
 
    }
 
  }
  if(pid==0){
    for(;;){
      ret = read(cfd,buf,sizeof(buf));
      if(ret==0){
        close(cfd);
        exit(1);
      }
      for(i=0;i<ret;++i){
        buf[i]=toupper(buf[i]);
 
      }
      write(cfd,buf,ret);
      write(STDOUT_FILENO,buf,ret);
    }
  }
  return 0;
}
 