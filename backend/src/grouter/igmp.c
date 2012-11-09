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
extern igmp_table_entry_t *igmp_route_tbl;

igmp_table_entry_t *createIGMPGroupEntry(uchar gr_addr[]) {
    igmp_table_entry_t *t_entry = (igmp_table_entry_t *)malloc(sizeof(igmp_table_entry_t));
    memcpy(t_entry->group_addr, gr_addr, sizeof(t_entry->group_addr));
    t_entry->next = NULL;
    t_entry->hosts = NULL;
}

igmp_host_entry_t *createIGMPHostEntry(uchar h_addr[]) {
    igmp_host_entry_t *h_entry = (igmp_host_entry_t *)malloc(sizeof(igmp_host_entry_t));
    memcpy(h_entry->host_addr, h_addr, sizeof(h_entry->host_addr));
    h_entry->next = NULL;
}

igmp_table_entry_t *addMCastGroup(igmp_table_entry_t *tbl_head, unsigned char gr_addr[]) {

    igmp_table_entry_t *new_entry = createIGMPGroupEntry(gr_addr);

    verbose(1, "[addMCastGroup]:: Adding multicast group to IGMP table");
    igmp_table_entry_t *iterator;

    iterator = tbl_head;

    //head was null so create new mcast group and return it;
    if(iterator == NULL) {
        tbl_head = new_entry;
        return tbl_head;
    }

    while(iterator != NULL) {
        if(memcmp(iterator->group_addr, new_entry->group_addr, sizeof(iterator->group_addr)) == 0) {
            //already exists so return with already existing entry
            verbose(1, "[addMCastGroup]:: Entry already exists");
            free(new_entry);
            return tbl_head;
        } else {
            if(iterator->next != NULL) {
                iterator = (igmp_table_entry_t *)(iterator->next);
            } else {
                break;
            }
        }
    }
    iterator->next = new_entry;

    return tbl_head;
}

igmp_table_entry_t *deleteMCastGroup(igmp_table_entry_t *tbl_head, unsigned char gr_addr[]) {
    igmp_table_entry_t *t_iterator;
    igmp_table_entry_t *t_prev;

    igmp_host_entry_t *h_iterator;
    igmp_host_entry_t *h_temp;

    t_iterator = igmp_route_tbl;
    t_prev = t_iterator;

    //check firs entry
    if(memcmp(t_iterator->group_addr, gr_addr, sizeof(t_iterator->group_addr)) == 0) {
        if(t_iterator->next != NULL) {
            t_prev = t_iterator->next;
            free(t_iterator);
            igmp_route_tbl = t_prev;
            return 1;
        } else {
            free(t_iterator);
            t_prev = NULL;
            igmp_route_tbl = t_prev;
            return 1;
        }
    }

    //find target group from rest
    while(t_iterator != NULL) {
        if(memcmp(t_iterator->group_addr, gr_addr, sizeof(t_iterator->group_addr)) == 0) {
            unsigned char buffer[40];
            break;
        } else {
            if(t_iterator->next != NULL) {
                t_prev = t_iterator;
                t_iterator = t_iterator->next;
            } else {
                //target group doesn't exist
                return 1;
            }
        }
    }


    if(t_iterator->next == NULL) {
        t_prev->next = NULL;
    } else {
        t_prev->next = t_iterator->next;
    }

    if(t_iterator->hosts == NULL) {
        verbose(1, "no hosts to delete");
        h_iterator = NULL;
    } else {
        verbose(1, "deleting hosts");
        h_iterator = t_iterator->hosts;
    }

//    while(h_iterator != NULL) {
//        h_temp = h_iterator;
//        if(h_iterator->next != NULL) {
//            h_iterator = h_iterator->next;
//        }
//        free(h_temp);
//        h_temp = NULL;
//    }

    free(t_iterator);

    return 0;

}

int addHostToGroup(igmp_table_entry_t *tbl_head, unsigned char gr_addr[], unsigned char h_addr[]) {
    igmp_table_entry_t *t_iterator;
    igmp_host_entry_t *hosts;

    igmp_host_entry_t *new_host = createIGMPHostEntry(h_addr);

    t_iterator = tbl_head;

    //find target group
    while(t_iterator != NULL) {
        if(memcmp(t_iterator->group_addr, gr_addr, sizeof(t_iterator->group_addr)) == 0) {
            unsigned char buffer[40];
            verbose(1, "found target group %s\n", IP2Dot(buffer, t_iterator->group_addr));
            break;
        } else {
            if(t_iterator->next != NULL) {
                t_iterator = t_iterator->next;
            } else {
                //target group doesn't exist
                free(new_host);
                return 1;
            }
        }
    }

    //group exists but has no hosts;
    if(t_iterator->hosts == NULL) {
        t_iterator->hosts = new_host;
        verbose(1, "no hosts, adding host");
        return 0;
    } else {
        verbose(1, "has hosts, setting host iterator");
        hosts = t_iterator->hosts;
    }

    while(hosts != NULL) {
        if(memcmp(hosts->host_addr, new_host->host_addr, sizeof(hosts->host_addr)) == 0) {
            verbose(1, "host already exists in group");
            free(new_host);
            return 1;
        } else {
            if(hosts->next != NULL) {
                hosts = hosts->next;
            } else {
                break;
            }
        }
    }
    hosts->next = new_host;

    return 0;
}

igmp_host_entry_t *getHostsInGroup(igmp_table_entry_t *tbl_head, unsigned char gr_addr[]) {
    igmp_table_entry_t *iterator;

    iterator = tbl_head;

    while(iterator != NULL) {
        if(memcmp(iterator->group_addr, gr_addr, sizeof(iterator->group_addr)) == 0) {
            return iterator->hosts;
        } else {
            if(iterator->next != NULL) {
                iterator = iterator->next;
            } else {
                return NULL;
            }
        }
    }
    return NULL;
}

void printHosts(igmp_host_entry_t *host_head) {
    char buf[MAX_TMPBUF_LEN];
    igmp_host_entry_t *h_iterator;
    h_iterator = host_head;

    if(host_head == NULL) return;

    while(h_iterator != NULL) {
        verbose(1, "\tIGMP entry:: Host: %s", IP2Dot(buf, h_iterator->host_addr));
        if(h_iterator->next != NULL) {
            h_iterator = h_iterator->next;
        } else {
            return;
        }
    }
}

void printIGMPRouteTable(igmp_table_entry_t *tbl_head) {
    igmp_table_entry_t *t_iterator;
    igmp_host_entry_t *h_iterator;
    char buf[MAX_TMPBUF_LEN];

    t_iterator = tbl_head;

    if(t_iterator == NULL) return;

    while(t_iterator != NULL) {
        verbose(1, "IGMP entry:: Group: %s", IP2Dot(buf, t_iterator->group_addr));

        printHosts(t_iterator->hosts);

        if(t_iterator->next != NULL) {
            t_iterator = t_iterator->next;
        } else {
            return;
        }
    }
}

void IGMPProcessPacket(gpacket_t *in_pkt)
{
    ip_packet_t *ip_pkt = (ip_packet_t*) in_pkt->data.data;
    int ip_hdr_len = ip_pkt->ip_hdr_len * 4;
    igmp_pkt_hdr_t *igmp_hdr = (igmp_pkt_hdr_t *) ((uchar *) ip_pkt + ip_hdr_len);
    
    printf("igmp type: %d\n", igmp_hdr->type);
    printf("igmp version: %d\n", igmp_hdr->version);
    
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
    ip_packet_t *ip_pkt = (ip_packet_t*) in_pkt->data.data;
    int ip_hdr_len = ip_pkt->ip_hdr_len * 4;
    igmp_pkt_hdr_t *igmp_hdr = (igmp_pkt_hdr_t *) ((uchar *) ip_pkt + ip_hdr_len);
    
    //char buffer[100];
    //printf("report came from: %s\n",IP2Dot(buffer,ip_pkt->ip_src));
    
    
    //printf("report group: %s\n", IP2Dot(buffer, igmp_hdr->grp_addr));
    
    //add to table
    uchar group_address[4];
    group_address[0] = igmp_hdr->grp_addr[3];
    group_address[1] = igmp_hdr->grp_addr[2];
    group_address[2] = igmp_hdr->grp_addr[1];
    group_address[3] = igmp_hdr->grp_addr[0];
    char buffer[100];
    printf("group addr reverse: %s\n", IP2Dot(buffer, group_address));
    //igmp_table_entry_t *new_table_entry = createIGMPGroupEntry(group_address);
    igmp_route_tbl = addMCastGroup(igmp_route_tbl, group_address);
    
    uchar ip_src[4];
    ip_src[0] = ip_pkt->ip_src[3];
    ip_src[1] = ip_pkt->ip_src[2];
    ip_src[2] = ip_pkt->ip_src[1];
    ip_src[3] = ip_pkt->ip_src[0];
     
    //igmp_host_entry_t *new_host = createIGMPHostEntry(ip_src);
    addHostToGroup(igmp_route_tbl, group_address, ip_src);
    
   
    
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
//      printf("size: %d, %d, %d\n", sizeof(igmp_packet->unused), sizeof(igmp_packet->checksum), sizeof(igmp_packet->grp_addr));
        IGMPSendQueryMessage(ip, igmp_packet->grp_addr, 8);
//      printf("what? %d\n", i);
//      printf("igmp packet %d, %s\n", (igmp_packet->type), IP2Dot(malloc(16*8), igmp_packet->grp_addr));
//      printGPktFrame(out_gpacket, "IGMPBroadcast");
    }
}


void IGMPSendQueryMessage(uchar *dst_ip, uchar *grp_ip, int size)
{
    gpacket_t *out_pkt = (gpacket_t *) malloc(sizeof(gpacket_t));
    ip_packet_t *ipkt = (ip_packet_t *)(out_pkt->data.data);
    ipkt->ip_hdr_len = 5;
    igmp_pkt_hdr_t *igmphdr = (igmp_pkt_hdr_t *)((uchar *)ipkt + ipkt->ip_hdr_len*4);
    ushort cksum;
    char tmpbuf[64];
    
    igmphdr->version = 1;
    igmphdr->type = IGMP_QUERY;
    igmphdr->unused = 0;
    igmphdr->checksum = 0;
    igmphdr->grp_addr[0] = 225;
    igmphdr->grp_addr[1] = 0;
    igmphdr->grp_addr[2] = 0;
    igmphdr->grp_addr[3] = 36;

    cksum = checksum((uchar *)igmphdr, size/2);  // size = payload (given) + igmp_header
    igmphdr->checksum = htons(cksum);

    verbose(2, "[sendIGMPQueryMessage]:: Sending... IGMP query to  %s", IP2Dot(tmpbuf, dst_ip));

    // send the message to the IP routine for further processing
    // the IP should create new header .. provide needed information from here.
    // tag the message as new packet
    // IPOutgoingPacket(/context, packet, IPaddr, size, newflag, source)
    IPOutgoingPacket(out_pkt, dst_ip, size, 1, IGMP_PROTOCOL);
}
