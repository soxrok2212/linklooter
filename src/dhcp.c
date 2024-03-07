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
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "includes/dhcp.h"

/*
 * Create DHCP Discover message
 */
dhcp_message create_dhcp_discover() {
    dhcp_message dhcp_discover; /* Create DHCP message */
    srand(time(NULL));

    /* Convert MAC to uint8_t array */
    char *temp_mac = "00:11:22:33:44:55"; /* Temporary MAC */
    uint8_t mac[6];
    mac_to_array(temp_mac, mac);

    /* Populate DHCP request message */
    dhcp_discover.op = BOOTREQUEST;
    dhcp_discover.htype = HTYPE_ETHER;
    dhcp_discover.hlen = 6; /* MAC address is 6 bytes */
    dhcp_discover.hops = 0;
    dhcp_discover.xid = htonl(rand()); /* Does not need to be secure */
    dhcp_discover.secs = htons(0);
    dhcp_discover.flags = htons(0x8000); /* MSB is 1 since we don't have an IP */
    dhcp_discover.ciaddr.s_addr = htons(0); /* No IP yet */
    dhcp_discover.yiaddr.s_addr = htons(0); /* No IP yet */
    dhcp_discover.siaddr.s_addr = htons(0); /* No IP yet */
    dhcp_discover.giaddr.s_addr = htons(0); /* No IP yet */
    memcpy(dhcp_discover.chaddr, mac, sizeof(mac)); /* Copy MAC to DHCP */
    memset(dhcp_discover.sname, 0, DHCP_SNAME_LEN); /* Zero sname */
    memset(dhcp_discover.file, 0, DHCP_FILE_LEN); /* Zero file */
    memset(dhcp_discover.options, 0, DHCP_OPTION_LEN); /* Zero options */

    /* Set DHCP option 53 (discover) */
    dhcp_discover.options[0] = 53; /* DHCP Discover */
    dhcp_discover.options[1] = 1; /* Length 1 */
    dhcp_discover.options[2] = DHCPDISCOVER; /* 1 */
    return dhcp_discover;
}

/*
 * Send DHCP Discover
 */
int send_dhcp_discover() {
    int sockfd;
    struct sockaddr_in sin;
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("255.255.255.255"); /* Broadcast */
    sin.sin_port = htons(67); /* DHCP server port */
    dhcp_message dhcp_discover = create_dhcp_discover();
    if (sendto(sockfd, &dhcp_discover, sizeof(dhcp_message), 0, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("bye");
    }
    close(sockfd);
}
