//Mateusz Kitzol 333103

#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include "checking_util.h"

void create_icmp_header(struct icmp *header, pid_t pid, int seq_num) {
    header->icmp_type = ICMP_ECHO;
    header->icmp_code = 0;
    header->icmp_hun.ih_idseq.icd_id = htons(pid);
    header->icmp_hun.ih_idseq.icd_seq = htons(seq_num);
    header->icmp_cksum = 0;
    header->icmp_cksum = compute_icmp_checksum((u_int16_t*)header, sizeof(*header));
}

int setup_recipient_address(struct sockaddr_in *recipient, char *ip_address) {
    bzero(recipient, sizeof(*recipient));
    recipient->sin_family = AF_INET;
    if (inet_pton(AF_INET, ip_address, &recipient->sin_addr) < 1) {
        perror("inet_pton_error");
        return -1;
    }
    return 0;
}

int send_icmp_packets(int sock_fd, struct sockaddr_in recipient, pid_t my_pid, int initial_seq_num) {
    struct icmp header;
    int seq_num = initial_seq_num;

    for (int i = 0; i < 3; i++) {
        seq_num++;
        create_icmp_header(&header, my_pid, seq_num);
        ssize_t bytes_sent = sendto(sock_fd, &header, sizeof(header), 0, (struct sockaddr*)&recipient, sizeof(recipient));
        if (bytes_sent == -1) {
            perror("sendto error");
            return -1;
        }
    }

    return 0;
}