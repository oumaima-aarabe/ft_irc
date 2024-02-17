#include "Client.hpp"


void my_trim_(std::string& s, char delimiter) {
    size_t p = s.find_first_not_of(delimiter);
    s.erase(0, p);
    p = s.find_last_not_of(delimiter);
    if (std::string::npos != p)
        s.erase(p + 1);
}

std::pair<std::string, std::string> my_split_pair(const std::string& line, char delimiter) {
    std::pair<std::string, std::string> pair;
    size_t found = line.find(delimiter);
    std::string cmnd = line.substr(0, found);
    if (found == std::string::npos) {
      pair = make_pair(cmnd, "");
      return pair;
    }
    std::string val = line.substr(found+1);
    my_trim_(val, delimiter);
    my_trim_(val, '\r');
    my_trim_(val, '\n');
    my_trim_(cmnd, delimiter);
    pair = make_pair(cmnd, val);
    return pair;
}

void Server::my_split_buffer(Client &client, std::string delimiter) {
    std::pair<std::string, std::string> pair;
    size_t found = client.buffer.find(delimiter);
    puts("1");
    while (found != std::string::npos)
    {
      std::string rec = client.buffer.substr(0, found);
      my_trim_(rec, ' ');
      pair = my_split_pair(rec, ' ');
      parse_pair(client, pair);
      //Deliminer xxx xxx\r\n (if limechat) 
      if(delimiter.size() == 2)
        client.buffer = client.buffer.substr(found + 2);
      else if (delimiter.size() == 1)
        client.buffer = client.buffer.substr(found + 1);
      found = client.buffer.find(delimiter);
    }
    return;
}

void Server::parse_buffer_nc(Client &client)//from nc
{
  std::cout << "Parse using nc" << std::endl;
  my_split_buffer(client, "\n");
}

void Server::parse_buffer_limechat(Client &client)
{
    std::cout << "Parse using limeChat" << std::endl;
    my_split_buffer(client, "\r\n");
}
