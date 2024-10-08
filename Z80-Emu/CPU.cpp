#include "CPU.hpp"

Z80Emulator::Z80Emulator() {
    reset();
}

// Load program into memory
void Z80Emulator::loadProgram(const std::vector<uint8_t>& program, uint16_t startAddress) {
    for (size_t i = 0; i < program.size(); ++i) {
        memory[startAddress + i] = program[i];
    }
    regs.pc = startAddress;
}

// Run the emulator
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
    std::cout << "A: " << std::hex << (int)regs.a << "\n";
    std::cout << "B: " << std::hex << (int)regs.b << " C: " << std::hex << (int)regs.c << "\n";
    std::cout << "D: " << std::hex << (int)regs.d << " E: " << std::hex << (int)regs.e << "\n";
    std::cout << "H: " << std::hex << (int)regs.h << " L: " << std::hex << (int)regs.l << "\n";
    std::cout << std::endl;
}

// Reset registers and mem
void Z80Emulator::reset() {
    regs.pc = 0x0000;
    regs.sp = 0xFFFF;
    regs.a = regs.f = regs.b = regs.c = regs.d = regs.e = regs.h = regs.l = 0x00;
    memory.resize(MEMORY_SIZE, 0); // Clear memory
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
        regs.a ^= regs.a;
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