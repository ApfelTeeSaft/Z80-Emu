#ifndef ROMLOADER_HPP
#define ROMLOADER_HPP

#include <vector>
#include <string>
#include "CPU.hpp"

// Struct to represent a button
struct Button {
    char code;       // Button ASCII code (e.g., '0', '+', '-')
    std::string imprint;  // Imprint on the button (e.g., "0", "+")
    int span;        // How many columns the button spans
};

extern std::vector<std::vector<Button>> buttonLayout;
extern std::string buttonLayoutTitle;

bool loadROM(const std::string& filepath, Z80Emulator& emulator, uint16_t startAddress);

#endif