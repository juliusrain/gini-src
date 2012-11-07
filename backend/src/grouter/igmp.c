
#include "grouter.h"
#include "message.h"
#include "ip.h"
#include "protocols.h"
#include "igmp.h"
#include <stdio.h>

void IGMPProcessPacket(gpacket_t *in_pkt)
{
    ip_packet_t *ip_pkt = (ip_packet_t*) in_pkt->data.data;
    int ip_hdr_len = ip_pkt->ip_hdr_len * 4;
    igmp_pkt_hdr_t *igmp_hdr = (igmp_pkt_hdr_t *) ((uchar *) ip_pkt + ip_hdr_len);
    
    printf("igmp type: %d\n", igmp_hdr->type);
    printf("igmp version: %d\n", igmp_hdr->version);
    printf("igmp unused: %c\n", igmp_hdr->unused);
    printf("igmp unused: %d\n", igmp_hdr->checksum);
    switch (igmp_hdr->type) {
    
    case IGMP_REPORT:
        printf("got report\n");
        IGMPProcessReport(in_pkt);
        break;
    
    case IGMP_QUERY:
        printf("got a query\n");
    	break;
    }
}


void IGMPProcessReport(gpacket_t *in_pkt) {

}

//TODO: modify gpacket fields (model after ICMP example)
void IGMPBroadcast() {
	int count = 100, i;
	gpacket_t* out_gpacket = (gpacket_t*) malloc(sizeof(gpacket_t));
	//igmp_pkt_hdr_t* igmp_packet = (igmp_pkt_hdr_t *) malloc(sizeof(igmp_pkt_hdr_t));
	igmp_pkt_hdr_t* igmp_packet = (igmp_pkt_hdr_t *) out_gpacket->data.data;
	//ip_packet_t *ipkt = (ip_packet_t *)(out_pkt->data.data);
	for(i = 0; i < count; i++) {
		sleep(3);
		//igmp_packet->VerType = 17;
		igmp_packet->version = 1;
		igmp_packet->type = 1;
		//255.0.0.36
		//11100001 00000000 00000000 0100100
		//1887436836
		//igmp_packet->grp_addr = 1887436836;
		igmp_packet->grp_addr[3] = 225;
		igmp_packet->grp_addr[2] = 0;
		igmp_packet->grp_addr[1] = 0;
		igmp_packet->grp_addr[0] = 36;
//		printf("what? %d\n", i);
//		printf("igmp packet %d, %s\n", (igmp_packet->type), IP2Dot(malloc(16*8), igmp_packet->grp_addr));
//		printGPktFrame(out_gpacket, "IGMPBroadcast");
	}
}

