//Mateusz Kitzol 333103

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "checking_util.h"
#include "printing_util.h"
#include "waiting_util.h"
#include "sending_util.h"

#define TIMEOUT 1000
#define MAX_TTL 30

int main(int argc, char *argv[])
{
    //CHECK ARGUMENTS
    if (argc != 2) {
        printf("Usage: %s <destination IP>\n", argv[0]);
        return 1;
    }

    //INITIALIZE SOCKET
	int sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock_fd < 0) {
		fprintf(stderr, "socket error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}

    //INITIALIZE NECESSARY STRUCTS AND VARS
    struct pollfd ps;
    struct sockaddr_in recipient;
    struct timeval send_time, recv_time;
    bool done = false;
    pid_t my_pid = getpid();
    int seq_num = 0;
    initialize_pollfd(&ps, sock_fd);
    
    //SET UP RECIPIENT ADDRESS
    if(setup_recipient_address(&recipient, argv[1]) < 0)
        return EXIT_FAILURE;
    
    //HANDLE PACKETS LOGIC FOR EACH TTL
    for (int ttl = 1; ttl <= MAX_TTL && !done; ttl++) {
        long total_time = 0;
        int successful_responses = 0;
        char prev_ip[20] = "";
        char second_prev_ip[20] = "";

        //SET SOCKET OPTIONS
        if(setsockopt (sock_fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int)) < 0){
            perror("setsockopt function error");
            return EXIT_FAILURE;
        }

        //SEND PACKETS
        printf("%d. ", ttl);
        gettimeofday(&send_time, NULL);
        if (send_icmp_packets(sock_fd, recipient, my_pid, seq_num) == -1) {
            fprintf(stderr, "Failed to send ICMP packets.\n");
            return EXIT_FAILURE;
        }
        seq_num += 3;

        //HANDLE PACKETS RECEIVING
        while(successful_responses != 3){
            int ready = poll (&ps, 1, TIMEOUT);

            if(ready == -1){
                perror("poll error (ready == -1)");
                close(sock_fd);
                return EXIT_FAILURE;
            } else if(ready == 0 || ps.revents != POLLIN) {
                break;
            } else if(ps.revents == POLLIN){
                //RECEIVE PACKETS
                struct sockaddr_in sender;
                socklen_t sender_len = sizeof(sender);
                u_int8_t buffer[IP_MAXPACKET];   
                if (recvfrom (sock_fd, buffer, IP_MAXPACKET, 0, (struct sockaddr*)&sender, &sender_len) < 0) {
                    fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
                    return EXIT_FAILURE;
                }

                //HANDLE ONLY SUITABLE PACKETS
                if(suitable_response_to_handle(buffer, seq_num, my_pid)){
                    gettimeofday(&recv_time, NULL);
                    total_time += time_diff(send_time, recv_time);
                    successful_responses++; 
                    
                    char sender_ip_str[20];

                    if(inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str)) == NULL){
                        perror("inet_ntop");
                        close(sock_fd);
                        return EXIT_FAILURE;
                    }

                    if (strcmp(prev_ip, sender_ip_str) != 0 && strcmp(second_prev_ip, sender_ip_str) != 0) {
                        printf("%s ", sender_ip_str);
                        strncpy(second_prev_ip, prev_ip, sizeof(second_prev_ip));
                        strncpy(prev_ip, sender_ip_str, sizeof(prev_ip));
                    }

                    if (successful_responses == 3 && strcmp(sender_ip_str, argv[1]) == 0) {
                        done = true;
                        break;
                    }
                }
            } else {
                break;
            }
        }

        //PRINT RESULTS
        print_results(successful_responses, total_time);
    }

    close(sock_fd);
    return 0;
}