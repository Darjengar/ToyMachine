# TOY Machine Documentation

## Abbreviations
X,Y,Z is in {0,...,9,A,...,F}.

ADDR stands for a 8-bit value.

[ADDR] means load/store from/at ADDR.

[RX]   means load/store from/at ADDR written in RX.

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