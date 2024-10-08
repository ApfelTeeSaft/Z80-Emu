#include "CLI.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>

double evaluateExpression(Z80Emulator& emulator, const std::string& expr) {
    std::stringstream ss(expr);
    double result = 0;
    double num = 0;
    char op = '+';

    ss >> num;
    emulator.executeArithmeticInstruction(1, static_cast<int>(num));

    while (ss >> op >> num) {
        if (op == '+') {
            emulator.executeArithmeticInstruction(1, static_cast<int>(num));
        }
        else if (op == '-') {
            emulator.executeArithmeticInstruction(2, static_cast<int>(num));
        }
        else if (op == '*') {
            emulator.executeArithmeticInstruction(3, static_cast<int>(num));
        }
        else if (op == '/') {
            emulator.executeArithmeticInstruction(4, static_cast<int>(num));
        }
    }

    result = emulator.getAccumulator(); // final result from the CPU
    return result;
}

void cli(Z80Emulator& emulator) {
    std::string input;
    bool running = true;

    while (running) {
        std::cout << "emulator> ";
        std::getline(std::cin, input);
        std::stringstream ss(input);
        std::string command;
        ss >> command;

        if (command == "load") {
            std::string programData;
            uint16_t address;
            ss >> programData >> std::hex >> address;

            // hex to byte vec
            std::vector<uint8_t> program;
            for (size_t i = 0; i < programData.length(); i += 2) {
                uint8_t byte = std::stoi(programData.substr(i, 2), nullptr, 16);
                program.push_back(byte);
            }
            emulator.loadProgram(program, address);
            std::cout << "Program loaded at address: " << std::hex << address << std::endl;
        }
        else if (command == "run") {
            emulator.run();
        }
        else if (command == "step") {
            emulator.step();
        }
        else if (command == "registers") {
            emulator.displayRegisters();
        }
        else if (command == "reset") {
            emulator.reset();
            std::cout << "Emulator reset." << std::endl;
        }
        else if (command == "calc") {
            std::string expression;
            std::getline(ss, expression);

            expression.erase(0, expression.find_first_not_of(' '));
            expression.erase(expression.find_last_not_of(' ') + 1);

            try {
                double result = evaluateExpression(emulator, expression);
                std::cout << "Result: " << result << std::endl;
            }
            catch (const std::exception& e) {
                std::cerr << "Error calculating expression: " << e.what() << std::endl;
            }
        }
        else if (command == "exit") {
            running = false;
        }
        else {
            std::cout << "Unknown command: " << command << std::endl;
            std::cout << "Available commands: load, run, step, registers, reset, calc, exit" << std::endl;
        }
    }
}