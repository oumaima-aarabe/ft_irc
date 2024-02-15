#include "Message.hpp"

Message::Message(std::string &buffer) {
    //split by space cmnd and params in the buffer and store them in the private attributes.
    // call split function from utils
}

std::string Message::getCmd(void) const {
    return _cmd;
}

std::map<size_t, std::string> Message::getParams(void) const {
    return _params;
}