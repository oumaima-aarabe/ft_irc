#include "Server.hpp"

Server::Server(unsigned int port, std::string password){
  char hostnam[256];
  if (gethostname(hostnam, sizeof(hostnam)) == -1){
    std::cout << "error getting hostname\n";
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
    std::cerr << "socket failed" << std::endl;
    close(socket_fd);
    exit(-1);
  }
  // set socket options
  int checker = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
  if (checker < 0)
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
  // define this socket with ip and  port
  checker = bind(socket_fd, (struct sockaddr *)&ip4addr, sizeof ip4addr);
  if (checker == -1)
  {
    std::cerr << "bind failed" << std::endl;
    perror(NULL);
    close(socket_fd);
    exit(-3);
  }
  //listen for connections
  checker = listen(socket_fd, 10);
  if (checker == -1)
  {
    std::cerr << "listen failed" << std::endl;
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
      std::cout << "accept() failed" << std::endl;
    }
    return(-1);
  }
  std::cout << "New incoming connection " << new_sd << std::endl;
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
    std::cout << "recv failed()\n";
    return -1;
  }

  if (checker == 0)
  {
    // delete user
    printf("  Connection closed\n");
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


  std::cout << "users count: " << users.size() << std::endl;
  std::cout << "connections count: " << connections.size() << std::endl;
  std::cout << "buffer: <" << buffer << ">" << std::endl;

  //client authenticated , exist in users
    if (users.find(fds.fd) != users.end())
  {
    //Remove \r\n from limechat
    size_t found = content.find('\r');
    if (found != std::string::npos){
      if (content[found+1] == '\n'){
        content.erase(found, 2);
      }
    }
    //Remove \n from nc
    found = content.find('\n');
    if (found != std::string::npos){
      content.erase(found, 1);
    }
    users[fds.fd].buffer = content;

    //(imy & oumi's work)
    std::istrstream iss(users[fds.fd].buffer.c_str());
    char *command = NULL;
    iss >> command;
    std::cout << "command: " << command << std::endl;
    //check which command is the buffer
    // if (strcmp(command, "PONG") == 0)
    // {
    //   // return; //ignore pong messages from limechat
    // }
    // else if (strcmp(command, "JOIN") == 0)
    // {
    //   //parse buffer...
    // }
    // else if (strcmp(command, "MODE") == 0)
    // {
    //   //parse buffer...
    // }
    // else if (strcmp(command, "PRIVMSG") == 0)
    // {
  
    // }
    // else if (strcmp(command, "KICK") == 0)
    // {
     
    // }
    // else if (strcmp(command, "INVITE") == 0)
    // {
      
    // }
    // else if (strcmp(command, "TOPIC") == 0)
    // {

    // }
    // else if (strcmp(command, "PART") == 0)
    // {

    // }
    //add other commands if needed...
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
      std::cout << "poll() failed" << std::endl;
      break;
    }
    if (checker == 0)
    {
      std::cout << "poll() timeout" << std::endl;
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
          std::cout << "Error! revents = " << fds[i].revents << std::endl;
          close(fds[i].fd);
          fds.erase(fds.begin() + i);
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