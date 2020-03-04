#include "udpsvr.hpp"


//ip port
//使用命令行参数的方式获取两个信息
// ./svr ip port

int main(int argc,char *argv[]){
    if(argc!=3){
        printf("please input ./cli [ip] [port]\n");
        return 0;
    }
    //服务端的ip
    string ip=argv[1];
    uint16_t port=atoi(argv[2]);
    

    UdpSvr us;
    if(!us.createSockt()){
        return 0;
    }
    //不绑定，操作系统会自动给我们绑定一个，避免的只能有一个端口发送数据
   while(1){
    string buf;
    printf("client say:");
    fflush(stdout);
    cin>>buf;
    //接收数据
    //数据来源地址信息
    struct sockaddr_in dest_addr;
    dest_addr.sin_family=AF_INET;
    dest_addr.sin_port=htons(port);
    dest_addr.sin_addr.s_addr=inet_addr(ip.c_str());
    us.Send(buf,&dest_addr);
    us.Recv(buf,&dest_addr);

    printf("srv say:[%s]\n",buf.c_str());
   }
    us.Close();
    return 0;
}
