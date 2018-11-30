#ifndef CALC_H
#define CALC_H

/**
 * struct for operators
 */
typedef struct _Operator
{
    char sign;
    int precedence;
    int (*calculate)(int a, int b);
} Operator;

/**
 * TokenTypes possible in program
 */
typedef enum
{OPERAND,
 OPERATOR,
 PARENTHESIS
} TokenType;

/**
 * Data structure for stack
 */
typedef struct _Token
{
    TokenType type;
    /**
     * Data
     */
    union data
    {
        /**
         * Numerical values
         */
        struct { int value; } intVal;
        /**
         * Operators
         */
        struct { Operator operator; } operatorVal;
    } data;
} Token;

/**
 * Calculate sum
 * @param a first int
 * @param b second int
 * @return sum
 */
int calculatePlus(const int a, const int b);

/**
 * Calculate minus
 * @param a first int
 * @param b second int
 * @return sum
 */
int calculateMinus(const int a, const int b);

/**
 * Calculate product
 * @param a first int
 * @param b second int
 * @return product
 */
int calculateProduct(const int a, const int b);

/**
 * Calcuates division
 * @param a first int
 * @param b second int
 * @return result is succeeds, if division by zero exits program
 */
int calculateDivision(const int a, const int b);

/**
 * Calculate power
 * @param a first int
 * @param b power
 * @return result
 */
int calculatePower(const int a, const int b);

Operator Plus = {'+', 1, &calculatePlus};
Operator Minus = {'-', 1, &calculateMinus};
Operator Multiplication = {'*', 2, &calculateProduct};
Operator Division = {'/', 2, &calculateDivision};
Operator Power = {'^', 3, &calculatePower};

#endif //CALC_H
