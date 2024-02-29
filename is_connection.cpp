#include "server.hpp"

int Server::is_server_connection(){
  //accept connection from server and add it to fds 
  int new_sd = accept(socket_fd, NULL, NULL);
  if (new_sd < 0)
  {
    if (errno != EWOULDBLOCK)
    {
      Logger::error("accept() failed");
    }
    return(-1);
  }
  Logger::info("New incoming connection " + to_string(new_sd));
  struct  pollfd k;
  k.fd = new_sd;
  k.events = POLLIN;
  k.revents = 0;
  fds.push_back(k);

  return 0;
}

int Server::is_client_connection(struct pollfd fds){

  char buffer[1024] = {0};

  //read the buffer from client (user || new connection)
  int checker = recv(fds.fd, buffer, sizeof(buffer), 0);

  if (checker < 0)
  {
    Logger::error("recv() failed");
    return -1;
  }

  if (checker == 0)
  {
    printf("  Connection closed\n");
    std::map<int, Client>::iterator it = users.find(fds.fd);
    if (it != users.end()) {
        users.erase(it);
    }
    it = connections.find(fds.fd);
    if (it != connections.end()) {
      connections.erase(it);
      if (fds.fd)
        close(fds.fd);
    }
    return -1;
  }

  //handle cntrl+D
  std::string content = buffer;
  if (content.find('\n') == std::string::npos){
    if (users.find(fds.fd) != users.end())
    {
      users[fds.fd].buffer += buffer;
      return 0;
    }
    else if (connections.find(fds.fd) != connections.end())
    {
      connections[fds.fd].buffer += buffer;
      return 0;
    }
    else
    {
      connections.insert(std::pair<int, Client>(fds.fd, Client(fds, "", "", "", content)));
      return 0;
    }
  }

    //client authenticated , exist in users
    if (users.find(fds.fd) != users.end()){
    // parsing and executing cmnds
    std::vector<std::string> cmndBuffer;
    int clientFd = users[fds.fd].fds.fd; // to avoid in case the client closes the connection while processing the request
    
    //split by \r\n (from limechat) in case of multiple commands sent by client in quick succession
    if (content.find('\r') != std::string::npos){
      cmndBuffer = split(content, "\r\n");       
    }
    //split by \n (from nc)
    else if (content.find('\n') != std::string::npos){
      cmndBuffer = split(content, "\n");       
    }
    executeCommands(cmndBuffer, clientFd);
  }
  else
  {
    //new client
    if (connections.find(fds.fd) == connections.end())
    {
      connections.insert(std::pair<int, Client>(fds.fd, Client(fds, "", "", "", content)));
    }
    else
    {
      connections[fds.fd].buffer += content;
    }
    if (connections[fds.fd].buffer.find('\r') != std::string::npos)
      parse_buffer_limechat(connections[fds.fd]); //parse buffer with back slach r
    else
    {
      parse_buffer_nc(connections[fds.fd]); //parse buffer without backslash r
    }
    connections[fds.fd].buffer = "";
  }
  return 0;
}
