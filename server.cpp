#include "server.hpp"


void my_trim_(std::string& s) {
    size_t p = s.find_first_not_of(" ");
    s.erase(0, p);
    p = s.find_last_not_of(" ");
    if (std::string::npos != p)
        s.erase(p + 1);
}

std::map<int, std::string> my_split_(const std::string& line, char delimiter) {
    std::map<int, std::string> mp;
    std::string token;
    int index = 0;
    size_t found = line.find(delimiter);
    std::string cmnd = line.substr(0, found);
    std::string val = line.substr(found+1, line.size()-2);
    my_trim_(val);
    my_trim_(cmnd);
}

void parse_buffer(std::string buffer)//from nc
{
  
  //split buffer with space must be 2 params
  // the first one must be PASS, USER OR NICK
  // if PASS compare second one with thw password of server

}

void parse_buffer_br(std::string buffer)
{

}

int main(int ac, char **av)
{
  struct sockaddr_in ip4addr;
  int current_size = 0;
  int timeout;
  int new_sd;
  int on = 1;
  struct sockaddr_storage their_addr;
  std::vector<struct pollfd> fds; 
  std::string passe;

  if (ac != 3)
  {
    std::cout << "ERROR : ./exec port password" << std::endl;
    return(0);
  }

  passe = av[2];
  ip4addr.sin_port = htons(atoi(av[1]));
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
            new_sd = accept(socket_fd, NULL, NULL);
            if (new_sd < 0)
            {
              if (errno != EWOULDBLOCK)
              {
                std::cout << "accept() failed" << std::endl;
              }
              break;
            }
            std::cout << "New incoming connection " << new_sd << std::endl;
            struct  pollfd k;
            k.fd = new_sd;
            k.events = POLLIN;
            k.revents = 0;
            fds.push_back(k);
        }
        else
        {
            char buffer[1024] = {0};
            data_bind = recv(fds[i].fd, buffer, sizeof(buffer), 0);
            std::cout << "buf <";
            for (size_t i = 0; buffer[i] != '\0';i++)
            {
              if (isprint(buffer[i]))
                std::cout << buffer[i];
              else
                std::cout << (int)buffer[i];
            }
            std::cout << ">" << std::endl;
            if (data_bind < 0)
            {
              if (errno != EWOULDBLOCK)
              {
                perror("  recv() failed");
              }
              break;
            }

            if (data_bind == 0)
            {
              // delete user
              printf("  Connection closed\n");
              // close_conn = true;
              break;
            }

            std::string buf = buffer;
            if (buf.find('\r') != std::string::npos)
              parse_buffer_br(buf); //parse buffer with back slach r
            else
              parse_buffer(buf); //parse buffer without backslash r

            // int len = data_bind;
            //check if the user authenticated...
            // bool authenticated = false;
            // std::map<std::string , std::string> key_val;
            // std::string authData(buffer, len);
            // std::size_t found = authData.find("\r\n");
            // if (found != std::string::npos) {
            //     std::string username = authData.substr(0, found);
            //     authData = authData.substr(found + 2);
            //     found = authData.find("\r\n");
            //     if (found != std::string::npos) {
            //         std::string nickname = authData.substr(0, found);
            //         authData = authData.substr(found + 2);

            //         found = authData.find("\r\n");
            //         if (found != std::string::npos) {
            //             std::string password = authData.substr(0, found);
            //             authData = authData.substr(found + 2);
            //             found = password.find(" ");
            //             if (found != std::string::npos){
            //               std::string password_auth = password.substr(0, found);


            //             }
            //         }
            //     }
                
            // data_bind = send(fds[i].fd, buffer, len, 0);
            // if (data_bind < 0)
            // {
            //   std::cout << "send() failed" << std::endl;
            //   close_conn = true;
            //   break;
            // }
          // }
          // if (close_conn)
          // {
          //   close(fds[i].fd);
          //   fds[i].fd = -1;
          //   compress_array = true;
          // }
        }
      }
    }
  }
}
