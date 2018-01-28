#ifndef _BASE_H_
#define _BASE_H_

#include "pcap/pcap.h"

struct EtherHeader

{
	u_int8_t  ether_dhost[6];      /* destination eth addr */
	u_int8_t  ether_shost[6];      /* source ether addr    */
	u_int16_t ether_type;          /* packet type ID field */
};



/* 4 bytes IP address */

typedef struct IPAddress{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
};



/* IPv4 header */

typedef struct IPHeader{
	u_char  ver_ihl;        // Version (4 bits) + Internet header length (4 bits)
	u_char  tos;            // Type of service 
	u_short tlen;           // Total length 
	u_short identification; // Identification
	u_short flags_fo;       // Flags (3 bits) + Fragment offset (13 bits)
	u_char  ttl;            // Time to live
	u_char  proto;          // Protocol
	u_short crc;            // Header checksum
	IPAddress  saddr;      // Source address
	IPAddress  daddr;      // Destination address
	u_int   op_pad;         // Option + Padding
};



/* UDP header*/

typedef struct UDPHeader{
	u_short sport;          // Source port
	u_short dport;          // Destination port
	u_short len;            // Datagram length
	u_short crc;            // Checksum
};



struct TCPHeader

{
	u_int16_t th_sport;         /* source port */
	u_int16_t th_dport;         /* destination port */
	u_int32_t th_seq;             /* sequence number */
	u_int32_t th_ack;             /* acknowledgement number */
	u_int16_t th_len_resv_code;	 //   Datagram   length and reserved code
	u_int16_t th_win;           /* window */
	u_int16_t th_sum;           /* checksum */
	u_int16_t th_urp;           /* urgent pointer */
};

#endif