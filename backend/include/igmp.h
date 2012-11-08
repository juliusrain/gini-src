/*
 *
 *
 *
 *
 *
 *
 */



#ifndef __IGMP_H__
#define __IGMP_H__

#include "message.h"


#define IGMP_REPORT 2
#define IGMP_QUERY 1


typedef struct _igmp_host_entry_t {
    unsigned char host_addr[4];
    struct igmp_host_entry_t *next;
} igmp_host_entry_t;

typedef struct _igmp_table_entry_t {
    unsigned char group_addr[4];
    struct igmp_table_entry_t *next;
    igmp_host_entry_t *hosts;
} igmp_table_entry_t;

typedef struct _igmp_pkt_hdr_t {
    //unsigned char VerType; //00010001(17) | 00010010(18) 4-bit - 4-bit
    unsigned int version:4;
    unsigned int type:4;
    unsigned char unused; //8-bit
    unsigned short checksum; //16-bit
    unsigned char grp_addr[4]; //multicast group address 32-bit
} igmp_pkt_hdr_t;

igmp_table_entry_t *addMCastGroup(igmp_table_entry_t *tbl_head, igmp_table_entry_t *new_group);
igmp_host_entry_t *addHostToGroup(igmp_table_entry_t *tbl_head, igmp_table_entry_t *group, igmp_host_entry_t *new_host);
igmp_host_entry_t *getHostsInGroup(igmp_table_entry_t *tbl_head, igmp_table_entry_t *group);
void printHosts(igmp_host_entry_t *host_head);
void printIGMPRouteTable(igmp_table_entry_t *tbl_head);
void IGMPProcessPacket(gpacket_t *in_pkt);
void IGMPBroadcast();
void IGMPSendQueryMessage(uchar *dst_ip, uchar *grp_ip, int size);

#endif
