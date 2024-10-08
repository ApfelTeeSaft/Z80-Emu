#include "CPU.hpp"
#include <stdexcept>

// cpu type shit

Z80Emulator::Z80Emulator() {
    reset();
}

// Poopy result
double Z80Emulator::getAccumulator() const {
    return regs.a;
}


void Z80Emulator::loadProgram(const std::vector<uint8_t>& program, uint16_t startAddress) {
    for (size_t i = 0; i < program.size(); ++i) {
        memory[startAddress + i] = program[i];
    }
    regs.pc = startAddress;
}

void Z80Emulator::run() {
    bool running = true;
    while (running) {
        uint8_t opcode = memory[regs.pc++];
        executeOpcode(opcode);
    }
}

// Single-step
void Z80Emulator::step() {
    uint8_t opcode = memory[regs.pc++];
    executeOpcode(opcode);
}

// Display registers
void Z80Emulator::displayRegisters() const {
    std::cout << "PC: " << std::hex << regs.pc << "\n";
    std::cout << "SP: " << std::hex << regs.sp << "\n";
    std::cout << "A: " << regs.a << "\n";
    std::cout << "B: " << std::hex << (int)regs.b << " C: " << std::hex << (int)regs.c << "\n";
    std::cout << "D: " << std::hex << (int)regs.d << " E: " << std::hex << (int)regs.e << "\n";
    std::cout << "H: " << std::hex << (int)regs.h << " L: " << std::hex << (int)regs.l << "\n";
    std::cout << std::endl;
}

// Reset
void Z80Emulator::reset() {
    regs.pc = 0x0000;
    regs.sp = 0xFFFF;
    regs.a = 0.0;
    regs.f = regs.b = regs.c = regs.d = regs.e = regs.h = regs.l = 0x00;
    memory.resize(MEMORY_SIZE, 0);
}

// arithmetic operations for the calculator
void Z80Emulator::executeArithmeticInstruction(int operation, double operand) {
    switch (operation) {
    case 1: // Add
        regs.a += operand;
        break;
    case 2: // Subtract
        regs.a -= operand;
        break;
    case 3: // Multiply
        regs.a *= operand;
        break;
    case 4: // Divide
        if (operand == 0) throw std::runtime_error("Division by zero");
        regs.a /= operand;
        break;
    default:
        throw std::invalid_argument("Unknown instruction");
    }
}

// Execute the fetched opcode
void Z80Emulator::executeOpcode(uint8_t opcode) {
    switch (opcode) {
    case 0x00: // NOP
        break;

    case 0x01: // LD BC, nn
        regs.c = memory[regs.pc++];
        regs.b = memory[regs.pc++];
        break;

    case 0x06: // LD B, n
        regs.b = memory[regs.pc++];
        break;

    case 0x0E: // LD C, n
        regs.c = memory[regs.pc++];
        break;

    case 0x3E: // LD A, n
        regs.a = memory[regs.pc++];
        break;

    case 0x80: // ADD A, B
        regs.a += regs.b;
        break;

    case 0xAF: // XOR A (clear A)
        regs.a = 0.0;
        break;

    case 0xC3: // JP nn (JMP -> nn)
        regs.pc = memory[regs.pc] | (memory[regs.pc + 1] << 8);
        break;

    case 0xC9: // RET (Return)
        regs.pc = memory[regs.sp] | (memory[regs.sp + 1] << 8);
        regs.sp += 2;
        break;

    default:
        std::cerr << "Unknown opcode: " << std::hex << (int)opcode << std::endl;
        exit(1);
    }
}