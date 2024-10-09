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


    std::vector<uint8_t> programData = {
        0x3E, 0x0A,
        0x06, 0x14,
        0x0E, 0x20,
        0x80,
        0x81,
        0xC9
    };


    romFile.write(reinterpret_cast<const char*>(programData.data()), programData.size());


    const char* title = "Calculator";
    romFile.write(title, strlen(title) + 1);


    std::vector<uint8_t> buttonLayoutData = {

        '7', 1, '7',
        '8', 1, '8',
        '9', 1, '9',
        '/', 1, '/',
        '\n',


        '4', 1, '4',
        '5', 1, '5',
        '6', 1, '6',
        '*', 1, '*',
        '\n',


        '1', 1, '1',
        '2', 1, '2',
        '3', 1, '3',
        '-', 1, '-',
        '\n',


        '0', 2, '0',
        '=', 1, '=',
        '+', 1, '+',
        '\n',


        'C', 4, 'C',


        0xFF
    };


    romFile.write(reinterpret_cast<const char*>(buttonLayoutData.data()), buttonLayoutData.size());

    romFile.close();
    std::cout << "Calculator ROM created successfully!" << std::endl;
}

int main() {
    generateCalculatorROM("calculator.rom");
    return 0;
}