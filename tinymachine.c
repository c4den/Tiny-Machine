// Caden Peterson
// Professor Hensel
// CDA 3103
#include <stdio.h>
#include <stdlib.h>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define MAXMEMORYSIZE 10

// Program Counter: Register that holds the address of the next instruction executed
// Memory Address Register: Stores the address to a location in Main storage, to read or write from that location
// Memory Data Register: Used to store data being sent to or received from Main Storage
// Instruction Register: Stores the instruction to be executed by the processor
// Accumulator: Stores data to be used as input into the Arithmetic Logic unit

typedef struct Instruction
{
    int opCode;
    int deviceOrAddress;
} Instruction;

// Note: IM, MDR1, and IR are of type Instruction. All other CPU registers and Data Memory
// Are of type int

// State =      0               1           2           3           4           5       6           7           8          9
enum opCodes { OpFetch = 0, OpLoad = 1, OpAdd = 2, OpStore = 3, OpSub = 4, OpIn = 5, OpOut = 6, OpEnd = 7, OpJump = 8, OpSkipZ = 9};
void startPrompt();
void fileCheck(FILE *file);
void tinyMachine(int opCode, int address);

int programCounter = 10;
int memory[MAXMEMORYSIZE] = {0}; // this is our datamemory array, setting all values in it to 0
int accumulator;
int memoryAddressRegister;
int memoryDataRegister2;
int memoryAddressRegister2;

Instruction instructionMemory[MAXMEMORYSIZE];
Instruction instructionRegister;
Instruction memoryDataRegister;

void fetch(void);
void load(void);
void add(void);
void store(void);
void sub(void);
void in(void);
void out(void);
void jmp(void);
void skip(void);
void print(void);

//    # command args, contains arguments
int main(int argc, char **argv)
{
    FILE *file; // make the file pointer
    file = fopen(argv[1], "r"); // take the name of the file, read only
    fileCheck(file); // check if the file actually exists.
    startPrompt();

    // from here, the file has been checked, and we know it exists, we then do the start prompt.
    // I advise making some switch case stuff instead of using if/else loops, with each case being one of the enumerated
    // opCodes. I.e. switch(condition) case(OpLoad) break; case(OpAdd)
    // like that :)
    int opCode;
    int address;

    while (opCode = getc(file) != EOF) // while the next character in the file isn't the end of the file
    {
        address = getc(file); // get the address var from the file
        tinyMachine(opCode, address); // run it through the tinyMachine function (the meat of the program)
    }
    fclose(file);
    exit(0);
}

void startPrompt() {
    printf("Assembling Program...");
    printf("Program Assembled.");
    printf("Run.");
}

void fileCheck(FILE *file)
{
    if (file == NULL)
    {
        printf("No File Submitted");
        exit(132);
    }
}

// Fetch Cycle
// MAR <- PC
// PC <- PC + 1
// MDR <- IM[MAR]
// IR <- MDR
void fetch(void)
{
    memoryAddressRegister = programCounter;
    programCounter++;
    memoryDataRegister = instructionMemory[memoryAddressRegister];
    instructionRegister = memoryDataRegister;
}

// Load Cycle
// MAR2 <- IR.ADDRESS
// MDR2 <- DM[MAR2]
// A <- MDR2
void load(void)
{
    memoryAddressRegister2 = instructionRegister.deviceOrAddress;
    memoryDataRegister2 = memory[memoryAddressRegister];
    accumulator = memoryAddressRegister2;
}

// ADD CYCLE
// MAR2 <- IR.ADDRESS
// MDR2 <- DM[MAR2]
// A <- A + MDR2
void add(void)
{
    memoryAddressRegister2 = instructionRegister.deviceOrAddress;
    memoryDataRegister2 = memory[memoryAddressRegister2];
    accumulator = accumulator + memoryDataRegister2;
}

// STORE CYCLE
// MAR2 <- IR.ADDRESS
// MDR2 <- A
// DM[MAR2] <- MDR2
void store(void)
{
    memoryAddressRegister2 = instructionRegister.deviceOrAddress;
    memoryDataRegister2 = accumulator;
    memory[memoryAddressRegister2] = memoryDataRegister2;
}

// SUB CYCLE
// MAR2 <- IR.ADDRESS
// MDR2 <- DM[MAR]
// A <- A - MDR2
void sub(void)
{
    memoryAddressRegister2 = instructionRegister.deviceOrAddress;
    memoryDataRegister2 = memory[memoryAddressRegister];
    accumulator = accumulator - memoryDataRegister2;
}

// IN
// A <- INPUT FROM KEYBOARD
void in(void)
{
    printf("");
    scanf("%d", &accumulator);
}

// OUT
// SCREEN <- A

// END
// RUN <- 0

// JMP
// PC <- IR.ADDRESS
void jmp(void)
{
    programCounter = instructionRegister.deviceOrAddress;
}

// SKIP
// IF (A==0) PC <- PC + 1
void skip(void)
{
    if(accumulator == 0)
    {
        programCounter++;
    }
}

//function that prints out the variables
void print(void)
{
    printf("\nPC: %d | A: %d | MEM: [", programCounter, accumulator);
    for (int i = 0; i < 9; i++)
    {
        printf("%d", memory[i]);
    }
    printf("]\n");
}

//                      0           1           2           3           4           5       6           7           8          9
// enum opCodes { OpFetch = 0, OpLoad = 1, OpAdd = 2, OpStore = 3, OpSub = 4, OpIn = 5, OpOut = 6, OpEnd = 7, OpJump = 8, OpSkipZ = 9};

//start of tinymachine function that will deal with the File
void tinyMachine(int opCode, int address)
{
    switch(opCode)
    {
        case 0:
            printf("\nStarting Fetch/Execute Cycle...\n");
            fetch();
            print();
            break;
        case 1:
            printf("\nLoading from the address %d...\n", address);
            load();
            print();
            break;
        case 2:
            printf("\n Adding accumulator and value from the address %d\n", address);
            add();
            print();
            break;
        case 3:
            printf("\n Storing accumulator to memory location 0\n");
            store();
            print();
            break;
        case 4:
            printf("\n Please input a value: \n");
            sub();
            print();
            break;
        case 5:
            printf("\nAccumulator's current value is: %d\n", accumulator);
            in();
            print();
            break;
        case 6:
            out();
            printf("\n Program is now complete.\n");
            exit(1);
        case 7:
            printf("\n Setting the program counter to %d\n", address);
            jmp();
            print();
            break;
        case 8:
            printf("\nSkipping to the next instruction\n");
            skip();
            print();
            break;
        default:
            printf("\nThere was an error. Program is now exiting!\n");
            break;
    }
}