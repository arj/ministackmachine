Mini stack machine
==================

The mini stack machine is an implementation of a small toy language.
The base type of the language is uint16.

It supports the following commands:

| Opcode   | Mnemonic  | Stack behavior                  | Comment                                                    |
|----------|-----------|---------------------------------|------------------------------------------------------------|
| 0x0000   | CONST i   | s => s,i                        | Push a constant on the stack                               |
| 0x0001   | ADD       | s,v1,v2 => s,v                  | Add v=(v1+v2) to the stack                                 |
| 0x0002   | SUB       | s,v1,v2 => s,v                  | Add v=(v1-v2) to the stack                                 |
| 0x0003   | MUL       | s,v1,v2 => s,v                  | Add v=(v1\*v2) to the stack                                |
| 0x0004   | DIV       | s,v1,v2 => s,v                  | Add v=(v1/v2) to the stack                                 |
| 0x0005   | MOD       | s,v1,v2 => s,v                  | Add v=(v1 % v2) to the stack                               |
| 0x0006   | EQ        | s,v1,v2 => s,v                  | Push 1 if v1 == v2 otherwise 0                             |
| 0x0007   | LT        | s,v1,v2 => s,v                  | Push 1 if v1 < v2 otherwise 0                              |
| 0x0008   | NOT       | s,v => s,!v                     | Push 1 if v == 0 otherwise 1                               |
| 0x0009   | DUP       | s,v => s,v,v                    | Duplicates the top value                                   |
| 0x000A   | SWAP      | s,v1,v2 => s,v2,v1              | Swaps the upper two values                                 |
| 0x000B   | LDI       | s,i => s,s[i]                   | Load value from stack position                             |
| 0x000C   | STI       | s,i,v => s,v                    | Store value on stack position                              |
| 0x000D   | GETBP     | s => s,bp                       | Pushes the value of the base pointer                       |
| 0x000E   | GETSP     | s => s,sp                       | Pushes the value of the stack pointer                      |
| 0x000F   | INCSP m   | s => s,v1,...,vm                | Grow stack, v1,...,vm are undefined                        |
| 0x0010   | DECSP m   | s,v1,...,vm => s                | Shrink stack, v1,...,vm are undefined                      |
| 0x0011   | GOTO a    | s => s                          | Jump to instruction pc=a                                   |
| 0x0012   | IFZERO a  | s,v => s                        | If v == 0 then jump to pc=a                                |
| 0x0013   | IFNZERO a | s,v => s                        | If v /= 0 then jump to pc=a                                |
| 0x0014   | CALL m a  | s,v1,...,vm => s,r,bp,v1,...,vm | Make a call to a and create new stack frame                |
| 0x0015   | reserverd | -                               | -                                                          |
| 0x0016   | RET m     | s,r,b,v1,...,vm,v => s,v        | Return to return address with value v and drop stack frame |
| 0x0017   | PRINTI    | s,v => s                        | Print uint16 value v on console                            |
| 0x0018   | PRINTC    | s,v => s                        | Print uint16 value v interpreted as char on console        |
| 0x0019   | LDARGS    | s => s,v1,...,vm,c              | Load command arguments (uint16) and count on stack         |
| 0x0020   | STOP      | s => s                          | Stop execution                                             |
| 0x0021   | NOOP      | s => s                          | No operation                                               |

Error behavior
==============

If the opcode is unknown the stackmachine stops.
If the stack has too few elements execute the operation the behavior is undefined.
