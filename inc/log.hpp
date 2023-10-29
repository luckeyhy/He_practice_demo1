#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <iostream>

#define YELLOW "\033[33m"
#define RED "\033[31m"
#define GREEN "\033[32m"

#define INFO (std::cout << GREEN)
#define WARN (std::cout << YELLOW)
#define ERROR (std::cout << RED)
#define REND "\033[0m" << std::endl

#endif