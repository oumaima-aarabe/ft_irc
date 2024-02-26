#pragma once

#include "client.hpp"

void my_trim_(std::string& s, char delimiter);
std::vector<std::string> split(const std::string &input, const std::string &separator);
std::string setPrefix(std::string hostname, std::string nickname, std::string username, std::string buffer);
bool isNumber(const std::string &s);
std::string to_string(int i);