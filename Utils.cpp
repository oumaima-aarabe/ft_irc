#include "Utils.hpp"

void my_trim_(std::string& s, char delimiter) {
    size_t p = s.find_first_not_of(delimiter);
    s.erase(0, p);
    p = s.find_last_not_of(delimiter);
    if (std::string::npos != p)
        s.erase(p + 1);
}

std::vector<std::string> split(std::string & str, char delimiter) {
    std::vector<std::string> result;
    size_t pos = 0;
    std::string token;
    my_trim_(str, delimiter);
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        result.push_back(token);
        str.erase(0, pos + 1);
    }
    if (!str.empty()) {
        result.push_back(str);
    }
    return result;
}
