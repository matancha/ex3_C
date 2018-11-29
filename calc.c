#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "stack.h"

#define MAX_CHARS_LINE 100

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

const int NUM_OPERATORS = 5;
Operator Plus = {'+', 1};
Operator Minus = {'-', 1};
Operator Multiplication = {'*', 2};
Operator Division = {'/', 2};
Operator Power = {'^', 3};
Operator Null = {'N', 0};

Operator getOperator(char c)
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

int main()
{
    char infix[MAX_CHARS_LINE + 1] = {0};

    while (fgets(infix, sizeof(infix), stdin) != NULL)
    {
        Stack *tempStack = stackAlloc(sizeof(Token));
        Stack *resultStack = stackAlloc(sizeof(Token));
        printf("Infix: %s", infix);

        for (int i = 0; i < (int)strlen(infix); ++i)
        {
            if (infix[i] == '(')
            {
                Token token;
                token.type = PARENTHESES;
                push(tempStack, &token);
            }
            else if (infix[i] == ')')
            {
                while (isEmptyStack(tempStack) != 1 && ((Token *) peek(tempStack))->type !=
                PARENTHESES)
                {
                    Token temp;
                    pop(tempStack, &temp);
                    push(resultStack, &temp);
                }
                Token token;
                pop(tempStack, &token);
            }
            else
            {
                if (getOperator(infix[i]).sign != 'N')
                {
                    Token token;
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
                               && ((Token *) peek(tempStack))->type != PARENTHESES)
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
                    i = j-1;
                    strncpy(strNum, &infix[i], j-i);

                    char *error;
                    int num = (int)strtol(strNum, &error, 10);

                    Token token;
                    token.type = OPERAND;
                    token.data.intVal.value = num;
                    push(resultStack, &token);
                }
            }
        }

        while (isEmptyStack(tempStack) != 1)
        {
            Token temp;
            pop(tempStack, &temp);
            push(resultStack, &temp);
        }

        printf("Postfix: ");

        while (isEmptyStack(resultStack) != 1)
        {
            Token temp;
            pop(resultStack, &temp);
            push(tempStack, &temp);
        }

        while (isEmptyStack(tempStack) != 1)
        {
            Token temp;
            pop(tempStack, &temp);
            printf("%d ", temp.data.intVal.value);
            push(resultStack, &temp);
        }
        printf("\n");

        freeStack(&tempStack);
        freeStack(&resultStack);
    }
}