#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void generateCalculatorROM(const std::string& filepath) {
    std::ofstream romFile(filepath, std::ios::binary);

    if (!romFile.is_open()) {
        std::cerr << "Error: Unable to create ROM file." << std::endl;
        return;
    }

    // Program Section (First half)
    // test for now
    std::vector<uint8_t> programData = {
        0x3E, 0x0A,  // LD A, 0x0A
        0x06, 0x14,  // LD B, 0x14
        0x0E, 0x20   // LD C, 0x20
    };

    // Write the program data (first half of the ROM)
    romFile.write(reinterpret_cast<const char*>(programData.data()), programData.size());

    // Button Layout Section (Second half)
    // Each button has a code (ASCII) and an imprint size, followed by the imprint text
    std::vector<uint8_t> buttonLayoutData = {
        // Button '0'
        '0', 1, '0',  // ASCII '0', size 1, imprint '0'

        // Button '1'
        '1', 1, '1',  // ASCII '1', size 1, imprint '1'

        // Button '2'
        '2', 1, '2',  // ASCII '2', size 1, imprint '2'

        // Button '3'
        '3', 1, '3',  // ASCII '3', size 1, imprint '3'

        // Button '4'
        '4', 1, '4',  // ASCII '4', size 1, imprint '4'

        // Button '5'
        '5', 1, '5',  // ASCII '5', size 1, imprint '5'

        // Button '6'
        '6', 1, '6',  // ASCII '6', size 1, imprint '6'

        // Button '7'
        '7', 1, '7',  // ASCII '7', size 1, imprint '7'

        // Button '8'
        '8', 1, '8',  // ASCII '8', size 1, imprint '8'

        // Button '9'
        '9', 1, '9',  // ASCII '9', size 1, imprint '9'

        // Button '+'
        '+', 1, '+',  // ASCII '+', size 1, imprint '+'

        // Button '-'
        '-', 1, '-',  // ASCII '-', size 1, imprint '-'

        // Button '*'
        '*', 1, '*',  // ASCII '*', size 1, imprint '*'

        // Button '/'
        '/', 1, '/',  // ASCII '/', size 1, imprint '/'

        // Button '='
        '=', 1, '=',  // ASCII '=', size 1, imprint '='

        // Button 'C' (clear)
        'C', 1, 'C'   // ASCII 'C', size 1, imprint 'C'
    };

    // Write the button layout data
    romFile.write(reinterpret_cast<const char*>(buttonLayoutData.data()), buttonLayoutData.size());

    romFile.close();
    std::cout << "Calculator ROM created successfully!" << std::endl;
}

int main() {
    generateCalculatorROM("calculator.rom");
    return 0;
}