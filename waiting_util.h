//Mateusz Kitzol 333103

#ifndef WAITING_UTIL_H
#define WAITING_UTIL_H

#include <poll.h>
#include <sys/time.h>

void initialize_pollfd(struct pollfd *ps, int sock_fd);
long time_diff(struct timeval x , struct timeval y);

#endif