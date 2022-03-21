# TOY Machine Documentation

Toy Machine has a 16-bit Architecture.
Therefore the RAM is 256 16-bit words long and the machine
has 16x16-bit registers with register R0 always zero.

PC (stands for program counter) is an extra 8-bit register to store the current program execution address.

The program starts at memory address 0x10.

## Abbreviations
X,Y and Z are in {0,...,9,A,...,F}.

ADDR stands for a 8-bit value.

[ADDR] means load/store from/at ADDR.

[RX]   means load/store from/at ADDR written in RX.

## RAM Table
| Address     | Description         |
|-------------|---------------------|
| 0x00 - 0x0F | RAW binary data     |
| 0x10 - 0xFE | Program             |
| 0xFF        | IO-Address          |

## Operations

| Opcode | Operation | Description                                           | Code            |
| -------|-----------|-------------------------------------------------------|-----------------|
| 0      | halt      | Stops Toy Machine                                     | halt            |
| 1      | add       | Addition                                              | add RX, RY, RZ  |
| 2      | sub       | Subtraction                                           | sub RX, RY, RZ  |
| 3      | and       | Bitwise AND                                           | and RX, RY, RZ  |
| 4      | xor       | Bitwise exlusive OR                                   | xor RX, RY, RZ  |
| 5      | shl       | Shift left                                            | shl RX, RY, RZ  |
| 6      | shr       | Shift right                                           | shr RX, RY, RZ  |
| 7      | ldaddr    | Moves a 8-bit value into Register                     | ldaddr RX, ADDR |
| 8      | ld        | Loads a value from memory address                     | ld RX, [ADDR]   |
| 9      | str       | Stores a value at memory address                      | str RX, [ADDR]  |
| A      | ldi       | Loads a value from memory address written in register | ldi RX, [RY]    |
| B      | stri      | Stores a value at memory address written in register  | stri RX, [RY]   |
| C      | bz        | Jumps to address if Register is zero                  | bz RX, ADDR     |
| D      | bp        | Jumps to address if Register is > 0                   | bp RX, ADRR     |
| E      | jmpreg    | Jumps to address written in register                  | jmpreg [RX]     |
| F      | jmpl      | Jumps to address and stores old pc into register      | jmpl RX, ADDR   |