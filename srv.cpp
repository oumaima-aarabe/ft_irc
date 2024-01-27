#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <netdb.h>

int main(int ac, char **av){
    struct addrinfo hints, *res;
    int socket_fd;

    int server = socket(AF_INET6, SOCK_STREAM, 0);
    if (server < 0){
        std::cerr << "socket failed" << std::endl;
        exit (-1);
    }
    
    int bind = bind();
}
