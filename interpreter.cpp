#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include "interpreter.h"

interpreter::interpreter(const std::vector<uint16_t> &code)
: m_tracing(false), m_stopped(false), pc(0), sp(0), bp(0xFFFF), code(code), m_stack()
{
    m_stack.resize(std::numeric_limits<uint16_t>::max(), 0x00);
    m_stack[0] = 0xFFFF;

    this->code.push_back(mk_stop());
}

namespace {
    std::string printable_chars(const std::string& s) {

        std::stringstream result;

        for (auto& c : s) {
            switch (c) {
                case '\n':
                    result << "\\n";
                    break;
                case '\r':
                    result << "\\r";
                    break;
                case '\t':
                    result << "\\t";
                    break;
                default:
                    if ((c <= 0x20) || (c > 0x7f)) {
                        result << "\\x" << std::hex << std::setw(2) << std::setfill('0');
                        result << static_cast<uint16_t>(c);
                    } else {
                        result << c;
                    }
                    break;
            }
        }

        return result.str();
    }
}

void interpreter::step() {

    if (m_stopped) {
        return;
    }

    static const unsigned short VAL_TRUE = static_cast<unsigned short>(1);
    static const unsigned short VAL_FALSE = static_cast<unsigned short>(0);

    std::stringstream output;
    std::string trace_str;

    auto i = code.at(pc);

    if (m_tracing) {
        std::stringstream trace;
        trace << "pc=" << std::hex << std::internal << std::setw(4) << std::setfill('0') << pc << " ";
        trace << "sp=" << std::hex << std::internal << std::setw(4) << std::setfill('0') << sp << " ";
        trace << "bp=" << std::hex << std::internal << std::setw(4) << std::setfill('0') << bp << " ";
        trace << " [";

        if (sp >= 1) {
            trace << std::hex << std::internal << std::setw(4) << std::setfill('0') << m_stack[sp] << " ";
            trace <<std::hex << std::internal << std::setw(4) << std::setfill('0') << m_stack[sp-1] << " ... ]";
        } else if (sp == 0) {
            trace << std::hex << std::internal << std::setw(4) << std::setfill('0') << m_stack[sp];
            trace << "          ] ";
        } else {
            trace << "]";
        }

        trace << " ";
        trace << i;

        trace_str = trace.str();
    }

    ++pc;

    switch (i) {
        case mnemonic::CONST:
            // s => s,i
            ++sp;
            m_stack[sp] = code[pc];
            ++pc;
            break;
        case mnemonic::ADD:
            // s,v1,v2 => s,(v1 + v2)
            m_stack[sp-1] = m_stack[sp-1] + m_stack[sp];
            --sp;
            break;
        case mnemonic::SUB:
            // s,v1,v2 => s,(v1 - v2)
            m_stack[sp-1] = m_stack[sp-1] - m_stack[sp];
            --sp;
            break;
        case mnemonic::MUL:
            // s,v1,v2 => s,(v1 * v2)
            m_stack[sp-1] = m_stack[sp-1] * m_stack[sp];
            --sp;
            break;
        case mnemonic::DIV:
            // s,v1,v2 => s,(v1 / v2)
            m_stack[sp-1] = m_stack[sp-1] / m_stack[sp];
            --sp;
            break;
        case mnemonic::MOD:
            // s,v1,v2 => s,(v1 % v2)
            m_stack[sp-1] = m_stack[sp-1] % m_stack[sp];
            --sp;
            break;
        case mnemonic::EQ:
            // s,v1,v2 => s,(v1 == v2)
            m_stack[sp-1] = m_stack[sp-1] == m_stack[sp] ? VAL_TRUE : VAL_FALSE;
            --sp;
            break;
        case mnemonic::LT:
            // s,v1,v2 => s,(v1 < v2)
            m_stack[sp-1] = m_stack[sp-1] < m_stack[sp] ? VAL_TRUE : VAL_FALSE;
            --sp;
            break;
        case mnemonic::NOT:
            // s,v => s,!v
            m_stack[sp] = m_stack[sp] == VAL_FALSE ? VAL_TRUE : VAL_FALSE;
            break;
        case mnemonic::DUP:
            // s,v => s,v,v
            m_stack[sp+1] = m_stack[sp];
            ++sp;
            break;
        case mnemonic::SWAP:
            // s,v1,v2 => s,v2,v1
            std::swap(m_stack[sp], m_stack[sp-1]);
            break;
        case mnemonic::LDI:
            // s,i => s,s[i]
            m_stack[sp] = m_stack[m_stack[sp]];
            break;
        case mnemonic::STI: {
            // s,i,v => s,v

            auto i = m_stack[sp - 1];
            auto v = m_stack[sp];
            m_stack[i] = v;
            m_stack[sp - 1] = v;
            --sp;
            break;
        }
        case mnemonic::GETBP:
            m_stack[sp+1] = bp;
            ++sp;
            break;
        case mnemonic::GETSP:
            // s => s,sp
            m_stack[sp+1] = sp;
            ++sp;
            break;
        case mnemonic::INCSP: {
            sp += code[pc];
            ++pc;
            break;
        }
        case mnemonic::DECSP: {
            sp -= code[pc];
            ++pc;
            break;
        }
        case mnemonic::GOTO:
            pc = code[pc];
            break;
        case mnemonic::IFZERO:
            ++pc;
            if (m_stack[sp] == 0) {
                pc = code[pc-1];
            }
            --sp;
            break;
        case mnemonic::IFNZERO:
            ++pc;
            if (m_stack[sp] != 0) {
                pc = code[pc-1];
            }
            --sp;
            break;
        case mnemonic::CALL: {
            // s,v1,...,vm => s,r,bp,v1,...,vm
            auto m = code[pc];
            ++pc;
            // move arguments
            for (int idx = 0; idx < m; ++idx) {
                m_stack[sp + 2 - idx] = m_stack[sp - idx];
            }
            uint16_t stack_r  = static_cast<uint16_t>(sp - m + 1);
            uint16_t stack_bp = static_cast<uint16_t>(sp - m + 2);

            m_stack[stack_r] = pc; // return address
            m_stack[stack_bp] = bp; // old bp

            bp = static_cast<uint16_t>(stack_bp + 1); // one after old_bp
            sp = stack_bp + m;
            pc = code[pc];
            ++pc;
            break;
        }
        case mnemonic::TCALL: {
            // s,r,b,u1,...,un,v1,...,vm => s,r,b,v1,...,vm
            auto m = code[pc]; ++pc;
            auto n = code[pc]; ++pc;
            auto a = code[pc]; ++pc;
            // move vi arguments to uj
            for (int idx = 0; idx < m; ++idx) {
                m_stack[sp - n - idx] = m_stack[sp - idx];
            }

            sp = sp - m - n;

            pc = a;
            break;
        }
        case mnemonic::RET: {
            // s,r,b,v1,...,vm,v => s,v
            // bp points to current stackframe's v1.
            auto old_bp = m_stack[bp - 1];
            pc = m_stack[bp - 2];
            auto v = m_stack[sp];
            sp = static_cast<uint16_t>(bp - 2u);
            m_stack[sp] = v;
            bp = old_bp;
            break;
        }
        case mnemonic::PRINTI:
            output << m_stack[sp];
            --sp;
            break;
        case mnemonic::PRINTC:
            output << static_cast<char>(m_stack[sp]);
            --sp;
            break;
        case mnemonic::LDARGS:
            // s => s,i_1,...,i_n,n
            for (auto& cmd_arg : cmd_args) {
                m_stack[sp+1] = cmd_arg;
                ++sp;
            }
            m_stack[sp+1] = static_cast<uint16_t>(cmd_args.size());
            ++sp;
            break;
        case mnemonic::STOP:
            // s => s
            m_stopped = true;
            return;
        case mnemonic::NOOP:
            // s => s
            break;
    }

    if (!trace_str.empty()) {
        std::cout << std::setw(60) << std::left << printable_chars(output.str()) << trace_str << std::endl;
    } else {
        std::cout << output.str();
    }
}

bool interpreter::is_stopped() const {
    return m_stopped;
}

std::string interpreter::program() const {
    std::stringstream ss;

//    for (auto& i : instructions) {
//        ss << i << std::endl;
//    }

    return ss.str();
}

void interpreter::set_tracing(bool tracing) {
    m_tracing = tracing;
}

void interpreter::run() {
    while (!m_stopped) {
        step();
    }
}

void interpreter::set_command_line_arguments(const std::vector<uint16_t> &args) {
    cmd_args = args;
}

void interpreter::set_stack(const std::vector<uint16_t> &stack) {
    m_stack = stack;
}

const std::vector<uint16_t> &interpreter::stack() const {
    return m_stack;
}

interpreter::configs interpreter::registers() const {
    configs r;
    r.bp = bp;
    r.pc = pc;
    r.sp = sp;
    return r;
}
