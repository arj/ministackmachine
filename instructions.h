#ifndef STACKMACHINE_INSTRUCTIONS_H
#define STACKMACHINE_INSTRUCTIONS_H

#include <ostream>
#include <cstdint>
#include <vector>

enum mnemonic : uint16_t
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

std::vector<uint16_t> mk_const(uint16_t v);
uint16_t mk_add();
uint16_t mk_sub();
uint16_t mk_mul();
uint16_t mk_div();
uint16_t mk_mod();
uint16_t mk_eq();
uint16_t mk_lt();
uint16_t mk_not();
uint16_t mk_dup();
uint16_t mk_swap();
uint16_t mk_ldi();
uint16_t mk_sti();
uint16_t mk_getbp();
uint16_t mk_getsp();
std::vector<uint16_t> mk_incsp(uint16_t count);
std::vector<uint16_t> mk_decsp(uint16_t count);
std::vector<uint16_t> mk_goto(uint16_t address);
std::vector<uint16_t> mk_ifzero(uint16_t address);
std::vector<uint16_t> mk_ifnzero(uint16_t address);
std::vector<uint16_t> mk_call(uint16_t variable_count, uint16_t address);
std::vector<uint16_t> mk_tcall(uint16_t variable_count, uint16_t old_variable_count, uint16_t address);
std::vector<uint16_t> mk_ret(uint16_t variable_count);
uint16_t mk_printi();
uint16_t mk_printc();
uint16_t mk_ldargs();
uint16_t mk_stop();
uint16_t mk_noop();

class program {
public:
    void append(uint16_t code);
    void append(const std::vector<uint16_t>& code);

    const std::vector<uint16_t>& code() const;
private:
    std::vector<uint16_t> bytes;
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
