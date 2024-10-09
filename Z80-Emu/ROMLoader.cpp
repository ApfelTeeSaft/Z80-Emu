#include "ROMLoader.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>

std::vector<std::vector<Button>> buttonLayout;
std::string buttonLayoutTitle;

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

    size_t offset = 0;
    size_t halfSize = buffer.size() / 2;
    for (size_t i = 0; i < halfSize; ++i) {
        emulator.memory[startAddress + i] = buffer[i];
    }
    emulator.setProgramCounter(startAddress);

    buttonLayoutTitle.clear();
    while (buffer[offset] != '\0' && offset < 32) {
        buttonLayoutTitle += buffer[offset++];
    }
    offset = 32;

    std::cout << "Layout Title: " << buttonLayoutTitle << std::endl;

    buttonLayout.clear();
    while (offset < buffer.size()) {
        char code = static_cast<char>(buffer[offset++]);
        if (code == 0xFF) {
            break;
        }

        if (code == '\n') {
            buttonLayout.push_back(std::vector<Button>());
            continue;
        }

        if (offset + 2 >= buffer.size()) {
            std::cerr << "Error: Incomplete button data in ROM." << std::endl;
            break;
        }

        uint8_t span = buffer[offset++];
        uint8_t imprintSize = buffer[offset++];
        if (offset + imprintSize > buffer.size()) {
            std::cerr << "Error: Imprint size out of bounds." << std::endl;
            break;
        }

        std::string imprint(reinterpret_cast<const char*>(&buffer[offset]), imprintSize);
        offset += imprintSize;

        if (buttonLayout.empty()) {
            buttonLayout.push_back(std::vector<Button>());
        }
        buttonLayout.back().push_back(Button{ code, imprint, span });
        std::cout << "Parsed button: '" << imprint << "' with code: " << code << " and span: " << (int)span << std::endl;
    }

    return true;
}