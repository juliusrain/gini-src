
#include "igmp.h"
#include <stdio.h>

void IGMPProcessPacket(gpacket_t *in_pkt)
{

}

void IGMPBroadcast(){
	int count = 100, i;
	for(i = 0; i < count; i++){
		sleep(3);
		printf("what? %d\n", i);
	}
}

