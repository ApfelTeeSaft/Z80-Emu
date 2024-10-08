#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#define MEMORY_SIZE 65536

// Flags for the Z80 processor
enum Z80Flags {
    FLAG_S = 0x80,  // Sign flag
    FLAG_Z = 0x40,  // Zero flag
    FLAG_H = 0x10,  // Half carry flag
    FLAG_PV = 0x04, // Parity/overflow flag
    FLAG_N = 0x02,  // Subtract flag
    FLAG_C = 0x01   // Carry flag
};

// Z80 Reg
struct Z80Registers {
    uint16_t pc;  // Program counter
    uint16_t sp;  // Stack pointer
    uint8_t a, f, b, c, d, e, h, l;  // 8-bit registers
};

class Z80Emulator {
public:
    Z80Emulator();
    void reset();
    void loadProgram(const std::vector<uint8_t>& program, uint16_t startAddress);
    void setProgramCounter(uint16_t address);
    void run();
    void step();
    void displayRegisters() const;
    std::vector<uint8_t> memory;

    void setButtonLayout(const std::map<std::string, std::string>& layout);
    std::string getButtonImprint(const std::string& button) const;
    uint16_t getProgramCounter() const {
        return regs.pc;
    }


private:
    Z80Registers regs;
    std::unordered_map<uint8_t, std::function<void()>> opcodeMap;  // Map for opcode execution
    std::map<std::string, std::string> buttonLayout;  // Button layout and imprints

    void setFlag(Z80Flags flag, bool condition);
    bool getFlag(Z80Flags flag) const;

    uint8_t getButtonState();
    void sendToDisplay(uint8_t value);
};

#endif