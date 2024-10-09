#include "ROMLoader.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>

// Global variable to store the button layout
std::vector<std::vector<Button>> buttonLayout;

// Global variable to store the layout title
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

    // Load the program into memory
    size_t offset = 0;
    size_t halfSize = buffer.size() / 2;
    for (size_t i = 0; i < halfSize; ++i) {
        emulator.memory[startAddress + i] = buffer[i];
    }
    emulator.setProgramCounter(startAddress);

    // Parse the title (null-terminated string)
    buttonLayoutTitle.clear();
    while (buffer[offset] != '\0') {
        buttonLayoutTitle += buffer[offset++];
    }
    offset++;  // Move past the null terminator

    std::cout << "Layout Title: " << buttonLayoutTitle << std::endl;

    // Parse the button layout
    buttonLayout.clear();
    while (offset < buffer.size()) {
        char code = static_cast<char>(buffer[offset++]);  // Button code
        if (code == '\n') {
            buttonLayout.push_back(std::vector<Button>());  // Start a new row
            continue;
        }

        uint8_t span = buffer[offset++];  // Button column span
        uint8_t imprintSize = buffer[offset++];  // Imprint size
        std::string imprint((char*)&buffer[offset], imprintSize);  // Imprint text
        offset += imprintSize;

        if (buttonLayout.empty()) {
            buttonLayout.push_back(std::vector<Button>());  // Ensure there's a row for buttons
        }
        buttonLayout.back().push_back(Button{ code, imprint, span });
        std::cout << "Parsed button: '" << imprint << "' with code: " << code << " and span: " << (int)span << std::endl;
    }

    return true;
}