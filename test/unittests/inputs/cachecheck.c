#include <stdint.h>
#include <stdlib.h>

#define ETH_ADDR_LEN 6

struct beui16 { uint16_t x; } __attribute__((packed));
struct beui32 { uint32_t x; } __attribute__((packed));
struct beui64 { uint64_t x; } __attribute__((packed));
typedef struct beui16 beui16_t;
typedef struct beui32 beui32_t;
typedef struct beui64 beui64_t;

struct eth_addr {
  uint8_t addr[ETH_ADDR_LEN];
} __attribute__ ((packed));

struct eth_hdr {
  struct eth_addr dest;
  struct eth_addr src;
  beui16_t type;
} __attribute__ ((packed));

typedef beui32_t ip_addr_t;

struct ip_hdr {
  /* version / header length */
  uint8_t _v_hl;
  /* type of service */
  uint8_t _tos;
  /* total length */
  beui16_t len;
  /* identification */
  beui16_t id;
  /* fragment offset field */
  beui16_t offset;
  /* time to live */
  uint8_t ttl;
  /* protocol*/
  uint8_t proto;
  /* checksum */
  uint16_t chksum;
  /* source and destination IP addresses */
  ip_addr_t src;
  ip_addr_t dest;
} __attribute__ ((packed));

struct tcp_hdr {
  beui16_t src;
  beui16_t dest;
  beui32_t seqno;
  beui32_t ackno;
  uint16_t _hdrlen_rsvd_flags;
  beui16_t wnd;
  uint16_t chksum;
  beui16_t urgp;
} __attribute__((packed));

struct pkt_tcp {
  struct eth_hdr eth;
  struct ip_hdr  ip;
  struct tcp_hdr tcp;
} __attribute__ ((packed));

struct flow_key {
  ip_addr_t local_ip;
  ip_addr_t remote_ip;
  beui16_t local_port;
  beui16_t remote_port;
} __attribute__((packed));

int cache_fn1(int * a) {
  struct pkt_tcp * p = (struct pkt_tcp *) malloc (sizeof(struct pkt_tcp));
  struct flow_key key;
  key.local_ip = p->ip.dest;
  key.remote_ip = p->ip.src;
  key.local_port = p->tcp.dest;
  key.remote_port = p->tcp.src;

  free(p);

  return 0;
}
