#include <iostream>
#include <map>
#include "instructions.h"
#include "interpreter.h"


using code = std::vector<uint16_t>;

code binary_example_program1() {
    return {0x00, 0x47, 0x18, 0x00, 0x6f, 0x18,
            0x00, 0x6f, 0x18, 0x00, 0x64, 0x18,
            0x00, 0x20, 0x18, 0x00, 0x62, 0x18,
            0x00, 0x79, 0x18, 0x00, 0x65, 0x18,
            0x00, 0x10, 0x18, 0x20};
}

std::vector<instruction> example_program1() {

    return {
        instruction(mnemonic::CONST, {'H'}),
        instruction(mnemonic::PRINTC),
        instruction(mnemonic::CONST, {'e'}),
        instruction(mnemonic::PRINTC),
        instruction(mnemonic::CONST, {'l'}),
        instruction(mnemonic::PRINTC),
        instruction(mnemonic::CONST, {'l'}),
        instruction(mnemonic::PRINTC),
        instruction(mnemonic::CONST, {'o'}),
        instruction(mnemonic::PRINTC),
        instruction(mnemonic::CONST, {' '}),
        instruction(mnemonic::PRINTC),
        instruction(mnemonic::CONST, {'W'}),
        instruction(mnemonic::PRINTC),
        instruction(mnemonic::CONST, {'o'}),
        instruction(mnemonic::PRINTC),
        instruction(mnemonic::CONST, {'r'}),
        instruction(mnemonic::PRINTC),
        instruction(mnemonic::CONST, {'l'}),
        instruction(mnemonic::PRINTC),
        instruction(mnemonic::CONST, {'d'}),
        instruction(mnemonic::PRINTC),
        instruction(mnemonic::CONST, {'\n'}),
        instruction(mnemonic::PRINTC),
        instruction(mnemonic::CONST, {4711}),
        instruction(mnemonic::CONST, {89}),
        instruction(mnemonic::ADD),
        instruction(mnemonic::PRINTI),
        instruction(mnemonic::STOP)
    };
}

using symbolic_program = std::vector<std::pair<uint16_t, instruction>>;

namespace {
    const uint16_t NO_LABEL = 0xFFFF;
}

symbolic_program example_call() {

    return {
            {NO_LABEL, instruction(mnemonic::CONST, {'B'})},
            {NO_LABEL, instruction(mnemonic::PRINTC)},
            {NO_LABEL, instruction(mnemonic::CONST, {'1'})},
            {NO_LABEL, instruction(mnemonic::CONST, {'2'})},
            {NO_LABEL, instruction(mnemonic::CALL, {2, 0x1000})},
            {NO_LABEL, instruction(mnemonic::PRINTC)},
            {NO_LABEL, instruction(mnemonic::CONST, {'A'})},
            {NO_LABEL, instruction(mnemonic::PRINTC)},
            {NO_LABEL, instruction(mnemonic::STOP)},
            //
            {0x1000, instruction(mnemonic::GETBP)},
            {NO_LABEL, instruction(mnemonic::LDI)},
            {NO_LABEL, instruction(mnemonic::PRINTC)},
            //
            {NO_LABEL, instruction(mnemonic::GETBP)},
            {NO_LABEL, instruction(mnemonic::CONST, {1})},
            {NO_LABEL, instruction(mnemonic::ADD)},
            {NO_LABEL, instruction(mnemonic::LDI)},
            {NO_LABEL, instruction(mnemonic::PRINTC)},
            //
            {NO_LABEL, instruction(mnemonic::CONST, {'R'})},
            {NO_LABEL, instruction(mnemonic::RET, {0})}
    };
}

symbolic_program recursion() {

    return {
            {0x1000, instruction(mnemonic::CONST, {'A'})},
            {NO_LABEL, instruction(mnemonic::PRINTC)},
            {NO_LABEL, instruction(mnemonic::TCALL, {0, 0, 0x1000})}
    };
}

symbolic_program print_cmd_args() {

    return {
            {NO_LABEL, instruction(mnemonic::LDARGS)},
            {0x1000, instruction(mnemonic::DUP)},
            {NO_LABEL, instruction(mnemonic::IFZERO, {0x9000})},
            {NO_LABEL, instruction(mnemonic::DUP)},
            {NO_LABEL, instruction(mnemonic::GETSP)},
            {NO_LABEL, instruction(mnemonic::SWAP)},
            {NO_LABEL, instruction(mnemonic::SUB)},
            {NO_LABEL, instruction(mnemonic::CONST, {1})},
            {NO_LABEL, instruction(mnemonic::SUB)},
            {NO_LABEL, instruction(mnemonic::LDI)},
            {NO_LABEL, instruction(mnemonic::PRINTI)},
            {NO_LABEL, instruction(mnemonic::CONST, {' '})},
            {NO_LABEL, instruction(mnemonic::PRINTC)},
            {NO_LABEL, instruction(mnemonic::CONST, {1})},
            {NO_LABEL, instruction(mnemonic::SUB)},
            {NO_LABEL, instruction(mnemonic::GOTO, {0x1000})},
            {0x9000  , instruction(mnemonic::STOP)}
    };
}


std::vector<instruction> symbolic_program_to_instructions(const symbolic_program& sprog) {
    std::map<uint16_t, uint16_t> labels;

    uint16_t idx = 0;
    for (auto& instr : sprog) {
        if (instr.first != NO_LABEL) {
            labels.emplace(instr.first, idx);
        }
        ++idx;
    }

    std::vector<instruction> result;
    // rewrite all labels
    for (auto& instr : sprog) {
        instruction i = instr.second;
        if (i.mnem() == mnemonic::CALL) {
            i.arg(1) = labels[i.arg(1)];
        } else if (i.mnem() == mnemonic::TCALL) {
            i.arg(2) = labels[i.arg(2)];

        } else if (i.mnem() == mnemonic::GOTO
                || i.mnem() == mnemonic::IFZERO
                || i.mnem() == mnemonic::IFNZERO) {
            i.arg(0) = labels[i.arg(0)];
        }

        result.push_back(i);
    }

    return result;
}

int main() {
    interpreter interp(binary_example_program1());
    //interpreter interp(symbolic_program_to_instructions(example_call()));
    //interpreter interp(symbolic_program_to_instructions(print_cmd_args()));
    interp.set_command_line_arguments({0x1,0x2,0x3,0x04,0x05});
    interp.set_command_line_arguments({5,4,3,2,1});

    //std::cout << interp.program() << std::endl;

    interp.set_tracing(false);

    interp.run();

    return 0;
}