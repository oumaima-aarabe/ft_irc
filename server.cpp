#include "server.hpp"

Server::Server(unsigned int port, std::string password){
  this->port = port;
  this->password = password;
}

Server::~Server(){
  
}

void parse_pair(std::pair<std::string, std::string> pair)
{

}

void my_trim_(std::string& s, char delimiter) {
    size_t p = s.find_first_not_of(delimiter);
    s.erase(0, p);
    p = s.find_last_not_of(delimiter);
    if (std::string::npos != p)
        s.erase(p + 1);
}

std::pair<std::string, std::string> my_split_(const std::string& line, char delimiter) {
    std::pair<std::string, std::string> pair;
    size_t found = line.find(delimiter);
    std::string cmnd = line.substr(0, found);
    std::string val = line.substr(found+1);
    my_trim_(val, delimiter);
    my_trim_(val, '\n');
    my_trim_(cmnd, delimiter);
    pair = make_pair(cmnd, val);
    return pair;
}

std::vector<std::pair<std::string, std::string> > my_split_buffer(const std::string& line, std::string delimiter) {
    std::vector<std::pair<std::string, std::string> > pairs;
    std::pair<std::string, std::string> pair;
    size_t found = line.find(delimiter);
    while (found != std::string::npos)
    {
      std::string rec = line.substr(0, found);
      my_trim_(rec, ' ');
      pair = my_split_(rec, ' ');
      parse_pair(pair);
      found = line.find(delimiter);
    }
    return pairs;
}

void Server::parse_buffer_nc(std::string buffer)//from nc
{
    //split buffer with space must be 2 params
  // the first one must be PASS, USER OR NICK
  // if PASS compare second one with thw password of server
  my_split_buffer(buffer, "\n");

}

void Server::parse_buffer_limechat(std::string buffer)
{

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

int Server::is_client_connection(int i){

  char buffer[1024] = {0}; //memset \0

  //read the buffer from client (user || new connection)
  int checker = recv(fds[i].fd, buffer, sizeof(buffer), 0);
  if (checker < 0)
  {
    if (errno != EWOULDBLOCK)
    {
      perror("  recv() failed");
    }
    return -1;
  }

  if (checker == 0)
  {
    // delete user
    printf("  Connection closed\n");
    return -1;
  }

  std::string buf = buffer;
  if (buf.find('\r') != std::string::npos)
    parse_buffer_limechat(buf); //parse buffer with back slach r
  else
    parse_buffer_nc(buf); //parse buffer without backslash r
  return 0;
}

void Server::waiting_for_connctions(){

  int timeout = (3 * 60 * 1000);
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
          break;
        }
        if (fds[i].fd == socket_fd)
        {
            if(is_server_connection() == -1)
              break;
        }
        else
        {
            if (is_client_connection(i) == -1)
              break;
        }
      }
    }
  }
}