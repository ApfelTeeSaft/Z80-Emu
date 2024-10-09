#include "CPU.hpp"
#include <iostream>
#include <stdexcept>

std::map<uint8_t, uint8_t> ioPorts;

Z80Emulator::Z80Emulator() {
    reset();
    regs.pc = 0x100;


    opcodeMap = {
        {0x3E, [this]() { regs.a = memory[regs.pc++]; }},
        {0x06, [this]() { regs.b = memory[regs.pc++]; }},
        {0x0E, [this]() { regs.c = memory[regs.pc++]; }},
        {0x1E, [this]() { regs.e = memory[regs.pc++]; }},
        {0x26, [this]() { regs.h = memory[regs.pc++]; }},
        {0x2E, [this]() { regs.l = memory[regs.pc++]; }},
        {0x7F, [this]() {}},
        {0x78, [this]() { regs.a = regs.b; }},
        {0x79, [this]() { regs.a = regs.c; }},
        {0x7A, [this]() { regs.a = regs.d; }},
        {0x7B, [this]() { regs.a = regs.e; }},
        {0x7C, [this]() { regs.a = regs.h; }},
        {0x7D, [this]() { regs.a = regs.l; }},
        {0x77, [this]() { memory[(regs.h << 8) | regs.l] = regs.a; }},
        {0x7E, [this]() { regs.a = memory[(regs.h << 8) | regs.l]; }},
        {0x0A, [this]() { regs.a = memory[(regs.b << 8) | regs.c]; }},
        {0x1A, [this]() { regs.a = memory[(regs.d << 8) | regs.e]; }},
        {0xEA, [this]() {
            uint16_t address = memory[regs.pc] | (memory[regs.pc + 1] << 8);
            regs.pc += 2;
            memory[address] = regs.a;
        }},
        {0x01, [this]() { regs.c = memory[regs.pc++]; regs.b = memory[regs.pc++]; }},
        {0x11, [this]() { regs.e = memory[regs.pc++]; regs.d = memory[regs.pc++]; }},
        {0x21, [this]() { regs.l = memory[regs.pc++]; regs.h = memory[regs.pc++]; }},
        {0x31, [this]() { regs.sp = memory[regs.pc] | (memory[regs.pc + 1] << 8); regs.pc += 2; }},
        {0x80, [this]() { regs.a += regs.b; setFlag(FLAG_Z, regs.a == 0); setFlag(FLAG_S, regs.a & 0x80); }},
        {0x81, [this]() { regs.a += regs.c; setFlag(FLAG_Z, regs.a == 0); setFlag(FLAG_S, regs.a & 0x80); }},
        {0x82, [this]() { regs.a += regs.d; setFlag(FLAG_Z, regs.a == 0); setFlag(FLAG_S, regs.a & 0x80); }},
        {0x83, [this]() { regs.a += regs.e; setFlag(FLAG_Z, regs.a == 0); setFlag(FLAG_S, regs.a & 0x80); }},
        {0x86, [this]() { regs.a += memory[(regs.h << 8) | regs.l]; setFlag(FLAG_Z, regs.a == 0); setFlag(FLAG_S, regs.a & 0x80); }},
        {0x90, [this]() { regs.a -= regs.b; setFlag(FLAG_Z, regs.a == 0); setFlag(FLAG_S, regs.a & 0x80); }},
        {0x91, [this]() { regs.a -= regs.c; setFlag(FLAG_Z, regs.a == 0); setFlag(FLAG_S, regs.a & 0x80); }},
        {0x92, [this]() { regs.a -= regs.d; setFlag(FLAG_Z, regs.a == 0); setFlag(FLAG_S, regs.a & 0x80); }},
        {0x96, [this]() { regs.a -= memory[(regs.h << 8) | regs.l]; setFlag(FLAG_Z, regs.a == 0); setFlag(FLAG_S, regs.a & 0x80); }},
        {0xC3, [this]() { regs.pc = memory[regs.pc] | (memory[regs.pc + 1] << 8); }},
        {0xCA, [this]() {
            if (getFlag(FLAG_Z)) {
                regs.pc = memory[regs.pc] | (memory[regs.pc + 1] << 8);
            }
 else {
  regs.pc += 2;
}
}},
{0xC9, [this]() { regs.pc = memory[regs.sp] | (memory[regs.sp + 1] << 8); regs.sp += 2; }},
{0xCD, [this]() { regs.sp -= 2; memory[regs.sp] = regs.pc & 0xFF; memory[regs.sp + 1] = (regs.pc >> 8); regs.pc = memory[regs.pc] | (memory[regs.pc + 1] << 8); }},
{0x04, [this]() { regs.b++; setFlag(FLAG_Z, regs.b == 0); }},
{0x05, [this]() { regs.b--; setFlag(FLAG_Z, regs.b == 0); }},
{0x0C, [this]() { regs.c++; setFlag(FLAG_Z, regs.c == 0); }},
{0x0D, [this]() { regs.c--; setFlag(FLAG_Z, regs.c == 0); }},
{0x24, [this]() { regs.h++; setFlag(FLAG_Z, regs.h == 0); }},
{0x25, [this]() { regs.h--; setFlag(FLAG_Z, regs.h == 0); }},
{0x34, [this]() { memory[(regs.h << 8) | regs.l]++; setFlag(FLAG_Z, memory[(regs.h << 8) | regs.l] == 0); }},
{0x35, [this]() { memory[(regs.h << 8) | regs.l]--; setFlag(FLAG_Z, memory[(regs.h << 8) | regs.l] == 0); }},
{0xDB, [this]() {
    uint8_t port = memory[regs.pc++];
    if (port == 0x01) {
        regs.a = getButtonState();
    }
else {
 regs.a = ioPorts[port];
}
std::cout << "IN A, (" << std::hex << (int)port << ") = " << std::hex << (int)regs.a << std::endl;
}},
{0xD3, [this]() {
    uint8_t port = memory[regs.pc++];
    ioPorts[port] = regs.a;
    if (port == 0x02) {
        sendToDisplay(regs.a);
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


    for (size_t i = 0; i < program.size(); ++i) {
        memory[startAddress + i] = program[i];
    }


    setProgramCounter(startAddress);
    std::cout << "Program loaded at address: " << std::hex << startAddress << std::endl;
}

void Z80Emulator::run() {
    bool running = true;
    while (running) {
        uint8_t opcode = memory[regs.pc++];
        if (opcode == 0x00) {

            continue;
        }

        std::cout << "Executing opcode: " << std::hex << (int)opcode << " at PC: " << std::hex << regs.pc - 1 << std::endl;
        auto it = opcodeMap.find(opcode);
        if (it != opcodeMap.end()) {
            it->second();
        }
        else {
            std::cerr << "Unknown opcode: " << std::hex << (int)opcode << std::endl;
        }
    }
}

// Load immediate value into register A
void Z80Emulator::loadImmediateA(uint8_t value) {
    regs.a = value;  // Directly load value into the A register
    std::cout << "Loaded " << (int)value << " into register A" << std::endl;
}

// Load immediate value into register B
void Z80Emulator::loadImmediateB(uint8_t value) {
    regs.b = value;  // Directly load value into the B register
    std::cout << "Loaded " << (int)value << " into register B" << std::endl;
}

// Add an instruction (opcode) to memory
void Z80Emulator::addInstructionToMemory(uint8_t opcode) {
    memory[regs.pc++] = opcode;  // Add the instruction at the current PC and increment PC
    std::cout << "Added opcode 0x" << std::hex << (int)opcode << " to memory at PC " << std::hex << (int)(regs.pc - 1) << std::endl;
}

// Get the value from register A
uint8_t Z80Emulator::getRegisterA() const {
    return regs.a;
}


void Z80Emulator::step() {
    uint8_t opcode = memory[regs.pc++];
    auto it = opcodeMap.find(opcode);
    if (it != opcodeMap.end()) {
        it->second();
    }
    else {
        std::cerr << "Unknown opcode: " << std::hex << (int)opcode << std::endl;

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


uint8_t Z80Emulator::getButtonState() {
    return 0x01;
}

void Z80Emulator::sendToDisplay(uint8_t value) {
    std::cout << "Displaying value: " << std::hex << (int)value << std::endl;
}


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