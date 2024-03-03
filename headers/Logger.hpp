#pragma once

#include "Server.hpp"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

class Logger {
    public:
        static void error(std::string msg) {
            std::cout << RED << msg << RESET << std::endl;
        }

        static void warning(std::string msg) {
            std::cout << YELLOW << msg << RESET << std::endl;
        }

        static void info(std::string msg) {
            std::cout << GREEN << msg << RESET << std::endl;
        }

        static void debug(std::string msg) {
            std::cout << CYAN << msg << RESET << std::endl;
        }
};