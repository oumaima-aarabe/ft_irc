#include "../headers/Server.hpp"

int Server::is_server_connection(){
  //accept connection from server and add it to fds 
  socklen_t ip4addrSize = sizeof(ip4addr);
  int new_sd = accept(socket_fd, (struct sockaddr*)&ip4addr, &ip4addrSize);
  if (new_sd < 0)
  {
    if (errno != EWOULDBLOCK)
    {
      Logger::error("accept() failed");
    }
    return(-1);
  }
  hostNames[new_sd] = inet_ntoa(ip4addr.sin_addr);
  Logger::info("New incoming connection " + to_string(new_sd));
  struct  pollfd k;
  k.fd = new_sd;
  k.events = POLLIN;
  k.revents = 0;
  fds.push_back(k);

  return 0;
}

int Server::is_client_connection(struct pollfd fd_struct, int i){

  char buffer[1024] = {0};

  //read the buffer from client (user || new connection)
  int checker = recv(fd_struct.fd, buffer, sizeof(buffer), 0);
  if (checker <= 0)
  {
    //handle cntrl+C
    Logger::info("Connection closed");
    std::map<int, Client>::iterator it = connections.find(fd_struct.fd);
    if (it != connections.end()){
      connections.erase(it);
    }
    it = users.find(fd_struct.fd);
    if (it != users.end())
    {
      commandInfo cmd = {"SIG", std::vector<std::string>()};
      ft_quit(cmd, *this, users[fd_struct.fd]);
      users.erase(it);
    }
    fds.erase(fds.begin() + i);
    close(fd_struct.fd);
    return -1;
  }

  //handle cntrl+D
  std::string content = buffer;
  if (content.find('\n') == std::string::npos)
  {
    if (users.find(fd_struct.fd) != users.end())
    {
      users[fd_struct.fd].buffer += content;
      return 0;
    }
    else if (connections.find(fd_struct.fd) != connections.end())
    {
      connections[fd_struct.fd].buffer += content;
      return 0;
    }
    else
    {
      connections.insert(std::pair<int, Client>(fd_struct.fd, Client(fd_struct, "", "", "", "", content)));
      return 0;
    }
  }

    //client authenticated , exist in users
    if (users.find(fd_struct.fd) != users.end())
    {
      // parsing and executing cmnds
      std::vector<std::string> cmndBuffer;
      // to avoid in case the client closes the connection while processing the request
      int clientFd = users[fd_struct.fd].fds.fd;
      // if there is a buffer from previous recv stopped by ^D
      content = users[fd_struct.fd].buffer + content;
      users[fd_struct.fd].buffer = "";
      //split by \r\n (from limechat) in case of multiple commands sent by client in quick succession
      if (content.find('\r') != std::string::npos)
        cmndBuffer = split(content, "\r\n");
      //split by \n (from nc)
      else if (content.find('\n') != std::string::npos)
        cmndBuffer = split(content, "\n");
      executeCommands(cmndBuffer, clientFd);
  }
  else
  {
    //new client
    if (connections.find(fd_struct.fd) == connections.end())
      connections.insert(std::pair<int, Client>(fd_struct.fd, Client(fd_struct, "", "", "", "", content)));
    else
      connections[fd_struct.fd].buffer += content;
    if (connections[fd_struct.fd].buffer.find('\r') != std::string::npos)
      parse_buffer_limechat(connections[fd_struct.fd]); //parse buffer with back slach r
    else
      parse_buffer_nc(connections[fd_struct.fd]); //parse buffer without backslash r
    connections[fd_struct.fd].buffer = "";
  }
  return 0;
}
