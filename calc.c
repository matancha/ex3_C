#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "stack.h"
#include "calc.h"

#define MAX_CHARS_LINE 100

const int NUM_OPERATORS = 5;

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
            Token token;
            if (infix[i] == '(')
            {
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
                    strncpy(strNum, &infix[i], (size_t)j-i);

                    i = j-1;

                    char *error;
                    int num = (int)strtol(strNum, &error, 10);

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
            if (temp.type == OPERAND)
            {
                printf(" %d ", temp.data.intVal.value);
            }
            else if (temp.type == OPERATOR)
            {
                printf("%c", temp.data.operatorVal.operator.sign);
            }
            push(resultStack, &temp);
        }
        printf("\n");

        freeStack(&tempStack);
        freeStack(&resultStack);
    }
}