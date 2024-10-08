#ifndef CPU_HPP
#define CPU_HPP

#include <vector>
#include <cstdint>
#include <iostream>

// Define memory size
const int MEMORY_SIZE = 0xFFFF; // 64 KB

// Registers of Z80
struct Z80Registers {
    uint16_t pc;  // PC (program counter)
    uint16_t sp;  // Stack pointer
    uint8_t a;    // Accumulator
    uint8_t f;    // Flags
    uint8_t b, c; // BC reg pair
    uint8_t d, e; // DE reg pair
    uint8_t h, l; // HL reg pair
};

// Z80
class Z80Emulator {
public:
    Z80Emulator();

    void loadProgram(const std::vector<uint8_t>& program, uint16_t startAddress);
    void run();
    void step();
    void displayRegisters() const;
    void reset();

private:
    Z80Registers regs;
    std::vector<uint8_t> memory;
    void executeOpcode(uint8_t opcode);
};

#endif