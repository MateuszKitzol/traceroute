//Mateusz Kitzol 333103

#include <assert.h>
#include <stdint.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <stdbool.h>

u_int16_t compute_icmp_checksum (const void *buff, int length){
    const u_int16_t* ptr = buff;
    u_int32_t sum = 0;
    assert (length % 2 == 0);
    for (; length > 0; length -= 2)
        sum += *ptr++;
    sum = (sum >> 16U) + (sum & 0xffffU);
    return (u_int16_t)(~(sum + (sum >> 16U)));
}

bool suitable_response_to_handle(u_int8_t *buffer, int seq_num, pid_t my_pid) {
    int seq_num_min = seq_num - 2;
    int seq_num_max = seq_num;

    struct ip* ip_header = (struct ip*) buffer;
    u_int8_t* icmp_packet = buffer + 4 * ip_header->ip_hl;
    struct icmp* icmp_header = (struct icmp*) icmp_packet;

    if (icmp_header->icmp_type == ICMP_TIME_EXCEEDED) {
        struct ip* orig_ip_header = (struct ip*)(icmp_packet + ICMP_MINLEN);
        icmp_header = (struct icmp*)((unsigned char*)orig_ip_header + 4*orig_ip_header->ip_hl);
    } else if (icmp_header->icmp_type != ICMP_ECHOREPLY){
        return false;
    }

    uint16_t received_id = ntohs(icmp_header->icmp_hun.ih_idseq.icd_id);
    uint16_t received_seq = ntohs(icmp_header->icmp_hun.ih_idseq.icd_seq);

    if (received_id == my_pid && received_seq >= seq_num_min && received_seq <= seq_num_max) {
        return true;
    }

    return false;
}