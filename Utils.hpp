#pragma once

#include "client.hpp"

void my_trim_(std::string& s, char delimiter);
std::vector<std::string> split(const std::string input, const std::string &separator);
std::vector<std::string> split_space(const std::string &input);
std::string setPrefix(std::string hostname, std::string nickname, std::string realname);
bool isNumber(const std::string &s);
std::string to_string(int i);
int to_int(std::string i);
void toUpper(std::string &str);