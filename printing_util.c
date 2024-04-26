//Mateusz Kitzol 333103

#include <stdio.h>

void print_results(int successful_responses, int total_time) {
    if (successful_responses == 3) {
        long average_response_time = total_time / successful_responses;
        printf("%ldms\n", average_response_time);
    } else if (successful_responses > 0) {
        printf("???\n");
    } else {
        printf("*\n");
    }
}