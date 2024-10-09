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

// load the selected ROM
void loadRomFromFile(Z80Emulator& emulator) {
    std::wstring filepathW = openFileDialog();
    if (!filepathW.empty()) {
        std::string filepath = ws2s(filepathW);
        uint16_t startAddress = 0x100;  // Default start address
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
    static int pcValue = 0x100;

    ImGui::Begin("Emulator Control");

    if (ImGui::Button("Load ROM##loadrom")) {
        loadRomFromFile(emulator);
    }

    ImGui::Text("Program Counter: 0x%04X", emulator.getProgramCounter());

    if (ImGui::Button("Run##run")) {
        emulator.run();
        std::cout << "Emulator started" << std::endl;
    }

    if (ImGui::Button("Reset##reset")) {
        emulator.reset();
        std::cout << "Emulator reset" << std::endl;
    }

    if (ImGui::Button("Step##step")) {
        emulator.step();
        std::cout << "Stepped to next opcode" << std::endl;
    }

    ImGui::InputInt("Set PC##pcinput", &pcValue);
    if (ImGui::Button("Set PC##setpc")) {
        if (pcValue >= 0 && pcValue < 0xFFFF) {
            emulator.setProgramCounter(static_cast<uint16_t>(pcValue));
            std::cout << "Program Counter set to: 0x" << std::hex << pcValue << std::endl;
        }
        else {
            std::cerr << "Invalid Program Counter value!" << std::endl;
        }
    }

    ImGui::End();

    if (!buttonLayout.empty()) {
        ImGui::Begin("Calculator");

        for (const auto& row : buttonLayout) {
            for (const auto& button : row) {
                if (ImGui::Button(button.imprint.c_str(), ImVec2(50 * button.span, 50))) {
                    std::cout << "Button '" << button.imprint << "' pressed!" << std::endl;
                    // button input here
                }
                ImGui::SameLine();
            }
            ImGui::NewLine();
        }

        ImGui::End();
    }
}