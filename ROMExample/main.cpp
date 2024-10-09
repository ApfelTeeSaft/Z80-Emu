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

    // Write the program data (first half of the ROM)
    romFile.write(reinterpret_cast<const char*>(programData.data()), programData.size());

    // Add a title section to the ROM (null-terminated string)
    const char* title = "Calculator";
    romFile.write(title, strlen(title) + 1);  // Null-terminated string for the title

    // Button Layout Section (Second half)
    std::vector<uint8_t> buttonLayoutData = {
        // Row 1: '7', '8', '9', '/'
        '7', 1, '7',  // Button '7'
        '8', 1, '8',  // Button '8'
        '9', 1, '9',  // Button '9'
        '/', 1, '/',  // Button '/'
        '\n',         // Line break

        // Row 2: '4', '5', '6', '*'
        '4', 1, '4',  // Button '4'
        '5', 1, '5',  // Button '5'
        '6', 1, '6',  // Button '6'
        '*', 1, '*',  // Button '*'
        '\n',         // Line break

        // Row 3: '1', '2', '3', '-'
        '1', 1, '1',  // Button '1'
        '2', 1, '2',  // Button '2'
        '3', 1, '3',  // Button '3'
        '-', 1, '-',  // Button '-'
        '\n',         // Line break

        // Row 4: '0', '=', '+'
        '0', 2, '0',  // Button '0' spans two columns
        '=', 1, '=',  // Button '='
        '+', 1, '+',  // Button '+'
        '\n',         // Line break

        // Row 5: 'C' (clear)
        'C', 4, 'C'   // Button 'C' spans the entire row
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