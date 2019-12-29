#include "head.h"
 #define PORT 8087
 #define SERVER_IP "127.0.0.1"
 #define BUFF_SIZE 1024  
 #define FILE_NAME_MAX_SIZE 512  
 
 int main(int argc,char *argv[])
{
         if(argc!=3)
                  {    
                           printf("usage : %s ip filename and format\n",argv[0]);
                           exit(1);
                       }
              int sockfd;
                   struct sockaddr_in c_addr;
                        int c_addr_len=sizeof(struct sockaddr_in);
                             char buffer[BUFF_SIZE];
                                  sockfd=socket(AF_INET,SOCK_STREAM,0);
                                       if(-1==sockfd)
                                            　　 strerr("socket error");
                                            memset(&c_addr,0,c_addr_len);
                                                 c_addr.sin_family = AF_INET;
                                                      c_addr.sin_addr.s_addr = inet_addr(argv[1]);
                                                           c_addr.sin_port = htons(PORT);
                                                                int opt = 1;
                                                                     setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
                                                                          if(connect(sockfd,(struct sockaddr*)&c_addr,c_addr_len)==-1)
                                                                                   strerr("connect error");
                                                                               char file_name[FILE_NAME_MAX_SIZE + 1];
                                                                                    memset(file_name, 0, FILE_NAME_MAX_SIZE + 1);
                                                                                         printf("Please Input File Name it will transfer to Server: ");
                                                                                              scanf("%s", &file_name);
                                                                                                   memset(buffer, 0, BUFF_SIZE);
                                                                                                        strncpy(buffer, file_name, strlen(file_name)>BUFF_SIZE ? BUFF_SIZE : strlen(file_name));
                                                                                                             if(send(sockfd,buffer,BUFF_SIZE,0)==-1)
                                                                                                                      strerr("send error");
                                                                                                                  FILE *fp=fopen(argv[2],"wb");
                                                                                                                       if(fp==NULL)
                                                                                                                                strerr("fopen error");
                                                                                                                         　　else
                                                                                                                         {
                                                                                                                                      memset(buffer,0,BUFF_SIZE);
                                                                                                                                               int length=0;
                                                                                                                                                        while((length=recv(sockfd,buffer,BUFF_SIZE,0))>0)
                                                                                                                                                        {
                                                                                                                                                                         if(fwrite(buffer,sizeof(char),length,fp)<length)
                                                                                                                                                                                          strerr("fwrite error");
                                                                                                                                                                                      memset(buffer,0,BUFF_SIZE);
                                                                                                                                                                                               
                                                                                                                                                        }
                                                                                                                                                                 printf("receive file : %s from server successful\n",file_name);
                                                                                                                                                                      
                                                                                                                         }
                                                                                                                              fclose(fp);
                                                                                                                                   close(sockfd);
                                                                                                                                        return 0;
                                                                                                                                         
}
