#include "CPU.hpp"
#include <iostream>
#include <stdexcept>

std::map<uint8_t, uint8_t> ioPorts;

Z80Emulator::Z80Emulator() {
    reset();
    regs.pc = 0x100;

    // opcode map
    opcodeMap = {
        {0x3E, [this]() { regs.a = memory[regs.pc++]; }},               // LD A, n
        {0x06, [this]() { regs.b = memory[regs.pc++]; }},               // LD B, n
        {0x0E, [this]() { regs.c = memory[regs.pc++]; }},               // LD C, n
        {0x1E, [this]() { regs.e = memory[regs.pc++]; }},               // LD E, n
        {0x26, [this]() { regs.h = memory[regs.pc++]; }},               // LD H, n
        {0x2E, [this]() { regs.l = memory[regs.pc++]; }},               // LD L, n
        {0x7F, [this]() {}},                                            // LD A, A (No-op)
        {0x78, [this]() { regs.a = regs.b; }},                          // LD A, B
        {0x79, [this]() { regs.a = regs.c; }},                          // LD A, C
        {0x7A, [this]() { regs.a = regs.d; }},                          // LD A, D
        {0x7B, [this]() { regs.a = regs.e; }},                          // LD A, E
        {0x7C, [this]() { regs.a = regs.h; }},                          // LD A, H
        {0x7D, [this]() { regs.a = regs.l; }},                          // LD A, L
        {0x77, [this]() { memory[(regs.h << 8) | regs.l] = regs.a; }},  // LD (HL), A
        {0x7E, [this]() { regs.a = memory[(regs.h << 8) | regs.l]; }},  // LD A, (HL)
        {0x0A, [this]() { regs.a = memory[(regs.b << 8) | regs.c]; }},  // LD A, (BC)
        {0x1A, [this]() { regs.a = memory[(regs.d << 8) | regs.e]; }},  // LD A, (DE)
        {0xEA, [this]() {                                                // LD (nn), A
            uint16_t address = memory[regs.pc] | (memory[regs.pc + 1] << 8);
            regs.pc += 2;
            memory[address] = regs.a;
        }},
        {0x01, [this]() { regs.c = memory[regs.pc++]; regs.b = memory[regs.pc++]; }},  // LD BC, nn
        {0x11, [this]() { regs.e = memory[regs.pc++]; regs.d = memory[regs.pc++]; }},  // LD DE, nn
        {0x21, [this]() { regs.l = memory[regs.pc++]; regs.h = memory[regs.pc++]; }},  // LD HL, nn
        {0x31, [this]() { regs.sp = memory[regs.pc] | (memory[regs.pc + 1] << 8); regs.pc += 2; }},  // LD SP, nn
        {0x80, [this]() { regs.a += regs.b; setFlag(FLAG_Z, regs.a == 0); setFlag(FLAG_S, regs.a & 0x80); }},  // ADD A, B
        {0x81, [this]() { regs.a += regs.c; setFlag(FLAG_Z, regs.a == 0); setFlag(FLAG_S, regs.a & 0x80); }},  // ADD A, C
        {0x82, [this]() { regs.a += regs.d; setFlag(FLAG_Z, regs.a == 0); setFlag(FLAG_S, regs.a & 0x80); }},  // ADD A, D
        {0x83, [this]() { regs.a += regs.e; setFlag(FLAG_Z, regs.a == 0); setFlag(FLAG_S, regs.a & 0x80); }},  // ADD A, E
        {0x86, [this]() { regs.a += memory[(regs.h << 8) | regs.l]; setFlag(FLAG_Z, regs.a == 0); setFlag(FLAG_S, regs.a & 0x80); }},  // ADD A, (HL)
        {0x90, [this]() { regs.a -= regs.b; setFlag(FLAG_Z, regs.a == 0); setFlag(FLAG_S, regs.a & 0x80); }},  // SUB A, B
        {0x91, [this]() { regs.a -= regs.c; setFlag(FLAG_Z, regs.a == 0); setFlag(FLAG_S, regs.a & 0x80); }},  // SUB A, C
        {0x92, [this]() { regs.a -= regs.d; setFlag(FLAG_Z, regs.a == 0); setFlag(FLAG_S, regs.a & 0x80); }},  // SUB A, D
        {0x96, [this]() { regs.a -= memory[(regs.h << 8) | regs.l]; setFlag(FLAG_Z, regs.a == 0); setFlag(FLAG_S, regs.a & 0x80); }},  // SUB A, (HL)
        {0xC3, [this]() { regs.pc = memory[regs.pc] | (memory[regs.pc + 1] << 8); }},  // JP nn
        {0xCA, [this]() {                                                // JP Z, nn
            if (getFlag(FLAG_Z)) {
                regs.pc = memory[regs.pc] | (memory[regs.pc + 1] << 8);
            }
 else {
  regs.pc += 2;
}
}},
{0xC9, [this]() { regs.pc = memory[regs.sp] | (memory[regs.sp + 1] << 8); regs.sp += 2; }},  // RET
{0xCD, [this]() { regs.sp -= 2; memory[regs.sp] = regs.pc & 0xFF; memory[regs.sp + 1] = (regs.pc >> 8); regs.pc = memory[regs.pc] | (memory[regs.pc + 1] << 8); }},  // CALL nn
{0x04, [this]() { regs.b++; setFlag(FLAG_Z, regs.b == 0); }},  // INC B
{0x05, [this]() { regs.b--; setFlag(FLAG_Z, regs.b == 0); }},  // DEC B
{0x0C, [this]() { regs.c++; setFlag(FLAG_Z, regs.c == 0); }},  // INC C
{0x0D, [this]() { regs.c--; setFlag(FLAG_Z, regs.c == 0); }},  // DEC C
{0x24, [this]() { regs.h++; setFlag(FLAG_Z, regs.h == 0); }},  // INC H
{0x25, [this]() { regs.h--; setFlag(FLAG_Z, regs.h == 0); }},  // DEC H
{0x34, [this]() { memory[(regs.h << 8) | regs.l]++; setFlag(FLAG_Z, memory[(regs.h << 8) | regs.l] == 0); }},  // INC (HL)
{0x35, [this]() { memory[(regs.h << 8) | regs.l]--; setFlag(FLAG_Z, memory[(regs.h << 8) | regs.l] == 0); }},  // DEC (HL)
{0xDB, [this]() {  // IN A, (n)
    uint8_t port = memory[regs.pc++];
    if (port == 0x01) {
        regs.a = getButtonState();  // Simulated button state
    }
else {
 regs.a = ioPorts[port];
}
std::cout << "IN A, (" << std::hex << (int)port << ") = " << std::hex << (int)regs.a << std::endl;
}},
{0xD3, [this]() {  // OUT (n), A
    uint8_t port = memory[regs.pc++];
    ioPorts[port] = regs.a;
    if (port == 0x02) {
        sendToDisplay(regs.a);  // Simulated display output
    }
else {
 std::cout << "OUT (" << std::hex << (int)port << "), A = " << std::hex << (int)regs.a << std::endl;
}
}}
    };
}

void Z80Emulator::reset() {
    regs.pc = 0x0000;
    regs.sp = 0xFFFF;
    regs.a = regs.f = regs.b = regs.c = regs.d = regs.e = regs.h = regs.l = 0x00;
    memory.resize(MEMORY_SIZE, 0);
}

void Z80Emulator::setProgramCounter(uint16_t address) {
    regs.pc = address;
}

void Z80Emulator::loadProgram(const std::vector<uint8_t>& program, uint16_t startAddress) {
    if (startAddress + program.size() > MEMORY_SIZE) {
        std::cerr << "Error: Program size exceeds available memory!" << std::endl;
        return;
    }

    // Load into memory
    for (size_t i = 0; i < program.size(); ++i) {
        memory[startAddress + i] = program[i];
    }

    // Set the program counter to start address after loading the program
    setProgramCounter(startAddress);
    std::cout << "Program loaded at address: " << std::hex << startAddress << std::endl;
}

void Z80Emulator::run() {
    bool running = true;
    while (running) {
        uint8_t opcode = memory[regs.pc++];  // Fetch the next opcode
        if (opcode == 0x00) {
            // Ignore NOP (0x00) and just continue
            continue;
        }

        std::cout << "Executing opcode: " << std::hex << (int)opcode << " at PC: " << std::hex << regs.pc - 1 << std::endl;
        auto it = opcodeMap.find(opcode);
        if (it != opcodeMap.end()) {
            it->second();  // Execute the corresponding function
        }
        else {
            std::cerr << "Unknown opcode: " << std::hex << (int)opcode << std::endl;
        }
    }
}

void Z80Emulator::step() {
    uint8_t opcode = memory[regs.pc++];
    auto it = opcodeMap.find(opcode);
    if (it != opcodeMap.end()) {
        it->second();  // Execute function
    }
    else {
        std::cerr << "Unknown opcode: " << std::hex << (int)opcode << std::endl;
        // exit(1);
    }
}

void Z80Emulator::setFlag(Z80Flags flag, bool condition) {
    if (condition) {
        regs.f |= flag;
    }
    else {
        regs.f &= ~flag;
    }
}

bool Z80Emulator::getFlag(Z80Flags flag) const {
    return (regs.f & flag) != 0;
}

// Simulated Button and Display
uint8_t Z80Emulator::getButtonState() {
    return 0x01;  // Simulate button state (AFortButtonState::KYS)
}

void Z80Emulator::sendToDisplay(uint8_t value) {
    std::cout << "Displaying value: " << std::hex << (int)value << std::endl;
}

// Button Layout and Imprints
void Z80Emulator::setButtonLayout(const std::map<std::string, std::string>& layout) {
    buttonLayout = layout;
}

std::string Z80Emulator::getButtonImprint(const std::string& button) const {
    auto it = buttonLayout.find(button);
    if (it != buttonLayout.end()) {
        return it->second;
    }
    return "";
}

void Z80Emulator::displayRegisters() const {
    std::cout << "PC: " << std::hex << regs.pc << "\n";
    std::cout << "SP: " << std::hex << regs.sp << "\n";
    std::cout << "A: " << std::hex << (int)regs.a << "\n";
    std::cout << "Flags: " << std::hex << (int)regs.f << "\n";
    std::cout << "BC: " << std::hex << (int)regs.b << (int)regs.c << "\n";
    std::cout << "DE: " << std::hex << (int)regs.d << (int)regs.e << "\n";
    std::cout << "HL: " << std::hex << (int)regs.h << (int)regs.l << "\n";
}