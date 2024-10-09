#ifndef ROMLOADER_HPP
#define ROMLOADER_HPP

#include <vector>
#include <string>
#include "CPU.hpp"


struct Button {
    char code;
    std::string imprint;
    int span;
};

extern std::vector<std::vector<Button>> buttonLayout;
extern std::string buttonLayoutTitle;

bool loadROM(const std::string& filepath, Z80Emulator& emulator, uint16_t startAddress);

#endif