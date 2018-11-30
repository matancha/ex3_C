#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include "stack.h"
#include "calc.h"

#define MAX_CHARS_LINE 100

const int NUM_OPERATORS = 5;
const char LEFT_PARENTHESIS = '(';
const char RIGHT_PARENTHESIS = ')';
const char *ILLEGAL_STR_ERR = "Illegal parameter supplied";

int calculate_plus(int a, int b)
{
    return a + b;
}

int calculate_minus(int a, int b)
{
    return a - b;
}

int calculate_product(int a, int b)
{
    return a * b;
}

int calculate_division(int a, int b)
{
    return a / b;
}

int calculate_power(int a, int b)
{
    return (int)pow(a, b);
}

int calculate_nothing(int a, int b)
{
    return a + b;
}

void moveStackToStack(Stack *src, Stack *dest)
{
    while (isEmptyStack(src) != 1)
    {
        Token temp;
        pop(src, &temp);
        push(dest, &temp);
    }
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

Operator getOperator(const char c)
{
    Operator operators[] = {Plus, Minus, Multiplication, Division, Power, Null};
    for (int i = 0; i < NUM_OPERATORS; ++i)
    {
        if (operators[i].sign == c)
        {
            return operators[i];
        }
    }
    return Null;
}

void parseInput(const char *infix, Stack *tempStack, Stack *resultStack)
{
    for (int i = 0; i < (int)strlen(infix); ++i)
    {
        Token token;
        if (infix[i] == LEFT_PARENTHESIS)
        {
            token.type = PARENTHESIS;
            push(tempStack, &token);
        }
        else if (infix[i] == RIGHT_PARENTHESIS)
        {
            while (isEmptyStack(tempStack) != 1 && ((Token *) peek(tempStack))->type !=
                                                   PARENTHESIS)
            {
                Token temp;
                pop(tempStack, &temp);
                push(resultStack, &temp);
            }
            pop(tempStack, &token);
        }
        else
        {
            if (getOperator(infix[i]).sign != '0')
            {
                token.type = OPERATOR;
                token.data.operatorVal.operator = getOperator(infix[i]);
                if (isEmptyStack(tempStack) == 1)
                {
                    push(tempStack, &token);
                }
                else
                {
                    while (isEmptyStack(tempStack) != 1 &&
                           ((Token *) peek(tempStack))->type == OPERATOR
                           && getOperator(infix[i]).precedence <= ((Token *) peek(tempStack))
                            ->data.operatorVal.operator.precedence
                           && ((Token *) peek(tempStack))->type != PARENTHESIS)
                    {
                        Token temp;
                        pop(tempStack, &temp);
                        push(resultStack, &temp);
                    }
                    push(tempStack, &token);
                }
            }

            if (infix[i] >= '0' && infix[i] <= '9')
            {
                int j = i;
                while (infix[j] >= '0' && infix[j] <= '9')
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
                }
            }
        }
    }
}

int main()
{
    char infix[MAX_CHARS_LINE + 1] = {0};

    while (fgets(infix, sizeof(infix), stdin) != NULL)
    {
        Stack *tempStack = stackAlloc(sizeof(Token));
        Stack *resultStack = stackAlloc(sizeof(Token));
        printf("Infix: %s", infix);

        parseInput(infix, tempStack, resultStack);

        moveStackToStack(tempStack, resultStack);
        printf("Postfix: ");
        moveStackToStack(resultStack, tempStack);

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
                int result = temp.data.operatorVal.operator.calcuate(b.data.intVal.value,
                                                                     a.data.intVal.value);
                a.data.intVal.value = result;
                push(calculationStack, &a);
            }
            push(resultStack, &temp);
        }
        printf("\n");
        Token finalToken;
        pop(calculationStack, &finalToken);
        printf("The value is %d", finalToken.data.intVal.value);

        freeStack(&calculationStack);
        freeStack(&tempStack);
        freeStack(&resultStack);
    }
}