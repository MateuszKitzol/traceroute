//Mateusz Kitzol 333103

#ifndef CHECKING_UTIL_H
#define CHECKING_UTIL_H

#include <assert.h>
#include <stdint.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <stdbool.h>

u_int16_t compute_icmp_checksum (const void *buff, int length);
bool suitable_response_to_handle(u_int8_t *buffer, int seq_num, pid_t my_pid);

#endif