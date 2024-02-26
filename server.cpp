#include "server.hpp"

Server::Server(unsigned int port, std::string password){
  char hostnam[256];
  if (gethostname(hostnam, sizeof(hostnam)) == -1){
    Logger::error("Error getting hostname");
    return;
  }
  this->port = port;
  this->password = password;
  this->hostname = hostnam;
}

Server::~Server(){

}

void Server::create_server()
{
  int on = 1;

  ip4addr.sin_port = htons(port);
  ip4addr.sin_family = AF_INET;
  ip4addr.sin_addr.s_addr = INADDR_ANY;
  // create socket
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1)
  {
    Logger::error("socket() failed");
    close(socket_fd);
    exit(-1);
  }
  // set socket options
  int checker = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
  if (checker < 0)
  {
    Logger::error("setsockopt() failed");
    close(socket_fd);
    exit(-6);
  }
  // Set the socket to non-blocking mode
  if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) == -1)
  {
    Logger::error("fcntl() failed");
    close(socket_fd);
    exit(-2);
  }
  // define this socket with ip and  port
  checker = bind(socket_fd, (struct sockaddr *)&ip4addr, sizeof ip4addr);
  if (checker == -1)
  {
    Logger::error("bind() failed");
    perror(NULL);
    close(socket_fd);
    exit(-3);
  }
  //listen for connections
  checker = listen(socket_fd, 10);
  if (checker == -1)
  {
    Logger::error("listen() failed");
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

  char buffer[1024] = {0}; //memset \0

  //read the buffer from client (user || new connection)
  int checker = recv(fds.fd, buffer, sizeof(buffer), 0);

  if (checker < 0)
  {
    Logger::error("recv() failed");
    return -1;
  }

  if (checker == 0)
  {
    // delete user
    Logger::error("Connection closed");
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
      parse_buffer_nc(connections[fds.fd]); //parse buffer without backslash r
    connections[fds.fd].buffer = "";
  }
  return 0;
}

void Server::waiting_for_connections(){

  int timeout = (60 * 60 * 1000);
  int checker;
  
  while (true)
  {
    checker = poll(fds.data(), fds.size(), timeout);
    if (checker < 0)
    {
      Logger::error("poll() failed");
      break;
    }
    if (checker == 0)
    {
      Logger::warning("poll() timeout");
      break;
    }
    else
    {
      current_size = fds.size();
      for (int i = 0; i < current_size; i++)
      {
        if (fds[i].revents == 0)
        {
          continue;
        }
        if (fds[i].revents != POLLIN)
        {
          Logger::error("Error! revents = " + to_string(fds[i].revents));
          close(fds[i].fd);
          fds.erase(fds.begin() + i); //segfault in case of 20 connections at the same time
          std::map<int, Client>::iterator it = users.find(fds[i].fd);
          if (it != users.end()) {
              users.erase(it);
          }
          it = connections.find(fds[i].fd);
          if (it != connections.end()) {
              connections.erase(it);
          }
          continue;
        }
        if (fds[i].fd == socket_fd)
        {
            if(is_server_connection() == -1)
              break;
        }
        else
        {
            if (is_client_connection(fds[i]) == -1)
              break;
        }
      }
    }
  }
}

void Server::sendReply(const std::string &message, int clientFd)
{
	if (send(clientFd, message.c_str(), strlen(message.c_str()), 0) == -1)
		perror("send sys call failed\n");
}
