#include "udpsvr.hpp"


//ip port
//使用命令行参数的方式获取两个信息
// ./svr ip port

int main(int argc,char *argv[]){
    if(argc!=3){
        printf("please ./svr [ip] [port]\n");
        return 0;
    }
    string ip=argv[1];
    uint16_t port=atoi(argv[2]);
    

    UdpSvr us;
    if(!us.createSockt()){
        return 0;
    }


    if(!us.Bind(ip,port)){
        return 0;
    }

    //服务端数据
   while(1){ 
    //接收数据
    string buf;
    //数据来源地址信息
    struct sockaddr_in peer_addr;
    us.Recv(buf,&peer_addr);

    printf("client say:[%s]\n",buf.c_str());
    printf("server say:");
    fflush(stdout);
    cin>>buf;
    //发送数据
    us.Send(buf,&peer_addr);
   }
    us.Close();
    return 0;
}
