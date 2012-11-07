/*
 * Contains IGMP functions for processing processing and IGMP table routing
 * 
 *
 *
 *
 */

#include "grouter.h"
#include "message.h"
#include "ip.h"
#include "protocols.h"
#include "igmp.h"
#include <stdio.h>
#include <stdlib.h>

igmp_table_entry_t *addMCastGroup(igmp_table_entry_t *tbl_head, igmp_table_entry_t *new_entry) {
    igmp_table_entry_t *iterator;

    iterator = tbl_head;

    //head was null so create new mcast group and return it;
    if(iterator == NULL) {
        iterator = (igmp_table_entry_t *)malloc(sizeof(igmp_table_entry_t));
        return iterator;
    }

    while(iterator->next != NULL) {
        iterator = iterator->next;
    }
    iterator->next = new_entry;

    return iterator->next;
}

igmp_host_entry_t *addHostToGroup(igmp_table_entry_t *tbl_head, uchar gr_addr[], igmp_host_entry_t *new_host) {
    igmp_table_entry_t *t_iterator;
    igmp_host_entry_t *hosts;

    t_iterator = tbl_head;
    while(t_iterator != NULL) {
        if(memcmp(t_iterator->group_addr, gr_addr, sizeof(t_iterator->group_addr)) == 0) {
            break;
        }
        t_iterator = t_iterator->next;
        //target group doesn't exist, so return null;
        if(t_iterator == NULL) return NULL;
    }

    hosts = t_iterator->hosts;
    //group exists but has no hosts;
    if(hosts == NULL) {
        t_iterator->hosts = new_host;
        return t_iterator->hosts;
    }

    while(hosts->next != NULL) {
        hosts = hosts->next;
    }
    hosts->next = new_host;

    return hosts_next;
}

igmp_host_entry *getHostsInGroup(igmp_table_entry_t *tbl_head, uchar gr_addr[]) {
    igmp_table_entry_t *iterator;

    iterator = tbl_head;
    while(iterator != NULL) {
        if(memcmp(iterator->group_addr, gr_addr, sizeof(iterator->group_addr)) == 0) {
            return iterator->hosts;
        }
        iterator = iterator->next;
    }
    return NULL;
}

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

