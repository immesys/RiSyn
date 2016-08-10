/*
 * Copyright (C) 2016 University of California, Berkeley
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     net_gnrc_tcp_freebsd
 * @{
 *
 * @file
 * @brief       Internal API to the TCP frontend for GNRC
 *
 * @author      Sam Kumar <samkumar@berkeley.edu>
 *
 * This file describes the API that the TCP frontend presents to the TCP
 * protocol logic. The protocol logic interacts with other parts of the
 * kernel (GNRC, xtimer, etc.) via this API.
 * @}
 */

#ifndef GNRC_TCP_FREEBSD_INTERNAL_H_
#define GNRC_TCP_FREEBSD_INTERNAL_H_

#include <stdio.h>
#include "bsdtcp/ip6.h"
#include "bsdtcp/tcp.h"
#include "bsdtcp/tcp_fsm.h"
#include "bsdtcp/tcp_timer.h"
#include "bsdtcp/tcp_var.h"
#include "net/gnrc/pkt.h"

#define GNRC_TCP_FREEBSD_NUM_ACTIVE_SOCKETS 1
#define GNRC_TCP_FREEBSD_NUM_PASSIVE_SOCKETS 1

#define TIMERS_PER_ACTIVE_SOCKET 4

/* Possible return value from tcp_input. */
#define RELOOKUP_REQUIRED -1

#define IANA_TCP PROTNUM_TCP

/*struct tcp_hdr {
  uint16_t srcport;
  uint16_t dstport;
  uint32_t seqno;
  uint32_t ackno;
  uint8_t offset;
  uint8_t flags;
  uint16_t window;
  uint16_t chksum;
  uint16_t urgent;
};*/

#define hz 1000 // number of ticks per second
#define MICROS_PER_TICK 1000 // number of microseconds per tick

#define FRAMES_PER_SEG 3
#define FRAMECAP_6LOWPAN (122 - 22 - 12) // Fragmentation limit: maximum frame size of the IP and TCP headers

#define COMPRESSED_IP6HDR_SIZE (2 + 1 + 1 + 16 + 8) // IPHC header (2) + Next header (1) + Hop count (1) + Dest. addr (16) + Src. addr (8)

#define SIG_CONN_ESTABLISHED 0x01
#define SIG_RECVBUF_NOTEMPTY 0x02
#define SIG_RCVD_FIN         0x04

#define GNRC_TCP_FREEBSD_NUM_TIMERS (GNRC_TCP_FREEBSD_NUM_ACTIVE_SOCKETS * TIMERS_PER_ACTIVE_SOCKET)

#define CONN_LOST_NORMAL 0 // errno of 0 means that the connection closed gracefully

struct ip6_packet {
    // Dummy for now
    struct ip6_hdr ip6_hdr;
    struct ip_iovec* ip6_data;
};

void send_message(gnrc_pktsnip_t* pkt);
uint32_t get_ticks(void);
uint32_t get_millis(void);
void set_timer(struct tcpcb* tcb, uint8_t timer_id, uint32_t delay);
void stop_timer(struct tcpcb* tcb, uint8_t timer_id);
void accepted_connection(struct tcpcb_listen* tpl, struct in6_addr* addr, uint16_t port);
void connection_lost(struct tcpcb* tcb, uint8_t errnum);

uint16_t get_tcp_checksum(gnrc_pktsnip_t *tcpsnip, gnrc_pktsnip_t *ip6snip);

#endif // GNRC_TCP_FREEBSD_INTERNAL_H_
