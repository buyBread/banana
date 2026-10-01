#pragma once

#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    /*
        instruction word (target-endian u16):
            bit  15     source-line marker, ignored by retail
            bits 14..8  e_opcode
            bits 7..2   e_opcode_arg; alone decides the payload length (see A1FA70)
            bits 1..0   data width / 4

        names come from the milestone's opcode_t_str table; 40, 41, 59 and 98..107 are behavioral.
    */

    // sub_A22B70
    enum class e_opcode : u32 {
        NOP,                                          // retail: no-op; milestone: asserts corrupt bytecode
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
        GT,                                           // lhs >  rhs
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
        ECB,                                          // callback running a function remapped against a popped target instance
        SCB,                                          // callback running the linked function on the current instance
        ECO,                                          // ECB, one-shot
        SCO,                                          // SCB, one-shot
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
        INT,                                          // retail (53) : no-op; milestone (51): software breakpoint
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
        RAS,                                          // raise signal; GSIG/ISIG/LSIG picks the recipient
        AUTODEST,                                     // destroy instance after its last thread exits
        CASTUINT,                                     // num  -> uint
        CASTNUM,                                      // uint -> num
        BOUND,                                        // retail (69): no-op; milestone (66): upper-bound check on the numeric value at the top of the stack
        CGC,                                          // clear global callbacks by name (STR) or popped id (UINT_NULL)
        CIC,                                          // pop instance; clear its callbacks by name (STR) or popped id (UINT_NULL)
        RASA,                                         // RAS with a sized argument block
        INSTCHECK,                                    // retail (73): no-op; milestone (70): validates nullable top instance against inline script-type name
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
        PSH_STR,                                      // push permanent-string pointer
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
        PUSH_IMMEDIATE_WORD           = 98,           // push raw inline word (UINT); compiler meaning unknown
        COPY_TOP_WORD_TO_LOCAL        = 99,           // local = top without pop
        RELEASE_LOCAL_PLAIN_ARRAY_REF = 100,          // release plain-array ref in local
        RETAIN_PLAIN_ARRAY_REF        = 101,          // retain plain-array ref from local/top
        RELEASE_TOP_PLAIN_ARRAY_REF   = 102,          // release plain-array ref on top
        RETAIN_NESTED_ARRAY_ELEMENTS  = 103,          // retain every inner plain array
        RELEASE_NESTED_ARRAY_ELEMENTS = 104,          // release every inner plain array
        CLEAR_NESTED_ARRAY            = 105,          // release elements and clear outer array
        DESTROY_TRACKED_NESTED_ARRAY  = 106,          // untrack and recursively destroy array
        CREATE_TRACKED_NESTED_ARRAY   = 107           // create and recursively track empty array
    };

    /*
        opcode_arg_t: names come from the milestone's opcode_arg_t_str table.
        retail dropped one of the milestone's SDRE/SFRE/NUME/STRE between GV and JT, so from JT on
        retail values are the milestone's minus one. the extern forms (UNK_13..15, GSIG_SFRE..LSIG_SFRE,
        UINTE) were already rejected by the milestone linker and carry no payload in retail.
        linker kind (A1FA70) = value - 1.
    */
    enum class e_opcode_arg : u32 {
        NULL_ARG,       // no payload
        NUM,            // u32 float literal
        STR,            // permanent-string index -> character pointer
        WORD,           // u16
        PCR,            // u16 signed byte displacement from the end of the instruction
        SPR,            // u16 stack-relative local
        POPO,           // u16
        SDR,            // u16 object index, u16 byte offset; resolved per current instance
        SFR,            // u16 object index, u16 flattened function index -> script_function*
        LFR,            // u16 class index, u16 function index -> script_library_function*
        CLV,            // u16 class index, u16 permanent string -> script_library_class::find_instance
        PSIG,           // u32 signal hash
        GV,             // u16 offset, u16 block (1 = game) -> script variable address
        UNK_13,
        UNK_14,
        UNK_15,
        JT,             // u16 count, count displacements, u16 default
        GSIG_SFR,       // SFR, global recipient
        ISIG_SFR,       // SFR, script instance recipient
        LSIG_SFR,       // SFR, library (entity) recipient
        GSIG_SFRE,
        ISIG_SFRE,
        LSIG_SFRE,
        GSIG,           // no payload; global recipient
        ISIG,           // no payload; popped script instance recipient
        LSIG,           // no payload; popped library (entity) recipient
        OBJ,            // u16 object index -> script_object*
        UINT,           // raw u32
        UINTE,
        UINT_NULL,      // no payload
        EI,             // two permanent strings -> script_executable::resolve_extern_callback
        OBJ_REF,        // u16 object index -> script_object*
        WORD_PAIR,      // two u16
        NL,             // u16 count, count u32
        UNK_34,         // u16 object index -> script_object*; retail addition
        UNK_35          // u16 object index -> script_object*; retail addition
    };
}}} // treyarch::chuck::vm
