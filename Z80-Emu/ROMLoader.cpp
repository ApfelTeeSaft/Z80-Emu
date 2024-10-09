#include "ROMLoader.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

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

    // Load the program section into emulator memory (first half)
    size_t halfSize = buffer.size() / 2;
    for (size_t i = 0; i < halfSize; ++i) {
        emulator.memory[startAddress + i] = buffer[i];
    }
    emulator.setProgramCounter(startAddress);

    // Look for title section
    std::string titleStart = "[TITLE_START]";
    std::string titleEnd = "[TITLE_END]";

    offset = std::search(buffer.begin(), buffer.end(),
        titleStart.begin(), titleStart.end()) - buffer.begin();

    if (offset >= buffer.size()) {
        std::cerr << "Error: Title start identifier not found." << std::endl;
        return false;
    }
    offset += titleStart.size();

    buttonLayoutTitle.clear();
    while (offset < buffer.size() && std::string(buffer.begin() + offset, buffer.begin() + offset + titleEnd.size()) != titleEnd) {
        buttonLayoutTitle += static_cast<char>(buffer[offset++]);
    }

    // Move past the title end identifier
    offset += titleEnd.size();
    std::cout << "Layout Title: " << buttonLayoutTitle << std::endl;

    // Look for button layout start
    std::string buttonStart = "[BUTTONS_START]";
    std::string buttonEnd = "[BUTTONS_END]";
    offset = std::search(buffer.begin(), buffer.end(),
        buttonStart.begin(), buttonStart.end()) - buffer.begin();

    if (offset >= buffer.size()) {
        std::cerr << "Error: Button layout start identifier not found." << std::endl;
        return false;
    }
    offset += buttonStart.size();

    buttonLayout.clear();
    while (offset < buffer.size()) {
        char code = static_cast<char>(buffer[offset++]);
        if (std::string(buffer.begin() + offset, buffer.begin() + offset + buttonEnd.size()) == buttonEnd) {
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
