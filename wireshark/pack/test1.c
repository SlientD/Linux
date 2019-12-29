#include <stdio.h>
#include "pcap.h"
#include <unistd.h>
#include <stdlib.h>

int main()
{
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

                            /* 打印列表 */
                            for(d= alldevs; d != NULL; d= d->next)
                            {
                                        printf("%d. %s", ++i, d->name);
                                                if (d->description)
                                                                printf(" (%s)\n", d->description);
                                                        else
                                                                        printf(" (No description available)\n");
                                                            
                            }

                                if (i == 0)
                                {
                                            printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
                                                    return 0;
                                                        
                                }

                                    /* 不再需要设备列表了，释放它 */
                                    pcap_freealldevs(alldevs);

}

