#include "server.hpp"

int main()
{
  struct sockaddr_in ip4addr;
  char buffer[200];
  int current_size = 0;
  int timeout;
  int new_sd;
  int close_conn;
  int end_server = false;
  int compress_array = false;
  int on = 1;
  struct sockaddr_storage their_addr;
  std::vector<struct pollfd> fds; 

  ip4addr.sin_port = htons(3490);
  ip4addr.sin_family = AF_INET;
  ip4addr.sin_addr.s_addr = INADDR_ANY;
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1)
  {
    std::cerr << "socket failed" << std::endl;
    close(socket_fd);
    exit(-1);
  }
  int data_bind = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
  if (data_bind < 0)
  {
    perror("setsockopt() failed");
    close(socket_fd);
    exit(-6);
  }
  // Set the socket to non-blocking mode
  if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) == -1)
  {
    std::cerr << "fcntl failed" << std::endl;
    close(socket_fd);
    exit(-2);
  }
  data_bind = bind(socket_fd, (struct sockaddr *)&ip4addr, sizeof ip4addr);
  if (data_bind == -1)
  {
    std::cerr << "bind failed" << std::endl;
    perror(NULL);
    close(socket_fd);
    exit(-3);
  }
  data_bind = listen(socket_fd, 10);
  if (data_bind == -1)
  {
    std::cerr << "listen failed" << std::endl;
    close(socket_fd);
    exit(-4);
  }
  struct  pollfd k;
  k.fd = socket_fd;
  k.events = POLLIN;
  fds.push_back(k);
  timeout = (3 * 60 * 1000);
  int i;
  
  while (true)
  {
    data_bind = poll(fds.data(), fds.size(), timeout);
    if (data_bind < 0)
    {
      std::cout << "poll() failed" << std::endl;
      break;
    }
    if (data_bind == 0)
    {
      std::cout << "poll() timeout" << std::endl;
      break;
    }
    else
    {
      current_size = fds.size();
      for (i = 0; i < current_size; i++)
      {
        if (fds[i].revents == 0 || fds[i].revents == 17)
        {
          continue;
        }
        if (fds[i].revents != POLLIN)
        {
          std::cout << "Error! revents = " << fds[i].revents << std::endl;
          end_server = true;
          break;
        }
        if (fds[i].fd == socket_fd)
        {
          while (true)
          {
            new_sd = accept(socket_fd, NULL, NULL);
            if (new_sd < 0)
            {
              if (errno != EWOULDBLOCK)
              {
                std::cout << "accept() failed" << std::endl;
                end_server = true;
              }
              break;
            }
            std::cout << "New incoming connection " << new_sd << std::endl;
            struct  pollfd k;
            k.fd = new_sd;
            k.events = POLLIN;
            fds.push_back(k);
          }
        }
        else
        {
          close_conn = false;
          while (true)
          {
            std::cout << "buf <"<<buffer<< ">" << std::endl;
            data_bind = recv(fds[i].fd, buffer, sizeof(buffer), 0);
            if (data_bind < 0)
            {
              if (errno != EWOULDBLOCK)
              {
                perror("  recv() failed");
                close_conn = true;
              }
              break;
            }

            if (data_bind == 0)
            {
              // delete users
              printf("  Connection closed\n");
              close_conn = true;
              break;
            }
            int len = data_bind;
            //check if the user authenticated...
            bool authenticated = false;
            std::map<std::string , std::string> key_val;
            std::string authData(buffer, len);
            std::size_t found = authData.find("\r\n");
            if (found != std::string::npos) {
                std::string username = authData.substr(0, found);
                authData = authData.substr(found + 2);
                found = authData.find("\r\n");
                if (found != std::string::npos) {
                    std::string nickname = authData.substr(0, found);
                    authData = authData.substr(found + 2);

                    found = authData.find("\r\n");
                    if (found != std::string::npos) {
                        std::string password = authData.substr(0, found);
                        authData = authData.substr(found + 2);
                        found = password.find(" ");
                        if (found != std::string::npos){
                          std::string password_auth = password.substr(0, found);


                        }
                    }
                }
                
        }
            data_bind = send(fds[i].fd, buffer, len, 0);
            if (data_bind < 0)
            {
              std::cout << "send() failed" << std::endl;
              close_conn = true;
              break;
            }
          }
          if (close_conn)
          {
            close(fds[i].fd);
            fds[i].fd = -1;
            compress_array = true;
          }
        }
      }
    }
  }
}
