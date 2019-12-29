#include <pcap.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
 
void processPacket(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
        pcap_dump(arg, pkthdr, packet);
            printf("Received Packet Size: %d\n", pkthdr->len);
                return;

}
 
int main()
{
        char errBuf[PCAP_ERRBUF_SIZE], * devStr;
       devStr = pcap_lookupdev(errBuf);
        if(devStr)
                    printf("success: device: %s\n", devStr);
            else
            {
                        printf("error: %s\n", errBuf);
                                exit(1);
                                    
            }
            while(devStr){
               devStr = pcap_lookupdev(errBuf);
                   if (devStr)
                            printf("success: device: %s\n", devStr);
                    else
                    {
                                printf("error: %s\n", errBuf);
                                        exit(1);
                                            
                    }
            }
                        /* open a device, wait until a packet arrives */
                        pcap_t * device = pcap_open_live(devStr, 65535, 1, 0, errBuf);
                            if (!device)
                            {
                                        printf("error: pcap_open_live(): %s\n", errBuf);
                                                exit(1);
                                                    
                            }
                             
                                /*open pcap write output file*/
                                pcap_dumper_t* out_pcap;
                                    out_pcap  = pcap_dump_open(device,"pack.pcap");
                                     
                                        /*Loop forever & call processPacket() for every received packet.*/
                                        pcap_loop(device, 20, processPacket, (u_char *)out_pcap);
                                         
                                            /*flush buff*/
                                            pcap_dump_flush(out_pcap);
                                                
                                                pcap_dump_close(out_pcap);
                                                    pcap_close(device);
                                                        return 0;

}
