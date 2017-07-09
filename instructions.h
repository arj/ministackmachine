#ifndef STACKMACHINE_INSTRUCTIONS_H
#define STACKMACHINE_INSTRUCTIONS_H

#include <ostream>
#include <cstdint>
#include <vector>

enum class mnemonic
{
    CONST = 0x00,
    ADD = 0x01,
    SUB = 0x02,
    MUL = 0x03,
    DIV = 0x04,
    MOD = 0x05,
    EQ = 0x06,
    LT = 0x07,
    NOT = 0x08,
    DUP = 0x09,
    SWAP = 0x0A,
    LDI = 0x0B,
    STI = 0x0C,
    GETBP = 0x0D,
    GETSP = 0x0E,
    INCSP = 0x0F,
    DECSP = 0x10,
    GOTO = 0x11,
    IFZERO = 0x12,
    IFNZERO = 0x13,
    CALL = 0x14,
    TCALL = 0x15,
    RET = 0x16,
    PRINTI = 0x17,
    PRINTC = 0x18,
    LDARGS = 0x19,
    STOP = 0x20,
    NOOP = 0x21
};

//! Get the number of arguments the mnemonic requires.
//! \param m the mnemonic
//! \return the number of arguments
unsigned int argument_count(mnemonic m);

std::ostream& operator<<(std::ostream& str, const mnemonic& m);

class instruction
{
public:
    instruction(mnemonic m, const std::vector<uint16_t>& arguments);
    instruction(mnemonic m);

    mnemonic mnem() const {
        return m_m;
    }

    uint16_t& arg(size_t idx) {
        return m_args[idx];
    }

    uint16_t arg(size_t idx) const {
        return m_args[idx];
    }

private:
    mnemonic m_m;
    std::vector<uint16_t> m_args;
};

std::ostream& operator<<(std::ostream& str, const instruction& instr);

std::vector<instruction> from_binary_list(const std::vector<uint16_t>& code);

#endif //STACKMACHINE_mnemonicS_H
