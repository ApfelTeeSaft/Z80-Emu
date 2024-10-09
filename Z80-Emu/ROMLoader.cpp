#include "ROMLoader.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include "CPU.hpp"

std::vector<std::vector<Button>> buttonLayout;

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
    for (size_t i = 0; i < halfSize; ++i) {
        emulator.memory[startAddress + i] = buffer[i];
    }
    emulator.setProgramCounter(startAddress);

    buttonLayout.clear();
    size_t offset = halfSize;
    uint8_t rowCount = buffer[offset++];  // First byte: number of rows

    for (uint8_t row = 0; row < rowCount; ++row) {
        std::vector<Button> rowButtons;
        while (offset < buffer.size()) {
            char code = static_cast<char>(buffer[offset++]);  // Button code
            uint8_t span = buffer[offset++];  // Button column span
            uint8_t imprintSize = buffer[offset++];  // Imprint size
            std::string imprint((char*)&buffer[offset], imprintSize);  // Imprint text
            offset += imprintSize;

            rowButtons.push_back(Button{ code, imprint, span });
            if (rowButtons.size() >= 4) {
                break;
            }
        }
        buttonLayout.push_back(rowButtons);
    }

    return true;
}