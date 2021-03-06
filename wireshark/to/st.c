#define _CRT_SECURE_NO_WARNINGS 1
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pcap.h>
#include<mysql/mysql.h>


unsigned int timeout = 7;   //超时时间7秒
#define PORT 3333


#define BACKLOG 1
#define MAXRECVLEN 65535
char trans[MAXRECVLEN];
char sql_insert[MAXRECVLEN];
void change(int i,char tmp[]){
    int low=i%16;
    int high=i/16;
    if(low<=9){
        tmp[1]='0'+low;
    }
    else{
        tmp[1]='a'+low-10;
    }
    if(high<=9){
        tmp[0]='0'+high;
    }
    else{
        tmp[0]='a'+high-10;
    }
}


void processPacket(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    MYSQL *conn_ptr;                           
    //连接数据库
    int ret = 0;
    conn_ptr = mysql_init(NULL);//初始化                                                
    if(!conn_ptr)
    {
        printf("mysql_init failed!\n");                                                                
        return ;
    }
    ret = mysql_options(conn_ptr,MYSQL_OPT_CONNECT_TIMEOUT,(const char*)&timeout);//设置超时>    选项
    if(ret) 
    {
        printf("Options Set ERRO!\n");                        
    }
    conn_ptr = mysql_real_connect(conn_ptr,"192.168.43.78","root","root","packet",0,NULL    ,0);//连接MySQ      L testdb数据库

    if(conn_ptr)
    {                                                               
        printf("Connection Succeed!\n");
    }

    int *count = (int *)arg;          
    printf("Packet Count: %d\n", ++(*count));
    printf("Received Packet Size: %d\n", pkthdr->len);
    printf("Payload:\n");
    int i=0;
    int j=0;
    char tmp[2];
    memset(trans,0,MAXRECVLEN);
    for( i=0; i < pkthdr->len; ++i ){
        change((int)packet[i],tmp);
        trans[j++]=tmp[0];
        trans[j++]=tmp[1];
        trans[j++]=' ';
        printf("%02x ", packet[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");                                                                               
    }
    printf("\n\n");
//    printf("transInProcess=%s\n\n",trans);
    memset(sql_insert,0,MAXRECVLEN);
    
    sprintf(sql_insert,"insert into Info(time,packet) VALUES(now(),'%s');",trans);
    ret=mysql_query(conn_ptr,sql_insert);
 
    
    if(!ret)
   {
       printf("Inserted %lu rows\n",(unsigned long)mysql_affected_rows(conn_ptr));//返回上次UPDATE更改行数
   }
   else
   {
       printf("Connect Erro:%d %s\n",mysql_errno(conn_ptr),mysql_error(conn_ptr));//返回错误代码、错误消息
   }
   usleep(600000);
   mysql_close(conn_ptr);
}


int Get()
{
    char errBuf[PCAP_ERRBUF_SIZE], * devStr;
    //devStr = pcap_lookupdev(errBuf);
    pcap_if_t *alldevs;
    pcap_if_t *d;
    int i=0;
    char errbuf[PCAP_ERRBUF_SIZE];
    /* 获取本地机器设备列表 */
    if (pcap_findalldevs( &alldevs, errbuf   ) == -1)                
    {
        fprintf(stderr,"Error in pcap_findalldevs_ex: %s\n", errbuf);
        exit(1);                                                                
    }
    for(d= alldevs; d != NULL; d= d->next)                                
    {
        printf("%d. %s\n", ++i, d->name);
    }
    char getPort[32];
    gets(getPort);
    for(d= alldevs; d != NULL; d= d->next)
    {if(strcmp(d->name,getPort)==0)
           {   
               printf("%d. %s", ++i, d->name);
               break;
           } 
    }
    devStr=d->name;
    if (devStr)
        printf("success: device: %s\n", devStr);
    else
    {
        printf("error: %s\n", errBuf);
        exit(1);
     }
    /* open a device, wait until a packet arrives */
    pcap_t * device = pcap_open_live(devStr, 65535, 1, 0, errBuf);
    if (!device)
    {
        printf("error: pcap_open_live(): %s\n", errBuf);
        exit(1);                                                   
    }
    /* construct a filter */
    struct bpf_program filter;
    pcap_compile(device, &filter, "ip", 1, 0);
    pcap_setfilter(device, &filter);
    int count = 0;
    /*Loop forever & call processPacket() for every received pa      cket.*/
    
        pcap_loop(device, -1, processPacket, (u_char *)&count);
            
    
    printf("transInGet=%s\n\n",trans);
    pcap_close(device);
    return 0; 
 }                            
int main(int argc, char *argv[])
{
        //char buf[MAXRECVLEN];
       // int listenfd, connectfd;   /* socket descriptors */
       // struct sockaddr_in server; /* server's address information */
       // struct sockaddr_in client; /* client's address information */
       // socklen_t addrlen;
        //连接数据库
       // int ret = 0;
       // conn_ptr = mysql_init(NULL);//初始化                                                
     //  // if(!conn_ptr)
     //   {
     //       printf("mysql_init failed!\n");
     //       return -1;
     //   }
     //   ret = mysql_options(conn_ptr,MYSQL_OPT_CONNECT_TIMEOUT,(const char*)&timeout);//设置超时>    选项
     //   if(ret) 
     //   {
     //       printf("Options Set ERRO!\n");                        
     //   }
     //   conn_ptr = mysql_real_connect(conn_ptr,"localhost","root","123456","test1",0,NULL,0);//连接MySQL testdb数据库
     //   if(conn_ptr)
     //   {                                                               
     //    printf("Connection Succeed!\n");
     //   }
        Get();
        //printf("transInMain=%s\n\n",trans);

        



        /* Create TCP socket */
        //if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        //{
         //   /* handle exception */
          //  perror("socket() error. Failed to initiate a socket");
          //  exit(1);                                        
       // }
        /* set socket option */
        //int opt = SO_REUSEADDR;
       // setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
       // bzero(&server, sizeof(server));
       // server.sin_family = AF_INET;
       // server.sin_port = htons(PORT);
        //server.sin_addr.s_addr = htonl(INADDR_ANY);
        //if (bind(listenfd, (struct sockaddr *)&server, sizeof(server)) == -1)
        //{
            /* handle exception */
         //   perror("Bind() error.");
        //    exit(1);
       // }
       // else{
        //    printf("Bind() success");
       // }
        //if (listen(listenfd, BACKLOG) == -1)
       // {
         //   perror("listen() error. \n");
        //    exit(1);
       // }
        //addrlen = sizeof(client);

       // while (1){
        //    if ((connectfd = accept(listenfd, (struct sockaddr *)&client, &addrlen)) == -1)
         //   {
          //      perror("accept() error. \n");
           //     exit(1);                                                                                            
          //  }
           // struct timeval tv;
            //gettimeofday(&tv, NULL);
           // printf("You got a connection from client's ip %s, port %d at time %ld.%ld\n", inet_ntoa(client.sin_addr), htons(client.sin_port), tv.tv_sec, tv.tv_usec);
           // int iret = -1;
          //  if (1)
           // {
                //iret = recv(connectfd, buf, MAXRECVLEN, 0);
               // if (iret>0)
               //{
                //    printf("%s\n", buf);                                                                
                //}
                //else
                 // {
                 //   close(connectfd);
                //    break;  
                //}
                /* print client's ip and port */
          //  Get();
            //printf("transInMain=%s\n\n",trans);
            //send(connectfd, "shangjjj", iret, 0); /* send to the client welcome message */
          //  close(connectfd);
         //   }
        //}
       // close(listenfd); /* close listenfd */
        return 0;
}
