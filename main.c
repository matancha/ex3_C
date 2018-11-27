#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "stack.h"

int main()
{
    char infix[100];
    Stack *stack = stackAlloc(sizeof(int));

    while (fgets(infix, sizeof(infix), stdin) != NULL)
    {
        printf("Infix: %s", infix);

        char *symbol = strtok(infix , " ");
        while (symbol != NULL)
        {
            char *endptr;
            int operand = (int)strtol(symbol, &endptr, 10);
            push(stack, &operand);

            symbol = strtok(NULL, " ");
        }
    }

    for(int i = 1; i <= 4; i++)
    {
        int headData;
        pop(stack,&headData);
        printf("top value is: %d\n",headData);
    }

    freeStack(&stack);
}

/*int main()
{
  int i, num = 10;
  printf("Generating list with %d ints\n", num);
  Stack *stack = stackAlloc(sizeof(int)); 
  for(i = 1; i <= num; i++) {
    push(stack,&i);
  }
  for(i = 1; i <= num-2; i++) {
    int headData;
    pop(stack,&headData);
    printf("top value is: %d\n",headData);
  }
  freeStack(&stack);
  return 0;
}*/
