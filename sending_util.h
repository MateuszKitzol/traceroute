//Mateusz Kitzol 333103

#ifndef SENDING_UTIL_H
#define SENDING_UTIL_H

#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include "checking_util.h"

void create_icmp_header(struct icmp *header, pid_t pid, int seq_num);
int setup_recipient_address(struct sockaddr_in *recipient, char *ip_address);
int send_icmp_packets(int sock_fd, struct sockaddr_in recipient, pid_t my_pid, int initial_seq_num);

#endif