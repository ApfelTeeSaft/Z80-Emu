#include "ROMLoader.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

// Define buttons vector
std::vector<Button> buttons;

bool loadROM(const std::string& filepath, Z80Emulator& emulator, uint16_t startAddress) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open ROM file: " << filepath << std::endl;
        return false;
    }

    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();

    if (buffer.size() < 2) {
        std::cerr << "Error: ROM file is too small." << std::endl;
        return false;
    }

    size_t halfSize = buffer.size() / 2;

    std::cout << "Program section loaded into memory from " << std::hex << startAddress << std::endl;
    for (size_t i = 0; i < halfSize; ++i) {
        emulator.memory[startAddress + i] = buffer[i];
        std::cout << "Memory[" << std::hex << (startAddress + i) << "] = " << std::hex << (int)emulator.memory[startAddress + i] << std::endl;
    }
    emulator.setProgramCounter(startAddress);

    buttons.clear();
    size_t offset = halfSize;
    while (offset < buffer.size()) {
        char code = static_cast<char>(buffer[offset++]);
        uint8_t imprintSize = buffer[offset++];
        std::string imprint((char*)&buffer[offset], imprintSize);
        offset += imprintSize;

        buttons.push_back(Button{ code, imprint });
        std::cout << "Button '" << code << "' is imprinted with '" << imprint << "'" << std::endl;
    }

    return true;
}