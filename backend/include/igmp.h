#ifndef __IGMP_H__
#define __IGMP_H_


typedef struct _igmp_pkt_hdr_t {
    unsigned char VerType; //00010001(17) | 00010010(18) 4-bit - 4-bit
    unsigned char unused; //8-bit
    unsigned short checksum; //8-bit
    unsigned long grp_addr; //multicast group address 32-bit
} igmp_pkt_hdr_t;


#endif
