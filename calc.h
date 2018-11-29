#ifndef CALC_H
#define CALC_H

typedef struct {
    char sign;
    int precedence;
} Operator;

typedef enum {OPERAND, OPERATOR, PARENTHESES} TokenType;

typedef struct _Token {
    TokenType type;
    union data {
        struct { int value; } intVal;
        struct { Operator operator; } operatorVal;
    } data;
} Token;

Operator Plus = {'+', 1};
Operator Minus = {'-', 1};
Operator Multiplication = {'*', 2};
Operator Division = {'/', 2};
Operator Power = {'^', 3};
Operator Null = {'0', 0};

#endif //CALC_H
