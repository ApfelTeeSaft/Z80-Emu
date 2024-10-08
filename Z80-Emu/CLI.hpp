#ifndef CLI_HPP
#define CLI_HPP

#include "CPU.hpp"
#include <string>

void cli(Z80Emulator& emulator);
double evaluateExpression(const std::string& expr);

#endif