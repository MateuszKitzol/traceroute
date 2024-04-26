//Mateusz Kitzol 333103

#include <poll.h>
#include <sys/time.h>

void initialize_pollfd(struct pollfd *ps, int sock_fd) {
    ps->fd = sock_fd;
    ps->events = POLLIN;
}

long time_diff(struct timeval x , struct timeval y)
{
    long x_ms , y_ms , diff;
    x_ms = x.tv_sec*1000 + x.tv_usec/1000;
    y_ms = y.tv_sec*1000 + y.tv_usec/1000;
    diff = y_ms - x_ms;
    return diff;
}