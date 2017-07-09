#ifndef STACKMACHINE_INTERPRETER_H
#define STACKMACHINE_INTERPRETER_H


#include "instructions.h"

class interpreter {
public:
    interpreter(const std::vector<uint16_t>& instructions);
    void set_command_line_arguments(const std::vector<uint16_t>& args);
    void set_stack(const std::vector<uint16_t>& stack);

    struct configs {
        uint16_t pc;
        uint16_t sp;
        uint16_t bp;
    };

    configs registers() const;

    const std::vector<uint16_t>& stack() const;

    void run();
    void step();
    bool is_stopped() const;
    void set_tracing(bool tracing);
    std::string program() const;

private:
    bool m_tracing;
    bool m_stopped;
    uint16_t pc;
    uint16_t sp;
    uint16_t bp;
    std::vector<uint16_t> code;
    std::vector<uint16_t> m_stack;
    std::vector<uint16_t> cmd_args;
};

#endif //STACKMACHINE_INTERPRETER_H
