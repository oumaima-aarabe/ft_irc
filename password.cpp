#include "client.hpp"

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