#include "CLI.hpp"
#include "CPU.hpp"
#include "ROMLoader.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <windows.h>
#include <commdlg.h>
#include <string>

std::string openFileDialog() {
    OPENFILENAME ofn;
    wchar_t szFile[260];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
    ofn.lpstrFilter = L"ROM Files\0*.rom\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn) == TRUE) {
        // Convert the wide string (wchar_t*) to a regular string (std::string)
        std::wstring ws(szFile);
        std::string filepath(ws.begin(), ws.end());
        return filepath;
    }

    return "";
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

            // Convert hex to byte vec
            std::vector<uint8_t> program;
            for (size_t i = 0; i < programData.length(); i += 2) {
                uint8_t byte = std::stoi(programData.substr(i, 2), nullptr, 16);
                program.push_back(byte);
            }
            emulator.loadProgram(program, address);
            std::cout << "Program loaded at address: " << std::hex << address << std::endl;
        }
        else if (command == "loadrom") {
            uint16_t address;
            ss >> std::hex >> address;

            std::string filepath = openFileDialog();
            if (!filepath.empty()) {
                if (loadROM(filepath, emulator, address)) {
                    emulator.setProgramCounter(address);
                    std::cout << "ROM loaded successfully from " << filepath << " into address " << std::hex << address << std::endl;
                    std::cout << "Program Counter after ROM load: " << std::hex << emulator.getProgramCounter() << std::endl;  // Add this log
                }
                else {
                    std::cerr << "Failed to load ROM." << std::endl;
                }
            }
            else {
                std::cerr << "No file selected." << std::endl;
            }
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
        else if (command == "button") {
            std::string button;
            ss >> button;
            std::string imprint = emulator.getButtonImprint(button);
            if (!imprint.empty()) {
                std::cout << "Button '" << button << "' is imprinted with '" << imprint << "'" << std::endl;
            }
            else {
                std::cout << "Button '" << button << "' has no imprint or is not defined." << std::endl;
            }
        }
        else if (command == "exit") {
            running = false;
        }
        else {
            std::cout << "Unknown command: " << command << std::endl;
            std::cout << "Available commands: load, loadrom, run, step, registers, reset, button, exit" << std::endl;
        }
    }
}