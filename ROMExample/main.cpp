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
    std::vector<uint8_t> programData = {
        0x3E, 0x0A,  // LD A, 0x0A
        0x06, 0x14,  // LD B, 0x14
        0x0E, 0x20,  // LD C, 0x20
        0x80,        // ADD A, B
        0x81,        // ADD A, C
        0xC9         // RET
    };
    romFile.write(reinterpret_cast<const char*>(programData.data()), programData.size());

    // Write the title (null-terminated string, with a fixed size of 32 bytes)
    std::string title = "Calculator";
    title.resize(32, '\0');  // Ensure the title has enough space and is null-terminated
    romFile.write(title.c_str(), title.size());

    // Button Layout Section
    std::vector<uint8_t> buttonLayoutData = {
        '7', 1, 1, '7',  // Button '7'
        '8', 1, 1, '8',  // Button '8'
        '9', 1, 1, '9',  // Button '9'
        '/', 1, 1, '/',  // Button '/'
        '\n',            // Line break
        '4', 1, 1, '4',  // Button '4'
        '5', 1, 1, '5',  // Button '5'
        '6', 1, 1, '6',  // Button '6'
        '*', 1, 1, '*',  // Button '*'
        '\n',            // Line break
        '1', 1, 1, '1',  // Button '1'
        '2', 1, 1, '2',  // Button '2'
        '3', 1, 1, '3',  // Button '3'
        '-', 1, 1, '-',  // Button '-'
        '\n',            // Line break
        '0', 2, 1, '0',  // Button '0' (span 2 columns)
        '=', 1, 1, '=',  // Button '='
        '+', 1, 1, '+',  // Button '+'
        '\n',            // Line break
        'C', 4, 1, 'C',  // Button 'C' (span 4 columns)
        0xFF             // End marker for button layout
    };

    romFile.write(reinterpret_cast<const char*>(buttonLayoutData.data()), buttonLayoutData.size());

    romFile.close();
    std::cout << "Calculator ROM created successfully!" << std::endl;
}


int main() {
    generateCalculatorROM("calculator.rom");
    return 0;
}