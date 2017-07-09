#include <gtest/gtest.h>

#include "../instructions.h"
#include "../interpreter.h"

TEST(Interpreter, ConstTest) {
    interpreter interp(mk_const(0x1234));
    interp.step();

    ASSERT_EQ(0x1234, interp.stack()[1]);
    ASSERT_EQ(0x0001, interp.registers().sp);
    ASSERT_EQ(0x0002, interp.registers().pc);
}

TEST(Interpreter, AddTest) {
    program p;
    p.append(mk_const(0x0001));
    p.append(mk_const(0x0001));
    p.append(mk_add());
    interpreter interp(p.code());
    interp.step();
    interp.step();
    interp.step();

    ASSERT_EQ(0x0002, interp.stack()[1]);
    ASSERT_EQ(0x0001, interp.registers().sp);
    ASSERT_EQ(0x0005, interp.registers().pc);
}

TEST(Interpreter, SubTest) {
    program p;
    p.append(mk_const(0x0001));
    p.append(mk_const(0x0001));
    p.append(mk_sub());
    interpreter interp(p.code());
    interp.step();
    interp.step();
    interp.step();

    ASSERT_EQ(0x0000, interp.stack()[1]);
    ASSERT_EQ(0x0001, interp.registers().sp);
    ASSERT_EQ(0x0005, interp.registers().pc);
}

TEST(Interpreter, MulTest) {
    program p;
    p.append(mk_const(0x0002));
    p.append(mk_const(0x0002));
    p.append(mk_mul());
    interpreter interp(p.code());
    interp.step();
    interp.step();
    interp.step();

    ASSERT_EQ(0x0004, interp.stack()[1]);
    ASSERT_EQ(0x0001, interp.registers().sp);
    ASSERT_EQ(0x0005, interp.registers().pc);
}

TEST(Interpreter, DivTest) {
    program p;
    p.append(mk_const(0x0008));
    p.append(mk_const(0x0004));
    p.append(mk_div());
    interpreter interp(p.code());
    interp.step();
    interp.step();
    interp.step();

    ASSERT_EQ(0x0002, interp.stack()[1]);
    ASSERT_EQ(0x0001, interp.registers().sp);
    ASSERT_EQ(0x0005, interp.registers().pc);
}

TEST(Interpreter, ModTest) {
    program p;
    p.append(mk_const(0x0009));
    p.append(mk_const(0x0004));
    p.append(mk_mod());
    interpreter interp(p.code());
    interp.step();
    interp.step();
    interp.step();

    ASSERT_EQ(0x0001, interp.stack()[1]);
    ASSERT_EQ(0x0001, interp.registers().sp);
    ASSERT_EQ(0x0005, interp.registers().pc);
}

TEST(Interpreter, EqTrueTest) {
    program p;
    p.append(mk_const(0x0001));
    p.append(mk_const(0x0001));
    p.append(mk_eq());
    interpreter interp(p.code());
    interp.step();
    interp.step();
    interp.step();

    ASSERT_EQ(0x0001, interp.stack()[1]);
    ASSERT_EQ(0x0001, interp.registers().sp);
    ASSERT_EQ(0x0005, interp.registers().pc);
}

TEST(Interpreter, EqFalseTest) {
    program p;
    p.append(mk_const(0x0001));
    p.append(mk_const(0x0000));
    p.append(mk_eq());
    interpreter interp(p.code());
    interp.step();
    interp.step();
    interp.step();

    ASSERT_EQ(0x0000, interp.stack()[1]);
    ASSERT_EQ(0x0001, interp.registers().sp);
    ASSERT_EQ(0x0005, interp.registers().pc);
}

TEST(Interpreter, LtTrueTest) {
    program p;
    p.append(mk_const(0x0001));
    p.append(mk_const(0x0002));
    p.append(mk_lt());
    interpreter interp(p.code());
    interp.step();
    interp.step();
    interp.step();

    ASSERT_EQ(0x0001, interp.stack()[1]);
    ASSERT_EQ(0x0001, interp.registers().sp);
    ASSERT_EQ(0x0005, interp.registers().pc);
}

TEST(Interpreter, LtFalse1Test) {
    program p;
    p.append(mk_const(0x0001));
    p.append(mk_const(0x0001));
    p.append(mk_lt());
    interpreter interp(p.code());
    interp.step();
    interp.step();
    interp.step();

    ASSERT_EQ(0x0000, interp.stack()[1]);
    ASSERT_EQ(0x0001, interp.registers().sp);
    ASSERT_EQ(0x0005, interp.registers().pc);
}

TEST(Interpreter, LtFalse2Test) {
    program p;
    p.append(mk_const(0x0005));
    p.append(mk_const(0x0001));
    p.append(mk_lt());
    interpreter interp(p.code());
    interp.step();
    interp.step();
    interp.step();

    ASSERT_EQ(0x0000, interp.stack()[1]);
    ASSERT_EQ(0x0001, interp.registers().sp);
    ASSERT_EQ(0x0005, interp.registers().pc);
}

TEST(Interpreter, NotTrueTest) {
    program p;
    p.append(mk_const(0x0001));
    p.append(mk_not());
    interpreter interp(p.code());
    interp.step();
    interp.step();

    ASSERT_EQ(0x0000, interp.stack()[1]);
    ASSERT_EQ(0x0001, interp.registers().sp);
    ASSERT_EQ(0x0003, interp.registers().pc);
}

TEST(Interpreter, NotFalseTest) {
    program p;
    p.append(mk_const(0x0000));
    p.append(mk_not());
    interpreter interp(p.code());
    interp.step();
    interp.step();

    ASSERT_EQ(0x0001, interp.stack()[1]);
    ASSERT_EQ(0x0001, interp.registers().sp);
    ASSERT_EQ(0x0003, interp.registers().pc);
}

TEST(Interpreter, DupTest) {
    program p;
    p.append(mk_const(0x4711));
    p.append(mk_dup());
    interpreter interp(p.code());
    interp.step();
    interp.step();

    ASSERT_EQ(0x4711, interp.stack()[1]);
    ASSERT_EQ(0x4711, interp.stack()[2]);
    ASSERT_EQ(0x0002, interp.registers().sp);
    ASSERT_EQ(0x0003, interp.registers().pc);
}

TEST(Interpreter, SwapTest) {
    program p;
    p.append(mk_const(0x1234));
    p.append(mk_const(0x5678));
    p.append(mk_swap());
    interpreter interp(p.code());
    interp.step();
    interp.step();
    interp.step();

    ASSERT_EQ(0x5678, interp.stack()[1]);
    ASSERT_EQ(0x1234, interp.stack()[2]);
    ASSERT_EQ(0x0002, interp.registers().sp);
    ASSERT_EQ(0x0005, interp.registers().pc);
}

TEST(Interpreter, LdiTest) {
    program p;
    p.append(mk_const(0x1234));
    p.append(mk_const(0x0001));
    p.append(mk_ldi());
    interpreter interp(p.code());
    interp.step();
    interp.step();
    interp.step();

    ASSERT_EQ(0x1234, interp.stack()[1]);
    ASSERT_EQ(0x1234, interp.stack()[2]);
    ASSERT_EQ(0x0002, interp.registers().sp);
    ASSERT_EQ(0x0005, interp.registers().pc);
}

TEST(Interpreter, StiTest) {
    program p;
    p.append(mk_const(0x0005));
    p.append(mk_const(0x5678));
    p.append(mk_sti());
    interpreter interp(p.code());
    interp.step();
    interp.step();
    interp.step();

    ASSERT_EQ(0x5678, interp.stack()[1]);
    ASSERT_EQ(0x5678, interp.stack()[5]);
    ASSERT_EQ(0x0001, interp.registers().sp);
    ASSERT_EQ(0x0005, interp.registers().pc);
}

TEST(Interpreter, GetBPTest) {
    program p;
    p.append(mk_getbp());
    interpreter interp(p.code());
    interp.step();

    ASSERT_EQ(0xffff, interp.stack()[1]);
    ASSERT_EQ(0x0001, interp.registers().sp);
    ASSERT_EQ(0x0001, interp.registers().pc);
}

TEST(Interpreter, GetSPTest) {
    program p;
    p.append(mk_getsp());
    p.append(mk_getsp());
    interpreter interp(p.code());
    interp.step();
    interp.step();

    ASSERT_EQ(0x0000, interp.stack()[1]);
    ASSERT_EQ(0x0001, interp.stack()[2]);
    ASSERT_EQ(0x0002, interp.registers().sp);
    ASSERT_EQ(0x0002, interp.registers().pc);
}

TEST(Interpreter, IncSpTest) {
    program p;
    p.append(mk_incsp(10));
    interpreter interp(p.code());
    interp.step();

    ASSERT_EQ(0x000A, interp.registers().sp);
    ASSERT_EQ(0x0002, interp.registers().pc);
}

TEST(Interpreter, DecSpTest) {
    program p;
    p.append(mk_incsp(10));
    p.append(mk_decsp(5));
    interpreter interp(p.code());
    interp.step();
    interp.step();

    ASSERT_EQ(0x0005, interp.registers().sp);
    ASSERT_EQ(0x0004, interp.registers().pc);
}

TEST(Interpreter, GotoTest) {
    program p;
    p.append(mk_goto(0x0009));
    interpreter interp(p.code());
    interp.step();

    ASSERT_EQ(0x0000, interp.registers().sp);
    ASSERT_EQ(0x0009, interp.registers().pc);
}

TEST(Interpreter, IfZeroTrueTest) {
    program p;
    p.append(mk_const(0x0000));
    p.append(mk_ifzero(0x0009));
    interpreter interp(p.code());
    interp.step();
    interp.step();

    ASSERT_EQ(0x0000, interp.registers().sp);
    ASSERT_EQ(0x0009, interp.registers().pc);
}

TEST(Interpreter, IfZeroFalseTest) {
    program p;
    p.append(mk_const(0x0001));
    p.append(mk_ifzero(0x0009));
    interpreter interp(p.code());
    interp.step();
    interp.step();

    ASSERT_EQ(0x0000, interp.registers().sp);
    ASSERT_EQ(0x0004, interp.registers().pc);
}

TEST(Interpreter, IfNZeroTrueTest) {
    program p;
    p.append(mk_const(0x0001));
    p.append(mk_ifnzero(0x0009));
    interpreter interp(p.code());
    interp.step();
    interp.step();

    ASSERT_EQ(0x0000, interp.registers().sp);
    ASSERT_EQ(0x0009, interp.registers().pc);
}

TEST(Interpreter, IfNZeroFalseTest) {
    program p;
    p.append(mk_const(0x0000));
    p.append(mk_ifnzero(0x0009));
    interpreter interp(p.code());
    interp.step();
    interp.step();

    ASSERT_EQ(0x0000, interp.registers().sp);
    ASSERT_EQ(0x0004, interp.registers().pc);
}

TEST(Interpreter, StopTest) {
    program p;
    p.append(mk_stop());
    interpreter interp(p.code());
    interp.step();

    ASSERT_EQ(0x0000, interp.registers().sp);
    ASSERT_EQ(0x0001, interp.registers().pc);
    ASSERT_TRUE(interp.is_stopped());
}

TEST(Interpreter, NoopTest) {
    program p;
    p.append(mk_noop());
    interpreter interp(p.code());
    interp.step();

    ASSERT_EQ(0x0000, interp.registers().sp);
    ASSERT_EQ(0x0001, interp.registers().pc);
}

TEST(Interpreter, PrintiTest) {
    program p;
    p.append(mk_const(0x0001));
    p.append(mk_printi());
    interpreter interp(p.code());
    interp.step();
    interp.step();

    ASSERT_EQ(0x0000, interp.registers().sp);
    ASSERT_EQ(0x0003, interp.registers().pc);
}

TEST(Interpreter, PrintcTest) {
    program p;
    p.append(mk_const('A'));
    p.append(mk_printc());
    interpreter interp(p.code());
    interp.step();
    interp.step();

    ASSERT_EQ(0x0000, interp.registers().sp);
    ASSERT_EQ(0x0003, interp.registers().pc);
}

TEST(Interpreter, LdargsTest) {
    program p;
    p.append(mk_ldargs());
    interpreter interp(p.code());
    interp.set_command_line_arguments({1,2,3});
    interp.step();

    ASSERT_EQ(0x0001, interp.stack()[1]);
    ASSERT_EQ(0x0002, interp.stack()[2]);
    ASSERT_EQ(0x0003, interp.stack()[3]);
    ASSERT_EQ(0x0004, interp.registers().sp);
    ASSERT_EQ(0x0001, interp.registers().pc);
}

TEST(Interpreter, CallTest) {
    FAIL();
}

TEST(Interpreter, TCallTest) {
    FAIL();
}

TEST(Interpreter, RetTest) {
    FAIL();
}