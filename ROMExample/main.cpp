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

    // Program Section: Z80 Machine Code
    // Simple calculator operations (addition and subtraction test)
    std::vector<uint8_t> programData = {
        0x3E, 0x02,  // LD A, 2     (load 2 into A)
        0x06, 0x03,  // LD B, 3     (load 3 into B)
        0x80,        // ADD A, B    (A = A + B, i.e., A = 2 + 3 = 5)
        0xC3, 0x00, 0x10,  // JP 0x1000  (jump to 0x1000 for further operations)
        0xC9         // RET         (return to the caller)
    };
    romFile.write(reinterpret_cast<const char*>(programData.data()), programData.size());

    // Write title section with delimiters
    std::string titleStart = "[TITLE_START]";
    std::string title = "Calculator";
    std::string titleEnd = "[TITLE_END]";

    romFile.write(titleStart.c_str(), titleStart.size());
    romFile.write(title.c_str(), title.size());
    romFile.write(titleEnd.c_str(), titleEnd.size());

    // Button Layout Section with delimiters
    std::string buttonStart = "[BUTTONS_START]";
    romFile.write(buttonStart.c_str(), buttonStart.size());

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
    };

    romFile.write(reinterpret_cast<const char*>(buttonLayoutData.data()), buttonLayoutData.size());

    std::string buttonEnd = "[BUTTONS_END]";
    romFile.write(buttonEnd.c_str(), buttonEnd.size());

    romFile.close();
    std::cout << "Calculator ROM created successfully!" << std::endl;
}

int main() {
    generateCalculatorROM("calculator.rom");
    return 0;
}