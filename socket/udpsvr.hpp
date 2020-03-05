#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
using namespace std;

class UdpSvr{
    public:
        UdpSvr(){
            _Sock=-1;
        }
        ~UdpSvr(){
            _Sock=-1;
        }

        //创建套接字
        bool createSockt(){
            _Sock=socket(AF_INET,SOCK_DGRAM,17);
            if(_Sock < 0)
            perror("socket");
            return true;

        }
        //绑定信息
        bool Bind(string& ip,uint16_t port){
            //ip+port
            struct sockaddr_in addr;  //定义一个ipv4数据结构
            addr.sin_family=AF_INET;
            addr.sin_port=htons(port);
//两个字节，牵扯到大小端,所以主机字节序要变为网络字节序
            addr.sin_addr.s_addr=inet_addr(ip.c_str());            
            int ret=bind(_Sock,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
            if(ret < 0){
                perror("bind");
                return false;
            }
            return true;
        
        
        
        
        }

        //发送数据
        bool Send(string &buf,struct sockaddr_in *destaddr){
            //在调用sizeof计算地址信息长度的时候，不能直接使用sizeof(destaddr),计算出来的会使指针大小
            int sendsize=sendto(_Sock,buf.c_str(),buf.size(),0,(struct sockaddr*)destaddr,sizeof(struct sockaddr_in));
            if(sendsize < 0){
                perror("send");
                return false;
            }
            return true;
        }
        //接收数据
        bool Recv(string &buf,struct sockaddr_in* srcaddr){
            char tmp[1024]={0};
            socklen_t socklen=sizeof(struct sockaddr_in);
            int recvsize=recvfrom(_Sock,tmp,sizeof(tmp)-1,0,(struct sockaddr*)srcaddr,&socklen);
            if(recvsize < 0){
                perror("recvfrom");
                return false;

            }
            buf.assign(tmp,recvsize);
            return true;
        
        
        }
        //关闭
        void Close(){
            close(_Sock);
            _Sock=-1;
        }
private:
    int _Sock;


};
