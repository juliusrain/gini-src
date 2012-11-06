#ifndef __IGMP_H__
#define __IGMP_H__

#include "message.h"

#define IGMP_REPORT 2
#define IGMP_QUERY 1


void IGMPProcessPacket(gpacket_t *in_pkt);
void IGMPBroadcast();

typedef struct _igmp_pkt_hdr_t {
    unsigned char VerType; //00010001(17) | 00010010(18) 4-bit - 4-bit
    unsigned char unused; //8-bit
    unsigned short checksum; //8-bit
    unsigned long grp_addr; //multicast group address 32-bit
} igmp_pkt_hdr_t;


#endif
