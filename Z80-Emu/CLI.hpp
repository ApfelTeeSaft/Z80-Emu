#ifndef CLI_HPP
#define CLI_HPP

#include "CPU.hpp"


double evaluateExpression(Z80Emulator& emulator, const std::string& expr);

void cli(Z80Emulator& emulator);

#endif