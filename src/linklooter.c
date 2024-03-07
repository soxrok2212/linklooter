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

#include <getopt.h>

#include "dhcp.c"
#include "version.h"
#include "includes/linklooter.h"

static const char *option_string = "a:hv";
static const struct option long_options[] = {
	{ "attack-mode",	required_argument,	0, 'a' },
	{ "help",			no_argument,		0, 'h' },
	{ "version",		no_argument,		0, 'v' },
	{ 0, 0, 0, 0 }
};

void usage() {
	printf(
			"\n"
			" LinkLooter %s: a link layer attack tool\n"
			" Copyright (c) 2024, soxrok2212 <soxrok2212@gmail.com>\n"
			"\n"
			" Required Arguments:\n"
			"\n"
			"	-a, --attack-mode	: Attack mode to use\n"
			"\n"
			" Optional Arguments:\n"
			"\n"
			"	-h, --help			: Display help\n"
			"	-v, --version		: Display version\n"
			"\n", LONG_VERSION
	      );
	exit(0);
}

int process_args() {
	send_dhcp_discover();
	return 0;
}

int main(int argc, char *argv[]) {
	if (argc < 2)
		usage();
	int opt = 0;
	int long_index = 0;
	opt = getopt_long(argc, argv, option_string, long_options, &long_index);
	char *attack_mode;
	while (opt != -1) {
		switch (opt) {
			case 'a':
					attack_mode = optarg;
				break;
			case 'h':
				usage();
				break;
			default:
				usage();
				break;
		}
		opt = getopt_long(argc, argv, option_string, long_options, &long_index);
	}
	process_args();
	return 0;
}