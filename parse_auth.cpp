#include "client.hpp"


void my_trim_(std::string& s, char delimiter) {
    size_t p = s.find_first_not_of(delimiter);
    s.erase(0, p);
    p = s.find_last_not_of(delimiter);
    if (std::string::npos != p)
        s.erase(p + 1);
}

// std::string find_until_delimiter(const std::string& str, size_t start_index, char delimiter) {
//     size_t delimiter_index = str.find(delimiter, start_index);
//     if (delimiter_index != std::string::npos) {
//         return str.substr(start_index, delimiter_index - start_index);
//     } else {
//         return "";
//     }
// }

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

std::vector<std::pair<std::string, std::string> > Server::my_split_buffer(Client &client, std::string delimiter) {
    std::vector<std::pair<std::string, std::string> > pairs;
    std::pair<std::string, std::string> pair;
    size_t found = client.buffer.find(delimiter);
    while (found != std::string::npos)
    {
      std::string rec = client.buffer.substr(0, found);
      my_trim_(rec, ' ');
      pair = my_split_pair(rec, ' ');
      std::cout << "pair.first:<" << pair.first << ">" << std::endl;
      std::cout << "pair.second:<" << pair.second << ">" << std::endl;
      parse_pair(client, pair);
      client.buffer = client.buffer.substr(found + 1);
      found = client.buffer.find(delimiter, found);
    }
    return pairs;
}

void Server::parse_buffer_nc(Client &client)//from nc
{
  std::pair<std::string, std::string> pair;
  std::vector<std::pair<std::string, std::string> > p;
  std::cout << "Parse using nc" << std::endl;
  p = my_split_buffer(client, "\n");
}

void Server::parse_buffer_limechat(Client &client)
{
    std::cout << "Parse using limeChat" << std::endl;
    std::pair<std::string, std::string> pair;
    std::vector<std::pair<std::string, std::string> > p;
    std::cout << "Parse using nc" << std::endl;
    p = my_split_buffer(client, "\r\n");
}
