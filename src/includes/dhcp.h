/*
 * LinkLooter: a layer 2 attack tool
 *
 * MIT License
 *
 * Copyright (c) 2024 soxrok2212
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <arpa/inet.h>
#include <stdint.h>

#define DHCP_UDP_OVERHEAD	(14 + 20 + 8) /* Ethernet header + IP header + UDP header */
#define DHCP_SNAME_LEN		64
#define DHCP_FILE_LEN		128
#define DHCP_FIXED_NON_UDP	236
#define DHCP_FIXED_LEN		(DHCP_FIXED_NON_UDP + DHCP_UDP_OVERHEAD) /* Everything but options */
#define DHCP_MTU_MAX		1500
#define DHCP_OPTION_LEN		(DHCP_MTU_MAX - DHCP_FIXED_LEN)
#define BOOTP_MIN_LEN		300
#define DHCP_MIN_LEN		548

/* DHCP message structure */
typedef struct {
	uint8_t op; /* Message op code / message type. */
	uint8_t htype; /* Hardware address type. */
	uint8_t hlen; /* Hardware address length */
	uint8_t hops; /* Client sets to zero */
	uint32_t xid;/* Transaction ID, a random number chosen by the client */
	uint16_t secs; /* Filled by client, seconds elapsed since start. */
	uint16_t flags; /* Flags. */
	struct in_addr ciaddr; /* Client IP address. */
	struct in_addr yiaddr; /* 'Your' (client) IP address. */
	struct in_addr siaddr; /* IP address of next server. */
	struct in_addr giaddr; /* Relay agent IP address. */
	uint8_t chaddr[16]; /* Client hardware address. */
	char sname[DHCP_SNAME_LEN]; /* Optional server host name. */
	char file[DHCP_FILE_LEN]; /* Boot file name. */
	char options[DHCP_OPTION_LEN]; /* Optional parameters field. */
} dhcp_message;

/* DHCP message types */
#define BOOTREQUEST	1
#define BOOTREPLY	2

/* DHCP hardware type (htype) values */
#define HTYPE_ETHER	1 /* Ethernet 10Mbps */

/* DHCP cookie */
#define DHCP_OPTIONS_COOKIE	htonl(0x63825363)

/* DHCP options */
#define DHO_PAD								0
#define DHO_SUBNET_MASK						1
#define DHO_TIME_OFFSET						2
#define DHO_ROUTERS							3
#define DHO_TIME_SERVERS					4
#define DHO_NAME_SERVERS					5
#define DHO_DOMAIN_NAME_SERVERS				6
#define DHO_LOG_SERVERS						7
#define DHO_COOKIE_SERVERS					8
#define DHO_LPR_SERVERS						9
#define DHO_IMPRESS_SERVERS					10
#define DHO_RESOURCE_LOCATION_SERVERS		11
#define DHO_HOST_NAME						12
#define DHO_BOOT_SIZE						13
#define DHO_MERIT_DUMP						14
#define DHO_DOMAIN_NAME						15
#define DHO_SWAP_SERVER						16
#define DHO_ROOT_PATH						17
#define DHO_EXTENSIONS_PATH					18
#define DHO_IP_FORWARDING					19
#define DHO_NON_LOCAL_SOURCE_ROUTING		20
#define DHO_POLICY_FILTER					21
#define DHO_MAX_DGRAM_REASSEMBLY			22
#define DHO_DEFAULT_IP_TTL					23
#define DHO_PATH_MTU_AGING_TIMEOUT			24
#define DHO_PATH_MTU_PLATEAU_TABLE			25
#define DHO_INTERFACE_MTU					26
#define DHO_ALL_SUBNETS_LOCAL				27
#define DHO_BROADCAST_ADDRESS				28
#define DHO_PERFORM_MASK_DISCOVERY			29
#define DHO_MASK_SUPPLIER					30
#define DHO_ROUTER_DISCOVERY				31
#define DHO_ROUTER_SOLICITATION_ADDRESS		32
#define DHO_STATIC_ROUTES					33
#define DHO_TRAILER_ENCAPSULATION			34
#define DHO_ARP_CACHE_TIMEOUT				35
#define DHO_IEEE802_3_ENCAPSULATION			36
#define DHO_DEFAULT_TCP_TTL					37
#define DHO_TCP_KEEPALIVE_INTERVAL			38
#define DHO_TCP_KEEPALIVE_GARBAGE			39
#define DHO_NIS_DOMAIN						40
#define DHO_NIS_SERVERS						41
#define DHO_NTP_SERVERS						42
#define DHO_VENDOR_ENCAPSULATED_OPTIONS		43
#define DHO_NETBIOS_NAME_SERVERS			44
#define DHO_NETBIOS_DD_SERVER				45
#define DHO_NETBIOS_NODE_TYPE				46
#define DHO_NETBIOS_SCOPE					47
#define DHO_FONT_SERVERS					48
#define DHO_X_DISPLAY_MANAGER				49
#define DHO_DHCP_REQUESTED_ADDRESS			50
#define DHO_DHCP_LEASE_TIME					51
#define DHO_DHCP_OPTION_OVERLOAD			52
#define DHO_DHCP_MESSAGE_TYPE				53
#define DHO_DHCP_SERVER_IDENTIFIER			54
#define DHO_DHCP_PARAMETER_REQUEST_LIST		55
#define DHO_DHCP_MESSAGE					56
#define DHO_DHCP_MAX_MESSAGE_SIZE			57
#define DHO_DHCP_RENEWAL_TIME				58
#define DHO_DHCP_REBINDING_TIME				59
#define DHO_VENDOR_CLASS_IDENTIFIER			60
#define DHO_DHCP_CLIENT_IDENTIFIER			61
#define DHO_NWIP_DOMAIN_NAME				62
#define DHO_NWIP_SUBOPTIONS					63
#define DHO_USER_CLASS						77
#define DHO_FQDN							81
#define DHO_DHCP_AGENT_OPTIONS				82
#define DHO_AUTHENTICATE					90  /* RFC3118, was 210 */
#define DHO_CLIENT_LAST_TRANSACTION_TIME	91
#define DHO_ASSOCIATED_IP					92
#define DHO_V6_ONLY_PREFERRED				108 /* RFC8925 */
#define DHO_SUBNET_SELECTION				118 /* RFC3011! */
#define DHO_DOMAIN_SEARCH					119 /* RFC3397 */
#define DHO_VIVCO_SUBOPTIONS				124
#define DHO_VIVSO_SUBOPTIONS				125
#define DHO_END								255

/* DHCP message types */
#define DHCPDISCOVER		1
#define DHCPOFFER			2
#define DHCPREQUEST			3
#define DHCPDECLINE			4
#define DHCPACK				5
#define DHCPNAK				6
#define DHCPRELEASE			7
#define DHCPINFORM			8
#define DHCPLEASEQUERY		10
#define DHCPLEASEUNASSIGNED	11
#define DHCPLEASEUNKNOWN	12
#define DHCPLEASEACTIVE		13

/* DHCP string to MAC array */
void *mac_to_array(char *mac_str, uint8_t *mac) {
	if (sscanf(mac_str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx%*c", 
		&mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]) < 6) {
		fprintf(stderr, "E: unable to parse MAC address [%s]", mac_str);
		exit(1);
		}
}