#include "instructions.h"

instruction::instruction(mnemonic m, const std::vector<uint16_t>& arguments)
: m_m(m), m_args(arguments)
{
    auto count = argument_count(m);

    if (arguments.size() != count) {
        throw std::domain_error("Invalid number of arguments for mnemonic");
    }
}

instruction::instruction(mnemonic m)
: instruction(m, {}) {

}

unsigned int argument_count(mnemonic m)
{
    switch (m) {
        case mnemonic::CONST: return 1;
        case mnemonic::ADD: return 0;
        case mnemonic::SUB: return 0;
        case mnemonic::MUL: return 0;
        case mnemonic::DIV: return 0;
        case mnemonic::MOD: return 0;
        case mnemonic::EQ: return 0;
        case mnemonic::LT: return 0;
        case mnemonic::NOT: return 0;
        case mnemonic::DUP: return 0;
        case mnemonic::SWAP: return 0;
        case mnemonic::LDI: return 0;
        case mnemonic::STI: return 0;
        case mnemonic::GETBP: return 0;
        case mnemonic::GETSP: return 0;
        case mnemonic::INCSP: return 1;
        case mnemonic::DECSP: return 1;
        case mnemonic::GOTO: return 1;
        case mnemonic::IFZERO: return 1;
        case mnemonic::IFNZERO: return 1;
        case mnemonic::CALL: return 2;
        case mnemonic::TCALL: return 3;
        case mnemonic::RET: return 1;
        case mnemonic::PRINTI: return 0;
        case mnemonic::PRINTC: return 0;
        case mnemonic::LDARGS: return 0;
        case mnemonic::STOP: return 0;
        case mnemonic::NOOP: return 0;
    }
}

std::ostream& operator<<(std::ostream& str, const mnemonic &m) {
    switch (m) {
        case mnemonic::CONST: str << "CONST"; break;
        case mnemonic::ADD: str << "ADD"; break;
        case mnemonic::SUB: str << "SUB"; break;
        case mnemonic::MUL: str << "MUL"; break;
        case mnemonic::DIV: str << "DIV"; break;
        case mnemonic::MOD: str << "MOD"; break;
        case mnemonic::EQ: str << "EQ"; break;
        case mnemonic::LT: str << "LT"; break;
        case mnemonic::NOT: str << "NOT"; break;
        case mnemonic::DUP: str << "DUP"; break;
        case mnemonic::SWAP: str << "SWAP"; break;
        case mnemonic::LDI: str << "LDI"; break;
        case mnemonic::STI: str << "STI"; break;
        case mnemonic::GETBP: str << "GETBP"; break;
        case mnemonic::GETSP: str << "GETSP"; break;
        case mnemonic::INCSP: str << "INCSP"; break;
        case mnemonic::DECSP: str << "DECSP"; break;
        case mnemonic::GOTO: str << "GOTO"; break;
        case mnemonic::IFZERO: str << "IFZERO"; break;
        case mnemonic::IFNZERO: str << "IFNZERO"; break;
        case mnemonic::CALL: str << "CALL"; break;
        case mnemonic::TCALL: str << "TCALL"; break;
        case mnemonic::RET: str << "RET"; break;
        case mnemonic::PRINTI: str << "PRINTI"; break;
        case mnemonic::PRINTC: str << "PRINTC"; break;
        case mnemonic::LDARGS: str << "LDARGS"; break;
        case mnemonic::STOP: str << "STOP"; break;
        case mnemonic::NOOP: str << "NOOP"; break;
    }
    return str;
}

std::ostream &operator<<(std::ostream &str, const instruction &instr) {

    auto m = instr.mnem();
    str << m;
    auto arg_count = argument_count(m);
    for (size_t i=0;i<arg_count;++i) {
        str << " ";
        str << instr.arg(i);
    }

    if (m == mnemonic::CONST) {
        auto c = static_cast<char>(instr.arg(0));
        if (c == '\n') {
            str << " ; '\\n'";
        } else if (c == '\0') {
            str << " ; '\\0'";
        } else {
            str << " ; '" << c << "' 0x" << std::hex << instr.arg(0) << std::dec;
        }
    }

    return str;
}

std::vector<instruction> from_binary_list(const std::vector<uint16_t> &code) {

    std::vector<instruction> result;

    for (auto i = code.begin(); i < code.end(); ++i) {
        auto m = static_cast<mnemonic>(*i);
        std::vector<uint16_t> args;

        size_t arg_count = argument_count(m);
        for (auto arg = 0; arg < arg_count; ++arg) {
            ++i;
            args.push_back(*i);
        }

        result.emplace_back(m, args);
    }

    return result;
}
