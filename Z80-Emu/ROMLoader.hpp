#ifndef ROMLOADER_HPP
#define ROMLOADER_HPP

#include <vector>
#include <string>
#include "CPU.hpp"

struct Button {
    char code;       // Button ASCII code (e.g., '0', '+', '-')
    std::string imprint;  // Imprint on the button (e.g., "0", "+")
};

extern std::vector<Button> buttons;

bool loadROM(const std::string& filepath, Z80Emulator& emulator, uint16_t startAddress);

#endif