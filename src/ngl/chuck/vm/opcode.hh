#pragma  once

#include <cstdint>

namespace banana { namespace NGL { namespace chuck { namespace vm {
// A22B70
// 40, 41, 59 and 98...107 are behavioral names
// the rest are named in the milestone build
enum class e_opcode : uint32_t {
    NOP,                                          // retail no-op; milestone asserts corrupt bytecode
    ADD,                                          // lhs += rhs
    AND,                                          // lhs &= rhs
    BF,                                           // pop condition; branch if false
    BF2,                                          // peek condition; branch if false
    BRA,                                          // unconditional relative branch
    BSL,                                          // call slf through A21D60
    BSR,                                          // synchronously call linked script function
    BST,                                          // start linked script function as current-instance child thread
    BTH,                                          // pop target instance, remap script function, start target thread
    DEC,                                          // --top
    DIV,                                          // lhs /= rhs
    DUP,                                          // copy stack/local/member bytes
    EQ,                                           // lhs == rhs
    GE,                                           // lhs >= rhs
    GT,                                           // lhs > rhs
    INC,                                          // ++top
    KIL,                                          // headshot selected thread through A22820
    LE,                                           // lhs <= rhs
    LNT,                                          // !top
    LT,                                           // lhs < rhs
    MOD,                                          // lhs %= rhs
    MUL,                                          // lhs *= rhs
    NE,                                           // lhs != rhs
    NEG,                                          // -top
    NOT,                                          // ~top
    OR,                                           // lhs |= rhs
    POP,                                          // discard/store stack bytes
    PSH,                                          // push/load/construct value
    RET,                                          // restore frame or complete thread through A21E40
    SHL,                                          // lhs <<= rhs
    SHR,                                          // lhs >>= rhs
    SPA,                                          // adjust stack pointer by signed byte count
    SPA0,                                         // adjust stack pointer and zero new bytes
    SUB,                                          // lhs -= rhs
    XOR,                                          // lhs ^= rhs
    ECB,                                          // mode-0 callback on popped target instance
    SCB,                                          // mode-0 callback on current instance
    ECO,                                          // mode-1 callback on popped target instance
    SCO,                                          // mode-1 callback on current instance
    REGISTER_CALLBACK_TARGET_MODE1_REMAP = 40,    // ECO with forced target-function remap
    REGISTER_CALLBACK_TARGET_MODE0_REMAP = 41,    // ECB with forced target-function remap
    KL2,                                          // kill target threads running current linked function
    MAS,                                          // selector-driven current-instance massacre
    MS2,                                          // massacre linked function on popped target instance
    WAITFRAME,                                    // yield without completing thread
    PAE,                                          // next PSH/POP uses popped inline-array index
    CASTISTR,                                     // num -> signed decimal string
    CASTFSTR,                                     // num -> three-decimal string
    STR_ADD,                                      // concatenate strings
    STR_EQ,                                       // strcmp(lhs, rhs) == 0
    STR_NE,                                       // strcmp(lhs, rhs) != 0
    BRA_JT,                                       // pop index; branch through jump table/default
    INT,                                          // no retail handler; todo: check milestone behavior
    RETS,                                         // SPA + RET
    PARE,                                         // next PSH/POP uses popped indirect-array index
    ADDR,                                         // next PSH pushes lvalue address
    DC,                                           // create and thread-track empty plain dynamic array
    DD,                                           // create instance-owned empty plain dynamic array
    RELEASE_TRACKED_PLAIN_ARRAY = 59,             // untrack and release plain array
    DPS,                                          // dynamic-array push
    DPO,                                          // dynamic-array pop
    DSZ,                                          // dynamic-array size
    DPA,                                          // next PSH/POP uses popped dynamic-array index
    DCL,                                          // clear plain dynamic array
    RAS,                                          // raise event without argument block
    AUTODEST,                                     // destroy instance after its last thread exits
    CASTUINT,                                     // num       -> uint word
    CASTNUM,                                      // uint word -> num
    BOUND,                                        // no retail handler; todo: check milestone behavior
    CGC,                                          // clear global callbacks
    CIC,                                          // pop instance; clear its callbacks
    RASA,                                         // raise event with argument block
    INSTCHECK,                                    // no retail handler; todo: check milestone behavior
    BVSR,                                         // receiver-remapped synchronous script call
    INC_LOCAL,                                    // ++local
    DEC_LOCAL,                                    // --local
    LT_LOCAL,                                     // top = local < top
    DPA_LOCAL,                                    // dynamic-array index from local; 4-byte element
    PAE_LOCAL,                                    // inline-array index from local; 4-byte element
    ADD_LOCAL,                                    // top += local
    SUB_LOCAL,                                    // top -= local
    MUL_LOCAL,                                    // top *= local
    PSH_LM,                                       // push object member addressed through local
    POP_LM,                                       // pop into object member addressed through local
    PSH_NUM,                                      // push inline num literal
    PSH_NL,                                       // push inline num list
    PSH_STR,                                      // push inline string/reference word
    PAE_LIT,                                      // inline-array literal index; 4-byte element
    PAE_LIT12,                                    // inline-array literal index; 12-byte element
    DPA_LIT,                                      // dynamic-array literal index; 4-byte element
    DPA_LIT12,                                    // dynamic-array literal index; 12-byte element
    GT_LIT,                                       // top = top >  literal
    EQ_LIT,                                       // top = top == literal
    LT_LIT,                                       // top = top <  literal
    LE_LIT,                                       // top = top <= literal
    LNT_LOCAL,                                    // push !local
    CST,                                          // pop pointer; push allocated-and-flagged check
    PUSH_IMMEDIATE_WORD           = 98,           // push inline word; compiler meaning unknown
    COPY_TOP_WORD_TO_LOCAL        = 99,           // local = top without pop
    RELEASE_LOCAL_PLAIN_ARRAY_REF = 100,          // release plain-array ref in local
    RETAIN_PLAIN_ARRAY_REF        = 101,          // retain plain-array ref from local/top
    RELEASE_TOP_PLAIN_ARRAY_REF   = 102,          // release plain-array ref on top
    RETAIN_NESTED_ARRAY_ELEMENTS  = 103,          // retain every inner plain array
    RELEASE_NESTED_ARRAY_ELEMENTS = 104,          // release every inner plain array
    CLEAR_NESTED_ARRAY            = 105,          // release elements and clear outer array
    DESTROY_TRACKED_NESTED_ARRAY  = 106,          // untrack and recursively destroy array
    CREATE_TRACKED_NESTED_ARRAY   = 107           // create and recursively track empty array
}; // e_opcode
}}}} // banana::NGL::chuck::vm
