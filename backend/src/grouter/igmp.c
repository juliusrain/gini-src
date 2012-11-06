
#include "igmp.h"
#include "grouter.h"
#include <stdio.h>

void IGMPProcessPacket(gpacket_t *in_pkt)
{
    ip_packet_t *ip_pkt = (ip_packet_t*) in_pkt->data.data;
    int ip_hdr_len = ip_pkt->ip_hdr_len * 4;
    igmp_pkt_hdr_t *igmp_hdr = (igmp_pkt_hdr_t *) ((uchar *) ip_pkt + ip_hdr_len);
    
    switch (igmp_hdr->type) {
    
    case IGMP_REPORT:
        IGMP
        break;
    
    case IGMP_QUERY:
        IGMP
    
    }
}

void IGMPBroadcast(){
	int count = 100, i;
	for(i = 0; i < count; i++){
		sleep(3);
		printf("what? %d\n", i);
	}
}

