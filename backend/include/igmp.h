#ifndef __IGMP_H__
#define __IGMP_H__

#include "message.h"

void IGMPProcessPacket(gpacket_t *in_pkt);
void IGMPBroadcast();

typedef struct _igmp_pkt_hdr_t {
    //unsigned char VerType; //00010001(17) | 00010010(18) 4-bit - 4-bit
    unsigned int version:4;
    unsigned int type:4;
    unsigned char unused; //8-bit
    unsigned short checksum; //8-bit
    unsigned char grp_addr[4]; //multicast group address 32-bit
} igmp_pkt_hdr_t;


#endif
