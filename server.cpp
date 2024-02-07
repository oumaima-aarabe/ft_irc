#include "server.hpp"

Server::Server(unsigned int port, std::string password){
  this->port = port;
  this->password = password;
}

Server::~Server(){

}

int Server::if_nick_exist(std::string value){
  std::map<int, Client>::iterator it;
  for (it = users.begin();it != users.end();it++)
  {
    if (it->second.nickname == value)
      return (1);
  }
  for (it = connections.begin();it != connections.end();it++)
  {
    if (it->second.nickname == value)
      return (1);
  }
  return (0);
}

int Server::if_user_exist(std::string value){
  std::map<int, Client>::iterator it;
  for (it = users.begin();it != users.end();it++)
  {
    if (it->second.username == value)
      return (1);
  }
  for (it = connections.begin();it != connections.end();it++)
  {
    if (it->second.username == value)
      return (1);
  }
  return (0);
}

int  Server::parse_pass(Client client, std::string value){
  if (value != password)
  {
    std::cout << "ERROR : " << password << " not valid\n"; //SEND()
    return (-1);
  }
  else{
    client.password = password;
  }
  return (0);
}

int  Server::parse_nick(Client client, std::string value){
  if (!client.password.empty()){
    if (value.empty()){
      std::cout << "ERROR : 461 PARAMETR ENOUTHG\n";//SEND()
      return (-1);
    }
    else if (if_nick_exist(value)) {
      std::cout << "error: already exist\n";//SEND()
      return (-1);
    }
    else
    {
      client.nickname = value;
    }
  }
  else
  {
    std::cout << "password not set" << std::endl;
    return (-1);
  }
  if (!client.username.empty()){
    //welcome message send()
    // users.insert({client.fds.fd, Client(client)});
    users[client.fds.fd] = Client(client);
    // users[client.fds.fd] = Client(client.fds, client.username, client.nickname, client.password, client.buffer);
  }
  return (0);
}

int  Server::parse_user(Client client, std::string value){
  if (!client.password.empty()){
    std::vector<std::string> ret = split_user(value, ' ');
    if (ret.size() != 4){
      std::cout << "error : must be 4\n" ;//send()
      return (-1);
    }
    else if (if_user_exist(ret[0])) {
      std::cout << "error: reregister\n";//SEND()
      return (-1);
    }
    else{
      client.username = ret[0];
    }
  }
  else
  {
    std::cout << "password not set" << std::endl;
    return (-1);
  }
  if (!client.nickname.empty()){
    //welcome message send()
    // users.insert({client.fds.fd, Client(client)});
    users[client.fds.fd] = Client(client);
    // users[client.fds.fd] = Client(client.fds, client.username, client.nickname, client.password, client.buffer);
  }
  return (0);
}


int  Server::parse_pair(Client client, std::pair<std::string, std::string> pair)
{
  std::cout << "first:<<" << pair.first << ">>" << std::endl; 
  std::cout << "second:<<" << pair.second << ">>" << std::endl;
  if (pair.first == "PASS")
  {
    if (parse_pass(client, pair.second) == -1)
      return (-1);
  }
  else if (pair.first == "USER")
  {
    if (parse_user(client, pair.second) == -1)
      return (-1);
  }
  else if (pair.first == "NICK")
  {
    if (parse_nick(client, pair.second) == -1)
      return (-1);
  }
  else
  {
    std::cout << "BAD COMMAND" << std::endl;
    return (-1);
  }
  return (0);
}

void my_trim_(std::string& s, char delimiter) {
    size_t p = s.find_first_not_of(delimiter);
    s.erase(0, p);
    p = s.find_last_not_of(delimiter);
    if (std::string::npos != p)
        s.erase(p + 1);
}

std::vector<std::string> Server::split_user(std::string& line, char delimiter) {
    std::vector<std::string> ret;
    my_trim_(line, ' ');
    size_t found = line.find(delimiter);
    if (found == std::string::npos)
      return ret;
    size_t i = 0;
    while (found != std::string::npos){
      std::string str = line.substr(i, found);
      my_trim_(str, ' ');
      ret.push_back(str);
      i = found;
      line = line.substr(found);
      my_trim_(line, ' ');
      found = line.find(delimiter);
    }
    if (!line.empty()){
      ret.push_back(line);
    }
    return (ret);
}
std::pair<std::string, std::string> my_split_pair(const std::string& line, char delimiter) {
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

std::vector<std::pair<std::string, std::string> > Server::my_split_buffer(Client client, std::string delimiter) {
    std::vector<std::pair<std::string, std::string> > pairs;
    std::pair<std::string, std::string> pair;
    size_t found = client.buffer.find(delimiter);
    while (found != std::string::npos)
    {
      std::string rec = client.buffer.substr(0, found);
      my_trim_(rec, ' ');
      pair = my_split_pair(rec, ' ');
      parse_pair(client, pair);
      found = client.buffer.find(delimiter);
    }
    return pairs;
}

void Server::parse_buffer_nc(Client client)//from nc
{
  //split buffer with space must be 2 params
  // the first one must be PASS, USER OR NICK
  // if PASS compare second one with thw password of server
  std::pair<std::string, std::string> pair;
  std::vector<std::pair<std::string, std::string> > p;
  std::cout << "Parse using nc" << std::endl;
  p = my_split_buffer(client, "\n");
  // if (client.buffer.find('\n') != client.buffer.end())
  // {

  // }
  pair = my_split_pair(client.buffer , ' ');
  // if (client.buffer[3] !- )

}

void Server::parse_buffer_limechat(Client client)
{
  std::cout << "Parse using limeChat" << std::endl;

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

int Server::is_client_connection(struct pollfd fds){

  char buffer[1024] = {0}; //memset \0

  //read the buffer from client (user || new connection)
  int checker = recv(fds.fd, buffer, sizeof(buffer), 0);
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
  //client exist in users
  if (users.find(fds.fd) != users.end())
  {
    // parse_cmnds(fds[i].fd);
  }
  else
  {
    //new client
    std::string buf = buffer;
    if (connections.find(fds.fd) == connections.end())
    {
      // connections.insert({fds.fd, Client(fds, "", "", "", buf)})
      connections[fds.fd] = Client(fds, "", "", "", buf);
    }
    connections[fds.fd].buffer = buf;
    if (connections[fds.fd].buffer.find('\r') != std::string::npos)
      parse_buffer_limechat(connections[fds.fd]); //parse buffer with back slach r
    else
      parse_buffer_nc(connections[fds.fd]); //parse buffer without backslash r
  }
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
            if (is_client_connection(fds[i]) == -1)
              break;
        }
      }
    }
  }
}