#include <iostream>
#include <sstream>
#include <iomanip>
#include "interpreter.h"

interpreter::interpreter(const std::vector<instruction> &instructions)
: m_tracing(false), m_stopped(false), pc(0), sp(0), bp(0xFFFF), instructions(instructions), stack()
{
    stack.resize(std::numeric_limits<uint16_t>::max(), 0x00);
    stack[0] = 0xFFFF;

    this->instructions.push_back(instruction(mnemonic::STOP));
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

    auto i = instructions.at(pc);

    if (m_tracing) {
        std::stringstream trace;
        trace << "pc=" << std::hex << std::internal << std::setw(4) << std::setfill('0') << pc << " ";
        trace << "sp=" << std::hex << std::internal << std::setw(4) << std::setfill('0') << sp << " ";
        trace << "bp=" << std::hex << std::internal << std::setw(4) << std::setfill('0') << bp << " ";
        trace << " [";

        if (sp >= 1) {
            trace << std::hex << std::internal << std::setw(4) << std::setfill('0') << stack[sp] << " ";
            trace <<std::hex << std::internal << std::setw(4) << std::setfill('0') << stack[sp-1] << " ... ]";
        } else if (sp == 0) {
            trace << std::hex << std::internal << std::setw(4) << std::setfill('0') << stack[sp];
            trace << "          ] ";
        } else {
            trace << "]";
        }

        trace << " ";
        trace << i;

        trace_str = trace.str();
    }

    ++pc;

    switch (i.mnem()) {
        case mnemonic::CONST:
            // s => s,i
            ++sp;
            stack[sp] = i.arg(0);
            break;
        case mnemonic::ADD:
            // s,v1,v2 => s,(v1 + v2)
            stack[sp-1] = stack[sp-1] + stack[sp];
            --sp;
            break;
        case mnemonic::SUB:
            // s,v1,v2 => s,(v1 - v2)
            stack[sp-1] = stack[sp-1] - stack[sp];
            --sp;
            break;
        case mnemonic::MUL:
            // s,v1,v2 => s,(v1 * v2)
            stack[sp-1] = stack[sp-1] * stack[sp];
            --sp;
            break;
        case mnemonic::DIV:
            // s,v1,v2 => s,(v1 / v2)
            stack[sp-1] = stack[sp-1] / stack[sp];
            --sp;
            break;
        case mnemonic::MOD:
            // s,v1,v2 => s,(v1 % v2)
            stack[sp-1] = stack[sp-1] % stack[sp];
            --sp;
            break;
        case mnemonic::EQ:
            // s,v1,v2 => s,(v1 == v2)
            stack[sp-1] = stack[sp-1] == stack[sp] ? VAL_TRUE : VAL_FALSE;
            --sp;
            break;
        case mnemonic::LT:
            // s,v1,v2 => s,(v1 < v2)
            stack[sp-1] = stack[sp-1] < stack[sp] ? VAL_TRUE : VAL_FALSE;
            --sp;
            break;
        case mnemonic::NOT:
            // s,v => s,!v
            stack[sp] = stack[sp] == VAL_FALSE ? VAL_TRUE : VAL_FALSE;
            break;
        case mnemonic::DUP:
            // s,v => s,v,v
            stack[sp+1] = stack[sp];
            ++sp;
            break;
        case mnemonic::SWAP:
            // s,v1,v2 => s,v2,v1
            std::swap(stack[sp], stack[sp-1]);
            break;
        case mnemonic::LDI:
            // s,i => s,s[i]
            stack[sp] = stack[stack[sp]];
            break;
        case mnemonic::STI:
            stack[stack[sp-1]] = stack[sp];
            stack[sp-1] = stack[sp];
            --sp;
            break;
        case mnemonic::GETBP:
            stack[sp+1] = bp;
            ++sp;
            break;
        case mnemonic::GETSP:
            // s => s,sp
            stack[sp+1] = sp;
            ++sp;
            break;
        case mnemonic::INCSP: {
            sp += i.arg(0);
            break;
        }
        case mnemonic::DECSP: {
            sp -= i.arg(0);
            break;
        }
        case mnemonic::GOTO:
            pc = i.arg(0);
            return;
        case mnemonic::IFZERO:
            if (stack[sp] == 0) {
                pc = i.arg(0);
            }
            --sp;
            break;
        case mnemonic::IFNZERO:
            if (stack[sp] != 0) {
                pc = i.arg(0);
            }
            --sp;
            break;
        case mnemonic::CALL: {
            // s,v1,...,vm => s,r,bp,v1,...,vm
            auto m = i.arg(0);
            // move arguments
            for (int idx = 0; idx < m; ++idx) {
                stack[sp + 2 - idx] = stack[sp - idx];
            }
            uint16_t stack_r  = static_cast<uint16_t>(sp - m + 1);
            uint16_t stack_bp = static_cast<uint16_t>(sp - m + 2);

            stack[stack_r] = pc; // return address
            stack[stack_bp] = bp; // old bp

            bp = static_cast<uint16_t>(stack_bp + 1); // one after old_bp
            sp = stack_bp + m;
            pc = i.arg(1);
            break;
        }
        case mnemonic::TCALL: {
            // s,r,b,u1,...,un,v1,...,vm => s,r,b,v1,...,vm
            auto m = i.arg(0);
            auto n = i.arg(1);
            auto a = i.arg(2);
            // move vi arguments to uj
            for (int idx = 0; idx < m; ++idx) {
                stack[sp - n - idx] = stack[sp - idx];
            }

            sp = sp - m - n;

            pc = a;
            break;
        }
        case mnemonic::RET: {
            // s,r,b,v1,...,vm,v => s,v
            // bp points to current stackframe's v1.
            auto old_bp = stack[bp - 1];
            pc = stack[bp - 2];
            auto v = stack[sp];
            sp = static_cast<uint16_t>(bp - 2u);
            stack[sp] = v;
            bp = old_bp;
            break;
        }
        case mnemonic::PRINTI:
            output << stack[sp];
            --sp;
            break;
        case mnemonic::PRINTC:
            output << static_cast<char>(stack[sp]);
            --sp;
            break;
        case mnemonic::LDARGS:
            // s => s,i_1,...,i_n,n
            for (auto& cmd_arg : cmd_args) {
                stack[sp+1] = cmd_arg;
                ++sp;
            }
            stack[sp+1] = static_cast<uint16_t>(cmd_args.size());
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

    for (auto& i : instructions) {
        ss << i << std::endl;
    }

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
