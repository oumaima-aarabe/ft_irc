#include "srv.hpp"
int main(){
    struct sockaddr_in ip4addr;
    struct addrinfo hints, *res;
    socklen_t addr_size;
    char buf[512];
    struct pollfd fds[200];
    int    nfds = 1;
    char buffer[80];
    int current_size = 0;
    int timeout;
    int new_sd;
    int    close_conn;
    int end_server = FALSE;

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
    // Set the socket to non-blocking mode
    if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) == -1) {
        std::cerr << "fcntl failed" << std::endl;
        close(socket_fd);
        exit(-2);
    }
    int data_bind = bind(socket_fd, res->ai_addr, res->ai_addrlen); 
    if (data_bind == -1)
    {
        std::cerr << "bind failed" << std::endl;
        close (socket_fd);
        exit(-3);
    }
    listen(socket_fd, 5);
    if (data_bind == -1){
        std::cerr << "listen failed" << std::endl;
        close(socket_fd);
        exit(-4);
    }
    int new_fd;
    new_fd = accept(socket_fd, NULL, NULL);
    if (data_bind != new_fd){
        std::cerr << "accept failed" << std::endl;
        close(socket_fd);
        exit(-5);
    }
    fds[0].fd = socket_fd;
    fds[0].events = POLLIN;
    timeout = (3 * 60 * 1000);

    while (true){
        data_bind = poll(fds, nfds, timeout);
        if (data_bind < 0){
            std::cout << "poll() failed" << std::endl;
            break;
        }
        if (data_bind == 0){
            std::cout << "poll() timeout" << std::endl;
            break;
        }
        else{
        current_size = nfds;
        for (i = 0; i < current_size; i++)
        {
          if(fds[i].revents == 0){
            continue;
          }
          if(fds[i].revents != POLLIN)
          {
            std::cout << "Error! revents = " << fds[i].revents << std::endl;
            end_server = TRUE;
            break;
          }
          if (fds[i].fd == socket_fd)
          {
            do
            {
              new_sd = accept(listen_sd, NULL, NULL);
              if (new_sd < 0)
              {
                if (errno != EWOULDBLOCK)
                {
                  std::cout << "accept() failed" << std::endl;
                  end_server = TRUE;
                }
                break;
              }
              std::cout << "New incoming connection " << new_sd<< std::endl;
              fds[nfds].fd = new_sd;
              fds[nfds].events = POLLIN;
              nfds++;
            } while (new_sd != -1);
          }
          else{
          close_conn = FALSE;
          do
          {
            rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
            if (rc < 0)
            {
              if (errno != EWOULDBLOCK)
              {
                perror("  recv() failed");
                close_conn = TRUE;
              }
              break;
            }

            if (rc == 0)
            {
              printf("  Connection closed\n");
              close_conn = TRUE;
              break;
            }

            len = rc;
            printf("  %d bytes received\n", len);
            rc = send(fds[i].fd, buffer, len, 0);
            if (rc < 0)
            {
              perror("  send() failed");
              close_conn = TRUE;
              break;
            }

          }while(TRUE);
          if (close_conn)
          {
            close(fds[i].fd);
            fds[i].fd = -1;
            compress_array = TRUE;
          }
        }
        }
        }
        }
}
