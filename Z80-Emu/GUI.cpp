#include "GUI.hpp"
#include "ROMLoader.hpp"
#include "CPU.hpp"
#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
#include "imgui.h"
#include <locale>
#include <codecvt>

int operand1 = 0, operand2 = 0;
char operation = '\0';  // The operator (+, -, *, /)
bool isOperand1Set = false;
bool operationEntered = false;

std::wstring openFileDialog() {
    wchar_t fileName[MAX_PATH] = L"";
    OPENFILENAMEW ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = L"ROM Files\0*.rom\0All Files\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ofn.lpstrDefExt = L"rom";

    if (GetOpenFileNameW(&ofn)) {
        return std::wstring(fileName);
    }

    return L"";
}

std::string ws2s(const std::wstring& wideStr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wideStr);
}


void loadRomFromFile(Z80Emulator& emulator) {
    std::wstring filepathW = openFileDialog();
    if (!filepathW.empty()) {
        std::string filepath = ws2s(filepathW);
        uint16_t startAddress = 0x100;
        if (loadROM(filepath, emulator, startAddress)) {
            emulator.setProgramCounter(startAddress);
            std::cout << "ROM loaded: " << filepath << " at start address: " << std::hex << startAddress << std::endl;
        }
        else {
            std::cerr << "Failed to load ROM: " << filepath << std::endl;
        }
    }
}

void renderImGui(Z80Emulator& emulator) {
    std::cout << "ImGui Render Loop Running" << std::endl;

    static int pcValue = 0x100;

    ImGui::Begin("Emulator Control");

    if (ImGui::Button("Load ROM##loadrom")) {
        loadRomFromFile(emulator);
        std::cout << "Load ROM button pressed." << std::endl;
    }

    ImGui::Text("Program Counter: 0x%04X", emulator.getProgramCounter());

    if (ImGui::Button("Run##run")) {
        emulator.run();
        std::cout << "Run button pressed." << std::endl;
    }

    if (ImGui::Button("Reset##reset")) {
        emulator.reset();
        std::cout << "Reset button pressed." << std::endl;
    }

    if (ImGui::Button("Step##step")) {
        emulator.step();
        std::cout << "Step button pressed." << std::endl;
    }

    ImGui::InputInt("Set PC##pcinput", &pcValue);
    if (ImGui::Button("Set PC##setpc")) {
        if (pcValue >= 0 && pcValue < 0xFFFF) {
            emulator.setProgramCounter(static_cast<uint16_t>(pcValue));
            std::cout << "Set Program Counter to: 0x" << std::hex << pcValue << std::endl;
        }
    }

    ImGui::End();

    // Calculator buttons section
    if (!buttonLayout.empty()) {
        ImGui::Begin(buttonLayoutTitle.c_str());

        for (const auto& row : buttonLayout) {
            for (const auto& button : row) {
                std::string buttonLabel = button.imprint + "##button" + std::to_string(rand());  // Unique button ID

                // Check if the button is pressed
                if (ImGui::Button(buttonLabel.c_str(), ImVec2(50 * button.span, 50))) {
                    char pressedKey = button.imprint[0];

                    std::cout << "Button pressed: " << pressedKey << std::endl;

                    // Handle numbers (0-9)
                    if (pressedKey >= '0' && pressedKey <= '9') {
                        int value = pressedKey - '0';  // Convert char to int

                        if (!isOperand1Set) {
                            operand1 = operand1 * 10 + value;
                            emulator.loadImmediateA(operand1);
                            std::cout << "Operand 1: " << operand1 << std::endl;
                        }
                        else {
                            operand2 = operand2 * 10 + value;
                            emulator.loadImmediateB(operand2);
                            std::cout << "Operand 2: " << operand2 << std::endl;
                        }
                    }
                    else if (pressedKey == '+' || pressedKey == '-' || pressedKey == '*' || pressedKey == '/') {
                        operation = pressedKey;
                        isOperand1Set = true;
                        operationEntered = true;

                        std::cout << "Operation: " << operation << std::endl;

                        // Map operator to Z80 instruction
                        switch (operation) {
                        case '+':
                            emulator.addInstructionToMemory(0x80);  // ADD A, B
                            break;
                        case '-':
                            emulator.addInstructionToMemory(0x90);  // SUB A, B
                            break;
                        }
                    }
                    else if (pressedKey == '=') {
                        if (operationEntered) {
                            emulator.run();  // Run the emulator
                            uint8_t result = emulator.getRegisterA();
                            std::cout << "Result: " << (int)result << std::endl;
                        }

                        // Reset for next calculation
                        operand1 = 0;
                        operand2 = 0;
                        operation = '\0';
                        isOperand1Set = false;
                        operationEntered = false;
                    }
                    else if (pressedKey == 'C') {
                        // Clear everything
                        operand1 = 0;
                        operand2 = 0;
                        operation = '\0';
                        isOperand1Set = false;
                        operationEntered = false;
                        emulator.reset();
                        std::cout << "Calculator cleared." << std::endl;
                    }
                }
                ImGui::SameLine();
            }
            ImGui::NewLine();
        }

        ImGui::End();
    }
    std::cout << "ImGui Render Loop Finished" << std::endl;
}