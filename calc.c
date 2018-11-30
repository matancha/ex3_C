#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "stack.h"
#include "calc.h"

#define MAX_CHARS_LINE 100

const int NUM_OPERATORS = 5;
const char LEFT_PARENTHESIS = '(';
const char RIGHT_PARENTHESIS = ')';
const char *ILLEGAL_STR_ERR = "Illegal parameter supplied";
const char *DIVISION_ZERO_ERR = "Division by 0!";
const char *MEMORY_ALLOCATION_ERR = "Memory allocation failed";
const char *INFIX_MSG = "Infix: ";
const char *POSTFIX_MSG = "Postfix: ";

/**
 * Calculate sum
 * @param a first int
 * @param b second int
 * @return sum
 */
int calculatePlus(const int a, const int b)
{
    return a + b;
}

/**
 * Calculate minus
 * @param a first int
 * @param b second int
 * @return sum
 */
int calculateMinus(const int a, const int b)
{
    return a - b;
}

/**
 * Calculate product
 * @param a first int
 * @param b second int
 * @return product
 */
int calculateProduct(const int a, const int b)
{
    return a * b;
}

/**
 * Calcuates division
 * @param a first int
 * @param b second int
 * @return result is succeeds, if division by zero exits program
 */
int calculateDivision(const int a, const int b)
{
    if (b == 0)
    {
        fprintf(stderr, "%s", DIVISION_ZERO_ERR);
        exit(EXIT_FAILURE);
    }
    return a / b;
}

/**
 * Calculate power
 * @param a first int
 * @param b power
 * @return result
 */
int calculatePower(const int a, const int b)
{
    return (int)pow(a, b);
}

/**
 * Move one element from stack src to stack dest
 * @param src stack src
 * @param dest stack dest
 */
void moveStackToStack(Stack *src, Stack *dest)
{
    Token temp;
    pop(src, &temp);
    push(dest, &temp);
}

/**
 * Assigns string value to int variable. If failure is detected, exits the program.
 * @param str string to convert
 * @param variable int pointer
 */
void convertStrToInt(const char *str, int *variable)
{
    char *endptr;
    *variable = (int)strtol(str, &endptr, 10);
    if (*variable == 0 && (errno != 0 && endptr == str))
    {
        fprintf(stderr, "%s", ILLEGAL_STR_ERR);
        exit(EXIT_FAILURE);
    }
}

/**
 * Checks if char is the sign attributed to an operator
 * @param c sign
 * @param operators list of operators
 * @return 1 if true, 0 else
 */
int isOperator(const char c, const Operator operators[])
{
    for (int i = 0; i < NUM_OPERATORS; ++i)
    {
        if (operators[i].sign == c)
        {
            return 1;
        }
    }
    return 0;
}

/**
 * Gets the operator corresponding to c sign
 * @param c sign
 * @param operators list of operators
 * @return operator, Plus is default but can't be reached (function called only after checking)
 */
Operator getOperator(const char c, const Operator operators[])
{
    for (int i = 0; i < NUM_OPERATORS; ++i)
    {
        if (operators[i].sign == c)
        {
            return operators[i];
        }
    }
    /* Can't get here - default value */
    return Plus;
}

/**
 * Parse input from user
 * @param infix input
 * @param tempStack temporary stack
 * @param resultStack result stack
 */
void parseInput(const char *infix, Stack *tempStack, Stack *resultStack)
{
    Operator operators[] = {Plus, Minus, Multiplication, Division, Power};
    for (int i = 0; i < (int)strlen(infix); ++i)
    {
        Token token;
        if (isdigit((int)infix[i]))
        {
            int j = i;
            while (isdigit((int)infix[j]))
            {
                j++;
            }
            char *strNum = malloc((j-i) * sizeof(char));
            if (strNum != NULL)
            {
                strncpy(strNum, &infix[i], (size_t)j-i);

                i = j-1;

                int num;
                convertStrToInt(strNum, &num);
                free(strNum);

                token.type = OPERAND;
                token.data.intVal.value = num;
                push(resultStack, &token);
                printf(" %d ", num);
            }
            else
            {
                fprintf(stderr, "%s", MEMORY_ALLOCATION_ERR);
            }
        }
        else if (infix[i] == LEFT_PARENTHESIS)
        {
            printf("%c", infix[i]);
            token.type = PARENTHESIS;
            push(tempStack, &token);
        }
        else if (infix[i] == RIGHT_PARENTHESIS)
        {
            printf("%c", infix[i]);
            while (isEmptyStack(tempStack) != 1 && ((Token *) peek(tempStack))->type != PARENTHESIS)
            {
                moveStackToStack(tempStack, resultStack);
            }
            pop(tempStack, &token);
        }
        else if (isOperator(infix[i], operators) == 1)
        {
            printf("%c", infix[i]);
            token.type = OPERATOR;
            token.data.operatorVal.operator = getOperator(infix[i], operators);
            if (isEmptyStack(tempStack) == 1)
            {
                push(tempStack, &token);
            }
            else
            {
                while (isEmptyStack(tempStack) != 1 &&
                       ((Token *) peek(tempStack))->type == OPERATOR
                       && token.data.operatorVal.operator.precedence <=
                       ((Token *) peek(tempStack))->data.operatorVal.operator.precedence
                       && ((Token *) peek(tempStack))->type != PARENTHESIS)
                {
                    moveStackToStack(tempStack, resultStack);
                }
                push(tempStack, &token);
            }
        }
    }
}

/**
 * Main program
 * @return exit status
 */
int main()
{
    char infix[MAX_CHARS_LINE + 1] = {0};

    while (fgets(infix, sizeof(infix), stdin) != NULL)
    {
        Stack *tempStack = stackAlloc(sizeof(Token));
        Stack *resultStack = stackAlloc(sizeof(Token));

        printf("%s", INFIX_MSG);
        parseInput(infix, tempStack, resultStack);
        printf("\n");

        while (isEmptyStack(tempStack) != 1)
        {
            moveStackToStack(tempStack, resultStack);
        }
        printf("%s", POSTFIX_MSG);
        while (isEmptyStack(resultStack) != 1)
        {
            moveStackToStack(resultStack, tempStack);
        }

        Stack *calculationStack = stackAlloc(sizeof(Token));
        while (isEmptyStack(tempStack) != 1)
        {
            Token temp;
            pop(tempStack, &temp);
            if (temp.type == OPERAND)
            {
                printf(" %d ", temp.data.intVal.value);
                push(calculationStack, &temp);
            }
            else if (temp.type == OPERATOR)
            {
                printf("%c", temp.data.operatorVal.operator.sign);

                Token a;
                Token b;
                pop(calculationStack, &a);
                pop(calculationStack, &b);
                int result = temp.data.operatorVal.operator.calculate(b.data.intVal.value,
                                                                     a.data.intVal.value);
                a.data.intVal.value = result;
                push(calculationStack, &a);
            }
            push(resultStack, &temp);
        }
        printf("\n");
        Token finalToken;
        pop(calculationStack, &finalToken);
        printf("The value is %d\n", finalToken.data.intVal.value);

        freeStack(&calculationStack);
        freeStack(&tempStack);
        freeStack(&resultStack);
    }
}