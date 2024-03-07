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

#define DHCP_MAX_SIZE 576

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
	char sname[64]; /* Optional server host name. */
	char file[128]; /* Boot file name. */
	char options[312]; /* Optional parameters field. */
} dhcp_message;
