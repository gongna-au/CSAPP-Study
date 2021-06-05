#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main(){


    while(1){
    //创建套接字
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    //使用IPv4地址  地址族（Address Family），也就是地址类型  32位IP地址
    serv_addr.sin_family = AF_INET; 
    
    //具体的IP地址，用inte_addr进行了强制类型转化 32位IP地址sin_addr  
    //sin_addr是个结构体 里面是s_addr, s_addr  in_addr_t=unsigned long 
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  

    serv_addr.sin_port = htons(8082);  //端口
    //bind 的第一个参数是文件的标识符，第二个参数是sockaddr 结构体变量的指针，第三个参数是第二个参数的大小
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    //进入监听状态，等待用户发起请求
    //第一个参数为文件的标识符，第二个参数是请求队列的最大长度
    listen(serv_sock, 20);

    //接收客户端请求
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    //当套接字处于监听状态时，可以通过 accept() 函数来接收客户端请求
    //int accept(int sock, struct sockaddr *addr, socklen_t *addrlen);  //Linux
    //sock 为服务器端套接字
    //addr 为 sockaddr_in 结构体变量
    //addrlen 为参数 addr 的长度，可由 sizeof() 求得。
    //accept() 返回一个新的套接字来和客户端通信，addr 保存了客户端的IP地址和端口号，
    //而 sock 是服务器端的套接字，大家注意区分。后面和客户端通信时，要使用这个新生成的套接字，而不是原来服务器端的套接字。
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    //向客户端发送数据
    printf("accept!\n");
    char str[] = "http://127.0.0.1";

    //ssize_t write(int fd, const void *buf, size_t nbytes);
    //fd 为要写入的文件的描述符，buf 为要写入的数据的缓冲区地址，nbytes 为要写入的数据的字节数
    write(clnt_sock, str, sizeof(str));
   
    //关闭套接字
    close(clnt_sock);
    close(serv_sock);
    }
    
    return 0;
}


