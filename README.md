# Z80 Emulator - Personal Research Project

This project is a simple Z80 CPU emulator written in C++. It is part of a personal research project into how emulators work and how one might emulate hardware such as a CPU, memory, and basic functionality. This project uses the Z80 processor, which was popular in early computers and calculators like the TI-83.

The emulator includes a command-line interface (CLI) for interacting with the emulation, loading programs, stepping through instructions, displaying registers, and even performing simple calculations using a basic arithmetic evaluator.

**Note:** A graphical user interface (GUI) using **ImGui** and more complex mathematical functionality (multiplication and division) will be added in future updates.

## Features
- **Z80 CPU Emulation**: Emulates basic Z80 instructions such as `LD`, `ADD`, `XOR`, `RET`, and more.
- **Memory Management**: Provides basic memory functionality for loading programs into memory.
- **CLI Interface**: Offers an interactive CLI for loading programs, running or stepping through the CPU, viewing registers, resetting the emulator, and performing basic arithmetic operations.
- **Arithmetic Evaluator**: Includes a simple calculator that supports addition and subtraction of integers and floating-point numbers. Future updates will support multiplication (`*`) and division (`/`).

## Project Structure
- `main.cpp`: Entry point for the program.
- `CPU.hpp`/`CPU.cpp`: Handles the Z80 CPU emulation.
- `CLI.hpp`/`CLI.cpp`: Provides the command-line interface for interacting with the emulator.

## Setup (Visual Studio 2022)

### Prerequisites
- **Visual Studio 2022**: You need Visual Studio 2022 installed with C++ development tools.

### Opening the Project in Visual Studio

1. Clone the repository or download the source files.
2. Open **Visual Studio 2022**.
3. Open the .sln project file within Visual Studio 2022.

### Building and Running

1. Set the build configuration to **Debug** or **Release**.
2. Press **Ctrl + F5** to build and run the emulator in the console window.
3. You can interact with the emulator using the CLI commands described below.

## CLI Commands

Below is a list of available commands that can be used within the emulator's CLI:

### 1. `load`
Loads a program into the emulator's memory at a specific address.

#### Syntax:
```
load <hex_program> <start_address>
```

- `<hex_program>`: A string of hexadecimal values representing the program (e.g., `3E42420680C9`).
- `<start_address>`: The memory address where the program should be loaded (in hex).

#### Example:
```bash
load 3E42420680C9 0000
```

This command loads the following program at address `0x0000`:
- `LD A, 0x42`
- `LD B, 0x42`
- `ADD A, B`
- `RET`

### 2. `run`
Runs the loaded program continuously until the program ends or encounters a `RET` instruction.

#### Syntax:
```
run
```

#### Example:
```bash
run
```

This will start executing the loaded program from the program counter (`PC`).

### 3. `step`
Executes a single instruction from the program and updates the registers.

#### Syntax:
```
step
```

#### Example:
```bash
step
```

This will execute one instruction and print the current state of the CPU after execution.

### 4. `registers`
Displays the current values of the CPU's registers, including the program counter (`PC`), stack pointer (`SP`), accumulator (`A`), and general-purpose registers.

#### Syntax:
```
registers
```

#### Example:
```bash
registers
```

This will output the current values of all CPU registers in hexadecimal format.

### 5. `reset`
Resets the CPU registers and clears memory.

#### Syntax:
```
reset
```

#### Example:
```bash
reset
```

This will reset the emulator to its initial state.

### 6. `calc`
Performs basic arithmetic calculations using addition and subtraction with integers or floating-point numbers.

#### Syntax:
```
calc <expression>
```

- `<expression>`: A mathematical expression containing numbers and `+`/`-` operators. You can use both integers and floating-point numbers.

#### Example:
```bash
calc 5 + 3.2 - 1.1 + 6
```

This will output:
```
Result: 13.1
```

### 7. `exit`
Exits the emulator.

#### Syntax:
```
exit
```

#### Example:
```bash
exit
```

This will terminate the emulator and return to your terminal.

## Example Workflow

1. **Load a program**:
    ```bash
    load 3E42420680C9 0000
    ```
    This loads the program at address `0x0000`.

2. **Step through the program**:
    ```bash
    step
    step
    ```

3. **View the register values**:
    ```bash
    registers
    ```

4. **Run the program continuously**:
    ```bash
    run
    ```

5. **Perform a simple calculation**:
    ```bash
    calc 5 + 5 - 2
    ```

6. **Reset the emulator**:
    ```bash
    reset
    ```

7. **Exit the emulator**:
    ```bash
    exit
    ```

## Future Work:

This project is part of ongoing research. Future work:
- **Adding a GUI**: A graphical user interface (GUI) using **ImGui** will be added in a future update.
- **More Complex Math**: Support for multiplication (`*`) and division (`/`) will be added to the calculator program soon.
- Support for more Z80 instructions.
- Implementing more accurate timing and interrupts.
- Adding memory-mapped I/O and display simulation.
- Improving the CLI with additional commands for debugging.
- External GUI for displaying CPU Calls and Memory R/W actions.

## License
This project is for educational and research purposes only.