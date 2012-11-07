
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
    
    switch (igmp_hdr->type) {
    
    case IGMP_REPORT:
        break;
    
    case IGMP_QUERY:

    	break;
    }
}

//TODO: modify gpacket fields (model after ICMP example)
void IGMPBroadcast() {
	int count = 100, i;
	gpacket_t* out_gpacket = (gpacket_t*) malloc(sizeof(gpacket_t));
	//igmp_pkt_hdr_t* igmp_packet = (igmp_pkt_hdr_t *) malloc(sizeof(igmp_pkt_hdr_t));
	igmp_pkt_hdr_t* igmp_packet = (igmp_pkt_hdr_t *) out_gpacket->data.data;
	uchar ip[4];
	char *dot = "192.168.2.3";
	Dot2IP(dot, ip);
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
		IGMPSendQueryMessage(ip, igmp_packet->grp_addr, 1);
		printf("what? %d\n", i);
//		printf("igmp packet %d, %s\n", (igmp_packet->type), IP2Dot(malloc(16*8), igmp_packet->grp_addr));
//		printGPktFrame(out_gpacket, "IGMPBroadcast");
	}
}


void IGMPSendQueryMessage(uchar *dst_ip, uchar *grp_ip, int size)
{
	gpacket_t *out_pkt = (gpacket_t *) malloc(sizeof(gpacket_t));
	ip_packet_t *ipkt = (ip_packet_t *)(out_pkt->data.data);
	ipkt->ip_hdr_len = 6;
	igmp_pkt_hdr_t *igmphdr = (igmp_pkt_hdr_t *)((uchar *)ipkt + ipkt->ip_hdr_len*4);
	ushort cksum;
	char tmpbuf[64];
	
	igmphdr->version = 1;
	igmphdr->type = IGMP_QUERY;
	igmphdr->checksum = 0;
	memcpy(igmphdr->grp_addr, &grp_ip, sizeof(igmphdr->grp_addr));

	cksum = checksum((uchar *)igmphdr, size/2);  // size = payload (given) + igmp_header
	igmphdr->checksum = htons(cksum);

	verbose(2, "[sendIGMPQueryMessage]:: Sending... IGMP query to  %s", IP2Dot(tmpbuf, dst_ip));

	// send the message to the IP routine for further processing
	// the IP should create new header .. provide needed information from here.
	// tag the message as new packet
	// IPOutgoingPacket(/context, packet, IPaddr, size, newflag, source)
	IPOutgoingPacket(out_pkt, dst_ip, size, 1, IGMP_PROTOCOL);
}
