#pragma once

#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <optional>

typedef double f64;
typedef float f32;

typedef uint64_t u64;
typedef int64_t s64;

typedef uint32_t u32;
typedef int32_t s32;

typedef uint16_t u16;
typedef int16_t s16;

typedef uint8_t u8;
typedef int8_t s8;

extern char* compiler_exec;

enum TokenType {
    _exit,
    int_lit,
    semi,
    o_paren,
    c_paren,
    ident,
    let,
    eq,
    add
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

#define fatal_error(message) {\
    fprintf(stderr, "%s: %s\n", compiler_exec, message);\
    throw std::runtime_error("*** COMPILATION FAILED");\
}
