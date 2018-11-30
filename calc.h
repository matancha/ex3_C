#ifndef CALC_H
#define CALC_H

typedef struct _Operator {
    char sign;
    int precedence;
    int (*calcuate)(int a, int b);
} Operator;

typedef enum {OPERAND, OPERATOR, PARENTHESIS} TokenType;

typedef struct _Token {
    TokenType type;
    union data {
        struct { int value; } intVal;
        struct { Operator operator; } operatorVal;
    } data;
} Token;

int calculate_plus(int a, int b);
int calculate_minus(int a, int b);
int calculate_product(int a, int b);
int calculate_division(int a, int b);
int calculate_power(int a, int b);

Operator Plus = {'+', 1, &calculate_plus};
Operator Minus = {'-', 1, &calculate_minus};
Operator Multiplication = {'*', 2, &calculate_product};
Operator Division = {'/', 2, &calculate_division};
Operator Power = {'^', 3, &calculate_power};

#endif //CALC_H
