#include "srv.hpp"
int main(){
    struct sockaddr_in ip4addr;
    struct addrinfo hints, *res;
    socklen_t addr_size;
    char buf[512];

    memset(&hints, 0, sizeof hints);

    ip4addr.sin_family = AF_INET;
    ip4addr.sin_port = htons(3490);
    inet_pton(AF_INET, "127.0.0.1", &ip4addr.sin_addr.s_addr);
    getaddrinfo("localhost", "3490", &hints, &res);
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd ==-1){
        std::cerr << "socket failed" << std::endl;
        close (socket_fd);
        exit (-1);
    } 
    // int result = connect(socket_fd, res->ai_addr, res->ai_addrlen);
    // if (result == -1)
    // {
    //     std::cerr << "connexion failed" << std::endl;
    //     close (socket_fd);
    //     exit (-1);
    // }
    fcntl(socket_fd, F_SETFL, O_NONBLOCK);
    int data_bind = bind(socket_fd, res->ai_addr, res->ai_addrlen); 
    if (data_bind == -1)
    {
        std::cerr << "bind failed" << std::endl;
        close (socket_fd);
        exit(-2);
    }
    listen(socket_fd, 5);
    if (data_bind == -1){
        std::cerr << "listen failed" << std::endl;
        close(socket_fd);
        exit(-3);
    }
    int new_fd;
    new_fd = accept(socket_fd, NULL, NULL);
    if (data_bind != new_fd){
        std::cerr << "accept failed" << std::endl;
        close(socket_fd);
        exit(-4);
    }

}
