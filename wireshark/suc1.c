#include <pcap.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
 
void processPacket(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
        int *count = (int *)arg;
         
            printf("Packet Count: %d\n", ++(*count));
                printf("Received Packet Size: %d\n", pkthdr->len);
                    printf("Payload:\n");
                     int i;
                        for(i=0; i < pkthdr->len; ++i)
                        {
                                    printf("%02x ", packet[i]);
                                            if ((i + 1) % 16 == 0)
                                            {
                                                            printf("\n");
                                                                    
                                            }
                                                
                        }
                            printf("\n\n");
                                return;

}
 
int main()
{
        char errBuf[PCAP_ERRBUF_SIZE], * devStr;
          
            //devStr = pcap_lookupdev(errBuf);
        pcap_if_t *alldevs;
        pcap_if_t *d;
        int i=0;
        char errbuf[PCAP_ERRBUF_SIZE];

                        /* 获取本地机器设备列表 */
                        if (pcap_findalldevs( &alldevs, errbuf ) == -1)
                        {
                                    fprintf(stderr,"Error in pcap_findalldevs_ex: %s\n", errbuf);
                                            exit(1);
                                                
                        }
        for(d= alldevs; d != NULL; d= d->next)
        {
            if(strcmp(d->name,"ens33")==0)
            {        printf("%d. %s", ++i, d->name);
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
            sleep(30);        
                        /* open a device, wait until a packet arrives */
                        pcap_t * device = pcap_open_live(devStr, 65535, 1, 0, errBuf);
                            if (!device)
                            {
                                        printf("error: pcap_open_live(): %s\n", errBuf);
                                                exit(1);
                                                    
                            }
                             
                                int count = 0;
                                    /*Loop forever & call processPacket() for every received packet.*/
                                    pcap_loop(device, -1, processPacket, (u_char *)&count);
                                     
                                        pcap_close(device);
                                            return 0;

}
