#include "server.hpp"

void Server::create_server()
{

  ip4addr.sin_port = htons(port);
  ip4addr.sin_family = AF_INET;
  ip4addr.sin_addr.s_addr = INADDR_ANY;
  // create socket
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1)
  {
    std::cerr << "socket failed" << std::endl;
    if (socket_fd)
      close(socket_fd);
    exit(-1);
  }
  // set socket options
  int on = 1;
  int checker = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
  if (checker < 0)
  {
    perror("setsockopt() failed");
    if (socket_fd)
      close(socket_fd);
    exit(-6);
  }
  // Set the socket to non-blocking mode
  if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) == -1)
  {
    std::cerr << "fcntl failed" << std::endl;
    if (socket_fd)
      close(socket_fd);
    exit(-2);
  }
  // define this socket with ip and  port
  checker = bind(socket_fd, (struct sockaddr *)&ip4addr, sizeof ip4addr);
  if (checker == -1)
  {
    std::cerr << "bind failed" << std::endl;
    perror(NULL);
    if (socket_fd)
      close(socket_fd);
    exit(-3);
  }
  //listen for connections
  checker = listen(socket_fd, 1024);
  if (checker == -1)
  {
    std::cerr << "listen failed" << std::endl;
    if (socket_fd)
      close(socket_fd);
    exit(-4);
  }
  //add server fd to pollfds
  struct  pollfd k;
  k.fd = socket_fd;
  k.events = POLLIN;
  k.revents = 0;
  fds.push_back(k);
}

void Server::waiting_for_connections(){

  int timeout = 0;
  int checker;
  
  while (true)
  {
    checker = poll(&fds[0], fds.size(), timeout);
    if (checker < 0)
    {
      Logger::error("poll() failed");
      break;
    }
    // if (checker == 0)
    // {
    //   Logger::error("poll() timeout");
    //   break;
    // }
    else
    {
      current_size = fds.size();
      for (int i = 0; i < current_size; i++)
      {
        // if (fds[i].revents == (POLLIN | POLLHUP | POLLERR))
        // {
        //   break;
        // }
        if (fds[i].revents == 0)
        {
          continue;
        }
        if (!(fds[i].revents & POLLIN))
        {
          // std::cout << "Error! revents = " << fds[i].revents << std::endl;
          continue;
        }
        if (fds[i].fd == socket_fd)
        {
            if(is_server_connection() == -1)
              break;
        }
        else
        {
            if (is_client_connection(fds[i], i) == -1)
              break;
        }
      }
    }
  }
}
