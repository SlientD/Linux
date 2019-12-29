#include "head.h"
#define BUFF_SIZE 1024
#define PORT 8087
#define FILE_NAME_SIZE 512
int main(int argc,char *argv[])
{

        struct sockaddr_in s_addr,c_addr;
            int s_sockfd,c_sockfd;
                char buf[BUFF_SIZE];
                    int c_addr_len=sizeof(struct sockaddr_in);
                        int s_addr_len=sizeof(struct sockaddr_in);
                            s_sockfd=socket(AF_INET,SOCK_STREAM,0);
                                if(s_sockfd==-1)
                                        strerr("socket error");
                                    memset(&s_addr,0,s_addr_len);
                                        s_addr.sin_family=AF_INET;
                                            s_addr.sin_port=htons(PORT);
                                                s_addr.sin_addr.s_addr=htonl(INADDR_ANY);
                                                    int opt=1;
                                                        setsockopt(s_sockfd,SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
                                                            if(bind(s_sockfd,(struct sockaddr*)&s_addr,s_addr_len)==-1)
                                                                    strerr("bind error");
                                                                if(listen(s_sockfd,5)==-1)
                                                                        strerr("listen error");
                                                                    
                                                                    while(1)
                                                                    {
                                                                                printf("listening to connect........\n");
                                                                                        c_sockfd=accept(s_sockfd,(struct sockaddr*)&c_addr,&c_addr_len);
                                                                                                if(c_sockfd==-1)
                                                                                                            strerr("accept error");
                                                                                                        
                                                                                                        memset(buf,0,BUFF_SIZE);
                                                                                                                if(recv(c_sockfd,buf,BUFF_SIZE,0)==-1)
                                                                                                                            strerr("recv error");
                                                                                                                        char filename[FILE_NAME_SIZE+1];        
                                                                                                                                memset(filename,0,FILE_NAME_SIZE+1);
                                                                                                                                        strncpy(filename,buf,strlen(buf)>FILE_NAME_SIZE? FILE_NAME_SIZE:strlen(buf));
                                                                                                                                                filename[strlen(buf)]='\0';
                                                                                                                                                        printf("%s\n",filename);
                                                                                                                                                                
                                                                                                                                                                FILE *fp=fopen(filename,"rb");
                                                                                                                                                                        if(NULL==fp)
                                                                                                                                                                                    strerr("fopen error");
                                                                                                                                                                                else
                                                                                                                                                                                {
                                                                                                                                                                                                memset(buf,0,BUFF_SIZE);    
                                                                                                                                                                                                            int length=0;
                                                                                                                                                                                                                        while((length=fread(buf,sizeof(char),BUFF_SIZE,fp))>0)
                                                                                                                                                                                                                        {
                                                                                                                                                                                                                                            if(send(c_sockfd,buf,length,0)==-1)
                                                                                                                                                                                                                                                                strerr("send error");
                                                                                                                                                                                                                                                            memset(buf,0,BUFF_SIZE);
                                                                                                                                                                                                                                                                        
                                                                                                                                                                                                                        }
                                                                                                                                                                                                                                    fclose(fp);
                                                                                                                                                                                                                                                printf("FILE %s transfer successful\n",filename);
                                                                                                                                                                                                                                                        
                                                                                                                                                                                }  
                                                                                                                                                                                        close(c_sockfd);

                                                                    }
                                                                            
                                                                        close(s_sockfd);
                                                                            return 0;

}
